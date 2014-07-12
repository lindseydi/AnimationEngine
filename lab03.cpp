/*

#pragma once
#include <stdlib.h>

// glew
#include <glew.h>

#include "Pose.h"
// standard
#include <assert.h>
#include <math.h>

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
#include "RigidBody.h"
#include "CollisionDetector.h"


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
double time;
double step;

double u;
GLfloat PofU[7];

Model box;
Model planeXNeg, planeYNeg, planeZNeg;
Model planeXPos, planeYPos, planeZPos;

vector<polygon> planes;
const int numBalls = 20;
vector<RigidBody*> balls;


//================================
// init
//================================
//init helper function in order to create no overlapping balls from the start
void changePosition(RigidBody* body){
	float x, y, z;
	x = Helper::randBetween_float(-9.0, 9.0);
	y = Helper::randBetween_float(9.75, 3.0);
	z = Helper::randBetween_float(-9.0, 9.0);
	body->pose->position = vertex3(x, y, z); 
}

void init(void) {
	// init something before main loop...


	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	 //Problem: glewInit failed, something is seriously wrong.
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	//time data
	time =0.0;
	step = 0.03333;

	scene = new Scene();

	vertex3 blue = vertex3(0.0, 0.0, 1.0);
	planeXNeg = Model(new Pose(), blue);
	planeYNeg = Model(new Pose(), blue);
	planeZNeg = Model(new Pose(), blue);
	planeXPos = Model(new Pose(), blue);
	planeYPos = Model(new Pose(), blue);
	planeZPos = Model(new Pose(), blue);
	planeZNeg.mesh.loadPlane(20, 20, -10, 'z', true);
	planeYNeg.mesh.loadPlane(20, -10, 20, 'y', false);
	planeXNeg.mesh.loadPlane(-10, 20, 20, 'x', false);
	planeZPos.mesh.loadPlane(20, 20, 10, 'z', false);
	planeYPos.mesh.loadPlane(20, 10, 20, 'y', false);		
	planeXPos.mesh.loadPlane(10, 20, 20, 'x', true);
	
	//create Balls
	for(int i=0; i<numBalls; i++){
		float x, y, z;
		x = Helper::randBetween_float(-9.0, 9.0);
		y = Helper::randBetween_float(9.75, 5.0);
		z = Helper::randBetween_float(-9.0, 9.0);
		vertex3 randPosition = vertex3(x, y, z);
		
		RigidBody* body = new RigidBody(new Pose(randPosition, vertex3()));
	
		while(CollisionDetector::intersects(body, balls)){
				changePosition(body);
		}
		balls.push_back(body);
		scene->models.push_back(body);
	}

	//ADD Rigid Bodies and Planes to main file data structures
	planes.push_back(planeXNeg.mesh.edges.at(0));
	planes.push_back(planeYNeg.mesh.edges.at(0));
	planes.push_back(planeZNeg.mesh.edges.at(0));
	//no need to add ypos
	planes.push_back(planeXPos.mesh.edges.at(0));
	planes.push_back(planeZPos.mesh.edges.at(0));

		//draw BOXES instead of planes !
	planeYNeg.mesh.loadBox(20.0, 0.5, 20.0);
	planeYNeg.pose->position = vertex3(0.0, -10.25, 0.0);
	planeYNeg.updateTransform();

	planeXNeg.mesh.loadBox(0.5, 20.0, 20.0);
	planeXNeg.pose->position = vertex3(-10.25, 0.0, 0.0);
	planeXNeg.updateTransform();

	planeXPos.mesh.loadBox(0.5, 20.0, 20.0);
	planeXPos.pose->position = vertex3(10.25, 0.0, 0.0);
	planeXPos.updateTransform();

	planeYPos.mesh.loadBox(20.0, 0.5, 20.0);
	planeYPos.pose->position = vertex3(0.0, 10.25, 0.0);
	planeYPos.updateTransform();

	planeZPos.mesh.loadBox(20.0, 20.0, 0.5);
	planeZPos.pose->position = vertex3(0.0, 0.0, 10.25);
	planeZPos.updateTransform();

	scene->models.push_back(&planeYNeg);
	scene->models.push_back(&planeXNeg);
	scene->models.push_back(&planeZPos);
	//scene->models.push_back(&planeYPos);	//Comment to remove top to box!
	scene->models.push_back(&planeXPos);

	Renderer::init();
}

Trajectory& createKeyFrames(void){
	Trajectory* path = new Trajectory();
	return *path;
}

////================================
//// update
////================================
void update( void ) {
	scene->update(time, step);

	//for debugging
	//for(unsigned int i=0; i<numBalls; ++i){
	//	balls.at(i)->mesh.color = vertex3(1.0, 0.0, 0.0);
	//}

	//for each wall in the scene
	for(unsigned int i=0; i<numBalls; ++i){
		for(unsigned int j=0; j<planes.size(); ++j){
			polygon poly = planes.at(j);
			if(CollisionDetector::intersects(balls.at(i), poly)){
				//printf("COLLISION!!\n\n\n");
				balls.at(i)->handlePolygonCollision(poly, time, step);
			}
		}
	}

	bool beenProcessed[numBalls];	
	for(int i= 0; i<numBalls; i++)
		beenProcessed[i] = false;
	//for each ball, go through every other ball. O(n^2)
	for(unsigned int i=0; i<numBalls; ++i){
		for(unsigned int j=0; j<balls.size(); ++j){
			if(i != j && !beenProcessed[i]){
				if(CollisionDetector::intersects(balls.at(i), balls.at(j))){
					//printf("\nBALL COLLSioN %d  %d\n", i, j);
					//balls.at(i)->mesh.color = vertex3(0.0, 1.0, 0.0);
					//balls.at(j)->mesh.color = vertex3(0.0, 0.0, 1.0);
					balls.at(i)->handleRigidBodyCollision(balls.at(j), time, step);
					beenProcessed[i] = true;
					beenProcessed[j] = true;
				}
			}
		}
	}
}


//================================
// render
//================================
void render( void ){
	    time += step;
	
	    // clear buffer
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glClearDepth (1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
		// render state
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);


		// enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_LIGHT1);
		//glEnable(GL_LIGHT2);

		// light source attributes
		GLfloat LightAmbient[]	= { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat LightDiffuse[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightSpecular[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightPosition0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		GLfloat LightPosition1[] = { 10.0f, 0.0f, 0.0f, 1.0f };
		GLfloat LightPosition2[] = {-10.0f, 0.0f, 0.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
		glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition0);
		
		glLightfv( GL_LIGHT1, GL_POSITION, LightPosition1 );
		glLightfv( GL_LIGHT1, GL_AMBIENT, LightAmbient );
		glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);
		glLightfv( GL_LIGHT1, GL_DIFFUSE, LightDiffuse );

		glLightfv( GL_LIGHT2, GL_POSITION, LightPosition2 );
		glLightfv( GL_LIGHT2, GL_AMBIENT, LightAmbient );
		glLightfv( GL_LIGHT2, GL_DIFFUSE, LightDiffuse );
		glLightfv(GL_LIGHT2, GL_SPECULAR, LightSpecular);
		
		// surface material attributes
		GLfloat material_Ka[]	= { 0.2f, 0.2f, 0.2f, 1.0f };
		GLfloat material_Kd[]	= { 1.0f, 1.0f, 1.0f, 1.0f  };
		GLfloat material_Ks[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat material_Ke[]	= { 0.0f , 0.0f , 0.0f , 1.0f };
		GLfloat material_Se		= 10;

		//glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material_Kd);
		//glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
		//glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
		//glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
		//glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
		//glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);
		

		//glTranslatef (0.0, 0.0, -5.0);

		//INSERT DRAW SCENE
		Renderer::draw(scene);
		//for now draw box!
		//Renderer::draw(new Pose(0.0, 0.0, -100.0, 15.0, 30.0, 0.0), &box);
		glutSwapBuffers();

		// disable lighting
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHT0);
		//glDisable(GL_LIGHT1);
		//glDisable(GL_LIGHT2);
		glDisable(GL_LIGHTING);
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
	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();
	//gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);
		
	
	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 200.0);
	//glTranslatef(0.0, 0.0, -10.0);

	//gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, -9.0, 0.0, 1.0, 0.0);
	//when rotating around y, must change x and z values
	//gluLookAt(10.0, 0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(10.0, 3.0, -10.0, 4.0, 0.0, -4.0, 0.0, 1.0, 0.0);
	gluLookAt(0.0, 30.0, -20.0, 0.0, -8.0, 8.0, 0.0, 1.0, 0.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {	
	// increase frame index
	g_frameIndex++;
	update();
	
	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );
}


void cleanup(){
	delete scene;
}


//void glutMouseFunc(int button, int state, int x, int y){
//	if (button == GLUT_LEFT_BUTTON)
//    {
//		printf("Mouse has been clicked!");
//		g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
//		if(g_bButton1Down == FALSE){
//			printf("Mouse has been lifted!");
//			//controlPoints.push_back(PoseKey(-10.0 + x/500.0 * 20.0, 10.0 - y/500.0 *20.0, 0.0, yawCP, pitchCP, 0.0, 0.0));
//			glFlush();
//		}
//	}
//}
//
//void MouseMotion(int x, int y)
//{
//  // If button1 pressed, zoom in/out if mouse is moved up/down.
//  if (g_bButton1Down)
//    {
//		printf("Being held down.");
//		glMatrixMode( GL_MODELVIEW );
//		glLoadIdentity();
//		glTranslatef (0.0, 0.0, -5.0);
//		//move back
//		glPushMatrix();
//			glRotatef(x* angleMultiplier, 1, 0, 0);
//			glRotatef(y*angleMultiplier, 0, 1, 0);
//			glutSolidTeapot(0.3);
//		glPopMatrix();
//		yawCP = x*angleMultiplier;
//		pitchCP = y*angleMultiplier;
//
//		//glutPostRedisplay();
//    }
//}


//================================
// main
//================================
int main( int argc, char** argv ){
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


//white light
/*
		// light source attributes
		GLfloat LightAmbient[]	= { 1.0f, 1.8f, 1.0f, 1.0f };
		GLfloat LightDiffuse[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightSpecular[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightPosition[] = { 0.0f, 10.0f, 0.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
		glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
		
		// surface material attributes
		GLfloat material_Ka[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat material_Kd[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat material_Ks[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
		GLfloat material_Se		= 10;
*/