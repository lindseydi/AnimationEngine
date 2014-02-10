#pragma once
#include <stdio.h>
#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>
using namespace Eigen;

class KeyFrame
{
	//Global functions
	static Quaternionf& vector3fToQuaternion(Vector3f euler)
	{
		float angle;

		Quaternionf retValue;
		angle = euler.x * 0.5;
		const float sr = sin(angle);
		const float cr = cos(angle);

		angle = euler.y * 0.5;
		const float sp = sin(angle);
		const float cp = cos(angle);

		angle = euler.z * 0.5;
		const float sy = sin(angle);
		const float cy = cos(angle);

		const float cpcy = cp * cy;
		const float spcy = sp * cy;
		const float cpsy = cp * sy;
		const float spsy = sp * sy;

		retValue.x = (float)(sr * cpcy - cr * spsy);
		retValue.y = (float)(cr * spcy + sr * cpsy);
		retValue.z = (float)(cr * cpsy - sr * spcy);
		retValue.w = (float)(cr * cpcy + sr * spsy);

		retValue.normalize();
		return retValue;
	}

	static Quaternionf& vector3fToQuaternion(float xin, float yin, float zin)
	{
		float angle;

		Quaternionf retValue;
		angle = xin * 0.5;
		const float sr = sin(angle);
		const float cr = cos(angle);

		angle = yin * 0.5;
		const float sp = sin(angle);
		const float cp = cos(angle);

		angle = zin * 0.5;
		const float sy = sin(angle);
		const float cy = cos(angle);

		const float cpcy = cp * cy;
		const float spcy = sp * cy;
		const float cpsy = cp * sy;
		const float spsy = sp * sy;

		retValue.x = (float)(sr * cpcy - cr * spsy);
		retValue.y = (float)(cr * spcy + sr * cpsy);
		retValue.z = (float)(cr * cpsy - sr * spcy);
		retValue.w = (float)(cr * cpcy + sr * spsy);

		 retValue.normalize();
		 return retValue;
	}

public:
	Vector3f position;
	Quaternionf orient;
	float time; //necessary?

	float *getPointer( void ) {
		return &position.x;
	}

	float at( int index ) {
		return (&position.x)[ index ];
	}

	KeyFrame(){
		this->position =  Vector3f(0.0, 0.0, 0.0);
		this->orient = Quaternionf(vector3fToQuaternion(1.0f, 0.0f, 0.0f));
		this->time = 0.0;
	}

	KeyFrame(Vector3f pos, Quaternionf orient, float time){
		this->position = pos;
		this->orient = orient;
		this->time = time;
	}
	//Euler Angles
	KeyFrame(float posX, float posY, float posZ, float orientX, float orientY, float orientZ, float time){
		this->position = Vector3f(posX, posY, posZ);
		this->orient = Quaternionf(vector3fToQuaternion(orientX, orientY, orientZ));
		this->time = time;
	}
	//Quaternionfs
	KeyFrame(float posX, float posY, float posZ, float orientX, float orientY, float orientZ, float orientW, float time){
		this->position = Vector3f(posX, posY, posZ);
		this->orient = Quaternionf(orientX, orientY, orientZ, orientW);
		this->time = time;
	}
	~KeyFrame(void){
		//something should go here
	}

	GLfloat getPosX(){
		return this->position.x;
	}

	GLfloat getPosY(){
		return this->position.y;
	}

	GLfloat getPosZ(){
		return this->position.z;
	}

	GLfloat getOrientX(){
		return (GLfloat)this->orient.x();
	}

	GLfloat getOrientY(){
		return (GLfloat)this->orient.y();
	}

	GLfloat getOrientZ(){
		return (GLfloat)this->orient.z();
	}

	GLfloat getOrientW(){
		return (GLfloat)this->orient.w();
	}

	float getTime(){
		return time;
	}

	void setPos(Vector3f in){
		this->position = in;
	}

	void setPos(GLfloat xin, GLfloat yin, GLfloat zin){
		this->position.x = xin;
		this->position.y = yin;
		this->position.z = zin;
	}

void print(){
		printf("%f, %f, %f, %f, %f, %f, %f \n", position.x, position.y, position.z, orient.X, orient.Y, orient.Z, orient.W);
	}

};