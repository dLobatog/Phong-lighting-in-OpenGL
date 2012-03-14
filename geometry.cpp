
/* File: geometry
 * Author: R. Pajarola
 * Date: Oct. 17, 2000
 * Description:
 *   3D geometry vector operations
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "geometry.h"


void add(Vector3f sum, const Vector3f v)
{
  int i;

  for (i = 0; i < 3; i++)
    sum[i] += v[i];
}


void add(Vector3f sum, const Vector3f v1, const Vector3f v2)
{
  int i;

  for (i = 0; i < 3; i++)
    sum[i] = v1[i] + v2[i];
}

void add4(Vector4f sum, const Vector4f v1, const Vector4f v2)
{
  int i;

  for (i = 0; i < 4; i++)
    sum[i] = v1[i] + v2[i];
}


void add(Vector3d sum, const Vector3d v1, const Vector3d v2)
{
  int i;

  for (i = 0; i < 3; i++)
    sum[i] = v1[i] + v2[i];
}


void sub(Vector3f diff, const Vector3f v)
{
  int i;

  for (i = 0; i < 3; i++)
    diff[i] -= v[i];
}


void sub(Vector3f diff, const Vector3f v1, const Vector3f v2)
{
  int i;

  for (i = 0; i < 3; i++)
    diff[i] = v1[i] - v2[i];
}


void sub(Vector3d diff, const Vector3d v1, const Vector3d v2)
{
  int i;

  for (i = 0; i < 3; i++)
    diff[i] = v1[i] - v2[i];
}


void scale(Vector3f mul, float s)
{
  int i;

  for (i = 0; i < 3; i++)
    mul[i] *= s;
}

void scale4(Vector4f mul, float s)
{
  int i;

  for (i = 0; i < 4; i++)
    mul[i] *= s;
}


void scale4(Vector4f mul, float s, const Vector4f v1)
{
  int i;

  for (i = 0; i < 4; i++)
    mul[i] = s * v1[i];
}


void scale(Vector3f mul, float s, const Vector3f v1)
{
  int i;

  for (i = 0; i < 3; i++)
    mul[i] = s * v1[i];
}


void scale(Vector3d mul, double s, const Vector3d v1)
{
  int i;

  for (i = 0; i < 3; i++)
    mul[i] = s * v1[i];
}


float dotProd(const Vector3f a, const Vector3f b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


double dotProd(const Vector3d a, const Vector3d b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


void vecProd(Vector3f n, const Vector3f u, const Vector3f v)
{
  n[0] = u[1]*v[2] - u[2]*v[1];
  n[1] = u[2]*v[0] - u[0]*v[2];
  n[2] = u[0]*v[1] - u[1]*v[0];
}


void vecProd(Vector3d n, const Vector3d u, const Vector3d v)
{
  n[0] = u[1]*v[2] - u[2]*v[1];
  n[1] = u[2]*v[0] - u[0]*v[2];
  n[2] = u[0]*v[1] - u[1]*v[0];
}


float normalize(Vector3f n)
{
  float norm;

  norm = sqrt(dotProd(n, n));
  scale(n, 1.0/norm);

  return norm;
}

void normalizeVector(Vector3f n, Vector3f input)
{
  float l = length(input);
  for(int i = 0; i<3; i++)
        n[i] = (input[i]/l);

}

double normalize(Vector3d n)
{
  double norm;

  norm = sqrt(dotProd(n, n));
  scale(n, 1.0/norm, n);

  return norm;
}


void normal(Vector3f n, const Vector3f p, const Vector3f q, const Vector3f r)
{
  Vector3f u, v;

  // right hand system, CCW triangle
  sub(u, q, p);
  sub(v, r, p);

  // plane normal
  vecProd(n, u, v);
  normalize(n);
}


void normal(Vector3d n, const Vector3d p, const Vector3d q, const Vector3d r)
{
  Vector3d u, v;

  // right hand system, CCW triangle
  sub(u, q, p);
  sub(v, r, p);

  // plane normal
  vecProd(n, u, v);
  normalize(n);
}


void emptyMatrix(Matrix3f m)
{
  int i, j;

  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
      m[j][i] = 0.0;
}


void emptyMatrix(Matrix4f m)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      m[j][i] = 0.0;
}


void setRowVectors(Matrix4f m, const Vector3f u, const Vector3f v, const Vector3f w)
{
  int i;

  for (i = 0; i < 3; i++) {
    m[0][i] = u[i];
    m[1][i] = v[i];
    m[2][i] = w[i];
  }
}


void setColVectors(Matrix4f m, const Vector3f u, const Vector3f v, const Vector3f w)
{
  int i;

  for (i = 0; i < 3; i++) {
    m[i][0] = u[i];
    m[i][1] = v[i];
    m[i][2] = w[i];
  }
}


void transpose(Matrix4f m, const Matrix4f m1)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      m[j][i] = m1[i][j];
}


void scaleMatrix(Matrix3f m, float s)
{
  int i, j;

  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
      m[j][i] *= s;
}


void scaleMatrix(Matrix4f m, float s)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      m[j][i] *= s;
}


void scaleMatrix(Matrix4f m, float s, const Matrix4f m1)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      m[j][i] = s * m1[j][i];
}


void addMatrix(Matrix3f m1, const Matrix3f m2)
{
  int i, j;

  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
      m1[j][i] += m2[j][i];
}


void addMatrix(Matrix4f m1, const Matrix4f m2)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      m1[j][i] += m2[j][i];
}


void addScaledMatrix(Matrix3f m1, const float s, const Matrix3f m2)
{
  int i, j;

  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
      m1[j][i] += s * m2[j][i];
}


void addScaledMatrix(Matrix4f m1, const float s, const Matrix4f m2)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      m1[j][i] += s * m2[j][i];
}


void subMatrix(Matrix3f m1, const Matrix3f m2)
{
  int i, j;

  for (j = 0; j < 3; j++)
    for (i = 0; i < 3; i++)
      m1[j][i] -= m2[j][i];
}


void subMatrix(Matrix4f m1, const Matrix4f m2)
{
  int i, j;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++)
      m1[j][i] -= m2[j][i];
}


void multMatrix(Matrix4f m, const Matrix4f m1, const Matrix4f m2)
{
  int i, j, k;
  float tmp;

  for (j = 0; j < 4; j++)
    for (i = 0; i < 4; i++) {
      tmp = 0.0;
      for (k = 0; k < 4; k++)
        tmp += m1[j][k] * m2[k][i];
      m[j][i] = tmp;
    }
}


void multVector(Vector3f u, const Matrix4f m, const Vector3f v)
{
  int i, j;
  float tmp;

  for (j = 0; j < 3; j++) {
    tmp = 0.0;
    for (i = 0; i < 3; i++)
      tmp += v[i] * m[j][i];
    tmp += m[j][3];
    u[j] = tmp;
  }
}


// Component-wise multiplication
void multVectors(Vector3f res, const Vector3f u, const Vector3f v)
{
  for (int j = 0; j < 3; j++)
		res[j] = u[j] * v[j];
}

// Component-wise multiplication
void multVectors4(Vector4f res, const Vector4f u, const Vector4f v)
{
  for (int j = 0; j < 4; j++)
		res[j] = u[j] * v[j];
}

float length(Vector3f input)
{
  float result = 0;
  for(int i = 0; i<3; i++)
        result += pow(input[i],2);

  result = sqrt(result);
  return result;
}

void negative(Vector3f n, Vector3f input)
{
   for(int i = 0; i<3; i++)
        n[i] = -input[i];
}


void copy(Vector4f n, Vector4f input)
{
   for(int i = 0; i<4; i++)
        n[i] = input[i];
}
