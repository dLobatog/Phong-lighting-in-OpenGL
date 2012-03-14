/*
 *  PLY.cpp
 *  PointProcessing
 *
 *  Created by Renato Pajarola on Wed Nov 05 2003.
 *  Copyright (c) 2003 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <glut/glut.h>
#else
#include <GL/glut.h>
#endif

#include "PLY.h"
#include "geometry.h"

extern int light;

// Light and other info
extern Vector3f light_pos, viewer_pos;
extern Vector4f light_color, black_color, ambient_light;

// Material properties
GLfloat ambient[4] = {0.2, 0.2, 0.2, 1.0};
GLfloat diffuse[4] = {0.7, 0.7, 1.0, 1.0};
GLfloat specular[4] = {0.5, 0.5, 0.5, 1.0};
GLfloat shininess[1] = {5.0};


PLYObject::PLYObject(FILE *in)
{
  int i;

  nproperties = 0;
  hasnormal = hastexture = false;

  nv = nf = 0;

  vertices = NULL;
  normals = NULL;
  colors = NULL;
  texcoords = NULL;
  faces = NULL;

  // init bounding box
  for (i = 0; i < 3; i++) {
    min[i] = FLT_MAX;
    max[i] = -FLT_MAX;
  }

  // default order
  for (i = 0; i < 11; i++)
    order[i] = -1;

  if (!checkHeader(in)) {
    fprintf(stderr, "Error: could not read PLY file.\n");
    return;
  }

  vertices = (Vector3f*)calloc(nv, sizeof(Vector3f));
  normals  = (Vector3f*)calloc(nv, sizeof(Vector3f));
  colors   = (Color3u*)calloc(nv, sizeof(Color3u));
  if (hastexture)
    texcoords = (Texture2f*)calloc(nv, sizeof(Texture2f));

  faces    = (Index3i*)calloc(nf, sizeof(Index3i));
  fnormals = (Vector3f*)calloc(nf, sizeof(Vector3f));

  readVertices(in);
  readFaces(in);
}


PLYObject::~PLYObject()
{
  // delete all allocated arrays

  if (vertices)
    free(vertices);
  if (normals)
    free(normals);
  if (colors)
    free(colors);
  if (texcoords)
    free(texcoords);
  if (faces)
    free(faces);
}


bool PLYObject::checkHeader(FILE *in)
{
  char buf[128], type[128], c[32];
  int i;

  // read ply file header
  fscanf(in, "%s\n", buf);
  if (strcmp(buf, "ply") != 0) {
    fprintf(stderr, "Error: Input file is not of .ply type.\n");
    return false;
  }
  fgets(buf, 128, in);
  if (strncmp(buf, "format ascii", 12) != 0) {
    fprintf(stderr, "Error: Input file is not in ASCII format.\n");
    return false;
  }

  fgets(buf, 128, in);
  while (strncmp(buf, "comment", 7) == 0)
    fgets(buf, 128, in);

  // read number of vertices
  if (strncmp(buf, "element vertex", 14) == 0)
    sscanf(buf, "element vertex %d\n", &nv);
  else {
    fprintf(stderr, "Error: number of vertices expected.\n");
    return false;
  }

  // read vertex properties order
  i = 0;
  fgets(buf, 128, in);
  while (strncmp(buf, "property", 8) == 0) {
    sscanf(buf, "property %s %s\n", type, c);
    if (strncmp(c, "x", 1) == 0)
      order[0] = i;
    else if (strncmp(c, "y", 1) == 0)
      order[1] = i;
    else if (strncmp(c, "z", 1) == 0)
      order[2] = i;

    else if (strncmp(c, "nx", 2) == 0)
      order[3] = i;
    else if (strncmp(c, "ny", 2) == 0)
      order[4] = i;
    else if (strncmp(c, "nz", 2) == 0)
      order[5] = i;

    else if (strncmp(c, "red", 3) == 0)
      order[6] = i;
    else if (strncmp(c, "green", 5) == 0)
      order[7] = i;
    else if (strncmp(c, "blue", 4) == 0)
      order[8] = i;

    else if (strncmp(c, "tu", 2) == 0)
      order[9] = i;
    else if (strncmp(c, "tv", 2) == 0)
      order[10] = i;

    i++;
    fgets(buf, 128, in);
  }
  nproperties = i;

  for (i = 0; i < 3; i++) {
    if (order[i] < 0) {
      fprintf(stderr, "Error: not enough vertex coordinate fields (nx, ny, nz).\n");
      return false;
   }
  }
  hasnormal = true;
  for (i = 3; i < 6; i++)
    if (order[i] < 0)
      hasnormal = false;
  hascolor = true;
  for (i = 6; i < 9; i++)
    if (order[i] < 0)
      hascolor = false;
  hastexture = true;
  for (i = 9; i < 11; i++)
    if (order[i] < 0)
      hastexture = false;

  if (!hasnormal)
    fprintf(stderr, "Warning: no normal coordinates used from file.\n");
  if (!hascolor)
    fprintf(stderr, "Warning: no color used from file.\n");
  if (!hastexture)
    fprintf(stderr, "Warning: no texture coordinates used from file.\n");

  // number of faces and face properties
  if (strncmp(buf, "element face", 12) == 0)
    sscanf(buf, "element face %d\n", &nf);
  else {
    fprintf(stderr, "Error: number of faces expected.\n");
    return false;
  }

  fgets(buf, 128, in);
  if (strncmp(buf, "property list", 13) != 0) {
    fprintf(stderr, "Error: property list expected.\n");
    return false;
  }

  fgets(buf, 128, in);
  while (strncmp(buf, "end_header", 10) != 0)
    fgets(buf, 128, in);

  return true;
}


void PLYObject::readVertices(FILE *in)
{
  char buf[128];
  int i, j;
  float values[32];

  // read in vertex attributes
  for (i = 0; i < nv; i++) {
    fgets(buf, 128, in);
    sscanf(buf,"%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f", &values[0], &values[1], &values[2], &values[3],
            &values[4], &values[5], &values[6], &values[7], &values[8], &values[9], &values[10], &values[11],
           &values[12], &values[13], &values[14], &values[15]);

    for (j = 0; j < 3; j++)
      vertices[i][j] = values[order[j]];
    if (hasnormal)
      for (j = 0; j < 3; j++)
        normals[i][j] = values[order[3+j]];
    if (hascolor)
      for (j = 0; j < 3; j++)
        colors[i][j] = (unsigned char)values[order[6+j]];
    if (hastexture)
      for (j = 0; j < 2; j++)
        texcoords[i][j] = values[order[9+j]];

    for (j = 0; j < 3; j++) {
      if (vertices[i][j] < min[j])
        min[j] = vertices[i][j];
      if (vertices[i][j] > max[j])
        max[j] = vertices[i][j];
    }
  }
}


void PLYObject::readFaces(FILE *in)
{
  char buf[128];
  int i, j, k;

  // read in face connectivity
  for (i = 0; i < nf; i++) {
    fgets(buf, 128, in);
    sscanf(buf, "%d %d %d %d", &k, &faces[i][0], &faces[i][1], &faces[i][2]);
    if (k != 3) {
      fprintf(stderr, "Error: not a triangular face.\n");
      exit(1);
    }
    // set up face normal
    normal(fnormals[i], vertices[faces[i][0]], vertices[faces[i][1]], vertices[faces[i][2]]);

    // accumulate normal information of each vertex
    if (!hasnormal)
      for (j = 0; j < 3; j++)
        for (k = 0; k < 3; k++)
          normals[faces[i][j]][k] += fnormals[i][k];
  }
  if (!hasnormal)
    for (i = 0; i < nv; i++)
      normalize(normals[i]);
  hasnormal = true;
}


void PLYObject::resize()
{
  int i;
  float minx, miny, minz, maxx, maxy, maxz;
  float size, scale;

  // get bounding box
  minx = miny = minz = FLT_MAX;
  maxx = maxy = maxz = -FLT_MAX;
  for (i = 0; i <nv; i++) {
    if (vertices[i][0] < minx)
      minx = vertices[i][0];
    if (vertices[i][0] > maxx)
      maxx = vertices[i][0];

    if (vertices[i][1] < miny)
      miny = vertices[i][1];
    if (vertices[i][1] > maxy)
      maxy = vertices[i][1];

    if (vertices[i][2] < minz)
      minz = vertices[i][2];
    if (vertices[i][2] > maxz)
      maxz = vertices[i][2];
  }

  // rescale vertex coordinates to be in [-1,1]
  size = 0.0;
  if (size < maxx-minx)
    size = maxx-minx;
  if (size < maxy-miny)
    size = maxy-miny;
  if (size < maxz-minz)
    size = maxz-minz;
  scale = 2.0 / size;
  for (i = 0; i < nv; i++) {
    vertices[i][0] = scale * (vertices[i][0] - minx) - 1.0;
    vertices[i][1] = scale * (vertices[i][1] - miny) - 1.0;
    vertices[i][2] = scale * (vertices[i][2] - minz) - 1.0;
  }
}

void PLYObject::invertNormals()
{
  int i, tmp;

  for (i = 0; i < nv; i++)
    scale(normals[i], -1.0);
  for (i = 0; i < nf; i++) {
    scale(fnormals[i], -1.0);
    tmp = faces[i][0];
    faces[i][0] = faces[i][2];
    faces[i][2] = tmp;
  }
}


void PLYObject::draw()
{

  // setup default material
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  glColor3fv(diffuse);

  // set lighting if enabled
	// Otherwise, compute colors
  if (light)
		{
			glEnable(GL_LIGHTING);
		}
  else
		{

			glDisable(GL_LIGHTING);
			for (int v=0; v<nv; v++)
				{
					// Instead of this, use the Phong illumination equation to find the color

                    // Phong equation with attenuation factor att
                    // If = (As*Am) + ((Al*Am) + Id + Is) * att

                    // Get ModelView

                    float M[16];
                    Matrix4f modelViewMatrix;

                    glGetFloatv(GL_MODELVIEW_MATRIX, M);
                    for (int i = 0; i < 3; i++) {
                        modelViewMatrix[i][3] = 0.0;
                        modelViewMatrix[3][i] = 0.0;
                        for (int j = 0; j < 3; j++) {
                            modelViewMatrix[i][j] = M[i*4+j];
                        }
                    }
                    modelViewMatrix[3][3] = 1.0;

                    // ModelView is already here in a Matrix4f for operations

                    Vector3f vVertex;
                    multVector(vVertex, modelViewMatrix, vertices[v]);

                    Vector4f lightSource;
                    copy(lightSource, light_pos);
                    //glGetLightfv(GL_LIGHT0, GL_POSITION, lightSource);

                    Vector3f lightDir;
                    sub(lightDir, lightSource, vVertex);

                    float d = length(lightDir);

                    // Get parameters for the ambient part
                    float Al[4] = {0.0f, 0.0f, 0.0f, 0.0f };
                    glGetLightfv( GL_LIGHT0, GL_AMBIENT, Al );

                    Vector4f As = {0.0f, 0.0f, 0.0f, 0.0f };
                    glGetFloatv( GL_LIGHT_MODEL_AMBIENT, As );

                    float Dl[4] = {0.0f, 0.0f, 0.0f, 0.0f };
                    glGetLightfv( GL_LIGHT0, GL_DIFFUSE, Dl );

                    float Sl[4] = {0.0f, 0.0f, 0.0f, 0.0f };
                    glGetLightfv( GL_LIGHT0, GL_SPECULAR, Sl );

                    Vector4f Am = {0.0f, 0.0f, 0.0f, 0.0f };
                    copy(Am, ambient);

                    float Dm[4] = {0.0f, 0.0f, 0.0f, 0.0f };
                    copy(Dm, diffuse);

                    float Sm[4] = {0.0f, 0.0f, 0.0f, 0.0f };
                    copy(Sm, specular);

                    float f = shininess[0];  // copy shininess

                    float constantAttenuation = 0.0f;
                    glGetLightfv( GL_LIGHT0, GL_CONSTANT_ATTENUATION, &constantAttenuation);

                    float linearAttenuation = 0.0f;
                    glGetLightfv( GL_LIGHT0, GL_LINEAR_ATTENUATION , &linearAttenuation );

                    float quadraticAttenuation = 0.0f;
                    glGetLightfv( GL_LIGHT0, GL_QUADRATIC_ATTENUATION , &quadraticAttenuation);

                    float att = (constantAttenuation +
                                (linearAttenuation*d) +
                                (quadraticAttenuation*d*d) );

					// Start the phong equation with the ambient component
					Vector4f temp1;
					multVectors4(temp1, As, Am);
					Vector4f temp2;
                    multVectors4(temp2, Al, Am);
                    scale4(temp2, att);
					Vector4f final_color;
					//final_color = (As * Am) + (Al * Am)*att;
					add(final_color, temp1, temp2);

                    // Calculate lambertTerm
                    Vector3f N;
                    normalizeVector(N, normals[v]);
                    Vector3f L;
                    normalizeVector(L, lightDir);

                    float lambertTerm = dotProd(N,L);

                    if (lambertTerm > 0.0){
                        // diffuse component
                        multVectors4(temp1,Dl,Dm);
                        scale4(temp1,lambertTerm);
                        add(final_color, final_color, temp1);

                        // specular component
                        Vector3f E;
                        normalizeVector(E,viewer_pos);
                        Vector3f R;
                        scale4(temp1,lambertTerm*2,N);
                        sub(R, temp1, L); // reflect(-L,N)

                        float dotProduct = dotProd(R,E);
                        float specular_factor;
                        if(dotProduct > 0.0){
                            specular_factor = pow(dotProduct, f);
                        } else {
                            specular_factor = 0; // pow(0.0, f);
                        }

                        multVectors4(temp1, Sl, Sm);
                        scale4(temp1, specular_factor*att, temp1);
                        add(final_color, final_color, temp1);
                        // printf("DM are %f , %f and %f", final_color[0], final_color[1], final_color[2]);
                    }


                    //Colors [v][i] should take the colors of final_color, how?
                    colors[v][0] = final_color[0]*255;
                    colors[v][1] = final_color[1]*255;
                    colors[v][2] = final_color[2]*255;
				}
		}

	// Now do the actual drawing of the model
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glBegin(GL_TRIANGLES);

  for (int i = 0; i < nf; i++)
    {
      for (int j = 0; j < 3; j++)
        {
            glColor3ubv((GLubyte*)colors[faces[i][j]]);
            glNormal3fv(normals[faces[i][j]]);	// vertex normal
            glVertex3fv(vertices[faces[i][j]]);	// vertex coordinates
        }
    }
  glEnd();

  glDisable(GL_POLYGON_OFFSET_FILL);
  if (hascolor)
    glDisable(GL_COLOR_MATERIAL);
}




void PLYObject::eat()
{
  float scale = 0.01;

  /* Jitter each vertex */
  for(int i=0 ; i<nv ;i++)
    for(int j=0 ; j<3 ; j++)
      {
        vertices[i][j] += scale*normals[i][j];
      }
}


void PLYObject::starve()
{
  float scale = -0.01;

  /* Jitter each vertex */
  for(int i=0 ; i<nv ;i++)
    for(int j=0 ; j<3 ; j++)
      {
        vertices[i][j] += scale*normals[i][j];
      }
}


void PLYObject::dance()
{
  /* This creates a random vector */
  Vector3f randomvector;
  randomvector[0] = rangerand(-0.1, 0.1, 30);
  randomvector[1] = rangerand(-0.1, 0.1, 30);
  randomvector[2] = rangerand(-0.1, 0.1, 30);


  /* Insert code here to add randomvector to all vertices in the PLYObject */
  /* Iterate over all the vertices and add randomvector to each one. */
  /* The vertex positions are stored in the array called 'vertices' */

	for (int v=0 ; v<nv ; v++)
		{
			vertices[v][0] += randomvector[0];
			vertices[v][1] += randomvector[1];
			vertices[v][2] += randomvector[2];
		}

}



/* A not too good pseudo-random number generator */
/* Returns a pseudo-random number between min and max */
double PLYObject::rangerand(double min, double max, long steps)
{
  return min + ((rand() % steps) * (max - min)) / steps;
}
