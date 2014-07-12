#include "Link.h"
#include "Joint.h"

Link::Link(): Model(){
	parent = NULL;
}

Link::Link(const ModelView& mesh, Pose* pose): Model(mesh, pose){
	//do nothing
}

Link::Link(const ModelView& mesh, Pose* pose, std::string name): Model(mesh, pose){
	this->name = name;
}

Link::Link(const Link& link_copy){
	for(unsigned int i=0; i<link_copy.children.size(); i++)
		this->children.push_back(link_copy.children.at(i));
	this->parent = link_copy.parent;
	this->mesh = link_copy.mesh;
	this->pose = link_copy.pose;
}

void Link::addChild(Joint* joint){
	joint->parent = this;
	children.push_back(joint);
}

void Link::update(){
	for(unsigned int i=0; i<children.size(); i++){
		Link* child_link;

		Joint* child = this->children.at(i);
		child_link = child->child;

		matrix4 previousLinkTransform = transform;
		matrix4 inboard = Pose(child->inboard_displacement, vertex3()).translate_object();
		//matrix4 inboard = PoseEuler(vertex3(), vertex3()).translate_object();
		matrix4 outboard = Pose(child->outboard_displacement, vertex3()).translate_object();
		//matrix4 outboard = PoseEuler(vertex3(), vertex3()).translate_object();
		//what does this mean in terms of rotation?
		child->rotate();
		matrix4 J1 = child->rotational_frame * child->local_transformation;
		matrix4 F2 = previousLinkTransform * inboard * J1 * outboard;
		
		//cout << "After Calculations:" << endl << F2.matrix << endl;
		//update for next time step, so you can just draw!
		child_link->transform = F2;

		child_link->update();
	}
}

bool Link::isNull(){
	if(this->children.size() == 0)
		return true;
	else
		return false;
}

//------------------------------------------------------------------------------------//
//----------------------------------Link Root---------------------------------------//

LinkRoot::LinkRoot(): Link(){
	path = new Trajectory(0);	//sets cycle to type circle	
	//1 is pendulum
	//2 is clamp
	generatePath();
}
	

LinkRoot::LinkRoot(const ModelView& mesh, Pose* pose): Link(mesh, pose){
	path = new Trajectory(1);
	generatePath();
}

LinkRoot::LinkRoot(const ModelView& mesh, Pose* pose, std::string name): Link(mesh, pose, name){
	path = new Trajectory(1);
	generatePath();
}

void LinkRoot::update(){
	pose =  &path->update();
	transform = pose->translate_object() * pose->getRotation();
	__super::update();
}

//TODO make a better path for it to follow!
void LinkRoot::generatePath(){
	path->resetPath();
	//attempts to create circular walk
	path->controlPoints.push_back(new PoseKey(vertex3(0.0, 0.0, 1.0)*5.0, vertex3((PI/2.0)+PI/2.0, 0.0, PI/2.0), 0));
	path->controlPoints.push_back(new PoseKey(vertex3(-sqrt(2.0)/2.0, 0.0, sqrt(2.0)/2.0) * 5.0, vertex3((3.0*PI/4.0)+PI/2.0, 0.0, PI/2.0), 120));
	path->controlPoints.push_back(new PoseKey(vertex3(-1.0, 0.0, 0.0) * 5.0, vertex3((PI)+PI/2.0, 0.0, PI/2.0), 240));
	path->controlPoints.push_back(new PoseKey(vertex3(-sqrt(2.0)/2.0, 0.0, -sqrt(2.0)/2.0) * 5.0, vertex3((5.0*PI/4.0)+PI/2.0, 0.0, PI/2.0), 360));
	path->controlPoints.push_back(new PoseKey(vertex3(0.0, 0.0, -1.0) * 5.0, vertex3((3.0*PI/2.0)+PI/2.0, 0.0, PI/2.0), 480));
	path->controlPoints.push_back(new PoseKey(vertex3((sqrt(2.0)/2.0), 0.0, (-sqrt(2.0)/2.0)) * 5.0, vertex3((7.0*PI/4.0)+PI/2.0, 0.0, PI/2.0), 600));
	//path->controlPoints.push_back(new PoseKey(vertex3(1.0, 0.0, 0.0)* 5.0, vertex3(PI/2.0, 0.0, PI/2.0), 720));
	//path->controlPoints.push_back(new PoseKey(vertex3(sqrt(2.0)/2.0, 0.0, sqrt(2.0)/2.0)* 5.0, vertex3((PI/4.0)+PI/2.0, 0.0, PI/2.0), 840));

	/*
	//-----------------------------------------------------------------------------------------------//
	path->controlPoints.push_back(new PoseKey(0.0, 0.0, 1.0, 0.0, PI/2.0, PI/2.0, 0));
	path->controlPoints.push_back(new PoseKey(-sqrt(2.0)/2.0, 0.0, sqrt(2.0)/2.0, 0.0, 3.0*PI/4.0,  PI/2.0, 120));
	path->controlPoints.push_back(new PoseKey(-1.0, 0.0, 0.0, PI, 0.0, PI/2.0, 240));
	path->controlPoints.push_back(new PoseKey(-sqrt(2.0)/2.0, 0.0, -sqrt(2.0)/2.0, 0.0, 5.0*PI/4.0,  PI/2.0, 360));
	path->controlPoints.push_back(new PoseKey(0.0, 0.0, -1.0, 0.0, 3.0*PI/2.0, 3.0*PI/2.0, 480));
	path->controlPoints.push_back(new PoseKey((sqrt(2.0)/2.0), 0.0, (-sqrt(2.0)/2.0),  0.0, 7.0*PI/4.0, PI/2.0, 600));
	path->controlPoints.push_back(new PoseKey(1.0, 0.0, 0.0, 0.0, 0.0, PI/2.0, 720));
	path->controlPoints.push_back(new PoseKey(sqrt(2.0)/2.0, 0.0, sqrt(2.0)/2.0, 0.0, PI/4.0, PI/2.0, 840));
	*/
	//path->controlPoints.push_back(new PoseKey(0.25, 0.0, 0.5, 0.0, 0.0, PI/2.0, 80));
	//path->controlPoints.push_back(new PoseKey(0.5, 0.0, 1.0,  PI/2.0, 0.0, PI/2.0, 150));
	//path->controlPoints.push_back(new PoseKey(0.7, 0.0, 0.6,  PI, 0.0, PI/2.0, 180));
	path->init();
}


