#pragma once;
#include "Node.h"

class Joint{
	public:
		vector<Node*> children;
		int numChildren;

		//assuming no position
		//but rotation?

	Joint(){
		numChildren = 0;
	}

	void rotate(Vector3f rotation){
		//cast to quaternion
	}

	void rotate(Quaternionf rotation){

	}

	void addNode(Node* newChild){
		children.push_back(newChild);
		int numChildren = children.size();
	}

	bool isNull(){
		if(numChildren != children.size()){
			numChildren = children.size();
		}			
		if(numChildren == 0)
			return true;
		else
			return false;
	}

};