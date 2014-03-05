#ifndef _VERTEX3_H_
#define _VERTEX3_H_

#pragma once
#include <math.h> 
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <Eigen/Geometry>
using namespace std;
using namespace Eigen;


class vertex3
{
public:
	Vector3f vertex;
	vertex3(void){
		vertex.x() = 0.0f;
		vertex.y() = 0.0f;
		vertex.z() = 0.0f;
	}	
	vertex3 (float value){
		vertex.x() = value;
		vertex.y() = value;
		vertex.z() = value;
	}
	vertex3(float xin, float yin, float zin){
		vertex.x() = xin;
		vertex.y() = yin;
		vertex.z() = zin;
	}

	void zero(void){
		vertex.x() = 0.0f;
		vertex.y() = 0.0f;
		vertex.z() = 0.0f;
	}

	vertex3 toVertex3(const Vector3f vector){
		return vertex3(vector.x(), vector.y(), vector.z());
	}

	float getx (void){
		return vertex.x();
	}

	float gety (void){
		return vertex.y();
	}
	float getz (void){
		return vertex.z();
	}

	void setx (float in){
		vertex.x() = in;
	}

	void sety (float in){
		vertex.y() = in;
	}
	void setz (float in){
		vertex.z() = in;
	}

	void set(float x, float y, float z){
		setx(x); sety(y); setz(z);
	}

	void set(const vertex3 in){
		this->vertex = in.vertex;
	}

	void set(const Vector3f in){
		vertex = in;
	}
	//probably unecessary
	vertex3& getObjectPointer(){
		return *this;
	}/*

	//WILL THIS WOrK??
	float* getPointer(void) {
		float ptr[3];
		ptr[0] = getx();
		ptr[1] = gety();
		ptr[2] = getz();
		return ptr;
	}

	float* getPointer(void){
		return &vertex;
	}

	vertex3 invert(){
		vertex3 retVal;
		retVal.vertex = this->vertex.inverse();
		return retVal;
	}
		*/

	vertex3 operator-(const vertex3& vec){
		vertex3 retVal;
		retVal.vertex = this->vertex - vec.vertex;
		return retVal;
	}

	vertex3 operator+(const vertex3& vec){
		vertex3 retVal;
		retVal.vertex = this->vertex + vec.vertex;
		return retVal;
	}

	vertex3 operator*( float scalar ){
		float X = vertex.x() * scalar;
		float Y = vertex.y() * scalar;
		float Z = vertex.z() * scalar;
		return vertex3(X, Y, Z);
	}

	Vector3f toEigen(const vertex3 nonEigenVector){
		return nonEigenVector.vertex;
	}

	float dotProduct(const vertex3 vec){
		//Return dot product of this * input
		return vertex.dot(vec.vertex);
	}

	vertex3 cross (const vertex3 vec){
		/*
		float X = ((y * vec.z) - (vec.y * z));
		float Y = ((z * vec.x) - (vec.z * x));
		float Z = ((x * vec.y) - (vec.x * y));
		return vertex3(X, Y, Z);
		*/
		Vector3f retVal = vertex.cross(vec.vertex);
		return  toVertex3(retVal);
	}

	void print(){
		printf("----%f  %f  %f\n", this->getx(), this->gety(), this->getz());
	}

	void normalize(void){
		/*
		float mag = sqrt(x*x + y*y + z*z);
		if(mag < .000001) {
			x = 0;
			y = 0;
			z = 0;
			return;
		}
		x = x/mag;
		y = y/mag;
		z = z/mag;
		*/
		vertex.normalize();
	}
/*
	void* toMatrix(void){
		matrix mat(4,1);
		mat(0,0) = this->x;
		mat(1,0) = this->y;
		mat(2,0) = this->z;
		mat(3,0) = 1.0;


		return &mat;
	}

*/
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif