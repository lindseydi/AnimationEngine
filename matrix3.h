#ifndef _MATRIX3_H_
#define _MATRIX3_H_


#pragma once
#include <stdlib.h>
#include <GL/glut.h>
#include <Eigen/Geometry>
using namespace Eigen;

class matrix3{
public:
	//Contains an Eigen Matrix in order to handle math.
	Matrix3f matrix;
	//Constructors
	void zero(){
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				this->matrix(i, j) = 0.0f;
			}
		}
	}

	matrix3(){
		matrix = Matrix3f();
		matrix.Zero();
	}

	matrix3(const matrix3& mat){
		matrix = Matrix3f(mat.matrix);
	}
	//Operators
	float operator()(int x, int y){
		//allows access to a certain row/col of the matrix
		return matrix(x, y);
	}

	matrix3& operator=(const matrix3& input){
		matrix = input.matrix;
		return *this;
	}

	matrix3 operator+(const matrix3& input){
		matrix3 retVal;
		retVal.matrix= matrix + input.matrix;
		return retVal;
	}

	matrix3& operator+=(const matrix3& input){
		matrix += input.matrix;
		return *this;
	}

	//Is this the correct direction?
	matrix3 operator*(double scalar){
		matrix3 retVal;
		retVal.matrix = matrix * scalar;
		return retVal;
	}

	//other funcs
	matrix3 transpose(){
		matrix.transpose();
	}

	void identity(){
		this->matrix.Identity();
	}

	void print(){
		for(int i=0; i<3; ++i){
			for(int j=0; j<3; ++j){
				printf("%f ", matrix(i, j));
			}
		printf("\n");
		}
	}

	//Will this work?
	float* getPtr(){
		return &matrix(0,0);
	}
	//Only necessary for 4x4 matrix
	//GLfloat* getGLPtr();

};

#endif