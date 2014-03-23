
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

	void loadBox(float length, float width, float height){
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
			polygon poly = polygon(a, b, c);
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