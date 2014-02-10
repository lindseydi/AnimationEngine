
#pragma once
#include <stdlib.h>
#include "KeyFrame.h"
// standard
#include <assert.h>
#include <math.h>

// glut

#include <stdio.h>
#include <sys/stat.h>

//headers

#include "polygon.h"
#include "Node.h"

#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Geometry>

#include <vector>

#include <GL/glut.h>
using namespace std;
using namespace Eigen;

//================================
// global variables
//================================
// screen size
int g_screenWidth  = 0;
int g_screenHeight = 0;

//Lookat Positions
float step = 0.2;
float posX; //will use for glulookat
float posZ; //will use for glulookat
float prevX;
float prevZ;
float dirX;
float dirZ;
float angle;
float X, Y, Z;//for translate updates
//vector to store interpolated values
vector<Vector3f> interpolatedValues;


bool g_bButton1Down;
Vector3f keyFramePos;
bool controlPointsCreated;
float yawCP;
float pitchCP;

float spacing;
bool catmullRom;
bool bspline;
float rot;


//ModelView knight("resources/knight.txt", true);
Node *man;
Node *thighRight;
Node *shinRight;
Node *footRight;

// frame index
int g_frameIndex = 0;

// angle for rotation
int g_angle = 0;

//Used for interpolating key frames
MatrixX3f M(4,4);
vector<KeyFrame> controlPoints;
int curFrame;
int curKeyFrameIndex;
int numFrames;
int nextKeyFrame; 

double u;
GLfloat PofU[7];

//what eigen matrix does this
void applyTransformation(MatrixX3f mat){
	float m[ 16 ];
	mat.transpose().ToArray( m );
	//mat.ToArray(m);
	glLoadMatrixf( m );
}

//--Helper functions


void drawKeys(void){
	glPointSize( 4 );
	glBegin(GL_POINTS);
	glColor3f( 1, 0, 0 );
		for(int i=0; i<controlPoints.size(); i++){
			glVertex3fv(controlPoints.at(i).position.vec);
		}
	glEnd();
}


void printKeys(void){
	for(int i=0; i<controlPoints.size(); i++){
		controlPoints.at(i).print();
	}
}
//--

//================================
// init
//================================
void init(void) {
	// init something before main loop...
	glClearColor(1.0, 1.0, 1.0, 0.0);

	controlPointsCreated = false;

	glMatrixMode(GL_PROJECTION);

	if(catmullRom){
		//Catmull Rom blending function
		printf("Interpolating a Catmull Rom Spline\n");
		M(0,0) = -0.5; M(0, 1) = 1.5; M(0, 2)= -1.5; M(0,3) = 0.5;
		M(1,0) =  1.0; M(1, 1) = -2.5; M(1, 2) = 2.0; M(1,3) = -0.5;
		M(2,0) =  -0.5; M(2, 1) = 0.0; M(2, 2) = 0.5; M(2,3) = 0.0;
		M(3,0) =  0.0; M(3, 1) = 1.0; M(3, 2) = 0.0; M(3,3) = 0.0;
	}else if(bspline){
		//B-Spline Blending Function
		printf("Interpolating a B-Spline\n");
		M(0,0) = -1.0; M(0, 1) = 3.0; M(0, 2)= -3.0; M(0,3) = 1.0;
		M(1,0) =  3.0; M(1, 1) = -6.0; M(1, 2) = 3.0; M(1,3) = 0.0;
		M(2,0) =  -3.0; M(2, 1) = 0.0; M(2, 2) = 3.0; M(2,3) = 0.0;
		M(3,0) =  1.0; M(3, 1) = 4.0; M(3, 2) = 1.0; M(3,3) = 0.0;
	}

	printf("Printing M from init\n");
	M.print();
	u =  0.0;
	curFrame = 0;

	//Special for a Catmull Rom Spline, the first point is...
	//Taken from page 456 of Textbook

	
	//printf("\n\n");
	//printf("\ncurFrame = %d and nextKeyFrame = %d]n", curFrame, nextKeyFrame, curKeyFrameIndex, numFrames);

}

int ClampInteger( int v, int min, int max ) {
	if ( v < min ) {
		return min;
	}
	if ( v > max ) {
		return max;
	}
	return v;
}

