#ifndef _JOINT_H_
#define _JOINT_H_

#pragma once;
#include "vertex3.h"
#include "matrix4.h"
//forward declaration
class Link;

class Joint{
	public:
		vertex3 inboard_displacement;
		vertex3 outboard_displacement;
		Link* child;
		Link* parent;
		PoseEuler local_pose;
		matrix4 local_transformation;
		float angle;	//limits rotation to one degree of freedom
		float angularVelocity;
		matrix4 rotational_frame;

	Joint();
	Joint(const PoseEuler& localPose);

	void addLink(Link* child);
	void translate(vertex3 movement);
	void rotate();

};

#endif