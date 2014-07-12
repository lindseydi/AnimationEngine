
#ifndef _POLYGON_H_
#define _POLYGON_H_

#pragma once

#include "vertex3.h"
#include <vector>

enum PolygonType{point = 1, line, triangle, quad, pentagon };

class polygon {
public:
	
	PolygonType type;
	vector<int> vertexIndices;	//how to save indices...TODO
	vertex3 center;
	vertex3 normal;
	float d;	///given point on the plane

	polygon(){
		//default
	}

	polygon(const polygon& poly){
		type = poly.type;
		vertexIndices = poly.vertexIndices;
		normal = poly.normal;
		d = poly.d;
	}

	polygon(vector<int> vertexIndices){
		this->vertexIndices.clear();
		for(unsigned int i=0; i < vertexIndices.size(); i++){
			this->vertexIndices.push_back(vertexIndices.at(i));
		}
		type = (PolygonType)vertexIndices.size();
		//calcNorm();
	}

	polygon(int a, int b, int c, const vertex3& normal){
		vertexIndices.push_back(a);
		vertexIndices.push_back(b);
		vertexIndices.push_back(c);
		type = triangle;
		this->normal = normal;
		//calcNorm();
	}

	polygon(int a, int b, int c, int d){
		vertexIndices.push_back(a);
		vertexIndices.push_back(b);
		vertexIndices.push_back(c);
		vertexIndices.push_back(d);
		type = quad;
		//calcNorm();
	}

	polygon(int a, int b, int c, int d, const vertex3& normal){
		vertexIndices.push_back(a);
		vertexIndices.push_back(b);
		vertexIndices.push_back(c);
		vertexIndices.push_back(d);
		type = quad;
		this->normal = normal;
		//calcNorm();
	}

	polygon(int a, int b, int c, int d, const vertex3& normal, float dVal){
		vertexIndices.push_back(a);
		vertexIndices.push_back(b);
		vertexIndices.push_back(c);
		vertexIndices.push_back(d);
		type = quad;
		this->normal = normal;
		this->d = dVal;
		//calcNorm();
	}

	void operator=(const polygon& poly){
		this->d = poly.d;
		this->normal = poly.normal;
		this->type = poly.type;
		this->vertexIndices = poly.vertexIndices;
	}
	
	int getType (void){
		type = (PolygonType)vertexIndices.size();	//for extra safety
		return type;
	}

	void setNormal(const vertex3& normal){
		this->normal = normal;
		getType();
	}

	void flip( void ) {
		std::vector<int> tempvertices;
		unsigned int m_vertexCount = vertexIndices.size();
		for( unsigned int i = 0; i < m_vertexCount; i++ ) {
			tempvertices.push_back( vertexIndices.at( i ) );
		}
		vertexIndices.erase( vertexIndices.begin( ), vertexIndices.end() );
		for( unsigned int i = m_vertexCount; i > 0; i-- ) {
			vertexIndices.push_back( tempvertices.at( i - 1 ) );
		}
		normal = normal * -1.0;
	}

	void calculateD(const vertex3& anyPoint){
		d = normal.dotProduct(anyPoint);
	}

	//deprecated
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

	void addVertex(int index){
		vertexIndices.push_back(index);
	}

	vertex3 getCenter(){
		if(type == triangle){
			return vertex3();
		}
		else if(type == quad){
			
		}
	}

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif  // _POLYGON_H_