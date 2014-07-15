#ifndef RANDOM_H
#define RANDOM_H

class Random{
	public:	
		Random();							 // constructor that gives a random seed to srand
		int random_int(int limit);			 // returns a random int from 0 to 'limit'
		float random_float(int limit); 	 	 // returns a random float from 0 to 'limit'
		float random_0_to_1();				 // returns a random float between 0 & 1
		float random_0_to_1(float offset);	 // returns a random float between 0 & 1, added/subtracted an offset value
};

#endif