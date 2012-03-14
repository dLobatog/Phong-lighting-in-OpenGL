#ifndef PLY_H
#define PLY_H

/*
 *  PLY.h
 *  PointProcessing
 *
 *  Created by Renato Pajarola on Wed Nov 05 2003.
 *  Copyright (c) 2003 UC Irvine. All rights reserved.
 *
 */

#include <stdio.h>

typedef float Vector3f[3];
typedef unsigned char Color3u[3];
typedef float Texture2f[2];
typedef int Index3i[3];


class PLYObject {
public:

  PLYObject(FILE *in);
  ~PLYObject();

  bool checkHeader(FILE *in);
  void readVertices(FILE *in);
  void readFaces(FILE *in);
  void resize();
  double rangerand(double min, double max, long steps);
  void invertNormals();
  void dance();
  void eat();
  void starve();

  void draw();
  
  int nproperties;		// number of vertex properties
  int order[11];		// order of x,y,z, nx,ny,nz, red,green,blue, tu,tv vertex properties
  bool hasnormal, hascolor, hastexture;

  int nv, nf;		// number of vertices, faces

  Vector3f min, max;

  Vector3f *vertices;		// array of point coordinates
  Vector3f *normals;		// array of point normals
  Color3u *colors;		// array of point colors
  Texture2f *texcoords;		// array of texture coords
  Index3i *faces;		// array of face indices
  Vector3f *fnormals;		// array of face normals
};

#endif
