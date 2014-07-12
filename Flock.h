#ifndef _BOIDACTOR_H_
#define _BOIDACTOR_H_

#include "Actor.h"

class BoidActor{

public:
	BoidActor(){
		model = ModelView();
		base = 0.3;
		height = 0.4;
		model.loadPyramid(base, height);
		velocity = vertex3(0.1f, 0.1f, 0.1f);
	}

	BoidActor(float posx, float posy, float posz){
		model = ModelView();
		velocity = vertex3(0.01f, 0.01f, 0.01f);	//what do I set this to?
		pose = Pose();
		pose.position.set(posx, posy, posz);
		motivationPosition = vertex3(-5.0, 5.0, 0.0);
		base = 0.3;
		height = 0.4;
		model.loadPyramid(base, height);
	}

	BoidActor(float posx, float posy, float posz, float base, float height){
		model = ModelView();
		velocity = vertex3(0.1f, 0.1f, 0.1f);
		pose = Pose();
		pose.position.set(posx, posy, posz);
		motivationPosition = vertex3(0.0, 0.0, 5.0);
		base = 0.3;
		height = 0.4;
		model.loadPyramid(base, height);
	}

	~BoidActor(){
		
	}


	void computeNeighbors(vector<BoidActor*> allBoids, float radius){
		//make better by onling looking ahead...
		neighbors.clear();
		for(unsigned int i=0; i<allBoids.size(); i++){
			//TODO is this correct?
			//supposed to check that angle is not obtuse(I.E when boid is looking forward there is a boid in it's path)
			vertex3 distToPotential = allBoids.at(i)->pose.position - this->pose.position;
			float angle = this->pose.orientation->dotProduct(distToPotential);
		//	printf("Dot-- %f, me -- %f, %f, %f --dist %f, %f, %f\n", angle, pose.orientation.getx(), pose.orientation.gety(), pose.orientation.getz(), distToPotential.getx(), distToPotential.gety(), distToPotential.getz());
			if(angle < 0){
				//if(this->distance(allBoids.at(i)) < radius)	//this checks that it's not REALLY far away
					neighbors.push_back(allBoids.at(i));
			}
		}
		//printf("THis boid has %d neighbors\n", neighbors.size());
	}

	Pose& computeNextLocation(){
		int numNeighbors = neighbors.size();
		vertex3 cohesion = computeCohesion();		//suggesion 0
		vertex3 separation = computeSeparation();	//suggestion 1
		vertex3 alignment = computeAlignment();		//suggestion 2
		vertex3 motivation = computeMotivation();	//suggestion 3
		vertex3 suggestions[4] = {cohesion, separation, alignment, motivation};
		int suggestion = 0;
		
		if(separation.isApprox(vertex3(0.0, 0.0, 0.0)) ){
			suggestion = 3;
			if(numNeighbors < 3 && !cohesion.isApprox(motivation)){
				suggestion = 0;
				velocity = vertex3(0.088, 0.008, 0.0);	
			}else if(numNeighbors > 2 && !alignment.isApprox(motivation)){
				suggestion = 2;
				velocity = vertex3(0.01, 0.01, 0.0);
			}
		}
		else {
			suggestion = 3;
			velocity = vertex3(0.1, 0.1, 0.08);
		}
		
		 vertex3 futurePos = vertex3();
	
		// printf("Suggesion is %d (%f, %f, %f)\n", suggestion, suggestions[suggestion].getx(),suggestions[suggestion].gety(), suggestions[suggestion].getz() );
		 futurePos =  moveAlongDirection(suggestions[suggestion]);

		 //printf("Current (%f, %f, %f)-- Future (%f, %f, %f)\n\n", this->pose.position.getx(), this->pose.position.gety(), this->pose.position.getz(), futurePos.getx(), futurePos.gety(), futurePos.getz());
		//return pose;
		Pose* next = new Pose(futurePos, suggestions[suggestion]);
		//printf("This boid has %d neighbors and is moving towards", numNeighbors);
		//suggestions[suggestion].print();
		return *next;
	};

	vertex3 computeCohesion(){
		if(neighbors.size() > 0){
			vertex3 perceivedCenter;
			vector<vertex3> futurePositions;
			futurePositions.resize(neighbors.size());
			for(unsigned int i=0; i<neighbors.size(); i++){
				vertex3 direction = neighbors.at(i)->computeMotivation();
				futurePositions.at(i) = neighbors.at(i)->moveAlongDirection(direction);
				perceivedCenter = perceivedCenter + futurePositions.at(i);
			}
			perceivedCenter = perceivedCenter / (float)neighbors.size();
			vertex3 diff =  this->pose.getEulerRepresentation() - perceivedCenter;
			//diff.normalize();
			//diff.print();
			return diff;
		}else{
			return computeMotivation();
		}
	}

