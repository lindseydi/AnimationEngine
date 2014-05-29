
#ifndef _POSE_H_
#define _POSE_H_


#include <stdlib.h>
#include "vertex3.h"
#include "quaternion.h"

class Pose{
	public:
		vertex3 position;
		vertex3* orientation;		//EULER ANGLE
		quaternion rotation_quat;	//QUATERNION		

		Pose(){
			//Default constructor
			position.zero();
			orientation = NULL;
			rotation_quat.set(0.0, 0.0, 0.0);
		}

		Pose(float x, float y, float z, float theta, float phi, float psi){
			position.set(x, y, z);
			orientation = new vertex3(theta, phi, psi);
			rotation_quat.set(theta, phi, psi);
		}

		Pose(float x, float y, float z, float theta, float phi, float psi, float w){
			position.set(x, y, z);
			orientation = NULL;
			rotation_quat.set(theta, phi, psi, w);
		}

		Pose(const vertex3& pos, const quaternion& rot){
			position = pos;
			orientation = NULL;
			rotation_quat = rot;
		}

		Pose(const Pose& pose){
			this->position = pose.position;
			this->orientation = pose.orientation;
			this->rotation_quat = pose.rotation_quat;
		}

		virtual void set(float x, float y, float z, float theta, float phi, float psi){
			position.set(x, y, z);
			orientation = new vertex3(theta, phi, psi);
			rotation_quat.set(theta, phi, psi);
		}

		//Will return an array of seven elements
		//returns quaternion data, since it is ALWAYS valid.
		float& operator[](unsigned int index)
		{
			switch(index){
				case 0:
					return this->position.vertex.x();
				break;
				case 1:
					return this->position.vertex.y();
				break;
				case 2:
					return this->position.vertex.z();
				break;
				case 3:
					return this->rotation_quat.quat.x();
				break;
				case 4:
					return this->rotation_quat.quat.y();
				break;
				case 5:
					return this->rotation_quat.quat.z();
				break;
				case 6:
					return this->rotation_quat.quat.w();
				break;
				default:
					printf("Something is wrong inside Pose[]\n");
					return this->position.vertex.x();
				break;
			}
		}

	//--Helper functions
	matrix4 translate_object(){
		matrix4 translate = matrix4();
		for(unsigned int i=0; i<4; i++){
			translate(i, i) = 1.0f;
		}
		translate(0, 3) = this->position.getx();
		translate(1, 3) = this->position.gety();
		translate(2, 3) = this->position.getz();
		//translate(2, 3) = this->position.getz()-5.0;


		return translate;
		//float m[ 16 ];
		//translate.transpose().ToArray( m );
		//glMultMatrixf( m );
	}

	matrix4 scale_object(float x, float y, float z){
		matrix4 scale = matrix4();
		scale(0, 0) = x;
		scale(1, 1) = y;
		scale(2, 2) = z;
		scale(3,3) = 1.0;


		return scale;
		//float m[16];
		//scale.transpose().ToArray(m);
		//glMultMatrixf(m);
	}
	
	virtual matrix4 getRotation(){
		matrix4 rot = matrix4();
		float X = this->rotation_quat.getx();
		float Y = this->rotation_quat.gety();
		float Z = this->rotation_quat.getz();
		float W = this->rotation_quat.getw();
		rot(0,0) = 1-2*Y*Y -2*Z*Z;
		rot(0,1) = 2*X*Y - 2*W*Z;
		rot(0,2) = 2*X*Z + 2*W*Y;

		rot(1,0) = 2*X*Y + 2*W*Z;
		rot(1,1) = 1-2*X*X - 2*Z*Z;
		rot(1,2) = 2*Y*Z - 2*W*X;

		rot(2,0) = 2*X*Z - 2*W*Y;
		rot(2,1) = 2*Y*Z + 2*W*X;
		rot(2,2) = 1- 2*X*X - 2*Y*Y;

		rot(3,3) = 1.0;

		return rot;
	}

	virtual matrix4 getEulerRotation(){
		float pitch, yaw, roll;
		pitch = orientation->getx();
		yaw = orientation->gety();
		roll = orientation->getz();
		matrix4 rotatePitch;
		matrix4 rotateYaw;
		matrix4 rotateRoll;
		matrix4 product;

		rotatePitch(0, 0) = 1;
		rotatePitch(1, 1) = cos(pitch);
		rotatePitch(1, 2) = -sin(pitch);
		rotatePitch(2, 1) = sin(pitch);
		rotatePitch(2, 2) = cos(pitch);
		rotatePitch(3, 3) = 1.0;

		rotateYaw(0, 0) = cos(yaw);
		rotateYaw(0, 2) = sin(yaw);
		rotateYaw(1, 1) = 1.0;
		rotateYaw(2, 0) = -sin(yaw);
		rotateYaw(2, 2) = cos(yaw);
		rotateYaw(3, 3) = 1.0;

		rotateRoll(0, 0) = cos(roll);
		rotateRoll(0, 1) = -sin(roll);
		rotateRoll(1, 0) = sin(roll);
		rotateRoll(1, 1) = cos(roll);
		rotateRoll(2, 2) = 1.0;
		rotateRoll(3, 3) = 1.0;

		product = rotatePitch * rotateYaw * rotateRoll;
		return product;
		//float m[16];
		//product.transpose().ToArray(m);
		//glMultMatrixf(m);
	}

	void print(){
		printf("%f, %f, %f ---- \n", position.getx(), position.gety(), position.getz());
	}

	/*
	PoseEuler toEulerPose(Pose& input){
		//be sure to test "toEuler method"
		return PoseEuler(input.position, input.rotation_quat.toEuler());
	}
	*/
	bool orientationValid(){
		if(orientation != NULL)
				return true;
			else
				return false;
	}

	vertex3 getEulerRepresentation(){
		if(orientationValid())
			return *(this->orientation);
		else{
			vertex3 retVal = vertex3();
			retVal = rotation_quat.toEuler();
			return retVal;
		}
	}
	
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif	//Pose

#ifndef _POSEKEY_H_
#define _POSEKEY_H_

class PoseKey: public Pose {
	public:
		int frame;	//always necessary? 

		PoseKey():Pose(){
			//Default constructor
			frame = 0;
		}

		PoseKey(const vertex3& pos, const quaternion& rot, int frame) : Pose(pos, rot){
			this->frame = frame;
		}

		PoseKey(float x, float y, float z, float theta, float phi, float psi, int frame): Pose(x, y, z, theta, phi, psi){
			this->frame = frame;
		}

		PoseKey(const PoseKey& posekey){
			this->frame = posekey.frame;
			this->rotation_quat = posekey.rotation_quat;
			this->orientation = posekey.orientation;
			this->position = posekey.position;
		}

		void set(float x, float y, float z, float theta, float phi, float psi, int frame){
			position.set(x, y, z);
			rotation_quat.set(theta, phi, psi);
			orientation->set(theta, phi, psi);
			this->frame = frame;
		}

		
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif	//PoseKey

