#pragma once
#include <stdlib.h>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;


class helper{
public:

	static matrix QuatToMatrix(){

	}
	/*
static matrix translate_object(vertex3 pos){
	matrix translate(4, 4);
	for(int i=0; i<4; i++){
		translate(i, i) = 1.0;
	}
	translate(0, 3) = pos.getx();
	translate(1, 3) = pos.gety();
	translate(2, 3) = pos.getz();


	return translate;
	//float m[ 16 ];
	//translate.transpose().ToArray( m );
	//glMultMatrixf( m );
}

static matrix translate_object(float x, float y, float z){
	matrix translate(4, 4);
	for(int i=0; i<4; i++){
		translate(i, i) = 1.0;
	}
	translate(0, 3) = x;
	translate(1, 3) = y;
	translate(2, 3) = z;


	return translate;
	//float m[ 16 ];
	//translate.transpose().ToArray( m );
	//glMultMatrixf( m );
}

static matrix scale_object(float x, float y, float z){
	matrix scale(4, 4);
	scale(0, 0) = x;
	scale(1, 1) = y;
	scale(2, 2) = z;
	scale(3,3) = 1.0;


	return scale;
	//float m[16];
	//scale.transpose().ToArray(m);
	//glMultMatrixf(m);
}

static matrix rotate_object(float pitch, float yaw, float roll){
	matrix rotatePitch(4, 4);
	matrix rotateYaw(4, 4);
	matrix rotateRoll(4,4);
	matrix product(4,4);

	rotatePitch(0, 0) = 1;
	rotatePitch(1, 1) = cos((PI/180.0)*pitch);
	rotatePitch(1, 2) = -sin((PI/180.0)*pitch);
	rotatePitch(2, 1) = sin((PI/180.0)*pitch);
	rotatePitch(2, 2) = cos((PI/180.0)*pitch);
	rotatePitch(3, 3) = 1.0;


	rotateYaw(0, 0) = cos((PI/180.0)*yaw);
	rotateYaw(0, 2) = sin((PI/180.0)*yaw);
	rotateYaw(1, 1) = 1.0;
	rotateYaw(2, 0) = -sin((PI/180.0)*yaw);
	rotateYaw(2, 2) = cos((PI/180.0)*yaw);
	rotateYaw(3, 3) = 1.0;

	rotateRoll(0, 0) = cos((PI/180.0)*roll);
	rotateRoll(0, 1) = -sin((PI/180.0)*roll);
	rotateRoll(1, 0) = sin((PI/180.0)*roll);
	rotateRoll(1, 1) = cos((PI/180.0)*roll);
	rotateRoll(2, 2) = 1.0;
	rotateRoll(3, 3) = 1.0;

	product = rotatePitch * rotateYaw * rotateRoll;

	return product;
	//float m[16];
	//product.transpose().ToArray(m);
	//glMultMatrixf(m);
}

static matrix pivot_object(float pitch, float yaw, float roll, vertex3 pivotPoint){
	matrix t;
	t = translate_object(pivotPoint);
	t = t * rotate_object(pitch, yaw, roll);
	t = t * translate_object(-pivotPoint.getx(), -pivotPoint.gety(), -pivotPoint.getz());
	return t;
}
*/

};