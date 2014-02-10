#pragma once


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

using namespace std;

#define PI 3.14159265

class ModelView
{
	vector <Vector3f> vertices;
	vector <polygon> edges;
	vector <Vector3f> normals;

public:
	ModelView(void){
		vertices.clear();
		edges.clear();
		normals.clear();
	}

	ModelView(string filename){
		read_file(filename);
	}

	ModelView(int vert_count, int edge_count){
		vertices.resize(vert_count);
		normals.resize(vert_count);
		edges.resize(edge_count);
	}

	Vector3f calcTriangleNorm(Vector3f vec0, Vector3f vec1, Vector3f vec2)
{
	Vector3f edge1 = vec1 - vec0;
	edge1.normalize();
	Vector3f edge2 = vec2 - vec0;
	edge2.normalize();
	Vector3f normal = edge1.cross(edge2);
	normal.normalize();
	return normal;
}

void generatenormals(void){
	int i;
	for(i=0; i<vertices.size(); i++){
		normals[i].x =0.0f;
		normals[i].y = 0.0f;
		normals[i].z = 0.0f;
	}
	
	for(i=0; i<edges.size(); i++){
		int type = edges[i].getType();
		int firstv = edges[i].geta();
		int secondv = edges[i].getb();
		int thirdv = edges[i].getc();
		int fourthv = edges[i].getd();
		Vector3f normal = calcTriangleNorm(vertices[firstv], vertices[secondv], vertices[thirdv]);
			normals[firstv] = normals[firstv] + normal;
			normals[secondv] = normals[secondv] + normal;
			normals[thirdv] = normals[thirdv] + normal;
			normals[fourthv] = normals[fourthv] + normal;
	}
	for(i=0; i<vertices.size(); i++){
		normals[i].normalize();
	}
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
	//Vector3f *vertices = ((Vector3f*)  malloc(vert_count*sizeof(Vector3f)));
	inFile >> str;
	edge_count = atoi(str.c_str());
	float x, y, z;


	vertices.resize(vert_count);
	normals.resize(vert_count);
	for(i=0; i<vert_count; i++){
		inFile >> str;
		x = (GLfloat) atof( str.c_str());
		inFile >> str;
		y = (GLfloat) atof(str.c_str());
		inFile >> str;
		z = (GLfloat) atof(str.c_str());
		Vector3f vertex(x, y, z);
		vertices[ i ] = vertex;
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

			//Put this information into the global vector connect
			polygon poly(a, b, c);
			edges[i] = poly;
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

			polygon poly(a, b, c, d);
			edges[i] = poly;
		}
	}

	generatenormals();
}

void texture_object(int m){
	GLuint m_wall_texture_id = m;
	glEnable( GL_TEXTURE_2D );

	glGenTextures(1, &m_wall_texture_id);
	glBindTexture( GL_TEXTURE_2D, m_wall_texture_id);
}

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
	
	/*
	if(chess){
	//Rotate back to normal and draw a teapot
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(-transX, -transY, -transZ);
	}
	*/
	//glTranslatef(0,-8,-4.5);
	//glutSolidTeapot(2);
}		
};

