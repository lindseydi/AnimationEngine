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

Scene* scene;
ModelView box;

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
	Actor* actor = new Actor(box);
	scene->actors.push_back(actor);
}

Trajectory& createKeyFrames(void){
	Trajectory* path = new Trajectory();
	return *path;
}

//
//void drawHierarchy(Node* head, matrix3 transform){
//
//	Node* ptr = head;
//	matrix3 T = transform;
//	printf("\nPrinting Transform parameter from drawHierarchy");
//	transform.print();
//		//T builds on itself with each iteration
//		matrix3 V(4,1);
//		V(0, 0) = ptr->V.x;
//		V(1, 0) = ptr->V.y;
//		V(2, 0) = ptr->V.z;			//V(2,0) = -10.0;
//		V(3, 0) = 1.0;
//
//		matrix3 temp = T * V;
//		ptr->V = temp.toVector3f();
//
//	glPushMatrix();			
//			matrix3 temp2 = helper::pivot_object(ptr->articulation.getx(), ptr->articulation.gety(), ptr->articulation.getz(), ptr->V);
//			temp2 = ptr->transformation * helper::translate_object(ptr->V);
//			ptr->transformation = ptr->transformation * transform;
//			T = temp2 * ptr->transformation;
//			applyTransformation(T);
//			ptr->draw();
//		
//		//ptr = ptr->child;
//		//traverse tree
//		for(unsigned int i=0; i<ptr->children.size(); i++){
//			drawHierarchy(ptr->children.at(i), T);
//		}
//	glPopMatrix();
//}
////================================
//// update
////================================
//void update( void ) {
//	//necessary?	
//}


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

		glTranslatef (0.0, 0.0, -5.0);

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
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)w/(GLfloat)h, 1.0, 2000.0);

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

	box = ModelView();
	box.loadBox(1.0, 1.0, 1.0);
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