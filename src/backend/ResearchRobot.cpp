#include "backend/ResearchRobot.h"
#include "backend/Robot_management.h"

#include <iostream>

bool ResearchRobot::flagDeployedThisRound = false;


/*- -------------------------------------------------------------- -*/
ResearchRobot::ResearchRobot(int id, int width, int height) : Vehicle(width, height) {
	set_type('S');
	set_num_id(id);

	// setting the robot status
    def_status = 50;
	set_status(def_status);

	// initially the robot hasn't deployed any flags
	flagsDeployed = 0; 
}

/*- -------------------------------------------------------------- -*/
void ResearchRobot::operate(Map& m){
	flagDeployedThisRound = false;

	checkRobotDeath();	// checking to see if the robot must be removed from the map


	if (m.dangers[get_pos_x()][get_pos_y()] >= 0.6) {	// this area is considered dangerous
		m.flags[get_pos_x()][get_pos_y()] = true; 		// setting down a flag so that other robots can avoid this area
		
		flagsDeployed++; 				// increase the flags counter for this robot
		flagDeployedThisRound = true;	// deployed a flag
	}
}

/*- -------------------------------------------------------------- -*/
int ResearchRobot::get_flagsDeployed() {
	return flagsDeployed; 	// returns the flags deployed by this robot
}

/*- -------------------------------------------------------------- -*/
bool ResearchRobot::get_flagDeployedThisRound(){
	return flagDeployedThisRound; // returns true if a flag was deployed in this round
}

/*- -------------------------------------------------------------- -*/
void ResearchRobot::export_info(bool& successfulOperation){
	successfulOperation = get_flagDeployedThisRound();
}

void ResearchRobot::export_total_info(int& total_successfullOps){
    total_successfullOps = flagsDeployed; // returns the number of the flags that were deployed
}
/*- -------------------------------------------------------------- -*/
