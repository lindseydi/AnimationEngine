#if 0
#pragma once
#include <stdlib.h>

// glew
#include <glew.h>


#include "Pose.h"
// standard
#include <assert.h>
#include <math.h>

// glut

#include "Movie.h"
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
#include "Flock.h"

#include <iostream>
#include <vector>

#include <GL/glut.h>
#include <GL/glu.h>
using namespace std;

//===============================
// movie variables
//===============================
#define GENERATE_MOVIE 1
Movie output;
std::string title;                          // Window Title
std::string filetitle;                      // Movie file title prefix
char filename[128];                         // Buffer for filename of a frame render to file


//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

// frame index
int g_frameIndex = 0;

Scene* scene;
ModelView box;

double u;
GLfloat PofU[7];


//================================
// init
//================================
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

	//responsible for global timing in the scene
	//g_frameIndex = 0;
	scene = new Scene();

	//create pyramid for debugging
	box = ModelView();
	box.loadPyramid(1.0, 1.0);

	//Actor* actor = new Actor(ModelView("resources/knight.txt"));
	Model* actor = new Model(new Pose());
	actor->mesh.loadBox(12.0, 12.0, 12.0);
	Flock* flock = new Flock(vertex3(-10.0, 10.0, 0.0));

	//scene->actors.push_back(actor);
	scene->flock = flock;

	//scene->models.push_back(actor);

	Renderer::init();

	//movie init
	title = "Reynolds Boid Simulation";
	filetitle = "reynolds_boid_simulation";
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
	scene->update();
}

//================================
// render
//================================
void render( void ){
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
		GLfloat LightAmbient[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat LightDiffuse[]	= { 0.3f, 0.3f, 0.3f, 1.0f };
		GLfloat LightSpecular[]	= { 0.4f, 0.4f, 0.4f, 1.0f };
		GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT , LightAmbient );
		glLightfv(GL_LIGHT0, GL_DIFFUSE , LightDiffuse );
		glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
		
		// surface material attributes
		GLfloat material_Ka[]	= { 0.8f, 0.0f, 0.0f, 1.0f };
		GLfloat material_Kd[]	= { 0.8f, 0.0f, 0.0f, 1.0f };
		GLfloat material_Ks[]	= { 0.8f, 0.0f, 0.0f, 1.0f };
		GLfloat material_Ke[]	= { 0.1f , 0.1f , 0.1f , 1.0f };
		GLfloat material_Se		= 10;

		glMaterialfv(GL_FRONT, GL_AMBIENT	, material_Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE	, material_Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR	, material_Ks);
		glMaterialfv(GL_FRONT, GL_EMISSION	, material_Ke);
		glMaterialf (GL_FRONT, GL_SHININESS	, material_Se);

		//glTranslatef (0.0, 0.0, -5.0);
		//gluLookAt(0.0, 0.0, -100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

		//INSERT DRAW SCENE
		Renderer::draw(scene);
		//for now draw box!
		//Renderer::draw(new Pose(0.0, 0.0, -100.0, 15.0, 30.0, 0.0), &box);
		glutSwapBuffers();

	    if( GENERATE_MOVIE ) {
			 output.write_frame( filename, g_frameIndex, g_screenWidth, g_screenHeight);
		}


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
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 0.1, 1000.0);
	//glTranslatef(0.0, 0.0, -10.0);

	gluLookAt(0.0, 0.0, -30.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

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


//================================
// main
//================================
int main( int argc, char** argv ) {
	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize( 640, 480 ); 
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

#endif //toggles current main file