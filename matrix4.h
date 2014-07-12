
#ifndef _MATRIX4_H_
#define _MATRIX4_H_


#pragma once
#include <stdlib.h>
#include <GL/glut.h>

#include "Vector4.h"

#include <Eigen/Geometry>

class matrix4{
public:
	//Contains an Eigen Matrix in order to handle math.
	Eigen::Matrix4f matrix;
	//Constructors

	void zero(){
		for(unsigned int i=0; i<4; ++i){
			for(unsigned int j=0; j<4; ++j){
				this->matrix(i, j) = 0.0f;
			}
		}
	}

	matrix4(){
		this->matrix = Eigen::Matrix4f();
		this->zero();
		//matrix.Zero();
	}

	matrix4(float number){
		//does nothing with the float at all.
		this->matrix = Eigen::Matrix4f();
		this->identity();
		//matrix.Zero();
	}

	matrix4(float a, float b, float c, float d, 
			float aa, float bb, float cc, float dd,
			float aaa, float bbb, float ccc, float ddd,
			float aaaa, float bbbb, float cccc, float dddd){
				this->matrix = Eigen::Matrix4f();
				matrix(0, 0) = a;
				matrix(0, 1) = b;
				matrix(0, 2) = c;
				matrix(0,3) = d;
				matrix(1, 0) = aa;
				matrix(1, 1) = bb;
				matrix(1, 2) = cc;
				matrix(1, 3) = dd;
				matrix(2, 0) = aaa;
				matrix(2, 1) = bbb;
				matrix(2, 2) = ccc;
				matrix(2, 3) = ddd;
				matrix(3, 0) = aaaa;
				matrix(3, 1) = bbbb;
				matrix(3, 2) = cccc;
				matrix(3, 3) = dddd;
	}

	matrix4(const matrix4& mat){
		matrix = Eigen::Matrix4f(mat.matrix);
	}

	//Operators
	float& operator()(int x, int y){
		//allows access to a certain row/col of the matrix
		return matrix(x, y);
	}

	matrix4& operator=(const matrix4& input){
		matrix = input.matrix;
		return *this;
	}

	matrix4& operator+(const matrix4& input){
		matrix4* retVal;
		retVal->matrix= matrix + input.matrix;
		return *retVal;
	}

	matrix4& operator+=(const matrix4& input){
		matrix += input.matrix;
		return *this;
	}

	void identity(){
		//TODO check that this will set teh matrix to it's identity
		this->matrix = this->matrix.Identity();
	}

	//Is this the correct direction?
	matrix4 operator*(double scalar){
		matrix4 retVal;
		retVal.matrix = matrix * scalar;
		return retVal;
	}

	matrix4 operator*(const matrix4& other){
		matrix4 retVal  = matrix4();
		retVal.matrix = matrix * other.matrix;
		return retVal;
	}

	Vector4& operator*(const Vector4& vec){
		Vector4* retVal;
		retVal->vector = this->matrix * vec.vector;
		return *retVal;
	}

	//other funcs
	matrix4 transpose() const{
		matrix4 transpose;
		transpose.matrix = matrix.transpose();
		return transpose;
	}

	void transposeInPlace(){
		this->matrix.transposeInPlace();
	}

	matrix4 invert(){
		matrix4 retVal;
		retVal.matrix = matrix.inverse();
		return retVal;
	}

	void print(){
		for(unsigned int i=0; i<4; ++i){
			for(unsigned int j=0; j<4; ++j){
				printf("%f ", matrix(i, j));
			}
		printf("\n");
		}
	}

	//Will this work?
	float* getPtr(){
		return &matrix(0,0);
	}

	//Will this work?
	GLfloat* getGLPtr(){
		GLfloat* ret = new GLfloat[16];
		for(unsigned int i=0; i<4; ++i){
			for(unsigned int j=0; j<4; ++j){
				ret[i*j] = (GLfloat)this->matrix(i, j);
			}
		}
		return ret;
	}

	// return an array that openGL can understand
	void ToArray(float *m) {
		for ( int i = 0; i < 4; ++i ) {
			for ( int j = 0; j < 4; ++j ) {
				m[ i * 4 + j ] = this->matrix( i, j);
			}
		}
	}

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif

#ifndef _MATRIX1X4_H_
#define _MATRIX1x4_H_

class matrix1x4 {
	public:
		Eigen::RowVector4f matrix;

	matrix1x4(){
		matrix = Eigen::RowVector4f();
	}

	matrix1x4(const matrix1x4& mat){
		matrix = Eigen::RowVector4f(mat.matrix);
	}
/*
    // TODO: double check that this method is correct
	//Operators
	float& operator()(int index){
		//allows access to a certain row/col of the matrix
		return matrix(index, 0);
	}
*/
	float& operator()(int index){
		return matrix(0, index);
	}

	matrix1x4& operator=(const matrix1x4& input){
		matrix = input.matrix;
		return *this;
	}

	matrix1x4& operator+(const matrix1x4& input){
		matrix1x4* retVal;
		retVal->matrix= matrix + input.matrix;
		return *retVal;
	}


	void identity(){
		//TODO check that this will set teh matrix to it's identity
		this->matrix.Identity();
	}

	//Is this the correct direction?
	matrix1x4 operator*(double scalar){
		matrix1x4 retVal;
		retVal.matrix = matrix * scalar;
		return retVal;
	}

	//TEST
	// 1x4 * 4x4 = 1x4- thus return a 1x4
	matrix1x4 operator*(const matrix4& other){
		matrix1x4 retVal;
		retVal.matrix = matrix * other.matrix;
		return retVal;
	}

	//TEST
	//1x4 * 4x1 = 1x1 -	thus return a scalar
	float operator*(const Vector4& vec){
		float retVal;
		retVal = (this->matrix * vec.vector)(0, 0);			//get the 0, 0 term from the matrix tODO
		return retVal;
	}

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif