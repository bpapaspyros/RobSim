#include "backend/Random.h"
#include <ctime>
#include <cstdlib>

/*- -------------------------------------------------------------- -*/
Random::Random(){}

/*- -------------------------------------------------------------- -*/
int Random::random_int(int lim){
	return rand()%lim;					// random number in [0, lim) (integer)
}

float Random::random_float(int lim){
	return (float)(rand()%lim); 		// random number in [0, lim) (float)
}

float Random::random_0_to_1(){
	return (float)( (random_int(100)+1)/100.0);	// random number in [0, 1]
}

/*- -------------------------------------------------------------- -*/
float Random::random_0_to_1(float offset){
	return (float)((random_int(100)+1)/100.0 + offset); // random numbder in [0+offset, 1+offset]
}

/*- -------------------------------------------------------------- -*/
