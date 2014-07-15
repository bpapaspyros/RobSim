#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "Map.h"
#include "Vehicle.h"

class AnalysisRobot : public Vehicle {
	public:
		AnalysisRobot(int id, const int width, const int height);

	    virtual void operate(Map& m);	// robot's operation method
		float get_total_p();			// returns the total palladium picked up
		float get_total_g();			// returns the total platinum picked up
		float get_total_i();			// returns the total iridium picked up
		float get_total_res_picked();	// returns the sum of all the resources that were picked up
		float inventory[3];	// robot's current inventory
		float remLoad;  // the robot's remaining load
		
		// exporting data so that other classes can use them
		virtual void export_info(float& p, float& g, float& i, float& tot);
        virtual void export_total_info(float& tp, float& tg, float& ti, float& total_res);
	private:
		float def_load;	// the robot's default load
		float maxLoad;	// the robot's max load
		float total_p;	// total palladium picked up
		float total_g;	// total platinum picked up
		float total_i;	// total iridium picked up
};


#endif
