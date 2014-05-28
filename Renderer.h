#ifndef _RENDERER_H_
#define _RENDERER_H_


#include "ModelView.h"
#include "Pose.h"
#include "Link.h"
#include "Joint.h"
#include "Flock.h"
#include "Model.h"
#include "Scene.h"
#include "matrix4.h"
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
					for(unsigned int i=0; i< 3; i++){
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
					for(unsigned int i=0; i< 4; i++){
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
		//draw(actor->path);		//pointer?

		Pose inbetween = actor->update();
		draw(&inbetween, &actor->model);

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);

		//draws dot where teapot is supposed to be
		//draw(inbetween.position);
	}

	static void draw(Flock* flock){
		//printf("Flock render being executed");
		flock->update();
		for(unsigned int i=0; i<flock->boids.size(); i++){
			draw(&flock->boids.at(i)->getPose(), &flock->boids.at(i)->getModel());
			//draw(&flock->boids.at(i)->getPose());
		}
		
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
	}

	static void draw(Pose* pose, ModelView* model){
		glPushMatrix();
			matrix4 transform;
			//translate
			transform =	pose->translate_object();

			//rotate
			pose->rotation.normalize();
			transform = transform * pose->getRotation();
			//transform = pose->getRotation();
			//cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			//glutSolidTeapot(0.3);
			draw(*model);
		glPopMatrix();		
	}

	static void draw(PoseEuler* pose, ModelView* model){
		glPushMatrix();
			matrix4 transform;
			//translate
			transform =	pose->translate_object();

			//pose->orientation.normalize();
			pose->orientation.print();
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
			//transform =	pose->translate_object();

			//rotate
			pose->rotation.normalize();
			transform = transform * pose->getRotation();
			transform = pose->getRotation();
			//cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			draw(vertex3());
		glPopMatrix();		
	}

	
	static void draw(PoseEuler* pose){
		glPushMatrix();
			matrix4 transform;
			pose->rotation.normalize();
			transform = pose->getRotation();
			//cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			draw(vertex3());
		glPopMatrix();		
	}

	static void draw(Scene* scene){
		
		for(unsigned int i=0; i<scene->actors.size(); i++){
			draw(scene->actors.at(i));
		}
		if(scene->flock != NULL){
			draw(scene->flock);
		}
		if(scene->hierarchy != NULL){
			printf("Hierarchy attempting to draw\n");
			matrix4 identity = matrix4();
			identity.identity();
			draw(scene->hierarchy);
		}
	}
	
	static void draw(Link* hierarchy){

		//matrix4 local_transform(transform);
		//local_transform = local_transform * hierarchy->local_pose.translate_object();

		//local_transform = local_transform * hierarchy->local_pose.getRotation();
		//cout << "The matrix transform is:" << endl << transform.matrix << endl;
		Model* temp = (Model*)hierarchy;
		draw(*temp);

		if(!hierarchy->isNull()){
			for(unsigned int i=0; i<hierarchy->children.size(); i++){
				Link* temp = hierarchy->children.at(i)->child;
				draw(temp);
			}
		}
	}
	

	static void draw(const Model& model){
		//draw(model->pose, model->mesh);
		glPushMatrix();
			matrix4 temp = model.transform;
			//cout << "The matrix transform is:" << endl << temp.matrix << endl;
			applyTransformation(temp);
			draw(model.mesh);
		glPopMatrix();
	}
	/*
	static void draw(Joint* hierarchy, const matrix4& transform){
		matrix4 local_transform = matrix4();
		local_transform = hierarchy->local_transformation * transform;

		

		Link* child = hierarchy->child;
		assert(child != NULL);
		cout << "The matrix transform for " << child->name << " is " << endl << local_transform.matrix << endl;

		glPushMatrix();
			applyTransformation(local_transform);		
			//glutSolidTeapot(0.3);
			draw(child);
		glPopMatrix();
	
	
		if(!child->isNull()){
			for(unsigned int i=0; i<child->children.size(); i++){
				Joint* temp = child->children.at(i);
				draw(temp, local_transform);
			}
		}
	}
	*/

	static void applyTransformation(matrix4& mat){
		float m[16];
		matrix4 mat_transpose = mat.transpose();
		mat_transpose.ToArray(m);
		glLoadMatrixf( m );
	}

	

};

#endif