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
		draw(actor->path);		//pointer?

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
			cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			glutSolidTeapot(0.3);
		glPopMatrix();		
	}

	static void draw(Scene* scene){
		for(unsigned int i=0; i<scene->actors.size(); i++){
			draw(scene->actors.at(i));
		}
	}


	static void applyTransformation(matrix4& mat){
		float m[16];
		matrix4 mat_transpose = mat.transpose();
		mat_transpose.ToArray(m);
		glLoadMatrixf( m );
	}

	

};

#endif