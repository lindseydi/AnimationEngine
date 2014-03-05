
#ifndef _CAMERA_H_
#define _CAMERA_H_


#pragma once
#include "vertex3.h"
#include "matrix4.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	 // Projection

    void project( void );


    // Member Data

    vertex3 position;       // location of the camera

    vertex3 viewpoint;      // point where camera is focused


    vertex3 up;             // the up direction in world frame


    matrix4 view;           // the view matrix

    matrix4 perspective;    // the perspective matrix

    matrix4 projection;     // the projection matrix

    matrix4 projectionInv;  // the inverse projection matrix


    float near;            // distance to the near clipping plane

    float far;             // distance to the far clipping plane
};

#endif