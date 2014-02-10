#pragma once
#include "Modelview.h"
#include <stdio.h>
#include <vector>
#include "helper.h"
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

using namespace Eigen;
class Node{
public:
	ModelView model;
	vector<Node*> children;
	Vector3f V;
	MatrixX3f transformation;
	Vector3f articulation;
	Vector3f pivot;

	Node(void){
		//Should this be an option?
	}

	//Euler Angles
	Node(string modelFilename, Vector3f relativePos, Vector3f piv){
		model.read_file(modelFilename);
		V = relativePos;
		articulation = (0.0, 0.0, 0.0);
		pivot = piv;
	}

	void set(string filename, Vector3f link){
		model.read_file(filename);
		V = link;
	}

	void set(string filename, Vector3f link, Vector3f articulation, Vector3f piv){
		model.read_file(filename);
		V = link;
		this->articulation = articulation;
		this->pivot = piv;
	}

	void draw(){
		model.draw_object(0.0, 1.0, 1.0);
	}

	void setArticulation(float pitch, float yaw, float roll){
		articulation.setx(pitch);
		articulation.sety(yaw);
		articulation.setz(roll);
	}

	void setPivot(Vector3f piv){
		pivot = piv;
	}

	~Node(void){
		//something should go here
	}
};