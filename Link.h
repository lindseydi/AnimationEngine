#ifndef _LINK_H_
#define _LINK_H_


#include "Pose.h"
#include <vector>
#include "Model.h"
#include <string>

//forward declaration
class Joint;

class Link: public Model{
	public:
		//vertex3 cog; center of gravity
		std::vector<Joint*> children;
		Joint *parent;		
		std::string name;

		Link();
		
		Link(ModelView mesh, PoseEuler* pose);

		Link(ModelView mesh, PoseEuler* pose, std::string name);

		Link(Link& link_copy);

		void addChild(Joint* joint);

		void translate(vertex3 movement);

		void rotate(vertex3 rotate);

		void update();

		bool isNull();
};

#endif