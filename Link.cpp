#include "Link.h"
#include "Joint.h"

Link::Link(): Model(){
	parent = NULL;
}

Link::Link(ModelView mesh, PoseEuler* pose): Model(mesh, pose){
	//do nothing
}

Link::Link(ModelView mesh, PoseEuler* pose, std::string name): Model(mesh, pose){
	this->name = name;
}

Link::Link(Link& link_copy){
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
		matrix4 inboard = PoseEuler(child->inboard_displacement, vertex3()).translate_object();
		//matrix4 inboard = PoseEuler(vertex3(), vertex3()).translate_object();
		matrix4 outboard = PoseEuler(child->outboard_displacement, vertex3()).translate_object();
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