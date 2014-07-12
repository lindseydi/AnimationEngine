#include "Link.h"
#include "Joint.h"

Joint::Joint(){
	inboard_displacement = vertex3();
	outboard_displacement = vertex3();
	angle = 0.0f;
	child = NULL;
	parent = NULL;
	angularVelocity = 0.00001f;
	path = new Trajectory();
}

Joint::Joint(const Pose& localPose){
	inboard_displacement = vertex3();
	outboard_displacement = vertex3();
	angle = 0.0f;
	child = NULL;
	parent = NULL;
	this->local_pose = localPose;
	this->local_transformation = local_pose.getRotation() * local_pose.translate_object();
	angularVelocity = 0.00001f;
	path = new Trajectory();
}

void Joint::addLink(Link* child){
	child->parent = this;
	this->child = child;
	printf("Pointer has been set\n");
}

void Joint::translate(const vertex3& movement){
	//how much to move, or where to move to?
	//simply add how much it moves to it's pose

	Pose temp = Pose(movement, vertex3());
	local_transformation = local_transformation * temp.translate_object();
	//local_pose.position = local_pose.position + movement;
}

void Joint::rotate(){
	//not sure if this is how it works...
	//local_pose.orientation = local_pose.orientation + rotate;
	//PoseEuler temp = PoseEuler(vertex3(), vertex3(0.0, 0.0, angle));
//	local_transformation = local_transformation * ( temp.getRotation()* angularVelocity);
	
	Pose currentPose = path->update();

	float x, y;
	x = currentPose.position.getx();
	y = currentPose.position.gety();
	angle = -PI/2.0 + (float) (std::atan2(y, x) - std::atan2(1.0, 0.0));


	//REALLY MEANS: angle = angularVelocity * dt + angle0;
	//angle += angularVelocity;
	//std::cout << "x:" << x << ", y:" << y << ", theta:" << angle << std::endl;
	Pose temp = Pose(vertex3(), quaternion(angle, 0.0, 0.0));
	//Pose temp = Pose(vertex3(), quaternion(0.0, angle, 0.0));
	local_transformation = temp.getRotation();

	//local_transformation = ( temp.getRotation() * angularVelocity);

	//if(!isNull()){
	//	for(unsigned int i=0; i<children.size(); i++){
	//		this->children.at(i)->child->rotate(rotate);
	//	}
	//}
}

void Joint::addPointOnPath(PoseKey* controlPoint){
	PoseKey* temp = controlPoint;
	path->controlPoints.push_back(controlPoint);
}