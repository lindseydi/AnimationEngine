#ifndef _JOINT_H_
#define _JOINT_H_

//#include "Actor.h"
#include "vertex3.h"
#include "matrix4.h"
#include "Trajectory.h"
//forward declaration
class Link;

class Joint{
	public:
		vertex3 inboard_displacement;
		vertex3 outboard_displacement;
		Link* child;
		Link* parent;
		Pose local_pose;
		matrix4 local_transformation;
		float angle;	//limits rotation to one degree of freedom
		float angularVelocity;
		matrix4 rotational_frame;
		Trajectory* path;

	Joint();
	Joint(const Pose& localPose);

	void addLink(Link* child);
	void translate(vertex3 movement);
	void rotate();
	void addPointOnPath(PoseKey* controlPoint);

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif