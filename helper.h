#include <stdlib.h>

class Helper{
public:
	static float randBetween_float(float min, float max){
			float ret = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
			return ret;
	}


	static double randBetween_double(double min, double max){
			double ret = min + static_cast <double> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
			return ret;
	}

};