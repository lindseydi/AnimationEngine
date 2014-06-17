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
ModelView ball;
double time;
double step;

double u;
GLfloat PofU[7];
RigidBody sphere;

Model planeXNeg, planeYNeg, planeZNeg;
Model planeXPos, planeYPos, planeZPos;

vector<polygon*> planes;
vector<RigidBody> balls;


//================================
// init
//================================
void init(void) {
	// init something before main loop...


	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	glClearColor(1.0, 1.0, 1.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	//time data
	time =0.0;
	step = 0.01;

	//responsible for global timing in the scene
	//g_frameIndex = 0;
	scene = new Scene();
	//Actor* actor = new Actor(ModelView("resources/knight.txt"));
	ball = ModelView();
	ball.loadSphere(0.5, 10, 10);
	Actor* actor = new Actor(ball);
	//scene->actors.push_back(actor);

	planeXNeg = Model(new Pose());
	planeYNeg = Model(new Pose());
	planeZNeg = Model(new Pose());
	planeXPos = Model(new Pose());
	planeYPos = Model(new Pose());
	planeZPos = Model(new Pose());
	planeZNeg.mesh.loadPlane(20, 20, -10, 'z', true);
	planeYNeg.mesh.loadPlane(20, -10, 20, 'y', false);
	planeXNeg.mesh.loadPlane(-10, 20, 20, 'x', false);
	planeZPos.mesh.loadPlane(20, 20, 10, 'z', false);
	planeYPos.mesh.loadPlane(20, 10, 20, 'y', false);		//No Top to box!
	planeXPos.mesh.loadPlane(10, 20, 20, 'x', true);

	sphere = RigidBody(new Pose(0.0, -5.0, 0.0, 0.0, 1.0, 0.0));
	scene->models.push_back(&sphere);

	//scene->models.push_back(&planeZNeg);
	scene->models.push_back(&planeYNeg);
	//scene->models.push_back(&planeXNeg);
	//scene->models.push_back(&planeZPos);
	//scene->models.push_back(&planeYPos);
	//scene->models.push_back(&planeXPos);

	//ADD Rigid Bodies and Planes to main file data structures
	planes.push_back(&planeXNeg.mesh.edges.at(0));
	planes.push_back(&planeYNeg.mesh.edges.at(0));
	planes.push_back(&planeZNeg.mesh.edges.at(0));

	planes.push_back(&planeXPos.mesh.edges.at(0));
	planes.push_back(&planeZPos.mesh.edges.at(0));

	balls.push_back(sphere);
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
	polygon poly = planeYNeg.mesh.edges.at(0);
	if(CollisionDetector::intersects(sphere,  planeYNeg.mesh.edges.at(0))){
		printf("COLLISION!!\n\n\n");
	
		//is this all I have to do?
		//printf("Force %f , ", sphere.force.gety());

		//create an equal and opposite reaction
		//vertex3 normalForce = poly.normal.normalize() *sphere.force  * -1.0;
		vertex3 normalForce = poly.normal;
		float theta = atan2(normalForce.gety(),normalForce.getx());
		//normalForce.setx(normalForce.getx() * sphere.force.getMagnitude());
		//normalForce.sety(normalForce.gety() * sphere.force.getMagnitude());
		//normalForce.setz(normalForce.getz() * sphere.force.getMagnitude());
		normalForce = normalForce * cos(theta);
		//normalForce = normalForce * -1.0f;
		

		sphere.moveAlongOppositeDirection(time, step);
		//sphere.moveAlongOppositeDirection(time, step);
		//sphere.moveAlongOppositeDirection(time, step);
		sphere.applyNormalForce(normalForce, step);
		
	}
	//if(CollisionDetector::intersects(sphere, balls.at(1))){
	//	printf("COLLISION WITH SPHERE!!\n\n\n");

		//create an equal and opposite reaction for ONLY the 
	//	vertex3 normalForce = 


	//}
	//here, the reference is not being updated...
	//	printf("Force   %f\n", sphere.force.gety());
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

		// light source attributes
		GLfloat LightAmbient[]	= { 0.6f, 0.6f, 0.6f, 1.0f };
		GLfloat LightDiffuse[]	= { 1.0f, 1.0f, 1.0f, 1.0f };
		GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat LightPosition[] = { 0.0f, 400.0f, 0.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
		glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
		
		// surface material attributes
		GLfloat material_Ka[]	= { 0.11f, 0.06f, 0.11f, 1.0f };
		GLfloat material_Kd[]	= { 0.43f, 0.47f, 0.54f, 1.0f };
		GLfloat material_Ks[]	= { 0.33f, 0.33f, 0.52f, 1.0f };
		GLfloat material_Ke[]	= { 0.1f , 0.0f , 0.1f , 1.0f };
		GLfloat material_Se		= 10;

		
		glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
		glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
		glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);
		

		//glTranslatef (0.0, 0.0, -5.0);

		//INSERT DRAW SCENE
		Renderer::draw(scene);
		//for now draw box!
		//Renderer::draw(new Pose(0.0, 0.0, -100.0, 15.0, 30.0, 0.0), &box);
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

	//gluLookAt(0.0, 0.0, -10.0, 0.0, 0.0, -9.0, 0.0, 1.0, 0.0);
	//when rotating around y, must change x and z values
	//gluLookAt(10.0, 0.0, 0.0, 9.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(10.0, 3.0, -10.0, 4.0, 0.0, -4.0, 0.0, 1.0, 0.0);
	gluLookAt(0.0, 3.0, -40.0, 0.0, 0.0, -8.0, 0.0, 1.0, 0.0);

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