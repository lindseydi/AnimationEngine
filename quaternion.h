#ifndef _QUATERNION_H_
#define _QUATERNION_H_


#pragma once
#include "vertex3.h"
#include <Eigen/Geometry>
#include <math.h>
#include "matrix4.h"
//Needed for Quaternionf
using namespace Eigen;

class quaternion{
public:
	Quaternionf quat;
	//Default Constructor
	quaternion() {
		quat.x() = 0.0f;
		quat.y() = 0.0f;
		quat.z() = 0.0f;
		quat.w() = 0.0f;
	}
	//! Constructor
	quaternion(float x, float y, float z, float w) {
		this->quat.x() = x;
		this->quat.y() = y;
		this->quat.z() = z;
		this->quat.w() = w;
	}

	//! Constructor which converts euler angles (radians) to a quaternion
	quaternion(float x, float y, float z){
		*this = set(x, y, z);
	}

	//! Constructor which converts euler angles (radians) to a quaternion
	quaternion(const vertex3& vec){ 
		*this = set(vec.vertex.x(), vec.vertex.y(), vec.vertex.z());
	}

	~quaternion(void){
		//do nothing there are no pointers
	}

	quaternion& set(float x, float y, float z, float w)
	{
		this->quat.x() = x;
		this->quat.y() = y;
		this->quat.z() = z;
		this->quat.w() = w;
		return *this;
	}


	// sets new quaternion based on euler angles
	quaternion& set(float x, float y, float z)
	{
		float angle;

		angle = x * 0.5;
		const float sr = sin(angle);
		const float cr = cos(angle);

		angle = y * 0.5;
		const float sp = sin(angle);
		const float cp = cos(angle);

		angle = z * 0.5;
		const float sy = sin(angle);
		const float cy = cos(angle);

		const float cpcy = cp * cy;
		const float spcy = sp * cy;
		const float cpsy = cp * sy;
		const float spsy = sp * sy;

		quat.x() = (float)(sr * cpcy - cr * spsy);
		quat.y() = (float)(cr * spcy + sr * cpsy);
		quat.z() = (float)(cr * cpsy - sr * spcy);
		quat.w() = (float)(cr * cpcy + sr * spsy);

		return normalize();
	}

	float getx(){
		return quat.x();
	}

	float gety(){
		return quat.y();
	}

	float getz(){
		return quat.z();
	}
	
	float getw(){
		return quat.w();
	}
quaternion& quaternion::normalize()
{
		/*
		float magnitude = sqrt(W*W + X*X + Y*Y + Z*Z);
		this-> = this->W / magnitude;
		this->X = this->X /  magnitude;
		this->Y = this->Y / magnitude;
		this->Z = this->Z / magnitude;
		*/

		this->quat.normalize();
		return *this;
}

quaternion& operator=(const quaternion& other)
{
	this->quat = other.quat;
	return *this;
}

	// multiplication operator
quaternion operator*(const quaternion& input) const
{
	quaternion temp;

	temp.quat = this->quat * input.quat;
	return temp;
}


// multiplication operator
quaternion operator*(float scalar) const
{

	return quaternion(scalar*quat.x(), scalar*quat.y(), scalar*quat.z(), scalar*quat.w());
}


//*= multiplication operator
quaternion& quaternion::operator*=(float scalar)
{
	this->quat.x() *=scalar;
	this->quat.y() *=scalar;
	this->quat.z() *=scalar;
	this->quat.w() *=scalar;
	return *this;
}

//*= multiplication operator
quaternion& operator*=(const quaternion& input)
{
	this->quat *= input.quat;
	return *this;
}

quaternion& rotate(float angle, vertex3 axis){
	quaternion local_rotation;

	//axis is a unit vector
	local_rotation.quat.w()  = cosf( angle/2);
	local_rotation.quat.x() = axis.vertex.x() * sinf( angle/2 );
	local_rotation.quat.y() = axis.vertex.y() * sinf( angle/2 );
	local_rotation.quat.z() = axis.vertex.z() * sinf( angle/2 );

	*this *= local_rotation;
	return *this;
}

quaternion& rotate(float angle, float axisX, float axisY, float axisZ){
	quaternion local_rotation;
	
	//axis is a unit vector
	local_rotation.quat.w()  = cosf( angle/2);
	local_rotation.quat.x() = axisX * sinf( angle/2 );
	local_rotation.quat.y() = axisY * sinf( angle/2 );
	local_rotation.quat.z() = axisZ * sinf( angle/2 );
//	Quaternionf<float, 0> current_rotation =  this->quat.toRotationMatrix();


	return (*this = (local_rotation) * (*this));
}

float dot(quaternion& input){
	//return (this->X * input.X) + (this->Y * input.Y) + (this->Z * input.Z) + (this->W * input.W);
	float retVal = this->quat.dot(input.quat);
	return retVal;
}

/*
quaternion& slerp(quaternion a, quaternion b, float u)
{
	if ( u <= 0.0f )
        {
            return a;
        }
   
        if ( u >= 1.0f )
        {
            return b;
        }
   
		float coshalftheta = a.dotProduct(b);
        //coshalftheta = std::min (1.0f, std::max (-1.0f, coshalftheta));
        quaternion c = b;
   
        // Angle is greater than 180. We can negate the angle/quat to get the
        // shorter rotation to reach the same destination.
        if ( coshalftheta < 0.0f )
        {
            coshalftheta = -coshalftheta;
            c = c*-1;
        }
 
            if ( coshalftheta > 0.99f )
            {
            // Angle is tiny - save some computation by lerping instead.
                    quaternion r (Lerp (a, c, t));
                    return r;
            }
   
        float halftheta = acos (coshalftheta);
        float sintheta = sin (halfTheta);
   
        return (std::sin ((1.0f - t) * halftheta) * a + std::sin (t * halftheta) * c) / std::sin (halftheta);
    }
*/

vertex3 toEuler(){
	
	double X = this->getx();
	double Y= this->gety();
	double Z = this->getz();
	double W = this->getw();
	double test = X*Y + Z*W;
	double sqx = X*X;
    double sqy = Y*Y;
    double sqz = Z*Z;
    double heading = atan2(2*Y*W-2*X*Z , 1 - 2*sqy - 2*sqz);
	double attitude = asin(2*test);
	double bank = atan2(2*X*W-2*Y*Z , 1 - 2*sqx - 2*sqz);
	return vertex3(heading, attitude, bank);
}

matrix4 getRotation(){
	//TODO 
	//test this!
	float X =  quat.x();
	float Y =  quat.y();
	float Z =  quat.z();
	float W =  quat.w();

	matrix4 rot;
	rot(0,0) = 1-2*Y*Y -2*Z*Z;
	rot(0,1) = 2*X*Y - 2*W*Z;
	rot(0,2) = 2*X*Z + 2*W*Y;

	rot(1,0) = 2*X*Y + 2*W*Z;
	rot(1,1) = 1-2*X*X - 2*Z*Z;
	rot(1,2) = 2*Y*Z - 2*W*X;

	rot(2,0) = 2*X*Z - 2*W*Y;
	rot(2,1) = 2*Y*Z + 2*W*X;
	rot(2,2) = 1- 2*X*X - 2*Y*Y;

	rot.matrix(3,3) = 1.0;
	return rot;
}
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

};

#endif