void createKeyFrames(void){
	controlPoints.push_back(KeyFrame(-1.0,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(-1.5,	0.70,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(-0.7,	0.5,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(0.0,	1.0,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(0.25,	0.5,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(0.5,	0.0,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(0.75,	-0.5,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(0.25,	1.5,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(0.0,	0.5,	0.0,	0.0,	0.0,	0.0,	0.0));
	controlPoints.push_back(KeyFrame(-1.25, -0.5,	0.0,	0.0,	0.0,	0.0,	0.0));
}

void printInteraction(){
	   cout << "Interaction:" << endl;
	   cout << "Create ten Control Points to interpolate between" << endl << endl;
	    /*cout << "Left click to create." << endl;
		<< "Move mouse up/down to mainpulate yaw." << endl;
		<< "Move mouse left/right to manipulate pitch." << endl;
		*/
	    boolean preloaded;
		
		boolean quat;
		char tempchar;
		float tempx, tempy, tempz;
		float orientx, orienty, orientz, orientw;
		cout << "Please enter the amount of seconds between control points" << endl;
		cin >> spacing;

		cout << "Would you like to use Catmull Rom or B Splines?"<< endl <<" (Enter c for Catmull Rom) and b for BSplines" << endl;
		cin >> tempchar;
		if(tempchar == 'c'){
			catmullRom = true;
			bspline = false;
		}else if(tempchar == 'b'){
			catmullRom = false;
			bspline = true;
		}else{
			cout << "Please enter either 'c' for Catmull Rom or 'b' for B-Spline" << endl;
		}

		cout << "Would you like to use the preloaded Control Points? (y/n)" << endl;
		cin >> tempchar;
		if(tempchar == 'y'){
			createKeyFrames();
		}else{
			for(int i=0; i<10; i++){
				cout << "Would you like to create a Quaternion for your" << i << "th Key Frame? y/n"<< endl;
				cin >> tempchar;
				if(tempchar == 'y')
					quat = true;
				else
					quat = false;
				cout << "Enter X Position Value (please enter float between -2.0 and 2.0)" << endl;
				cin >> tempx;
				cout << "Enter Y Position Value (please enter float between -2.0 and 2.0)" << endl;
				cin >> tempy;
				cout << "Enter Z Position Value (please enter float between -2.0 and 2.0)" << endl;
				cin >> tempz;
				if(quat){
					cout << "Enter W Orientation Value (please enter float.)" << endl;
				cin >> orientw;
				}
				cout << "Enter X Orientation Value" << endl;
				cin >> orientx;
				cout << "Enter Y Orientation Value" << endl;
				cin >> orienty;
				cout << "Enter Z Orientation Value" << endl;
				cin >> orientz;
				if(quat)
					controlPoints.push_back(KeyFrame(tempx, tempy, tempz, orientx, orienty, orientz, orientw, 0.0));
				else
					controlPoints.push_back(KeyFrame(tempx, tempy, tempz, orientx, orienty, orientz, 0.0));
			}
		}
}


void drawHierarchy(Node* head, MatrixX3f transform){

	Node* ptr = head;
	MatrixX3f T = transform;
	printf("\nPrinting Transform parameter from drawHierarchy");
	transform.print();
		//T builds on itself with each iteration
		MatrixX3f V(4,1);
		V(0, 0) = ptr->V.x;
		V(1, 0) = ptr->V.y;
		V(2, 0) = ptr->V.z;			//V(2,0) = -10.0;
		V(3, 0) = 1.0;

		MatrixX3f temp = T * V;
		ptr->V = temp.toVector3f();

	glPushMatrix();			
			MatrixX3f temp2 = helper::pivot_object(ptr->articulation.getx(), ptr->articulation.gety(), ptr->articulation.getz(), ptr->V);
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
	/*
	if(controlPoints.size() >= 10){
		controlPointsCreated = true;
	}

	if(controlPointsCreated){
	*/
		curFrame++;

		u = (double)curFrame/(spacing * 60.0);
		//printf("U is %2f\n", u);
		// key frame
		curKeyFrameIndex =  (int)floor(u) ;
		u -= curKeyFrameIndex;
		curKeyFrameIndex = curKeyFrameIndex % controlPoints.size();
		// fraction

		//printf("Current Frame: %d, CurKeyFrameIndex: %d, and u : %2f\n", curFrame, curKeyFrameIndex, u);
		
		if(curFrame % 45 == 0){
				rot *=-1;
		}
		if(curFrame % 8 == 0){
			thighRight->articulation.setz(thighRight->articulation.getz() + rot);
			shinRight->articulation.setz(shinRight->articulation.getz() + -rot*2);
		}

		// T
		MatrixX3f T(1, 4);
		T(0, 0) = u*u*u;
		T(0, 1) = u*u;
		T(0, 2) = u;
		T(0, 3) = 1; 

		if(catmullRom){
			// control points indicies
			int maxIndex = controlPoints.size()-1;
			int k0 = ClampInteger( curKeyFrameIndex - 1, 0, maxIndex );
			int k1 = ClampInteger( curKeyFrameIndex    , 0, maxIndex );
			int k2 = ClampInteger( curKeyFrameIndex + 1, 0, maxIndex );
			int k3 = ClampInteger( curKeyFrameIndex + 2, 0, maxIndex );
			MatrixX3f G(4, 1);
			for(int i=0; i<7; i++) {
				// G
				G(0,0) = controlPoints.at( k0 ).at( i );
				G(1,0) = controlPoints.at( k1 ).at( i );
				G(2,0) = controlPoints.at( k2 ).at( i );
				G(3,0) = controlPoints.at( k3 ).at( i );
				//G.print();		
				// interpolation
				MatrixX3f interpolation = T*M*G;
				PofU[i] = (GLfloat) interpolation(0,0);
			//	printf("---%d %d %f %f\n", i, k0, PofU[i], G(2,0));
			}
			//printf("\n\n");
		}else if(bspline){
					// control points indicies
			int maxIndex = controlPoints.size()-1;
			int k0 = ClampInteger( curKeyFrameIndex,	 0, maxIndex );
			int k1 = ClampInteger( curKeyFrameIndex  +1, 0, maxIndex );
			int k2 = ClampInteger( curKeyFrameIndex + 2, 0, maxIndex );
			int k3 = ClampInteger( curKeyFrameIndex + 3, 0, maxIndex );
			MatrixX3f G(4, 1);
			for(int i=0; i<7; i++) {
				// G
				G(0,0) = controlPoints.at( k0 ).at( i );
				G(1,0) = controlPoints.at( k1 ).at( i );
				G(2,0) = controlPoints.at( k2 ).at( i );
				G(3,0) = controlPoints.at( k3 ).at( i );
				//G.print();		
				// interpolation
				MatrixX3f interpolation = T*M*G;
				PofU[i] = (GLfloat)(1.0/6.0) * interpolation(0,0);
				printf("---%d %d %f \n", i, k0, PofU[i]);
			}
			printf("\n\n");
		}
	//}
}

//================================
// render
//================================
void render( void ) {
	// clear buffer
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth (1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	drawKeys();

	//printKeys();	

	//If the user has finished creating all control Points, start interpolating
	//if(controlPointsCreated){
		//create an inbetween point
		KeyFrame inbetween(PofU[0], PofU[1], PofU[2], PofU[3], PofU[4], PofU[5], PofU[6], 0.0);
		interpolatedValues.push_back(inbetween.position);

		vector<Vector3f>::iterator it;

		//Draw path by drawing interpolated positions
		glPointSize( 3 );
		glBegin(GL_POINTS);
		glColor3f( 0, 0, 1 );
		int i=0;
		for (it =interpolatedValues.begin(); it< interpolatedValues.end(); it++, i++)
			glVertex3f(interpolatedValues.at(i).getPointer());
		glEnd();

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


		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();
		glTranslatef (0.0, 0.0, -5.0);

		glPushMatrix();
			//applyTransformation(helper::pivot_object(1, 1, 1, Vector3f(0.0, 0.0, 0.0)));		
			//glutSolidTeapot(0.5);
		glPopMatrix();
		//glPushMatrix();
			//glTranslatef (0.0, 0.0, -5.0);
		MatrixX3f transform(4,4);
		transform = helper::translate_object(inbetween.at(0), inbetween.at(1), inbetween.at(2)-5);
		//transform = transform * inbetween.orient.normalize().getRotation();
			
		drawHierarchy(man, transform);
		//glPopMatrix();
	//}

	// disable lighting
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glutSwapBuffers();
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
	update();
	
	// render
	glutPostRedisplay();

	// reset timer
	// 16 ms per frame ( about 60 frames per second )
	glutTimerFunc( 16, timer, 0 );
}
float angleMultiplier;
void glutMouseFunc(int button, int state, int x, int y){

	if (button == GLUT_LEFT_BUTTON)
    {
		printf("Mouse has been clicked!");
		g_bButton1Down = (state == GLUT_DOWN) ? TRUE : FALSE;
		if(g_bButton1Down == FALSE){
			printf("Mouse has been lifted!");
			controlPoints.push_back(KeyFrame(-10.0 + x/500.0 * 20.0, 10.0 - y/500.0 *20.0, 0.0, yawCP, pitchCP, 0.0, 0.0));
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

//================================
// main
//================================
int main( int argc, char** argv ) {
	//Set initial Global Variables
	posX = 5.0;
	posZ = 12.0;
	dirX = 0.0;
	dirZ = 0.0;
	angle = 0.0;
	X= 0.0;
	Y= 0.0;
	Z = 0.0;
	rot = 5.0;

	//Determine type of spline
	catmullRom = false;
	bspline = true;
	
	man = new Node("resources/hips.txt", Vector3f(0.0, 0.0, 0.0), Vector3f(0.0,0.0,0.0));
	printf("Hips don't lie");
	//man->transformation = translate_object(-1.0, 1.0, 0.0);


	//THIGH
	thighRight = new Node("resources/legbone.txt", Vector3f(0.2, -0.05, 0.0), Vector3f(-0.15, 0.0, 0.0));
	thighRight->setArticulation(0.0, 0.0, 10.0);
	
	
	//SHIN
	shinRight = new Node ("resources/legbone.txt", Vector3f(0.35, -0.5, 0.0), Vector3f(-0.15, 0.0, 0.0));
	shinRight->setArticulation(0.0, 0.0, -20.0);

	
	footRight = new Node ("resources/foot.txt", Vector3f(0.45, -1.0, 0.0), Vector3f(0.0, 0.0, 0.0));

	shinRight->children.push_back(footRight);
	thighRight->children.push_back(shinRight);
	man->children.push_back(thighRight);

	//create Keyframes
	//createKeyFrames();
	//figure out # of frames
	
	//printInteraction();
	createKeyFrames();
	catmullRom = true;
	bspline = false;

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
	return 0;
}