#pragma once
#include "Actor.h"
#include "Camera.h"

class Actor{
	public:
		vector<Actor> actors;
		Camera camera;
		//time?

	Actor(){
		camera = Camera();
	}

	void drawScene(){
		//TODO

		//do something with camera
		//GL_PROJECTION()
		//camera stuff


		//Then draw all of the actors
		int i;
		for(i=0; i<actors.size(); i++){
		//	actors.at(i).draw(time);
		}
	}
};