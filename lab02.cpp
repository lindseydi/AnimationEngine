
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

Scene scene;
ModelView box;
ModelView sphere;
Joint upperHip, lowerHip, knee, ankle;
Link pelvis, thigh, calf, foot;

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
	scene = Scene();
	sphere.loadSphere(0.3, 10, 10);
	//Actor* actor = new Actor(ModelView("resources/knight.txt"));
	//Actor* actor = new Actor(box);
	Actor* actor = new Actor(sphere);
	//scene.actors.push_back(actor);


	//create hierarchy!
	
	ModelView pelvisModel = ModelView();
	pelvisModel.loadBox(0.2, 1.2, 0.2);
	pelvis = Link(pelvisModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, PI/2), "pelvis");

	ModelView thighModel = ModelView();
	thighModel.loadBox(0.2, 1.2, 0.2);
	thigh = Link(thighModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "thigh");

	ModelView calfModel = ModelView();
	calfModel.loadBox(0.1, 0.1, 0.3);
	calf = Link(calfModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, -20.0), "calf");

	
	ModelView footModel = ModelView();
	footModel.loadBox(0.1, 0.2, 0.1);
	foot = Link(footModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, 90.0), "foot");

	lowerHip = Joint(PoseEuler());
	lowerHip.inboard_displacement = vertex3(0.0f, -0.7f, 0.0f);
	lowerHip.outboard_displacement = vertex3(0.0f, 0.7f, 0.0f);
	lowerHip.angle = PI/2.0;
	lowerHip.angularVelocity = 0.1;
	//lowerHip.rotational_frame = matrix4();
	lowerHip.rotational_frame = matrix4( 0, 0, 1, 0,
                                        -1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 0, 1 );
	knee = Joint();
	ankle = Joint();

	//link the joints and links
	pelvis.addChild(&lowerHip);
	lowerHip.addLink(&thigh);
	//thigh.addChild(&knee);
	//knee.addLink(&calf);
	//calf.addChild(&ankle);
	//ankle.addLink(&foot);

	scene.hierarchy = &pelvis;
	printf("Hierarchy set");

	//scene.model = new Model(footModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, 90.0));
	//scene.models.push_back(new Link(pelvisModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, 0.0)));
	//scene.models.push_back(new Link(thighModel, new PoseEuler(0.0, -1.4, 0.0, 0.0, 0.0, 0.0)));

}

Trajectory& createKeyFrames(void){
	Trajectory* path = new Trajectory();
	return *path;
}

//================================
// update
//================================
void update( void ) {
	//necessary?	
	scene.update(scene.hierarchy);
}

//================================
// render
//================================
void render( void ){
		// clear buffer
		update();
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glClearDepth (1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
		// render state
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);


		// enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);

		// light source attributes
		GLfloat LightAmbient[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightDiffuse[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightSpecular[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightPosition[] = { 0.0f, 10.0f, 0.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
		glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
		
		// surface material attributes
		GLfloat material_Ka[]	= { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat material_Kd[]	= { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat material_Ks[]	= { 0.8f, 0.8f, 0.8f, 1.0f };
		GLfloat material_Ke[]	= { 0.8f , 0.8f , 0.8f , 1.0f };
		GLfloat material_Se		= 10;

		glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
		glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
		glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);

		glTranslatef (0.0, 0.0, -5.0);

		//INSERT DRAW SCENE
		Renderer::draw(&scene);

		//pelvis.transform.identity();
		//thigh.transform.identity();
		//lowerHip.local_transformation.identity();

		//for now draw box!
		//Renderer::draw(new Pose(0.0, 0.0, -100.0, 15.0, 30.0, 0.0), &box);
		//Renderer::draw(pelvis.model);
		glutSwapBuffers();

		// disable lighting
		glDisable(GL_LIGHT0);
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
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 1000.0);
	//glTranslatef(0.0, 0.0, -10.0);

	gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, -9.0, 0.0, 1.0, 0.0);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
	//delete scene;
}


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

	box = ModelView();
	box.loadBox(1.0, 1.0, 1.0);

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