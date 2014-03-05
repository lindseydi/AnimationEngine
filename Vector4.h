
#ifndef _VECTOR4_H_
#define _VECTOR4_H_


#pragma once
#include <math.h> 
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <Eigen/Geometry>


#include "matrix4.h"
using namespace std;
using namespace Eigen;


class Vector4
{
public:
	Vector4f vector;
	Vector4(void){
		vector.x() = 0.0f;
		vector.y() = 0.0f;
		vector.z() = 0.0f;
		vector.w() = 1.0f;
	}	
	Vector4 (float value){
		vector.x() = value;
		vector.y() = value;
		vector.z() = value;
		vector.w() = value;
	}
	Vector4(float xin, float yin, float zin, float win){
		vector.x() = xin;
		vector.y() = yin;
		vector.z() = zin;
		vector.w() = win;
	}

	void zero(void){
		vector.x() = 0.0f;
		vector.y() = 0.0f;
		vector.z() = 0.0f;
		vector.w() = 0.0f;
	}

	float getx (void){
		return vector.x();
	}

	float gety (void){
		return vector.y();
	}
	float getz (void){
		return vector.z();
	}

	float getw (void){
		return vector.w();
	}

	void setx (float in){
		vector.x()= in;
	}

	void sety (float in){
		vector.y()=in;
	}
	void setz (float in){
		vector.z()=in;
	}

	void setw (float in){
		vector.w() = in;
	}

	void set(float x, float y, float z, float w){
		setx(x); sety(y); setz(z); setw(w);
	}

	void set(const Vector4& in){
		this->vector = in.vector;
	}

	void operator=(const Vector4& vec){
		this->set(vec.vector);
	}

	Vector4 operator-(const Vector4& vec){
		Vector4 retVal;
		retVal.vector = retVal.vector - vec.vector;
		return retVal;
	}

	Vector4 operator+(const Vector4& vec){
		Vector4 retVal;
		retVal.vector = retVal.vector + vec.vector;
		return retVal;
	}

	Vector4 operator*( float scalar ){
		Vector4 retVal;
		retVal.vector = retVal.vector * scalar;
		return retVal;
	}

	//should this be a pointer?
	//4x1 * 4x4... this should not work!

	float& operator()(unsigned int i){
		return vector[i];
	}

	float dotProduct(const Vector4& vec){
		//Return dot product of this * input
		return vector.dot(vec.vector);
	}

	/* Do we ever use this?
	Vector4 cross (const Vector4& vec){

		float X = ((y * vec.z) - (vec.y * z));
		float Y = ((z * vec.x) - (vec.z * x));
		float Z = ((x * vec.y) - (vec.x * y));
		return Vector4(X, Y, Z);

		Vector4 retVal;
		retVal.vector = vector.cross(vec.vector);
		return retVal;
	}
	*/
	/*
	//Is this how this was supposed to work?
	float& element(int i){
		switch(i){
			case 0:
				return *vector.x;
			break;
			case 1:
				return *vector.y;
			break;
			case 2:
				return *vector.z;
			break;
			case 3:
				return *vector.w;
			break;
		}		
	}
	*/

	void print(){
		printf("----%f  %f  %f  %f\n", this->getx(), this->gety(), this->getz(), this->getw());
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
		vector.normalize();
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

	private:
		void set(const Vector4f& in){
			vector = in;
		}
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif