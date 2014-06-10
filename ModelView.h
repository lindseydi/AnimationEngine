
#ifndef _MODELVIEW_H_
#define _MODELVIEW_H_


#pragma once

//#include <boost/shared_ptr.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include <math.h>
#include <Windows.h>
#include <iostream>
#include <istream>
#include <string.h>
#include <fstream>
#include <string>
#include <vector>

#include "polygon.h"
#include "vertex3.h"

using namespace std;

#define PI 3.14159265

//typedef boost::shared_ptr<vertex3> vertex_ptr;

//class polygon;

class ModelView
{
public:
	
	vector<vertex3> vertices;
	vector<polygon> edges;
	vector<vertex3> normals;	//necessary but difficult to implement

	ModelView(void){
		vertices.clear();
		edges.clear();
		normals.clear();
	}
	
	ModelView(string filename){
		read_file(filename);
	}

	ModelView(int vert_count, int edge_count){
		//vertices.resize(vert_count);			//how to size a shared_ptr?
		normals.resize(vert_count);
		edges.resize(edge_count);
	}

	~ModelView(){
		//Necessary?
	}

	void clear(){
		this->vertices.clear();
		this->edges.clear();
		this->normals.clear();
	}

	void operator=(const ModelView& mesh){
		vertices = mesh.vertices;
		edges = mesh.edges;
		normals = mesh.normals;
	}

	void loadBox(float length, float width, float height){
		clear();
		//create polygons and vertices
		float halfLen = length/2;
		float halfWidth = width/2;
		float halfHeight = height/2;
		vertices.push_back(vertex3(-halfLen, -width/2, -height/2));
		vertices.push_back(vertex3(length/2, -width/2, -height/2));
		vertices.push_back(vertex3(length/2, -width/2, height/2));
		vertices.push_back(vertex3(-length/2, -width/2, height/2));

		vertices.push_back(vertex3(-length/2, width/2, -height/2));
		vertices.push_back(vertex3(length/2, width/2, -height/2));
		vertices.push_back(vertex3(length/2, width/2, height/2));
		vertices.push_back(vertex3(-length/2, width/2, height/2));	

		//TODO fix error
		edges.push_back(polygon(0, 1, 2, 3, calcTriangleNorm(0, 1, 2)));
		edges.push_back(polygon(0, 1, 5, 4, calcTriangleNorm(0, 1, 5)));
		edges.push_back(polygon(0, 3, 7, 4, calcTriangleNorm(0, 3, 7)));
		edges.push_back(polygon(6, 7, 4, 5, calcTriangleNorm(6, 7, 4)));
		edges.push_back(polygon(6, 5, 1, 2, calcTriangleNorm(6, 5, 1)));
		edges.push_back(polygon(6, 7, 3, 2, calcTriangleNorm(6, 7, 3)));

		//if needed for complex bounding, iterate through egdges and compute D for each polygon
	}

	void loadPlane(float xLen, float yLen, float zLen, char constantDir, bool flip){
		clear();
		//At least 1 len should be constant, and that is denoted with the fourth parameter
		float halfX = xLen/2.0;
		float halfY = yLen/2.0;
		float halfZ = zLen/2.0;
		vertex3 a, b, c, d;
		switch(constantDir){
			case 'x':
				a = vertex3(xLen, -halfY, -halfZ);
				b = vertex3(xLen, -halfY,  halfZ);
				c = vertex3(xLen,  halfY, halfZ);
				d = vertex3(xLen,  halfY,  -halfZ);
			break;
				
			case 'y' :
				a = vertex3(-halfX, yLen, halfZ);
				b = vertex3( halfX, yLen,  halfZ);
				c = vertex3( halfX, yLen, -halfZ);
				d = vertex3(-halfX,  yLen, -halfZ);
			break;

			case 'z':
				a = vertex3(-halfX, -halfY, zLen);
				b = vertex3( halfX,  -halfY, zLen);
				c = vertex3( halfX, halfY, zLen);
				d = vertex3(-halfX,  halfY, zLen);
			break;
		}
		vertices.push_back(a);
		vertices.push_back(b);
		vertices.push_back(c);
		vertices.push_back(d);

		vertex3 normal = calcTriangleNorm(0, 1, 2);
		//for(int i=0; i<4; i++)
		//	normals.push_back(normal);

		float dValue = a.dotProduct(normal);
		polygon p = polygon(0, 1, 2, 3, normal, dValue);
		if(flip){
			normal = normal* -1.0;
			p.flip();
		}
		//p.calculateD(a);
		edges.push_back(p);
	}
	
