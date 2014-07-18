#ifndef _BoidModel_H_
#define _BoidModel_H_

#pragma once
#include "Model.h"

class BoidModel : public Model{

public:
	BoidModel() : Model(){
		mesh = ModelView();
		mesh.loadPyramid(0.3, 0.4);
		velocity = vertex3();
		pose = new Pose(vertex3(), vertex3(0.0f, 0.0f, 1.57));
		acceleration = vertex3();
		radius = 0.05;
		maxForce = 0.008f;
		maxSpeed = .05f;
	}

	BoidModel(float posx, float posy, float posz, float theta) : Model(){
		mesh = ModelView();
		mesh.loadPyramid(0.3, 0.4);
		velocity = vertex3(cos(theta), sin(theta), 0.0);
		pose = new Pose(posx, posy, posz, 0.0f, 0.0f, theta);
		acceleration = vertex3();
		radius = 0.05;
		maxForce = 0.008f;
		maxSpeed = 0.1;
	}

	
	BoidModel(float posx, float posy, float posz, float theta, vertex3 color) : Model(){
		mesh = ModelView();
		mesh.loadPyramid(0.3, 0.4);
		velocity = vertex3(cos(theta), sin(theta), 0.0);
		pose = new Pose(posx, posy, posz, 0.0f, 0.0f, theta);
		acceleration = vertex3();
		radius = 0.05;
		maxForce = 0.008f;
		maxSpeed = 0.05;
		mesh.color = color;
	}

	~BoidModel(){
		
	}

	float distance(BoidModel* potentialneighbor){
		//d(p, q) = \sqrt{(p_1 - q_1)^2 + (p_2 - q_2)^2+(p_3 - q_3)^2}.
		float xx = this->pose->position.getx() - potentialneighbor->pose->position.getx();
		xx = xx * xx;
		float yy = this->pose->position.gety() - potentialneighbor->pose->position.gety();
		yy = yy * yy;
		float zz = this->pose->position.getz() - potentialneighbor->pose->position.getz();
		zz = zz * zz;
		float asdf = std::sqrt(xx + yy + zz);
		//printf("Distance %f\n", asdf);
		return asdf;
	}


	float distance(vertex3 spot){
		//d(p, q) = \sqrt{(p_1 - q_1)^2 + (p_2 - q_2)^2+(p_3 - q_3)^2}.
		float xx = this->pose->position.getx() - spot.getx();
		xx  = xx * xx;
		float yy = this->pose->position.gety() - spot.gety();
		yy = yy* yy;
		float zz = this->pose->position.getz() - spot.getz();
		zz = zz * zz;
		return std::sqrt(xx + yy + zz);
	}

	vertex3 seek(vertex3 motivation){
		vertex3 desired = motivation - this->pose->position;
		desired.normalize();
		desired = desired * maxSpeed;

		vertex3 steer = desired - this->velocity;
		steer.limit(maxForce);
		return steer;
	}

	void computeNeighbors(vector<BoidModel*> allBoids, float radius, int myIndex){
		//make better by onling looking ahead...
		neighbors.clear();
		for(unsigned int i=0; i<allBoids.size(); ++i){
			//TODO is this correct?
			//supposed to check that angle is not obtuse(I.E when boid is looking forward there is a boid in it's path)
			float d = distance(allBoids.at(i));
			if((d< 1.5) && (i != myIndex)){
				neighbors.push_back(allBoids.at(i));
			}			
		}
		//printf("THis boid has %d neighbors\n", neighbors.size());
	}

	vertex3 computeMotivation(vertex3 motivationPosition){
		//computes most obvious path
		return seek(motivationPosition);
	}

	vertex3 computeCohesion(){
		vertex3 sum = vertex3();
		int size = neighbors.size();
		for(unsigned int i=0; i<size; ++i){
			sum = sum + neighbors.at(i)->pose->position;
		}
		if(size > 0){
			sum = sum / size;
			return seek(sum);
		}else
			return vertex3();
	}

	vertex3 computeSeparation(){
		
		float desiredSeparation = 1.0f;
		vertex3 steer = vertex3();
		int count = 0;
		for(unsigned int i=0; i<neighbors.size(); ++i){
			float d = distance(neighbors.at(i));
			if((d > 0.0) && (d < desiredSeparation)){
				vertex3 diff = this->pose->position - neighbors.at(i)->pose->position;
				diff.normalize();
				diff = diff / d;
				steer = steer + diff;
				count++;
			}
		}
		if(count > 0){
			steer = steer / (float)count;
		}
		if(steer.getMagnitude() > 0.0){
			steer.normalize();
			steer = steer * maxSpeed;
			steer = steer - velocity;
			steer.limit(maxForce);
		}
		return steer;
	 }

