
#ifndef _POSE_H_
#define _POSE_H_


#include <stdlib.h>
#include "vertex3.h"
#include "quaternion.h"
#include <time.h>

class Pose{
	public:
		vertex3 position;
		quaternion rotation;

		Pose(){
			//Default constructor
			position.zero();
			rotation.set(0.0, 0.0, 0.0);
		}

		Pose(float x, float y, float z, float theta, float phi, float psi){
			position.set(x, y, z);
			rotation.set(theta, phi, psi);
		}

			Pose(float x, float y, float z, float theta, float phi, float psi, float w){
			position.set(x, y, z);
			rotation.set(theta, phi, psi, w);
		}

		Pose(const vertex3& pos, const quaternion& rot){
			position = pos;
			rotation = rot;
		}

		virtual void set(float x, float y, float z, float theta, float phi, float psi){
			position.set(x, y, z);
			rotation.set(theta, phi, psi);
		}

		//Will return an array of seven elements
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
					return this->rotation.quat.x();
				break;
				case 4:
					return this->rotation.quat.y();
				break;
				case 5:
					return this->rotation.quat.z();
				break;
				case 6:
					return this->rotation.quat.w();
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
		float X = this->rotation.getx();
		float Y = this->rotation.gety();
		float Z = this->rotation.getz();
		float W = this->rotation.getw();
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

	void print(){
		printf("%f, %f, %f ---- \n", position.getx(), position.gety(), position.getz());
	}

	
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

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

		void set(float x, float y, float z, float theta, float phi, float psi, int frame){
			position.set(x, y, z);
			rotation.set(theta, phi, psi);
			this->frame = frame;
		}

		
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

class PoseEuler: public Pose{
public:
	vertex3 orientation;
	PoseEuler(): Pose(){
		orientation = vertex3(0.0, 0.0, 0.0);
	}

	PoseEuler(float x, float y, float z, float theta, float phi, float psi){
			position.set(x, y, z);
			orientation = vertex3(theta, phi, psi);
	}

	PoseEuler(vertex3 pos, vertex3 orientation){
			position.set(pos);
			this->orientation = orientation;
	}

	virtual matrix4 getRotation(){
		float pitch, yaw, roll;
		pitch = orientation.getx();
		yaw = orientation.gety();
		roll = orientation.getz();
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
	void operator=(const PoseEuler& copy){
		this->position = copy.position;
		this->orientation = copy.orientation;
	}

	virtual void set(const PoseEuler& posein){
		this->position.set(posein.position);
		this->orientation.set(posein.orientation);
	}

public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif

