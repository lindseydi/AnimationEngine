#pragma once;
#include "Node.h" //neccesary??
#include "Joint.h"

class Actor{
public:
	Joint head;			//Not a physical head, but rather the top of the hierarchy

	Actor(){
		//use default constructor which should set it to 0,0,0
		head = Joint();
	}

	Actor(Joint root){
		//This is the joint that the actor is to start in world space
		head = root;
	}

	void addLink(Node prev, Joint connector){
		prev.nextJoint = connector;

		//Everything needed for the Joint information should have already been set
	}

	void drawRecursively(){
		//draw the actor at the timestep?

	}

};