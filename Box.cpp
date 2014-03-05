#pragma once
#include <stdlib.h>
#include "Pose.h"
// standard
#include <assert.h>
#include <math.h>

// glut

#include <stdio.h>
#include <sys/stat.h>

//headers

#include "polygon.h"
//#include "Node.h"
#include "Renderer.h"
#include "matrix3.h"
#include "matrix4.h"
#include "Trajectory.h"
#include "Scene.h"
#include "Modelview.h"

#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <GL/glu.h>
using namespace std;

//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

Scene* scene;

double u;
GLfloat PofU[7];

//================================
// init
//================================
void init(void) {
	// init something before main loop...
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	//responsible for global timing in the scene
	//g_frameIndex = 0;
	scene = new Scene();
	//Actor* actor = new Actor(ModelView("resources/knight.txt"));
	Actor* actor = new Actor();
	scene->actors.push_back(actor);
}

Trajectory& createKeyFrames(void){
	Trajectory* path = new Trajectory();
	return *path;
}

//================================
// render
//================================
void render( void ){
	Renderer::draw(scene);
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
	// screen size
	g_screenWidth  = w;
	g_screenHeight = h;	
	
	// viewport
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
}

//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {	
	// increase frame index
	g_frameIndex++;
	//update();
	
	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );
}


void cleanup(){
	delete scene;
}
/*
//================================
// main
//================================
int main( int argc, char** argv ) {

	printf( "Hello World\n" );

	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize( 500, 500 ); 
	glutInitWindowPosition( 100, 100 );
	glutCreateWindow( "Interpolation Example" );

	// init
	init();
	  
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	//glutMouseFunc(glutMouseFunc);
	//glutMotionFunc(MouseMotion);
	glutTimerFunc( 16, timer, 0 );

	// main loop
	glutMainLoop();
	
	cleanup();

	return 0;
}
*/