#ifndef SMARTBASE_H
#define SMARTBASE_H

#include "Map.h"

class SmartBase{
	public:
		SmartBase(Map m); // initializing variables
		void get_needs(float &p, float &g, float &i);	// gets the needs of the base according to the stock percentages
		void calc_exact_needs(int x, int y, float rem_load, float &p, float &g, float &i);	// calculates the exact resource that the current robot should pick up
	private:
		void calc_percentages();	// calculates percentage of each resource that is in stock
		char min_resource();		// calculates the minimum resource out of the three given 
		Map _m;
		static float p_per, g_per, i_per;	// resource percentages
};

#endif
