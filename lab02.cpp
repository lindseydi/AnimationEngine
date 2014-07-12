/*
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
Joint rightHip, leftHip;
LinkRoot pelvis;
Link rightLeg, leftLeg;

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
	pelvis = LinkRoot(pelvisModel, new Pose(0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "pelvis");
	//pelvis = Link(pelvisModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "pelvis");

	ModelView legModel = ModelView();
	legModel.loadBox(0.2, 1.2, 0.2);
	rightLeg = Link(legModel, new Pose(0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "rightLeg");

	leftLeg = Link(legModel, new Pose(0.0, 0.0, 0.0, 0.0, 0.0, 0.0), "leftLeg");


	ModelView calfModel = ModelView();
	calfModel.loadBox(0.1, 0.1, 0.3);
	Link calf = Link(calfModel, new Pose(0.0, 0.0, 0.0, 0.0, 0.0, -20.0), "calf");

	
	ModelView footModel = ModelView();
	footModel.loadBox(0.1, 0.2, 0.1);
	Link foot = Link(footModel, new Pose(0.0, 0.0, 0.0, 0.0, 0.0, 90.0), "foot");

	leftHip = Joint(Pose());
	leftHip.inboard_displacement = vertex3(0.0f, -0.7f, 0.0f);
	leftHip.outboard_displacement = vertex3(0.0f, 0.7f, 0.0f);
	leftHip.angle =PI/2.0;
	leftHip.angularVelocity = 0.1;
	leftHip.path->resetPath();
	leftHip.addPointOnPath(new PoseKey(-sqrt(2.0)/2.0, sqrt(2.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0, 0));
	leftHip.addPointOnPath(new PoseKey(-1.0/2.0, sqrt(3.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0, 45));
	leftHip.addPointOnPath(new PoseKey(0, 1.0, 0.0f, 0.0f, 0.0f, 0.0f, 90));
	leftHip.addPointOnPath(new PoseKey(1.0/2.0, sqrt(3.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0, 135));
	leftHip.addPointOnPath(new PoseKey(sqrt(2.0)/2.0, sqrt(2.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0f, 180));
	leftHip.rotational_frame = matrix4( 0, 0, 1, 0,
                                        -1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 0, 1 );
	rightHip = Joint(Pose());
	rightHip.inboard_displacement = vertex3(0.0f, 0.7f, 0.0f);
	rightHip.outboard_displacement = vertex3(0.0f, -0.7f, 0.0f);
	rightHip.angle = PI/2.0;
	rightHip.angularVelocity = 0.1;
	rightHip.path->resetPath();
	rightHip.addPointOnPath(new PoseKey(-sqrt(2.0)/2.0, -sqrt(2.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0, 0));
	rightHip.addPointOnPath(new PoseKey(-1.0/2.0, -sqrt(3.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0, 45));
	rightHip.addPointOnPath(new PoseKey(0, -1.0, 0.0f, 0.0f, 0.0f, 0.0f, 90));
	rightHip.addPointOnPath(new PoseKey(1.0/2.0, -sqrt(3.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0, 135));
	rightHip.addPointOnPath(new PoseKey(sqrt(2.0)/2.0, -sqrt(2.0)/2.0, 0.0f, 0.0f, 0.0f, 0.0f, 180));
	rightHip.rotational_frame = matrix4( 0, 0, 1, 0,
                                         -1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 0, 1 );

	//knee = Joint(PoseEuler());
	//knee.inboard_displacement = vertex3()
	//ankle = Joint();

	//link the joints and links
	pelvis.addChild(&leftHip);
	pelvis.addChild(&rightHip);
	leftHip.addLink(&rightLeg);
	rightHip.addLink(&leftLeg);
	//rightLeg.addChild(&knee);
	//knee.addLink(&calf);
	//calf.addChild(&ankle);
	//ankle.addLink(&foot);

	scene.hierarchy = &pelvis;
	printf("Hierarchy set");

	//scene.model = new Model(footModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, 90.0));
	//scene.models.push_back(new Link(pelvisModel, new PoseEuler(0.0, 0.0, 0.0, 0.0, 0.0, 0.0)));
	//scene.models.push_back(new Link(rightLegModel, new PoseEuler(0.0, -1.4, 0.0, 0.0, 0.0, 0.0)));

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
		GLfloat Light
		ent[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
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
		//rightLeg.transform.identity();
		//leftHip.local_transformation.identity();

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

//	gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, -9.0, 0.0, 1.0, 0.0);
	//when rotating around y, must change x and z values
	//gluLookAt(10.0, 0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	gluLookAt(10.0, 3.0, -10.0, 4.0, 0.0, -4.0, 0.0, 1.0, 0.0);


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

*/