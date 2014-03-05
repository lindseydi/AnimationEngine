#ifndef _RENDERER_H_
#define _RENDERER_H_


#include "ModelView.h"
#include "Pose.h"
#include "Scene.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>

class Renderer{

public:
	static void draw(vertex3 point){
		glPointSize( 4 );
		glBegin(GL_POINTS);
			glColor3f( 1, 0, 0 );	//RED
			glVertex3f((GLfloat)point.getx(), (GLfloat)point.gety(), (GLfloat)point.gety());
		glEnd();
	}

	static void draw(vertex3* point){
		glPointSize( 4 );
		glBegin(GL_POINTS);
			glColor3f( 1, 0, 0 );	//RED
			glVertex3f((GLfloat)point->getx(), (GLfloat)point->gety(), (GLfloat)point->gety());
		glEnd();
	}

	static void draw(Trajectory* path){
		vector<vertex3*>::iterator it;

		//Draw path by drawing interpolated positions
		glPointSize( 4 );
		glBegin(GL_POINTS);
		for (it = path->pointsAlongPath.begin(); it != path->pointsAlongPath.end(); it++){
			//draw(*it);
			//draws the dots
			glColor3f( 1, 0, 0 );	//RED
			//printf("Dot is %f, %f, %f\n", (*it)->getx(),(*it)->gety(), (*it)->gety());
			glVertex3f((GLfloat)(*it)->getx(), (GLfloat)(*it)->gety(), (GLfloat)(*it)->gety());
		}
		glEnd();
	}

	static void draw(ModelView model){
		//draw all of the polygons
		//glutSolidTeapot(0.3);
		 for (unsigned int i=0 ; i < model.edges.size(); i++){
			 polygon poly = model.edges.at(i);
			 switch(poly.type){
				case triangle:
					glBegin (GL_TRIANGLES);
					glNormal3f((GLfloat)poly.normal.getx(), (GLfloat)poly.normal.gety(), (GLfloat)poly.normal.getz());		//shouldn't I only have to do this once? TODO, not if shader
					for(int i=0; i< 3; i++){
						int index = poly.vertexIndices.at(i);
						GLfloat x = (GLfloat)model.vertices.at(index).getx();
						GLfloat y = (GLfloat)model.vertices.at(index).gety();
						GLfloat z = (GLfloat)model.vertices.at(index).getz();
						glVertex3f(x, y, z);
					}
					glEnd();
				break;
				case quad:
					glBegin (GL_QUADS);
					glNormal3f((GLfloat)poly.normal.getx(), (GLfloat)poly.normal.gety(), (GLfloat)poly.normal.getz());	
					for(int i=0; i< 4; i++){
						int index = poly.vertexIndices.at(i);
						GLfloat x = (GLfloat)model.vertices.at(index).getx();
						GLfloat y = (GLfloat)model.vertices.at(index).gety();
						GLfloat z = (GLfloat)model.vertices.at(index).getz();
						glVertex3f(x, y, z);
					}
					glEnd();
				break;
			}
		 }
	}

	static void draw(Actor* actor){
		draw(actor->path);
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
		//pointer?

		Pose inbetween = actor->path->update();
		draw(&inbetween, &actor->model);
		//draw(&inbetween);
		//inbetween.print();

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

		//draws dot where teapot is supposed to be
		//draw(inbetween.position);
	}

	static void draw(Pose* pose, ModelView* model){
		glPushMatrix();
			matrix4 transform;
			//translate
			transform =	pose->translate_object();

			//rotate
			pose->rotation.normalize();
			transform = transform * pose->getRotation();
			//cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			//glutSolidTeapot(0.3);
			draw(*model);
		glPopMatrix();		
	}

	static void draw(Pose* pose){
		glPushMatrix();
			matrix4 transform;
			//translate
			transform =	pose->translate_object();

			//rotate
			pose->rotation.normalize();
			transform = transform * pose->getRotation();
			//cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			glutSolidTeapot(0.3);
		glPopMatrix();		
	}

	static void draw(Scene* scene){
		// clear buffer
		glClearColor (0.0, 0.0, 0.0, 0.0);
		glClearDepth (1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
		// render state
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		for(unsigned int i=0; i<scene->actors.size(); i++){
			draw(scene->actors.at(i));
		}

				// disable lighting
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glutSwapBuffers();
	}


	static void applyTransformation(matrix4& mat){
		float m[16];
		matrix4 mat_transpose = mat.transpose();
		mat_transpose.ToArray(m);
		glLoadMatrixf( m );
	}

	

};

#endif