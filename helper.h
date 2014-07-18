#include <stdlib.h>
#include <time.h>

class Helper{
public:
	static void seed(){
		srand(time(0));
	}

	static float randBetween_float(float min, float max){
			float ret = min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
			return ret;
	}


	static double randBetween_double(double min, double max){
			double ret = min + static_cast <double> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
			return ret;
	}

	static int randBetween_int(int min, int max){
			double ret = min + static_cast <int> (rand()) /( static_cast <int> (RAND_MAX/(max-min)));
			return ret;
	}

};