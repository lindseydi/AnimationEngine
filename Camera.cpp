#include "stdafx.h"
#include "Camera.h"


Camera::Camera(void)
{
	position = Vector3f( 0.0, 0.0, 1.0 );

    viewpoint = Vector3f( 0.0, 0.0, 0.0 );

    up = Vector3f( 0.0, 1.0, 0.0 );


    view.identity();

    projection.identity();


    near = 1.0;

    far = 1000.0;
}


Camera::~Camera(void)
{
}

//------------------------------------------------------------------------------
// Projection

//------------------------------------------------------------------------------

/// Determine the camera projection matrix
void Camera::project( void ) {

    Matrix4 T, R;

    Vector3f U, V, N;

    Vector3f Vprime;


    Vprime = up;


    N = viewpoint - position;

    N.normalize();


    V = Vprime - ( N * Vector3f::dot( Vprime, N) );

    V.normalize();

    //up = V;

    U = Vector3f::cross( N, V );

    U.normalize();


    Vector3f Vinv = Vector3f::invert( position );


    T.identity();

    T.setTranslation( Vinv.x(), Vinv.y(), Vinv.z() );


    R.identity();

    R(0,0) = U.x();     R(0,1) = U.y();     R(0,2) = U.z();

    R(1,0) = V.x();     R(1,1) = V.y();     R(1,2) = V.z();

    R(2,0) = N.x();     R(2,1) = N.y();     R(2,2) = N.z();


    view = R * T;


    double h = 0.75;

    double d = near;

    double f = far;


    perspective.identity();

    perspective(0,0) = d / h;

    perspective(1,1) = d / h;

    perspective(2,2) = f / (f-d);

    perspective(2,3) = -d * f / (f-d);

    perspective(3,2) = 1.0;

    perspective(3,3) = 0.0;


    projection = perspective * view;

    projectionInv = Matrix4::inverse( projection );

}

