#include "backend/SimulationStatus.h"
#include "backend/Robot_management.h"

typedef unsigned uint;

SimulationStatus::SimulationStatus(){
    limit = 20;	// setting the upper limit of resources for the simulation to be successfull
}

bool SimulationStatus::success(Map m) {
	/* if the robots have picked up all the resources then the simulation has ended *
	 * successfully 																*/ 

    if ( (m.base_res[0]+m.base_res[1]+m.base_res[2]) > limit ){
        return true;
    }

    for (int i=0; i<m.get_height(); i++){
		for (int j=0; j<m.get_width(); j++){
			if ( !(m.resources_p[j][i]==0 && m.resources_g[j][i]==0 && m.resources_i[j][i]==0) )
				return false;
		}
	}

	return true;
}

/*- -------------------------------------------------------------- -*/
bool SimulationStatus::fail() {
	/* checking to see if the all robots are damaged, if at least once is active then the  *
	 * simulation is good to go until the others are repaired or this one is damaged too   */
	for (uint i=0; i<Robot_management::roblist.size(); i++) {
		if (Robot_management::roblist[i]->get_velocity()>0)
			return false;
	}

	return true;
}
