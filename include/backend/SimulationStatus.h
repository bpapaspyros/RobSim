#ifndef SIMULATIONSTATUS_H
#define SIMULATIONSTATUS_H

#include "Map.h"

class SimulationStatus {
	public:
        SimulationStatus();

		bool success(Map m);	// checks if the robots have picked up all the resources
		bool fail();			// checks if all the robots are damaged

        void set_limit(float nlim);	// upper limit for the resources that the base can hold (success)
    private:
        float limit;
};

#endif
