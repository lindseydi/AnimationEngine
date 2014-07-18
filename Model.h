#ifndef _MODEL_H_
#define _MODEL_H_


#include <stdlib.h>
#include "ModelView.h"
#include "matrix4.h"
#include "Pose.h"

enum EtypeModel{emesh, esphere, ePlane, eteapot};

class Model{
public:
	Model(){
		//NEVER USE ME
		mesh = ModelView();
		type = emesh;
		pose = NULL;
	}

	Model(Pose* pose){
		type = emesh;
		mesh.loadBox(1.0, 1.0, 1.0);
		this->pose = pose;
		this->transform = this->pose->getRotation() * this->pose->translate_object();
	}

	Model(Pose* pose, vertex3 color){
		type = emesh;
		mesh.loadBox(1.0, 1.0, 1.0);
		this->pose = pose;
		mesh.color = color;
		this->transform = this->pose->getRotation() * this->pose->translate_object();
	}

	Model(const ModelView& mesh, Pose* pose){
		type = emesh;
		this->mesh = mesh;
		this->pose = pose;
		this->transform = this->pose->getRotation() * this->pose->translate_object();
	}
	
	Model(const ModelView& mesh, Pose* pose, const vertex3& color){
		type = emesh;
		this->mesh = mesh;
		this->mesh.color = color;
		this->pose = pose;
		this->transform = this->pose->getRotation() * this->pose->translate_object();
	}

	Model(const Model& modelCopy){
		type = emesh;
		this->mesh = modelCopy.mesh;
		this->pose = modelCopy.pose;
	}

	vertex3 getEulerRepresentation(){
		pose->getEulerRepresentation();
	}

	void updateTransformEuler(){
		//pose = in;
		this->transform = this->pose->translate_object() * this->pose->getEulerRotation();
		//printf("Pose (%f, %f) %f\n", this->pose->position.getx(),this->pose->position.gety(), this->pose->orientation->getz());

	}

	void updateTransform(){
		//pose = in;
		this->transform = this->pose->translate_object()* this->pose->getRotation();
		//printf("Pose (%f, %f) %f\n", this->pose->position.getx(),this->pose->position.gety(), this->pose->orientation->getz());

	}

	virtual void update(){
		//printf("Executing parent update!\n");
	}

	virtual void update(double time, double dt){
		//printf("Executing parent update!\n");
	}


	public:
		EtypeModel type;
		ModelView mesh;
		Pose* pose;
		matrix4 transform;

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};
#endif //Model