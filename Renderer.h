#ifndef _RENDERER_H_
#define _RENDERER_H_


#include "ModelView.h"
#include "Pose.h"
#include "RigidBody.h"
#include "Link.h"
#include "Joint.h"
#include "Flock.h"
#include "Model.h"
#include "Scene.h"
#include "matrix4.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include "shader.h"

GLuint shader = 0;

class Renderer{

public:
	static void init(){
		shader = createShader( "shader/phong" );
		//as far as I can tell, the following commands do nothing
		glEnable(GL_LIGHTING); //Enable lighting
		glEnable(GL_LIGHT0); //Enable light #0
		glEnable(GL_LIGHT1); //Enable light #1
		glEnable(GL_NORMALIZE); //Have OpenGL automatically normalize our normals
		glShadeModel(GL_SMOOTH); //Enable smooth shading
	}

	static void draw(const vertex3& point){
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

	static void draw(const vertex3& vector_0, const vertex3& vector_1){
		glPointSize( 4 );
		glBegin(GL_LINES);
			glColor3f( 1, 0, 0 );	//RED
			glVertex3f((GLfloat)vector_0.getx(), (GLfloat)vector_0.gety(), (GLfloat)vector_0.gety());
			glVertex3f((GLfloat)vector_1.getx(), (GLfloat)vector_1.gety(), (GLfloat)vector_1.gety());
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

	static void draw(const RigidBody& rigidBody){
		//glColorMaterial(GL_FRONT, GL_DIFFUSE);
		//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL); //Enable color
		glColor3f((GLfloat)rigidBody.color.getx(), (GLfloat)rigidBody.color.gety(), (GLfloat)rigidBody.color.getz());
		glUseProgram( shader );
			glutSolidSphere(10, 10, 4);
		glUseProgram( 0 );
	}

	static void draw(const ModelView& model){
		//draw all of the polygons
		//glutSolidTeapot(0.3);
		//glColorMaterial(GL_FRONT, GL_DIFFUSE);
		//glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL); //Enable color
		//glColor3f((GLfloat)model.color.getx(), (GLfloat)model.color.gety(), (GLfloat)model.color.getz());
		glUseProgram( shader );
		 for (unsigned int i=0 ; i < model.edges.size(); i++){
			 polygon poly = model.edges.at(i);
			 switch(poly.type){
				case triangle:
					glBegin (GL_TRIANGLES);
					glNormal3f((GLfloat)poly.normal.getx(), (GLfloat)poly.normal.gety(), (GLfloat)poly.normal.getz());		//shouldn't I only have to do this once? TODO, not if shader
					for(unsigned int i=0; i< 3; i++){
						glColor3f((GLfloat)model.color.getx(), (GLfloat)model.color.gety(), (GLfloat)model.color.getz());
						int index = poly.vertexIndices.at(i);
						GLfloat x = (GLfloat)model.vertices.at(index).getx();
						GLfloat y = (GLfloat)model.vertices.at(index).gety();
						GLfloat z = (GLfloat)model.vertices.at(index).getz();
						vertex3 normal= model.normals.at(index);
						glNormal3f(normal.getx(), normal.gety(), normal.getz());
						glVertex3f(x, y, z);
					}
					glEnd();
				break;
				case quad:
					glBegin (GL_QUADS);
					//glNormal3f((GLfloat)poly.normal.getx(), (GLfloat)poly.normal.gety(), (GLfloat)poly.normal.getz());	
					for(unsigned int i=0; i< 4; i++){
						glColor3f((GLfloat)model.color.getx(), (GLfloat)model.color.gety(), (GLfloat)model.color.getz());
						int index = poly.vertexIndices.at(i);
						GLfloat x = (GLfloat)model.vertices.at(index).getx();
						GLfloat y = (GLfloat)model.vertices.at(index).gety();
						GLfloat z = (GLfloat)model.vertices.at(index).getz();
						vertex3 normal= model.normals.at(index);
						glNormal3f(normal.getx(), normal.gety(), normal.getz());
						glVertex3f(x, y, z);
					}
					glEnd();
				break;
			}
      	   //glDisable(GL_COLOR_MATERIAL); //Disable color
			glUseProgram( 0 );
			//To draw vertex normals
			//For debugging purposes
			#if 0
				glBegin (GL_LINES);
				glLineWidth(2.0);
				for (int index=0; index < model.vertices.size(); ++index){
					glColor3f( 1.0, 1.0, 0.0 );
					GLfloat x = (GLfloat)model.vertices.at(index).getx();
					GLfloat y = (GLfloat)model.vertices.at(index).gety();
					GLfloat z = (GLfloat)model.vertices.at(index).getz();
					glVertex3f(x, y, z);
					//glVertex3fv (model.vertices[i].getPointer());
					GLfloat x1 = (model.vertices.at(index).getx() + model.normals.at(index).getx()) * .5f ;
					GLfloat y1 = (model.vertices.at(index).gety() + model.normals.at(index).gety()) * .5f ;
					GLfloat z1 = (model.vertices.at(index).getz() + model.normals.at(index).getz()) * .5f ;
					glVertex3f(x1, y1, z1);
				}
				glEnd();
			#endif
		 }
	}

	static void draw(Actor* actor){
		//draw(actor->path);		//pointer?

		Pose inbetween = actor->update();
		draw(&inbetween, &actor->model);

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
		
	}

	static void draw(Pose* pose, ModelView* model){
		glPushMatrix();
			matrix4 transform;
			//translate
			transform =	pose->translate_object();

			//rotate
			pose->rotation_quat.normalize();
			transform = transform * pose->getRotation();
			//transform = pose->getRotation();
			//cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			//glutSolidTeapot(0.3);
			draw(*model);
		glPopMatrix();		
	}

	static void draw(Pose* pose){
		glPushMatrix();
			matrix4 transform;
			pose->rotation_quat.normalize();
			transform = pose->getRotation();
			//cout << "The matrix transform is:" << endl << transform.matrix << endl;

			applyTransformation(transform);		
			draw(vertex3());
		glPopMatrix();		
	}

	static void drawReferenceLines() {
    	glPushMatrix();
		draw(vertex3(), vertex3(2.0f, 0.0f, 0.0f));
		draw(vertex3(), vertex3(0.0f, 2.0f, 0.0f));
		draw(vertex3(), vertex3(0.0f, 0.0f, 2.0f));
		glPopMatrix();		
	}

	static void draw(Scene* scene){
		//create a frame of reference
		//drawReferenceLines();

		for(unsigned int i=0; i<scene->actors.size(); i++){
			draw(scene->actors.at(i));
		}
		if(scene->flock != NULL){
			draw(scene->flock);
		}
		if(scene->hierarchy != NULL){
			//printf("Hierarchy attempting to draw\n");
			draw(scene->hierarchy);
		}
		for(unsigned int i=0; i<scene->models.size(); ++i){
			draw(*scene->models.at(i));
		}	
	}

	static void draw(LinkRoot* hierarchy){
		//move the root with it's trajectory
		hierarchy->update();
		draw(hierarchy->path);
		
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
			if(model.type == 0){
				glColor3f(model.mesh.color.getx(), model.mesh.color.gety(), model.mesh.color.getz());
				draw(model.mesh);
			}else if(model.type == 1){
				glColor3f(model.mesh.color.getx(), model.mesh.color.gety(), model.mesh.color.getz());
				glutSolidSphere(1.0, 10, 10);
			}else if(model.type == 2){
				glutSolidTeapot(1.0);
			}
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



	static void applyTransformation(const matrix4& mat){
		float m[16];
		matrix4 mat_transpose = mat.transpose();
		mat_transpose.ToArray(m);
		glLoadMatrixf( m );
	}

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

};

#endif