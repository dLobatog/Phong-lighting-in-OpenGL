#ifndef GEOMETRY_H
#define GEOMETRY_H

#ifdef WIN32
  #define M_PI 3.14159265359
#endif

typedef float Vector3f[3];
typedef double Vector3d[3];

typedef float Vector4f[4];
typedef double Vector4d[4];

typedef float Matrix3f[3][3];
typedef double Matrix3d[3][3];

typedef float Matrix4f[4][4];
typedef double Matrix4d[4][4];


void add(Vector3f sum, const Vector3f v);
void add(Vector3f sum, const Vector3f v1, const Vector3f v2);
void add(Vector3d sum, const Vector3d v1, const Vector3d v2);
void add4(Vector4f sum, const Vector4f v1, const Vector4f v2);


void sub(Vector3f diff, const Vector3f v);
void sub(Vector3f diff, const Vector3f v1, const Vector3f v2);
void sub(Vector3d diff, const Vector3d v1, const Vector3d v2);

void scale(Vector3f mul, float s);
void scale4(Vector4f mul, float s);
void scale(Vector3f mul, float s, const Vector3f v1);
void scale(Vector3d mul, double s, const Vector3d v1);
void scale4(Vector4f mul, float s, const Vector4f v1);


float dotProd(const Vector3f a, const Vector3f b);
double dotProd(const Vector3d a, const Vector3d b);

void vecProd(Vector3f n, const Vector3f u, const Vector3f v);
void vecProd(Vector3d n, const Vector3d u, const Vector3d v);

float normalize(Vector3f n);
double normalize(Vector3d n);

void normalizeVector(Vector3f n, Vector3f input);

void normal(Vector3f n, const Vector3f p, const Vector3f q, const Vector3f r);
void normal(Vector3d n, const Vector3d p, const Vector3d q, const Vector3d r);


void emptyMatrix(Matrix3f m);
void emptyMatrix(Matrix4f m);

void setRowVectors(Matrix4f m, const Vector3f u, const Vector3f v, const Vector3f w);
void setColVectors(Matrix4f m, const Vector3f u, const Vector3f v, const Vector3f w);

void transpose(Matrix4f m, const Matrix4f m1);

void scaleMatrix(Matrix3f m, float s);

void scaleMatrix(Matrix4f m, float s);
void scaleMatrix(Matrix4f m, float s, const Matrix4f m1);

void addMatrix(Matrix3f m1, const Matrix3f m2);
void addMatrix(Matrix4f m1, const Matrix4f m2);

void addScaledMatrix(Matrix3f m1, const float s, const Matrix3f m2);
void addScaledMatrix(Matrix4f m1, const float s, const Matrix4f m2);

void subMatrix(Matrix3f m1, const Matrix3f m2);
void subMatrix(Matrix4f m1, const Matrix4f m2);

// m1 is left, m2 is right matrix, multiplication from right-to-left
void multMatrix(Matrix4f m, const Matrix4f m1, const Matrix4f m2);

// apply 4x4 matrix to 3D vector
void multVector(Vector3f u, const Matrix4f m, const Vector3f v);

// Component-wise multiplication
void multVectors(Vector3f res, const Vector3f u, const Vector3f v);
void multVectors4(Vector4f res, const Vector4f u, const Vector4f v);

float length(Vector3f input);
float length4(Vector4f input);

void negative(Vector3f n, Vector3f input);
void copy(Vector4f n, Vector4f input);


#endif
