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

};

#endif