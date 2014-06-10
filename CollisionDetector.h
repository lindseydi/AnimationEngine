#ifndef _CollisionDetector_H_
#define _CollisionDetector_H_

#include "Model.h"
#include "RigidBody.h"

class CollisionDetector{
	public:
		static bool intersects(RigidBody& sphere, const polygon& poly){
			//for a normalized plane (|p.n|= 1)
			//evaluating the plane equation
			//for a point gives the signed distance 
			//of the point to the plane
			float dist = sphere.pose->position.dotProduct(poly.normal) - poly.d;
			//If sphere center within +/- radius from plane, plane
			//intersects sphere
			dist = std::abs(dist);
			//printf("distance is %f vs. radius %f\n", dist, sphere.getRadius());
			return dist <= sphere.getRadius();
		}

		static bool intersects(RigidBody& sphere_a, RigidBody& sphere_b){
			//Calculate squared distance between centers
			vertex3 d = sphere_a.pose->position - sphere_b.pose->position;
			float dist2 = d.dotProduct(d);
			//Spheres intersect if squared distance is less than squared sum of radii
			float radiusSum = sphere_a.getRadius() + sphere_b.getRadius();
			return dist2 <= radiusSum * radiusSum;
		}
};

#endif