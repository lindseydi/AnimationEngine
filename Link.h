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
		
		Link(const ModelView& mesh, Pose* pose);

		Link(const ModelView& mesh, Pose* pose, std::string name);

		Link(const Link& link_copy);

		void addChild(Joint* joint);

		void translate(const vertex3& movement);

		void rotate(const vertex3& rotate);

		virtual void update();

		bool isNull();

		public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
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
		
		LinkRoot(const ModelView& mesh, Pose* pose);

		LinkRoot(const ModelView& mesh, Pose* pose, std::string name);

		LinkRoot(const LinkRoot& link_copy);

		virtual void update();

		void generatePath();

		public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif

