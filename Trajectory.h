
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

#pragma once
#include <stdlib.h>
#include <vector>
#include "Pose.h"
#include "Renderer.h"
#include "matrix4.h"
#include "Vector4.h"
#include <Eigen/Geometry>

class Trajectory{
	enum cubicSpline{catmullRom, bSpline};
	enum Cycle{circle, pendulum, clamp};
private:
		void init();

		void setSplineType();

		int clampIndex(unsigned int index);

public:
		Trajectory();
		Trajectory(vector<PoseKey*> cps);

		Trajectory(cubicSpline spline);

		~Trajectory();

		bool isCalculated();

		void generateGeneralControlPoints();
		
		virtual Pose& update();

//member data
		vector<PoseKey*> controlPoints;
		cubicSpline splineType;
		Cycle cycleType;
		vector<vertex3*> pointsAlongPath;


	private:
		Eigen::MatrixXf M;
		int startFrame;
		int endFrame;
		int u;
		unsigned int cpIndex;
		bool forward;		//Used for pendulum splines
		bool end;			//Used for clamped splines
		float PofU[7];
	public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
};

#endif