	vertex3 moveAlongDirection(vertex3 direction){
		//TODO  - check that this is correct!
		direction.normalize();
		float futureX = this->pose.position.getx() + (direction.getx() * this->velocity.getx());
		float futureY = this->pose.position.gety() + (direction.gety() * this->velocity.gety());
		float futureZ = this->pose.position.getz() + (direction.getz() * this->velocity.getz());
		//printf("future is (%f, %f, %f)\n", futureX, futureY, futureZ);
		return vertex3(futureX, futureY, futureZ);
	}

	float getRadius() const {
		return sqrt((base/2 * base/2) + (height/2 * height/2));
	}

	bool boidsIntersect(vertex3 myBoidPos, vertex3 otherBoidPos, float otherBoidRadius){
		//create a circle bounding box on the fly
		float distX = otherBoidPos.getx() - myBoidPos.getx();
		float distY = otherBoidPos.gety() - myBoidPos.gety();

		float magnitude = sqrt((distX * distX) + (distY * distY));
		if(magnitude < (this->getRadius() + otherBoidRadius))
			return true;
		else
			return false;
	}

	vertex3 computeMotivation(){
		//find food
		//compare orientation towards motivation
		vertex3 dist = motivationPosition - this->pose.position;
		return dist;
	}
	
	//why doesn't boidActor know about it's own methods?
	vertex3 computeSeparation(){
		//computes most obvious path
		vertex3 myFuturePos = moveAlongDirection(computeMotivation());
		//stores stupid future positions of all neighbors
		vector<vertex3> futurePositions;
		futurePositions.resize(neighbors.size());
		for(unsigned int i=0; i<neighbors.size(); i++){
			vertex3 direction = neighbors.at(i)->computeMotivation();
			futurePositions.at(i) = neighbors.at(i)->moveAlongDirection(direction);
			if(boidsIntersect(myFuturePos, futurePositions.at(i), neighbors.at(i)->getRadius())){
				//reduce speed
				this->velocity = this->velocity * .8;
				return computeMotivation();
			}
		}
		return vertex3();
	 }

	vertex3 computeAlignment(){
		//velocity matching
		//ignore for now
		vertex3 averageDirection = vertex3();
		for(unsigned int i=0; i<neighbors.size(); i++){
			averageDirection = averageDirection + neighbors.at(i)->pose.getEulerRepresentation();
		}
		averageDirection = averageDirection/ neighbors.size();
		return averageDirection - this->pose.getEulerRepresentation();
	}

	float distance(BoidActor* potentialneighbor){
		//d(p, q) = \sqrt{(p_1 - q_1)^2 + (p_2 - q_2)^2+(p_3 - q_3)^2}.
		float xx = this->pose.position.getx() * potentialneighbor->pose.position.getx();
		float yy = this->pose.position.gety() * potentialneighbor->pose.position.gety();
		float zz = this->pose.position.getz() * potentialneighbor->pose.position.getz();
		return std::sqrt(xx + yy + zz);
	}

	Pose& getPose(){
		return this->pose;
	}

	ModelView& getModel(){
		return this->model;
	}

	void setPose(Pose& pose){
		this->pose = pose;
	}

	protected:
	vector<BoidActor*> neighbors;
	Pose pose;
	float base, height;
	ModelView model;
	vertex3 velocity;
	vertex3 motivationPosition;


	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif


#ifndef _Flock_H_
#define _Flock_H_

#include "Actor.h"

class Flock: public Actor{
	
public:
	vector<BoidActor*> boids;

	Flock(){
		createBoids(10, -2.0, 2.0);
	}

	Pose& update(){
		//sense
		//create neighbors
		for(unsigned int i=0; i<boids.size(); i++){
			boids.at(i)->computeNeighbors(boids, 0.5);
		}
		//plan
		vector<Pose*> computedFutures;
		for(unsigned int i=0; i<boids.size(); i++){
			computedFutures.push_back(&boids.at(i)->computeNextLocation());
			vertex3 direction = computedFutures.back()->getEulerRepresentation();
			//computes the angle between direction and up vector
			double theta = atan2(direction.gety(), direction.getx()) - atan2(1.0, 0.0);
			//corrects  in order to have a "normalized" angle
			while(theta <= -PI){
				theta += 2*PI;
			}
			while(theta > PI){
				theta -= 2* PI;
			}
			computedFutures.back()->orientation = new vertex3(0.0, 0.0, (float)theta);
		}

		//act
		for(unsigned int i=0; i<boids.size(); i++){
			boids.at(i)->setPose(*computedFutures.at(i));
		}
		//return pose leader??
		Pose* fake = new Pose();
		return *fake;
	}

protected:
	float randBetween(float min, float max){
		float ret = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
		return ret;
	}

	void createBoids(int numBoids, float minRange, float maxRange){
		for(unsigned int i=0; i<numBoids; i++){
			float x = randBetween(minRange, maxRange);
			float y = randBetween(minRange, maxRange);
			float z = 0.0;
			printf("New Boid at (%f, %f, %f)\n", x, y, z);
			boids.push_back(new BoidActor(x,y,z));
		}
	}
	
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif	//Flock