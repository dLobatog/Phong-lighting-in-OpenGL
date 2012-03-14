#ifndef  VIEWMODULE_H
#define  VIEWMODULE_H


#ifdef __cplusplus
extern "C" {
#endif

#define IMAGE_WIDTH  1024
#define IMAGE_HEIGHT 1024

typedef struct{
	float fieldOfView;
	float aspect;
	float nearPlane;
	float farPlane;
}perspectiveData;

#ifdef __cplusplus
}
#endif


#endif
