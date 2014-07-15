#include "backend/SmartBase.h"
#include "backend/Random.h"
#include <cmath>

#include <iostream>

// initializing the percentages to 0, since the base starts with zero resources
float SmartBase::p_per = 0;
float SmartBase::g_per = 0; 
float SmartBase::i_per = 0;	

SmartBase::SmartBase(Map m) : _m(m) { }

/*- -------------------------------------------------------------- -*/
void SmartBase::get_needs(float &p, float &g, float &i) {
	calc_percentages();	// refreshing the percentages of resources in the base

	p = p_per;	// returning the palladium percentage
	g = g_per;	// returning the platinum percentage
	i = i_per;	// returning the iridium percentage
}

/*- -------------------------------------------------------------- -*/
void SmartBase::calc_percentages() {
	float total_res = 0;

	// calculating the total resources
	for (int i=0; i<3; i++) {
		total_res += _m.base_res[i];
	}

	// calculating the percentage of each resource
	p_per = ((float)_m.base_res[0]/total_res)*100 ;
	g_per = ((float)_m.base_res[1]/total_res)*100 ;
	i_per = ((float)_m.base_res[2]/total_res)*100 ;
}

/*- -------------------------------------------------------------- -*/
void SmartBase::calc_exact_needs(int x, int y, float rem_load, float &p, float &g, float &i) {
	calc_percentages();	// refresh the percentages of the resources in the base

	// initialize all values to zero
	p = 0;
	g = 0;
	i = 0;

	// resource in this position
	float cur_p = _m.resources_p[x][y];
	float cur_g = _m.resources_g[x][y];
	float cur_i = _m.resources_i[x][y];

	// total resources sum in this position
	float res_in_cur_pos = cur_p + cur_g + cur_i;
 	
 	// deciding what resources the robot will pick up

 	// initially the base doesn't have any resources so there is no need to calculate the base needs
 	if ( (p_per == 0 && g_per == 0 && i_per == 0) || (_m.base_res[0] == 0 && _m.base_res[1] == 0 && _m.base_res[2] == 0) ) {
 		if (rem_load - cur_p > 0 ) {
 			p = cur_p;
 		}

 		if (rem_load - cur_g > 0 ) {
 			g = cur_g;
 		}

 	 	if (rem_load - cur_i > 0 ) {
 			i = cur_i;
 		}	

 	}else if (rem_load > res_in_cur_pos) {
		// the robot can pick up all the resources 
 		p = cur_p;
 		g = cur_g;
 		i = cur_i; 

	}else{	// we have to chose what to pick up according to the base needs since we don't have
			// enough space on the robot to pick them all up
		switch (min_resource()){
			case 'p':
				if (rem_load>=cur_p)
					p = cur_p;		// we can pick up the remaining resources until we fill up the remaining load
				else{
					p = rem_load;	// filling up the remaining load
				}
				break;
			case 'g':
				if (rem_load>=cur_g)
					g = cur_g;		// we can pick up the remaining resources until we fill up the remaining load
				else{
					g = rem_load;	// filling up the remaining load
				}
				break;
			case 'i':
				if (rem_load>=cur_i)
					i = cur_i;		// we can pick up the remaining resources until we fill up the remaining load
				else{
					i = rem_load;	// filling up the remaining load
				}
				break;
			default:	// can't pick up anything 
				p = 0;
				g = 0;
				i = 0;
				break; 
		}

	}
}

/*- -------------------------------------------------------------- -*/
char SmartBase::min_resource() {
	// calculating which resource resource we need the most in the base
	int min = p_per;

	if (p_per==0 && g_per==0 && i_per==0)
		return '0';	// we haven't got any resources in stock yet

	if (min>g_per && i_per>g_per)
		return 'g';	// we need more platinum

	if (min>i_per && g_per>i_per)
		return 'i';	// we need more iridium

	return 'p'; 	// we need more palladium
}

/*- -------------------------------------------------------------- -*/

