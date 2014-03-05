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
		//Camera* camera;
		int frame;

	Scene(){
		//camera = new Camera();
		frame = 0;
	}

	~Scene(){
		while(!actors.empty()) delete actors.back(), actors.pop_back();
	}

};

#endif