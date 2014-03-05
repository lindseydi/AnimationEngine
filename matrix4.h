
#ifndef _MATRIX4_H_
#define _MATRIX4_H_


#pragma once
#include <stdlib.h>
#include <GL/glut.h>

#include "Vector4.h"

#include <Eigen/Geometry>
using namespace Eigen;

class matrix4{
public:
	//Contains an Eigen Matrix in order to handle math.
	Matrix4f matrix;
	//Constructors

	void zero(){
		for(int i=0; i<4; i++){
			for(int j=0; j<4; j++){
				this->matrix(i, j) = 0.0f;
			}
		}
	}

	matrix4(){
		matrix = Matrix4f();
		this->zero();
		//matrix.Zero();
	}

	matrix4(const matrix4& mat){
		matrix = Matrix4f(mat.matrix);
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
		this->matrix.Identity();
	}

	//Is this the correct direction?
	matrix4 operator*(double scalar){
		matrix4 retVal;
		retVal.matrix = matrix * scalar;
		return retVal;
	}

	matrix4 operator*(const matrix4& other){
		matrix4 retVal;
		retVal.matrix = matrix * other.matrix;
		return retVal;
	}

	Vector4& operator*(const Vector4& vec){
		Vector4* retVal;
		retVal->vector = this->matrix * vec.vector;
		return *retVal;
	}

	//other funcs
	matrix4 transpose(){
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
		for(int i=0; i<4; ++i){
			for(int j=0; j<4; ++j){
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
		for(int i=0; i<4; ++i){
			for(int j=0; j<4; ++j){
				ret[i*j] = (GLfloat)this->matrix(i, j);
			}
		}
		return ret;
	}

	// return an array that openGL can understand
	void ToArray(float *m) {
		for ( int i = 0; i < 4; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				m[ i * 4 + j ] = this->matrix( i, j);
			}
		}
	}

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

class matrix1x4 {
	public:
		RowVector4f matrix;

	matrix1x4(){
		matrix = RowVector4f();
	}

	matrix1x4(const matrix1x4& mat){
		matrix = RowVector4f(mat.matrix);
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