	vertex3 computeAlignment(){
		//velocity matching
		vertex3 sum = vertex3();
		unsigned int size = neighbors.size();
		for(unsigned int i=0; i<size; ++i){
			sum = sum + neighbors.at(i)->velocity;
		}
		if(size > 0){
			sum = sum / size;
			sum.normalize();
			sum = sum * maxSpeed;
			vertex3 steer = sum - this->velocity;
			steer.limit(maxForce);
			return steer;
		}else{
			return vertex3();
		}
	}

	
	void computeNextLocation(vertex3 motivationPosition){
		vertex3 motivation = computeMotivation(motivationPosition) * 0.1f;
		vertex3 separation = computeSeparation() * 1.5f;
		vertex3 align = computeAlignment() * .75;
		vertex3 cohesion = computeCohesion() * 1.0;
		/*
		if(!separation.isApprox(vertex3())){
			acceleration = acceleration + separation;
		}
		else{
			acceleration = acceleration + align + cohesion + motivation;
		}
		*/
		acceleration = acceleration + motivation;
		acceleration = acceleration + separation + align + cohesion;

		
		//printf("Sep: (%f, %f), Align (%f, %f), Coh (%f, %f), Acc: (%f, %f)\n", separation.getx(), separation.gety(), align.getx(), align.gety(), cohesion.getx(), cohesion.gety(), acceleration.getx(), acceleration.gety());
	}

	virtual void update(){
		//computeNextLocation(motivationPosition); this is taken care of from Flock update
		velocity = velocity + acceleration;
		velocity.limit(maxSpeed);
		this->pose->position =  this->pose->position + velocity;
		float theta = (float)atan2(velocity.gety(), velocity.getx());
		//theta = theta * -1.0; if -y/x
		theta += 3.0*PI/2.0;
		while(theta > (2.0*PI)){
			theta -= 2.0*PI;
		}while(theta < 0.0){
			theta += 2.0*PI;
		}
		this->pose->orientation->setz(theta);
		acceleration = vertex3();
		this->updateTransformEuler();
		if(pose->position.vertex.hasNaN())
			printf("err\n");

		//printf("Pos (%f, %f) Vel (%f, %f), theta %f \n", this->pose->position.getx(),
		//	this->pose->position.gety(), velocity.getx(), velocity.gety(), theta);
	}

	Pose* getPose(){
		return this->pose;
	}

	vertex3 getAcceleration(){
		return this->acceleration;
	}

	void setPose(Pose* pose){
		this->pose = pose;
	}

	void setAcceleration(vertex3 acceleration){
		this->acceleration = acceleration;
	}

	protected:
	vector<BoidModel*> neighbors;
	//Since this simulation is in 2D
	//position z =0
	//orientation will be 0, 0, theta;
	//which is all encompassed in the Model paent
	vertex3 velocity;
	float radius;
	float maxForce;
	float maxSpeed;
	vertex3 acceleration;


	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif


#ifndef _Flock_H_
#define _Flock_H_

#include "Actor.h"
#include "vertex3.h"

class Flock{
	
public:
	vector<BoidModel*> boids;
	vertex3 motivationPosition;

	Flock(){
		createBoids(80, -2.0, 2.0, -5.0, -2.0);
		this->motivationPosition = vertex3();
	}
	Flock(vertex3 motivationPosition){
		createBoids(100, -15.0, 15.0, -13.0, 6.0);
		this->motivationPosition = motivationPosition;
	}

	void update(){
		//sense
		//create neighbors
		for(unsigned int i=0; i<boids.size(); ++i){				
			boids.at(i)->computeNeighbors(boids, 1.25, i);
			//plan
			boids.at(i)->computeNextLocation(motivationPosition);
		}
		//boids.at(leaderIndex)->setAcceleration(
		//	boids.at(leaderIndex)->getAcceleration() + 
		//	boids.at(leaderIndex)->computeMotivation(motivationPosition)*1.5);
		//act
		//printf("\n\n");
		for(unsigned int i=0; i<boids.size(); ++i){
			//printf("#%d : ", i);
			boids.at(i)->update();
		}
	}
	
	static float randBetween(float min, float max){
		float ret = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
		return ret;
	}

	static vertex3 randColor(){
		float x, y, z;
		x = randBetween(0.0, 1.0);
		y = randBetween(0.0, 1.0);
		z = randBetween(0.0, 1.0);
		return vertex3(x, y, z);
	}

	protected:
	void createBoids(int numBoids, float minXRange, float maxXRange, float minYRange, float maxYRange){
		for(unsigned int i=0; i<numBoids; i++){
			float x = randBetween(minXRange, maxXRange);
			float y = randBetween(minYRange, maxYRange);
			float z = 0.0;
			float theta = randBetween(0, 2.0*PI);
			vertex3 color = randColor();
			printf("New Boid at (%f, %f, %f) theta %f\n", x, y, z, theta);
			boids.push_back(new BoidModel(x,y,z, theta, color));
		}
	}
	
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif	//Flock