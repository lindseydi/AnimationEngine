#ifndef _SCENE_H_
#define _SCENE_H_


#pragma once
#include "Actor.h"
//#include "Camera.h"

class Scene{
	private:
			int total_frames;
	public:
		vector<Actor*> actors;
		Flock* flock;
		//Camera* camera;
		LinkRoot* hierarchy;
		int frame;
		vector<Model*> models;

	Scene(){
		//camera = new Camera();
		frame = 0;
		flock = NULL;
		hierarchy = NULL;
	}

	~Scene(){
		while(!actors.empty()) delete actors.back(), actors.pop_back();
	}

	void update(Link* current){
		current->update();
		/*
		for(int i=0; i< current->children.size(); ++i){
			update(current->children.at(i)->child);
		}
		*/
	}

	void update(){
		for(unsigned int i=0; i<models.size(); ++i)
			models.at(i)->update();
		if(	this->flock != NULL)
			this->flock->update();
	}

	void update(double time, double dt){
		for(unsigned int i=0; i<models.size(); ++i)
			models.at(i)->update(time, dt);
	}

	
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif