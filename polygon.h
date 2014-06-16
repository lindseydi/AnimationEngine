
#ifndef _POLYGON_H_
#define _POLYGON_H_

#pragma once

#include <vector>
#include <Eigen/Geometry>

enum PolygonType{point = 1, line, triangle, quad, pentagon };

class polygon {
public:
	
	PolygonType type;
	vector<int> vertexIndicies;
	vector<vertex3*> verts;	//how to save indices...TODO
	vertex3 normal;
	float d;	///given point on the plane

	polygon(){
		//default
	}

	polygon(const polygon& poly){
		type = poly.type;
		verts = poly.verts;
		normal = poly.normal;
		d = poly.d;
	}

	polygon(vector<vertex3*> verts){
		this->verts.clear();
		for(unsigned int i=0; i < verts.size(); i++){
			this->verts.push_back(verts.at(i));
		}
		type = (PolygonType)verts.size();
		calcNorm();
		calculateD();
	}

	polygon(vertex3& a, vertex3& b, vertex3& c){
		verts.push_back(&a);
		verts.push_back(&b);
		verts.push_back(&c);
		type = triangle;
		calcNorm();
		calculateD();
	}

	polygon(vertex3& a, vertex3& b, vertex3& c, vertex3& d){
		verts.push_back(&a);
		verts.push_back(&b);
		verts.push_back(&c);
		verts.push_back(&d);
		type = quad;
		calcNorm();
		calculateD();
	}

	void operator=(polygon& poly){
		this->d = poly.d;
		this->normal = poly.normal;
		this->type = poly.type;
		this->verts = poly.verts;
	}
	
	int getType (void){
		type = (PolygonType)verts.size();	//for extra safety
		return type;
	}

	void setNormal(vertex3 normal){
		this->normal = normal;
		getType();
	}

	void flip( void ) {
		std::vector<vertex3*> tempvertices;
		int m_vertexCount = verts.size();
		for( unsigned int i = 0; i < m_vertexCount; i++ ) {
			tempvertices.push_back( verts.at( i ) );
		}
		verts.erase( verts.begin( ), verts.end() );
		for( unsigned int i = m_vertexCount; i > 0; i-- ) {
			verts.push_back( tempvertices.at( i - 1 ) );
		}
		//necessary?
		normal = normal * -1.0;
	}

	void calculateD(vertex3 anyPoint){
		d = normal.dotProduct(anyPoint);
	}

	void calculateD(){
		vertex3 anyPoint = *verts.front();
		d = normal.dotProduct(anyPoint);
	}

	//deprecated
	//Will it still work?
	void calcNorm()
	{
		//true for any plane
		//how do I use the methods created before now?
			vertex3 edge1 = verts.at(1) - verts.at(0);
			edge1.normalize();
			vertex3 edge2 = verts.at(2) - verts.at(0);
			edge2.normalize();
			vertex3 local_normal = edge1.cross(edge2);
			local_normal.normalize();
			this->normal = local_normal;
	}

	void addVertex(vertex3& index){
		verts.push_back(&index);
	}

	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif  // _POLYGON_H_