	void loadSphere(const float radius, const unsigned int arcSegments, const int slices){
		clear();
		float dtheta = 2.0 * PI / (float) arcSegments;
		float theta = 0.0f;
		float dphi = (float) PI / (float) (slices + 1);
		float phi = 0.0f;
		float x, y, z;

		//generate vertices
		//north pole
		vertices.push_back(vertex3(0.0f, radius, 0.0f));
		//south pole
		vertices.push_back(vertex3(0.0f, -radius, 0.0f));

		//slice verts
		for(unsigned int i=0; i< slices; i++){
			theta = 0.0;
			phi += dphi;
			for(unsigned int i=0;  i < arcSegments; i++){
				x = sin(phi) * cos(theta);
				y = cos(phi);
				z = sin(phi) * sin(theta);
				theta += dtheta;
				vertices.push_back(vertex3(x*radius, y* radius, z * radius));
			}
		}

		//generate polygons
		for( unsigned int slice = 0; slice < slices + 1; slice++ ) {
	        for( unsigned int i = 0; i < arcSegments; i++ ) {
				polygon p = polygon();
				if(slice == 0){
					//top cap - triads(?)
					p.addVertex(0);
					 if( i < arcSegments - 1 )
					   p.addVertex( i + 2 );
					 else
					   p.addVertex( 1 );
					p.addVertex( i + 1 );
			   } else if( slice > 0 && slice < slices ) {
					// Quads
					p.addVertex( arcSegments * (slice - 1) + i + 1 );
					if( i < arcSegments - 1 ) {
						p.addVertex( arcSegments * (slice - 1) + i + 2 );
						p.addVertex( arcSegments * (slice) + i + 2 );

					} else {
						p.addVertex( arcSegments * (slice - 1) + 1 );
						p.addVertex( arcSegments * (slice) + 1 );
					}
					p.addVertex( arcSegments * (slice) + i + 1 );
				} else if( slice == slices ) {
					// Bottom Cap - Triads
					p.addVertex( arcSegments * (slice - 1) + i + 1 );
					if( i < arcSegments - 1 ) {
						p.addVertex( arcSegments * (slice - 1) + i + 2 );
					} else {
						p.addVertex( arcSegments * (slice - 1) + 1 );
					}
					p.addVertex( this->vertices.size() - 1 );
				}
				p.setNormal(calcTriangleNorm(p));
				p.flip();      // the sphere is wrong handed for OpenGL. Flip here solves for all polys
				edges.push_back(p);
			}
		}
	}

	void loadPyramid(float base, float height){
		clear();
		float halfBase = base/2;
		float halfHeight = height/2;
		vertices.push_back(vertex3(0.0, halfHeight, 0.0));
		vertices.push_back(vertex3(-halfBase, -halfHeight, -halfBase));
		vertices.push_back(vertex3(halfBase, -halfHeight, -halfBase));
		vertices.push_back(vertex3(halfBase, -halfHeight, halfBase));
		vertices.push_back(vertex3(-halfBase, -halfHeight, halfBase));

		edges.push_back(polygon(1, 2, 3, 4, calcTriangleNorm(1, 2, 3)));
		edges.push_back(polygon(1, 2, 0, calcTriangleNorm(1, 2, 0)));
		edges.push_back(polygon(2, 3, 0, calcTriangleNorm(2, 3, 0)));
		edges.push_back(polygon(3, 4, 0, calcTriangleNorm(3, 4, 0)));
		edges.push_back(polygon(4, 1, 0, calcTriangleNorm(4, 1, 0)));



		//if needed for complex bounding, iterate through egdges and compute D for each polygon
	}

vertex3 calcTriangleNorm(vertex3 vec0, vertex3 vec1, vertex3 vec2)
{
	vertex3 edge1 = vec1 - vec0;
	edge1.normalize();
	vertex3 edge2 = vec2 - vec0;
	edge2.normalize();
	vertex3 normal = edge1.cross(edge2);
	normal.normalize();
	return normal;
}


vertex3 calcTriangleNorm(polygon poly){
	vertex3 edge1 = vertices.at(poly.vertexIndices.at(1)) - vertices.at(poly.vertexIndices.at(0));
	edge1.normalize();
	vertex3 edge2 = vertices.at(poly.vertexIndices.at(2)) - vertices.at(poly.vertexIndices.at(0));
	edge2.normalize();
	vertex3 local_normal = edge1.cross(edge2);
	local_normal.normalize();
	return local_normal;
}

vertex3 calcTriangleNorm(int a, int b, int c){
	vertex3 edge1 = vertices.at(b) - vertices.at(a);
	edge1.normalize();
	vertex3 edge2 = vertices.at(c) - vertices.at(a);
	edge2.normalize();
	vertex3 local_normal = edge1.cross(edge2);
	local_normal.normalize();
	return local_normal;
}

