#ifndef _Model_H_
#define _Model_H_

#include "ModelView.h"
#include "matrix4.h";
#include "Pose.h"

class Model{
public:
	Model(){
		//NEVER USE ME
		mesh = ModelView();
		pose = NULL;
	}

	Model(PoseEuler* pose){
		mesh.loadBox(1.0, 1.0, 1.0);
		this->pose = pose;
		this->transform = this->pose->getRotation() * this->pose->translate_object();
	}

	Model(ModelView mesh, PoseEuler* pose){
		this->mesh = mesh;
		this->pose = pose;
		this->transform = this->pose->getRotation() * this->pose->translate_object();
	}

	Model(const Model& modelCopy){
		this->mesh = modelCopy.mesh;
		this->pose = modelCopy.pose;
	}

	public:
		ModelView mesh;
		PoseEuler* pose;
		matrix4 transform;

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif //Model