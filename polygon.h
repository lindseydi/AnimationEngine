
#ifndef _POLYGON_H_
#define _POLYGON_H_

#pragma once

#include <boost/shared_ptr.hpp>

#include <vector>

enum PolygonType{point = 1, line, triangle, quad, pentagon };

class polygon {
public:
	
	PolygonType type;
	vector<int> vertexIndices;	//how to save indices...TODO
	vertex3 normal;

	polygon(){
		//default
	}

	polygon(const polygon& poly){
		//copy constructor
	}

	polygon(vector<int> vertexIndices){
		this->vertexIndices.clear();
		for(unsigned int i=0; i < vertexIndices.size(); i++){
			this->vertexIndices.push_back(vertexIndices.at(i));
		}
		type = (PolygonType)vertexIndices.size();
		calcNorm();
	}

	polygon(int a, int b, int c){
		vertexIndices.push_back(a);
		vertexIndices.push_back(b);
		vertexIndices.push_back(c);
		type = triangle;
	}

	polygon(int a, int b, int c, int d){
		vertexIndices.push_back(a);
		vertexIndices.push_back(b);
		vertexIndices.push_back(c);
		vertexIndices.push_back(d);
		type = quad;
	}
	
	int getType (void){
		type = (PolygonType)vertexIndices.size();	//for extra safety
		return type;
	}

	void calcNorm()
	{
		//true for any plane
		//how do I use the methods created before now?
		vertex3 edge1 = vertexIndices.at(1) - vertexIndices.at(0);
			edge1.normalize();
			vertex3 edge2 = vertexIndices.at(2) - vertexIndices.at(0);
			edge2.normalize();
			vertex3 local_normal = edge1.cross(edge2);
			local_normal.normalize();
			this->normal = local_normal;
	}
};

#endif  // _POLYGON_H_