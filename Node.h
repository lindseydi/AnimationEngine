#pragma once
#include "Modelview.h"
#include "Joint.h"
#include <stdio.h>
#include <vector>
#include "helper.h"
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;
class Node{
public:
	ModelView model;		//holds geometry of Node 
	//Once set, these should never change
	//Changes should be taken care of at the joint
	Vector3f translation;	//local	 and thus relative to previous node
	Vector3f articulation;	//local and thus relative to previous node
	Joint nextJoint;
	MatrixX4f transformation;

	Node(void){
		//Should this be an option?

	}

	//Euler Angles
	Node(string modelFilename, Vector3f translation, Vector3f articulation){
		model.read_file(modelFilename);
		this->translation = translation;
		this->articulation = articulation;
	}

	void draw(){
		model.draw_object(0.0, 1.0, 1.0);
	}

	void setArticulation(float pitch, float yaw, float roll){
		articulation.x = pitch;
		articulation.y = yaw;
		articulation.z = roll;
	}

	void setArticulation(Vector3f articulation){
		this->articulation = articulation;
	}

	void translate_object(Vector3f pos){
		MatrixX4f translate;
		for(int i=0; i<4; i++){
			translate(i, i) = 1.0;
		}
		translate(0, 3) = pos.x;
		translate(1, 3) = pos.y;
		translate(2, 3) = pos.z;


		transformation *= translate;
		//float m[ 16 ];
		//translate.transpose().ToArray( m );
		//glMultMatrixf( m );
	}

	void translate_object(float x, float y, float z){
		MatrixX4f translate;
		for(int i=0; i<4; i++){
			translate(i, i) = 1.0;
		}
		translate(0, 3) = x;
		translate(1, 3) = y;
		translate(2, 3) = z;


		transformation *= translate;
		//float m[ 16 ];
		//translate.transpose().ToArray( m );
		//glMultMatrixf( m );
	}

	void scale_object(float x, float y, float z){
		MatrixX4f scale(4, 4);
		scale(0, 0) = x;
		scale(1, 1) = y;
		scale(2, 2) = z;
		scale(3,3) = 1.0;


		transformation *= scale;
		//float m[16];
		//scale.transpose().ToArray(m);
		//glMultMatrixf(m);
	}

	 void rotateObject(){
		 //TODO
		 //If this doesn't use quaternions why does it work?
		MatrixX4f rotatePitch;
		MatrixX4f rotateYaw;
		MatrixX4f rotateRoll;
		MatrixX4f product;

		rotatePitch(0, 0) = 1;
		rotatePitch(1, 1) = cos((PI/180.0)*articulation.x);
		rotatePitch(1, 2) = -sin((PI/180.0)*articulation.x);
		rotatePitch(2, 1) = sin((PI/180.0)*articulation.x);
		rotatePitch(2, 2) = cos((PI/180.0)*articulation.x);
		rotatePitch(3, 3) = 1.0;


		rotateYaw(0, 0) = cos((PI/180.0)*articulation.y);
		rotateYaw(0, 2) = sin((PI/180.0)*articulation.y);
		rotateYaw(1, 1) = 1.0;
		rotateYaw(2, 0) = -sin((PI/180.0)*articulation.y);
		rotateYaw(2, 2) = cos((PI/180.0)*articulation.y);
		rotateYaw(3, 3) = 1.0;

		rotateRoll(0, 0) = cos((PI/180.0)*articulation.z);
		rotateRoll(0, 1) = -sin((PI/180.0)*articulation.z);
		rotateRoll(1, 0) = sin((PI/180.0)*articulation.z);
		rotateRoll(1, 1) = cos((PI/180.0)*articulation.z);
		rotateRoll(2, 2) = 1.0;
		rotateRoll(3, 3) = 1.0;

		product = rotatePitch * rotateYaw * rotateRoll;

		transformation *= product;
		//float m[16];
		//product.transpose().ToArray(m);
		//glMultMatrixf(m);
	}

	void setTransformation(){
		//How to do this?
		//float m[16];
		//product.transpose().ToArray(m);
		//glMultMatrixf(m);
	}

	~Node(void){
		//something should go here
	}
};