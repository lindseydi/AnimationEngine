#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "matrix3.h"
#include "Model.h"


class RigidBody: public Model{
	private:
		//constants:
		double radius;
		double k;

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
		vertex3 color;

		RigidBody(): Model(){
			radius = 1.0;
			k = 0.8;
			mesh.loadSphere(radius, 5, 10);
			mass = 1.0;
			pose = new Pose();
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
			radius = 1.0;
			k = 0.8;
			this->pose = initialLocation;
			mesh.loadSphere(radius, 5, 10);
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

		void moveAlongDirection(double time, double dt){
			//TODO  - check that this is correct!
			//check that orientation vertex is still valid
			if(!pose->orientationValid())
				printf("At some point this vertex has been a quaternion and then switched back. Be safe with your vectors!\n");

			vertex3 direction = pose->getEulerRepresentation();
			direction.normalize();
			
			// xf = v * dt + xi

			//float futureX = pose->position.getx() + (direction.getx() * this->linearVelocity.getx());
			//float futureY = pose->position.gety() + (direction.gety() * this->linearVelocity.gety());
			//float futureZ = pose->position.getz() + (direction.getz() * this->linearVelocity.getz());
			float futureX = pose->position.getx() + ((float)dt * this->linearVelocity.getx());
			float futureY = pose->position.gety() + ((float)dt * this->linearVelocity.gety());
			float futureZ = pose->position.getz() + ((float)dt * this->linearVelocity.getz());
			//printf("future is (%f, %f, %f)\n", futureX, futureY, futureZ);
			//return vertex3(futureX, futureY, futureZ);
			pose->position = vertex3(futureX, futureY, futureZ);
		}

		
		void moveAlongOppositeDirection(double time, double dt){
			//TODO  - check that this is correct!
			//check that orientation vertex is still valid
			if(!pose->orientationValid())
				printf("At some point this vertex has been a quaternion and then switched back. Be safe with your vectors!\n");

			vertex3 direction = pose->getEulerRepresentation() * -1.0;
			direction.normalize();
			
			// xf = v * dt + xi

			//float futureX = pose->position.getx() + (direction.getx() * this->linearVelocity.getx());
			//float futureY = pose->position.gety() + (direction.gety() * this->linearVelocity.gety());
			//float futureZ = pose->position.getz() + (direction.getz() * this->linearVelocity.getz());
			float futureX = pose->position.getx() + ((float)dt * -1.0*this->linearVelocity.getx());
			float futureY = pose->position.gety() + ((float)dt * -1.0*this->linearVelocity.gety());
			float futureZ = pose->position.getz() + ((float)dt * -1.0*this->linearVelocity.getz());
			//printf("future is (%f, %f, %f)\n", futureX, futureY, futureZ);
			//return vertex3(futureX, futureY, futureZ);
			pose->position = vertex3(futureX, futureY, futureZ);
		}

		void calculateProperties(double time, double dt){
			//update velocity and pose using acceleration!
			
			//CALCULATE NEXT POSITION
			//assumes a constant acceleration (even though technically it is not)
			linearVelocity = linearVelocity + (linearAcceleration * dt);
			moveAlongDirection(time, dt);	//is this skipping a time step? should they be flipped?
			printf("Force(y) %f, Velocity(y)  %f, Pos(y)  %f\n", force.gety(), linearVelocity.gety(), pose->position.gety());

			//CALCULATE NEXT ORIENTATION
			//first calculate inertia tensor
			//TODO matrix3 vs matrix4
			//make compatible!
			//omega = I(t)-1 * L(t)
			//angularVelocity =angularAcceleration.inverse() * angularMomentum;
			//pose->orientation = pose->orientation +  angularVelocity;
		}

		void calculateForces(){
			//reset force??
			force = vertex3();
			//gravity
			//F = ma
			force = force + vertex3(0.0f, (float)(mass * -9.8), 0.0f);

			//are there any collisions?
			//add damping factor for each collision

			//torque
			//torque = radius x each force;
			//add with each indiviudual force!
		}

		void applyNormalForce(vertex3& normalForce,  double dt){
			///this->force = this->force + normalForce;
			//v_f = v_i - (N (v_i* N) * (1+k))
			//linearVelocity = linearVelocity - (normalForce * linearVelocity.dotProduct(normalForce) * (1+k));
			linearVelocity = linearVelocity * -k;
			//linearVelocity = linearVelocity /2.0;
			//linearVelocity.setx(linearVelocity.getx() - (1-k)*(linearVelocity.getx() * normalForce.getx())* normalForce.getx());
			//linearVelocity.sety(linearVelocity.gety() - (1-k)*(linearVelocity.gety() * normalForce.gety())* normalForce.gety());
			//linearVelocity.setz(linearVelocity.getz() - (1-k)*(linearVelocity.getz() * normalForce.getz())* normalForce.getz());
		}

		void calculateAccelerations(){
			linearAcceleration = force / mass;
			//linearAcceleration = vertex3(0.0, -9.8, 0.0);
			//must make matrix compatible
			//matrix4 rotation = pose->getRotation();
			//angular acceleration might need to be a matrix...
			//I(t) = R(t) * I * R(t)T
			//angularAcceleration = rotation * I_object * rotation.transpose();
		}

		virtual void update(double time, double dt){
			//Here is where you need to add Ordinary differential equations...
			
			calculateAccelerations();
			calculateForces();
			calculateProperties(time, dt);
		//	printf("Position: (%f, %f, %f)\n", pose->position.getx(),  pose->position.gety(),  pose->position.getz());
			transform = this->pose->translate_object() * this->pose->getRotation();
		}

		float getRadius(){
			return radius;
		}

		public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};



#endif	//RigidBody