	void read_file(string filename)
{
	clear();
	int vert_count;
	int i;
	ifstream inFile;  
	inFile.open(filename);
	if (!inFile) {
		cerr << "Can't open input file "  << endl;
		//exit(1);
	}
	string str;
	int edge_count;
	inFile >> str;
	inFile >> str;
	vert_count = atoi(str.c_str());
	//vertex3 *vertices = ((vertex3*)  malloc(vert_count*sizeof(vertex3)));
	inFile >> str;
	edge_count = atoi(str.c_str());
	float x, y, z;


	vertices.resize(vert_count);
	normals.resize(vert_count);
	//no i necessary
	for(i=0; i<vert_count; i++){
		inFile >> str;
		x = (GLfloat) atof( str.c_str());
		inFile >> str;
		y = (GLfloat) atof(str.c_str());
		inFile >> str;
		z = (GLfloat) atof(str.c_str());
		vertices.push_back(vertex3(x, y, z));
		//vertices.at(i) = vertex3(x, y, z);
	}
	edges.resize(edge_count);
	for(i=0; i<edge_count; i++){
		inFile >> str;
		int numEdges = atoi(str.c_str());
		//triangle
		if(numEdges == 3){
			inFile >> str;
			int a = atoi(str.c_str()) - 1;
			inFile >> str;
			int b = atoi(str.c_str()) - 1;
			inFile >> str;
			int c = atoi(str.c_str()) -1;

			//Put this information into the global vector edges
			//TODO learn about pointers
			polygon poly = polygon(a, b, c, calcTriangleNorm(a, b, c));
			edges[i] = poly;	//interesting
			normals.at(a) = normals.at(a) + poly.normal;	//replaces generatenormals function
		}
		//rectangle
		if(numEdges == 4){
			 inFile >> str;
 			int a = atoi(str.c_str()) - 1;
			inFile >> str;
			int b = atoi(str.c_str()) - 1;
			inFile >> str;
			int c = atoi(str.c_str()) -1;
			inFile >> str;
			int d = atoi(str.c_str()) -1;

			polygon poly = polygon(a, b, c, d);
			edges[i] = poly;
			normals.at(a) = normals.at(a) + poly.normal;
		}
	}

}

void texture_object(int m){
	GLuint m_wall_texture_id = m;
	glEnable( GL_TEXTURE_2D );

	glGenTextures(1, &m_wall_texture_id);
	glBindTexture( GL_TEXTURE_2D, m_wall_texture_id);
}

/*
void draw_object(float obj_colorR, float obj_colorG, float obj_colorB)
{
	int i;
	int firstv;
	int secondv;
	int thirdv;
	int fourthv;
	int typepoly;
	
	glEnable(GL_COLOR_MATERIAL); //Enable color
	glColor3f(obj_colorR, obj_colorG, obj_colorB);
	 for (i=0 ; i < edges.size(); i++){
		 typepoly = edges[i].getType();
		 if(typepoly == 3){
			firstv = edges[i].geta();
			secondv = edges[i].getb();
			thirdv = edges[i].getc();
	 		glBegin (GL_TRIANGLES);
				glNormal3fv(normals[firstv].getPointer());
				glVertex3fv (vertices[firstv].getPointer());
				glNormal3fv(normals[secondv].getPointer());
				glVertex3fv (vertices[secondv].getPointer());
				glNormal3fv(normals[thirdv].getPointer());
				glVertex3fv (vertices[thirdv].getPointer());
		}
		if(typepoly == 4){
	 		glBegin (GL_QUADS);
			firstv = edges[i].geta();
			secondv = edges[i].getb();
			thirdv = edges[i].getc();
			fourthv = edges[i].getd();
				glColor3f(obj_colorR, obj_colorG, obj_colorB);
				glNormal3fv(normals[firstv].getPointer());
				glTexCoord2f( 0.0f, 0.0f );
				glVertex3fv (vertices[firstv].getPointer());
				glNormal3fv(normals[secondv].getPointer());
				glTexCoord2f(1.0f, 0.0f);
				glVertex3fv (vertices[secondv].getPointer());
				glNormal3fv(normals[thirdv].getPointer());
				glTexCoord2f(1.0f, 1.0f);
				glVertex3fv (vertices[thirdv].getPointer());
				glNormal3fv(normals[fourthv].getPointer());
				glTexCoord2f(0.0f, 1.0f);
				glVertex3fv (vertices[fourthv].getPointer());
				vertices[firstv].print();
				vertices[secondv].print();
				vertices[thirdv].print();
				vertices[fourthv].print();
		 }
		glEnd();
	}
	glDisable( GL_TEXTURE_2D );

	//To draw vertex normals
	//For debugging purposes
    //#if 0
	glBegin (GL_LINES);
	for (i=0 ; i < vertices.size(); i++){
		glColor3f( 1.0, 0.0, 1.0 );
		glVertex3fv (vertices[i].getPointer());
		glVertex3fv( (vertices[i] + normals[i] * .5f ).getPointer());
	}
    glEnd();
    //#endif
}
	*/
};

#endif