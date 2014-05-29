#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "matrix3.h"
#include "Model.h"

#

class RigidBody: public Model{
	private:
		//constants:
		double radius;


		void generateI_object(){
			matrix3 identity = matrix3();
			identity.identity();
			double multiplier = 2.0 * mass * radius * radius;
			multiplier = multiplier / 5.0;
			I_object = identity * multiplier;
		}

	public:
		//Pose location; already have this as part of Model!
		double mass;		//mass
		matrix3 I_object; //stays constant
		matrix3 I; //intertial tensor ~ angular mass - changes with translations & rotations
		vertex3 linearVelocity;
		vertex3 angularVelocity;
		vertex3 linearAcceleration;		//acceleration
		vertex3 angularAcceleration;	
		double linearMomentum;		//momentum
		double angularMomentum;		//angular momentum
		vertex3 force;	//force
		vertex3 torque;	//Torque


		RigidBody(): Model(){
			radius = 0.5;
			mesh.loadSphere(radius, 5, 10);
			mass = 1.0;
			linearAcceleration = vertex3();	//constant speed to start
			angularAcceleration = vertex3();	//constant speed to start
			linearVelocity = vertex3();	//still to start
			angularVelocity = vertex3();	//still to start
			linearMomentum = 0.0;
			angularMomentum = 0.0;
			force = vertex3();
			torque= vertex3();
			generateI_object();
		}

		RigidBody(Pose* initialLocation): Model(){
			radius = 0.5;
			this->pose = initialLocation;
			mesh.loadSphere(0.1, 5, 10);
			mass = 1.0;
			linearVelocity = vertex3();	//still to start
			angularVelocity = vertex3();	//still to start
			linearAcceleration = vertex3();	//constant speed to start
			angularAcceleration = vertex3();	//constant speed to start
			linearMomentum = 0.0;
			angularMomentum = 0.0;
			force = vertex3();
			torque = vertex3();
			generateI_object();
		}

		vertex3 moveAlongDirection(){
			//TODO  - check that this is correct!
			//check that orientation vertex is still valid
			
			if(!pose->orientationValid())
				printf("At some point this vertex has been a quaternion and then switched back. Be safe with your vectors!\n");

			vertex3 direction = pose->getEulerRepresentation();
			direction.normalize();
			
			float futureX = pose->position.getx() + (direction.getx() * this->linearVelocity.getx());
			float futureY = pose->position.gety() + (direction.gety() * this->linearVelocity.gety());
			float futureZ = pose->position.getz() + (direction.getz() * this->linearVelocity.getz());
			//printf("future is (%f, %f, %f)\n", futureX, futureY, futureZ);
			return vertex3(futureX, futureY, futureZ);
		}

		void calculateProperties(){
			//update velocity and pose using acceleration!
			
			//CALCULATE NEXT POSITION
			//assumes a constant acceleration (even though technically it is not)
			linearVelocity = linearVelocity + linearAcceleration * (1.0/30.0);
			pose->position = moveAlongDirection();	//is this skipping a time step? should they be flipped?

			//CALCULATE NEXT ORIENTATION
			//first calculate inertia tensor
			//TODO matrix3 vs matrix4
			//make compatible!
			//omega = I(t)-1 * L(t)
			//angularVelocity =angularAcceleration.inverse() * angularMomentum;
			//pose->orientation = pose->orientation +  angularVelocity;
		}

		void calculateForces(){
			//reset force?
			force = vertex3();

			//gravity
			//F = ma
			force = force + vertex3(0.0f, (float)(mass * 9.8), 0.0f);

			//are there any collisions?
			//add damping factor for each collision

			//torque
			//torque = radius x each force;
			//add with each indiviudual force!
		}

		void calculateAccelerations(){
			linearAcceleration = force / mass;

			//must make matrix compatible
			//matrix4 rotation = pose->getRotation();
			//angular acceleration might need to be a matrix...
			//I(t) = R(t) * I * R(t)T
			//angularAcceleration = rotation * I_object * rotation.transpose();
		}

		void update(){
			//Here is where you need to add Ordinary differential equations...
			
			calculateAccelerations();
			calculateForces();
			calculateProperties();

		}
};



#endif	//RigidBody