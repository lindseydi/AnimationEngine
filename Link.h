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
		
		Link(ModelView mesh, Pose* pose);

		Link(ModelView mesh, Pose* pose, std::string name);

		Link(Link& link_copy);

		void addChild(Joint* joint);

		void translate(vertex3 movement);

		void rotate(vertex3 rotate);

		virtual void update();

		bool isNull();
};

#endif


#ifndef _LINKROOT_H_
#define _LINKROOT_H_

#include "Link.h"
#include "Trajectory.h"

class LinkRoot: public Link{
	public:
		Trajectory* path;

		LinkRoot();
		
		LinkRoot(ModelView mesh, Pose* pose);

		LinkRoot(ModelView mesh, Pose* pose, std::string name);

		LinkRoot(LinkRoot& link_copy);

		virtual void update();

		void generatePath();

		public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif

