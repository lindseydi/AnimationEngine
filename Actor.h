
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
		model.loadBox(0.3, 0.3, 0.3);	//delete later
	}

	Actor(ModelView model){
		//This is the joint that the actor is to start in world space
		this->model = model;
		path = new Trajectory();
	}

	~Actor(){
		//don't have to do anything to model, right?
		delete path;
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

};

#endif