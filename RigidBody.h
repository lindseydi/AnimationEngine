#ifndef _RIGIDBODY_H_
#define _RIGIDBODY_H_

#include "matrix3.h"
#include "Model.h"
#include "Helper.h"


class RigidBody: public Model{
		public:
		//void generateI_object(){
		//	matrix3 identity = matrix3();
		//	identity.identity();
		//	double multiplier = 2.0 * mass * radius * radius;
		//	multiplier = multiplier / 5.0;
		//	I_object = identity * multiplier;
		//}

		//Pose location; already have this as part of Model!
		double radius;
		double k;
		double mass;		//mass
		//matrix3 I_object; //stays constant
		//matrix3 I; //intertial tensor ~ angular mass - changes with translations & rotations
		vertex3 linearVelocity;
		//vertex3 angularVelocity;
		vertex3 linearAcceleration;		//acceleration
		//vertex3 angularAcceleration;	
		vertex3 linearMomentum;		//momentum
		//double angularMomentum;		//angular momentum
		vertex3 force;	//force
		//vertex3 torque;	//Torque
		vertex3 color;

		RigidBody(): Model(){
			radius = 1.0;
			k = 0.8;
			type = esphere;
			mesh.loadSphere(radius, 5, 10);
			mass = 1.0;
			pose = new Pose();
			linearAcceleration = vertex3();	//constant speed to start
			//angularAcceleration = vertex3();	//constant speed to start
			linearVelocity = vertex3();	//still to start
			//angularVelocity = vertex3();	//still to start
			linearMomentum = vertex3(0.0, 0.0, 0.0);
			//angularMomentum = 0.0;
			force = vertex3();
			//torque= vertex3();
			//generateI_object();
		}

		RigidBody(Pose* initialLocation): Model(){
			type = esphere;
			radius = 1.0;
			k = 0.8;
			this->pose = initialLocation;
			mesh.loadSphere(radius, 5, 10);
			mass = 1.0;
			linearVelocity = vertex3();	//still to start
			//angularVelocity = vertex3();	//still to start
			linearAcceleration = vertex3();	//constant speed to start
			//angularAcceleration = vertex3();	//constant speed to start
			float x = Helper::randBetween_float(-4.0, 4.0);
			float y = Helper::randBetween_float(-2.0, 0.0);
			float z = Helper::randBetween_float(-4.0, 4.0);
			linearMomentum = vertex3(x, y, z);
			//angularMomentum = 0.0;
			force = vertex3();
			//torque = vertex3();
			//generateI_object();
		}

		RigidBody(const RigidBody& copy){
			//this->angularAcceleration = copy.angularAcceleration;
			//this->angularMomentum = copy.angularMomentum;
			//this->angularVelocity = copy.angularVelocity;
			this->color = copy.color;
			this->force =copy.force;
			//this->I = copy.I;
			//this->I_object = copy.I_object;
			this->k = copy.k;
			this->linearAcceleration = copy.linearAcceleration;
			this->linearMomentum = copy.linearMomentum;
			this->linearVelocity = copy.linearVelocity;
			this->mass = copy.mass;
			this->mesh = copy.mesh;
			type = esphere;
			this->pose = copy.pose;
			this->radius = copy.radius;
			//this->torque = copy.torque;
			this->transform = copy.transform;
		}

