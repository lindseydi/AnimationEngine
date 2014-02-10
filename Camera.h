#pragma once
#include <Eigen/Geometry>
#include <Eigen/Dense>

using namespace Eigen;

class Camera
{
public:
	Camera(void);
	~Camera(void);

	 // Projection

    void project( void );


    // Member Data

    Vector3f position;       // location of the camera

    Vector3f viewpoint;      // point where camera is focused


    Vector3f up;             // the up direction in world frame


    MatrixX4f view;           // the view matrix

    MatrixX4f perspective;    // the perspective matrix

    MatrixX4f projection;     // the projection matrix

    MatrixX4f projectionInv;  // the inverse projection matrix


    double near;            // distance to the near clipping plane

    double far;             // distance to the far clipping plane
};
