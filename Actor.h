#ifndef _ACTOR_H_
#define _ACTOR_H_

#pragma once

#include "ModelView.h"
#include "Trajectory.h"

class Actor{
public:
	//Model
	ModelView model;
	Trajectory* path;

	Actor(){
		//use default constructor which should set it to 0,0,0
		model = ModelView();
		path = new Trajectory();
	}

	Actor(Trajectory* trajectory){
		this->path = trajectory;
	}

	Actor(const ModelView& model){
		//This is the joint that the actor is to start in world space
		this->model = ModelView(model);
		this->path = new Trajectory();
	}

	Actor(Trajectory* trajectory, const ModelView& mesh){
		this->path = trajectory;
		this->model = mesh;
	}

	~Actor(){
		//don't have to do anything to model, right?
		delete path;
	}

	virtual Pose& update(){
		return path->update();
	}

	/*
	void addLink(Node prev, Joint connector){
		prev.nextJoint = connector;

		//Everything needed for the Joint information should have already been set
	}
	*/

	void setBasicPath(){
		path = new Trajectory();
	}

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif