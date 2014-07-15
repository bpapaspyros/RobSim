#include "backend/RescueRobot.h"
#include "backend/Robot_management.h"

#include <iostream>

bool RescueRobot::repairedThisRound = false;

/*- -------------------------------------------------------------- -*/
RescueRobot::RescueRobot(int id, int width, int height) : Vehicle(width, height), _width(width), _height(height) {
	set_type('R');
	set_num_id(id);

	// setting the robot status
	def_status = 100;
	set_status(def_status);

	// initially the rescue robot hasn't repaired any other robots
	repaired = 0;
}

/*- -------------------------------------------------------------- -*/
void RescueRobot::operate(Map& m){
	repairedThisRound = false;

	// creating an instance of the Robot_management class
	Robot_management r = Robot_management(m.get_width(), m.get_height()); 

	int num;		// robot's unique num id
	char type;		// robot's type

	checkRobotDeath();	// checking to see if the robot must be removed from the map

	// getting the rescue robot's current postition
	int cur_x = get_pos_x();
	int cur_y = get_pos_y();

	// checking west
	if (cur_x-1>0) {
		if ( r.checkForRobots(cur_x-1, cur_y, num, type) ) {
			repair(num);
		}
	}

	// checking east
	if (cur_x+1<_width) {
		if ( r.checkForRobots(cur_x+1, cur_y, num, type) ) {
			repair(num);
		}
	}

	// checking south
	if (cur_y+1<_height) {
		if ( r.checkForRobots(cur_x, cur_y+1, num, type) ) {
			repair(num);
		}
	}

	// checking north
	if (cur_y-1>0) {
		if ( r.checkForRobots(cur_x, cur_y-1, num, type) ) {
			repair(num);
		}
	}

	// checking current position
	// repairs robots in the current position except for itself
	if ( r.checkForRobots(cur_x, cur_y, num, type) ) {
		if ( !( type == get_type() && num == get_num_id()) )
			repair(num);
	}
}

/*- -------------------------------------------------------------- -*/
void RescueRobot::repair(int num_id) {
	Robot_management r = Robot_management(_width, _height);
	
	repaired++; // another robot is repaired
	repairedThisRound = true;

	r.roblist[num_id]->set_status(r.roblist[num_id]->def_status); // repairing the nearest robot to its default status 
}

/*- -------------------------------------------------------------- -*/
int RescueRobot::get_repaired() { return repaired; }
bool RescueRobot::get_repairedThisRound() { return repairedThisRound; }

/*- -------------------------------------------------------------- -*/
void RescueRobot::export_info(bool& successfulOperation){
	successfulOperation = get_repairedThisRound();
}

void RescueRobot::export_total_info(int& total_successfullOps){
    total_successfullOps = repaired;
}

/*- -------------------------------------------------------------- -*/
