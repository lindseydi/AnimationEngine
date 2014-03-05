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

/*
void drawHierarchy(Node* head, matrix3 transform){

	Node* ptr = head;
	matrix3 T = transform;
	printf("\nPrinting Transform parameter from drawHierarchy");
	transform.print();
		//T builds on itself with each iteration
		matrix3 V(4,1);
		V(0, 0) = ptr->V.x;
		V(1, 0) = ptr->V.y;
		V(2, 0) = ptr->V.z;			//V(2,0) = -10.0;
		V(3, 0) = 1.0;

		matrix3 temp = T * V;
		ptr->V = temp.toVector3f();

	glPushMatrix();			
			matrix3 temp2 = helper::pivot_object(ptr->articulation.getx(), ptr->articulation.gety(), ptr->articulation.getz(), ptr->V);
			temp2 = ptr->transformation * helper::translate_object(ptr->V);
			ptr->transformation = ptr->transformation * transform;
			T = temp2 * ptr->transformation;
			applyTransformation(T);
			ptr->draw();
		
		//ptr = ptr->child;
		//traverse tree
		for(int i=0; i<ptr->children.size(); i++){
			drawHierarchy(ptr->children.at(i), T);
		}
	glPopMatrix();
}
//================================
// update
//================================
void update( void ) {
	//necessary?	
}
*/

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
void glutMouseFunc(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON)
    {
		printf("Mouse has been clicked!");
		g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
		if(g_bButton1Down == FALSE){
			printf("Mouse has been lifted!");
			//controlPoints.push_back(PoseKey(-10.0 + x/500.0 * 20.0, 10.0 - y/500.0 *20.0, 0.0, yawCP, pitchCP, 0.0, 0.0));
			glFlush();
		}
	}
}

void MouseMotion(int x, int y)
{
  // If button1 pressed, zoom in/out if mouse is moved up/down.
  if (g_bButton1Down)
    {
		printf("Being held down.");
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glTranslatef (0.0, 0.0, -5.0);
		//move back
		glPushMatrix();
			glRotatef(x* angleMultiplier, 1, 0, 0);
			glRotatef(y*angleMultiplier, 0, 1, 0);
			glutSolidTeapot(0.3);
		glPopMatrix();
		yawCP = x*angleMultiplier;
		pitchCP = y*angleMultiplier;

		//glutPostRedisplay();
    }
}
*/

//================================
// main
//================================
int main( int argc, char** argv ) {
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