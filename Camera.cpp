#include "stdafx.h"
#include "Camera.h"


Camera::Camera(void)
{
	position = vertex3( 0.0, 0.0, 1.0 );

    viewpoint = vertex3( 0.0, 0.0, 0.0 );

    up = vertex3( 0.0, 1.0, 0.0 );


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

    matrix4 T, R;

    vertex3 U, V, N;

    vertex3 Vprime;


    Vprime = up;


    N = viewpoint - position;

    N.normalize();


    V = Vprime - ( N * Vprime.dotProduct(N) );

    V.normalize();

    //up = V;

    U = N.cross( V );

    U.normalize();


    vertex3 Vinv = position.invert();


    T.identity();

	//TODO what is this supposed to do?
    //T.setTranslation( Vinv.x(), Vinv.y(), Vinv.z() );


    R.identity();

    R(0,0) = U.getx();     R(0,1) = U.gety();     R(0,2) = U.getz();

    R(1,0) = V.getx();     R(1,1) = V.gety();     R(1,2) = V.getz();

    R(2,0) = N.getx();     R(2,1) = N.gety();     R(2,2) = N.getz();


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

    projectionInv = projection.invert();

}