		void moveAlongDirection(double time, double dt){
			//TODO  - check that this is correct!
			//check that orientation vertex is still valid
			//if(!pose->orientationValid())
			//	printf("At some point this vertex has been a quaternion and then switched back. Be safe with your vectors!\n");

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
		//	if(!pose->orientationValid())
		//		printf("At some point this vertex has been a quaternion and then switched back. Be safe with your vectors!\n");

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
			//linearMomentum = linearVelocity * mass;
			linearMomentum = linearMomentum + (force * dt);


			//assumes a constant acceleration (even though technically it is not)
			//linearVelocity = linearVelocity + (linearAcceleration * dt);
			linearVelocity = linearMomentum / mass;
			moveAlongDirection(time, dt);	//is this skipping a time step? should they be flipped?
			//printf("Momentum(x) %f, Velocity(x)  %f, Pos(x)  %f\n", linearMomentum.getx(), linearVelocity.getx(), pose->position.getx());
			//printf("Force(y) %f, Velocity(y)  %f, Pos(y)  %f\n\n", force.gety(), linearVelocity.gety(), pose->position.gety());

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
		
		void handlePolygonCollision(const polygon& poly, double time, double dt){
			polygon tempPoly = poly;
			vertex3 pointOfCollision = (tempPoly.center - this->pose->position) - (tempPoly.center - tempPoly.normal* this->radius);
		//	printf("Point of Collision(%f, %f, %f)\n", pointOfCollision.getx(), pointOfCollision.gety(), pointOfCollision.getz());
		
			vertex3 dirSphere = this->linearVelocity;
			dirSphere = dirSphere * -1.0f;
			dirSphere.normalize();

			tempPoly.normal.normalize();
			vertex3 axis = vertex3(dirSphere.cross(poly.normal));
			axis.normalize();
			double theta = acos(dirSphere.dotProduct(poly.normal));
		//	printf("Theta is %f\n", theta);

			//pretty high because using floats
			const double EPSILON = 1e-3;

			//if the cross product between the direction of the opposite direction of the ball and the normal is zero
			//they are either parrallel or perpendicular
			//since if the motion of the ball was parrallell to the normal the would not be a collision (this is why we multiply by -1)
			//this must mean they are perpendiular and therefore
			//we have a head on collision.
			if(theta < EPSILON){
				//back it up a time step
				this->moveAlongOppositeDirection(time, dt);
			
				//create an equal and opposite reaction
				this->linearMomentum = this->linearMomentum * -1.0;
				//but lessen the momemtum a bit to take account for the energy used in the collision
				this->linearMomentum = this->linearMomentum * this->k;
			}
			else{		//otherwise, the normal will project the ball at some angle
				//back the ball up a time step
				this->moveAlongOppositeDirection(time, dt);
			
				//calculate the new direction of the ball after the collsion takes place 
				//should be this...
				quaternion q = quaternion(axis.getx() * sin(theta), axis.gety() * sin(theta), axis.getz() * sin(theta), cos(theta));
				//quaternion q = quaternion( cos(theta), axis.getx() * sin(theta), axis.gety() * sin(theta), axis.getz() * sin(theta) );
				matrix3 rotation = q.getRotation_matrix3();

				//reverse momentum and give set it on the new angle
				this->linearMomentum = rotation * (this->linearMomentum * -1.0);
				this->linearMomentum = this->linearMomentum * this->k;
			}		
		}

		void handleRigidBodyCollision(RigidBody* sphere, double time, double step){
			//determine important variables
			const float EPSILON = 1e-3;
			vertex3 myCenter = this->pose->position;
			vertex3 otherCenter = sphere->pose->position;

			vertex3 myNormal = otherCenter - myCenter;
			vertex3 otherNormal = myCenter - otherCenter;
			myNormal.normalize();
			otherNormal.normalize();

			vertex3 myDirection = this->linearVelocity;
			myDirection.normalize();
			vertex3 otherDirection = sphere->linearVelocity;
			otherDirection.normalize();

			myDirection = myDirection * -1.0;
			otherDirection = otherDirection * -1.0;
			vertex3 myAxis = myDirection.cross(otherNormal);
			vertex3 otherAxis = otherDirection.cross(myNormal);


			myAxis.normalize();
			otherAxis.normalize();
			double myDotProduct = myDirection.dotProduct(otherNormal);
			double otherDotProduct = otherDirection.dotProduct(myNormal);
			//	printf("Dots %f, %f\n", myDotProduct, otherDotProduct);
			double myTheta = acos(myDirection.dotProduct(otherNormal));
			double otherTheta = acos(otherDirection.dotProduct(myNormal));


			//first move back a step
			this->moveAlongOppositeDirection(time, step);
			sphere->moveAlongOppositeDirection(time, step);

			//normalize will return NaN if it is the zero vector
			if(((myTheta != myTheta) && (otherTheta != otherTheta)) || ((myTheta < EPSILON ) && (otherTheta < EPSILON))){
				//head on collision

				//switch momentums
				vertex3 tempMomentum = this->linearMomentum;
				this->linearMomentum = sphere->linearMomentum;
				sphere->linearMomentum = tempMomentum;

				this->linearMomentum = linearMomentum * this->k;
				sphere->linearMomentum = sphere->linearMomentum *sphere->k;
			} else if(((myTheta != myTheta)||(myTheta < EPSILON)) && (fabs(otherTheta - PI/2.0) < EPSILON)){
				//if the second ball is not in motion
				//first ball stops and second ball steals all momentum
				this->linearMomentum = vertex3();
				sphere->linearMomentum = sphere->linearMomentum + this->linearMomentum;

				sphere->linearMomentum = sphere->linearMomentum * sphere->k;
			} else if(((otherTheta != otherTheta) || (otherTheta < EPSILON)) && (fabs(myTheta - PI/2) < EPSILON)){
				//the opposite
				this->linearMomentum = this->linearMomentum + sphere->linearMomentum;
				sphere->linearMomentum = vertex3();

				//p = mv1 + mv2
				this->linearMomentum = linearMomentum * this->k;
			}else{
				//balls bounce off of each other at an angle
				//printf("Balls hit at angle, %f %f\n", myTheta, otherTheta);
				quaternion myQuat = quaternion(myAxis.getx() * sin(myTheta), 
												myAxis.gety() * sin(myTheta), 
												myAxis.getz() * sin(myTheta), 
												cos(myTheta));
				matrix3 myTransform = myQuat.getRotation_matrix3();


				quaternion otherQuat = (myAxis.getx() * sin(otherTheta), 
												otherAxis.gety() * sin(otherTheta), 
												otherAxis.getz() * sin(otherTheta), 
												cos(otherTheta));
				matrix3 otherTransform = otherQuat.getRotation_matrix3();

				//vertex3 myNewDir = myTransform * this->linearMomentum * -1.0f;
				//myNewDir.normalize();
				//vertex3 otherNewDir = otherTransform * sphere->linearMomentum * -1.0f;	
				//otherNewDir.normalize();

				//set new momentums
				this->linearMomentum = myTransform * this->linearMomentum * -1.0;
				this->linearMomentum = this->linearMomentum * this->k;
				sphere->linearMomentum = otherTransform * sphere->linearMomentum * -1.0;
				sphere->linearMomentum = sphere->linearMomentum * sphere->k;
			}
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