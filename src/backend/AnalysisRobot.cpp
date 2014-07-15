#include "backend/AnalysisRobot.h"
#include "backend/Robot_management.h"
#include "backend/Random.h"
#include "backend/SmartBase.h"

#include <iostream>
#include <cmath>


/*- -------------------------------------------------------------- -*/
AnalysisRobot::AnalysisRobot(int id, int width, int height) : Vehicle(width, height) {
	// giving our robot a type and a unique id
	set_type('A');
	set_num_id(id);

	// setting the robot status
    def_status = 50;
	set_status(def_status);

	// setting the initial load of the robot
	def_load = 10.0;
	maxLoad = def_load;
	remLoad = def_load;

	// initially the robot hasn's picked up any resources
	total_p = 0.0;
	total_g = 0.0;
	total_i = 0.0;

	// setting the inventory to 0
	for (int i=0; i<3; i++)
		inventory[i] = 0;
}

/*- -------------------------------------------------------------- -*/
void AnalysisRobot::operate(Map& m){
	SmartBase s = SmartBase(m); // making an instance of the SmartBase class

	float p, g, i;		// values for the 3 elements that the robot has to pick up

	checkRobotDeath();	// checking to see if the robot must be removed from the map

	// checking to see if the robot needs to go to the base
	if (remLoad<1)
		gotoBase = true;
	else 
		gotoBase = false;

	// robot has to return to the base
	if (gotoBase==true){
		// teleport the robot out of the map and into the base
		set_pos_x(-1);
		set_pos_y(-1);

		for (int i=0; i<3; i++){
			m.base_res[i] += inventory[i];
			inventory[i] = 0;
		}

		remLoad = def_load;		// reset the remaining load since we got back to the base
	}else{
		// exiting the base and getting back into the map
		if (get_pos_x()==-1 || get_pos_y()==-1) {
			returnToMap();
		}
		
		// using method to calculate what is the best amount of resources to pick up
		s.calc_exact_needs(get_pos_x(), get_pos_y(), remLoad, p, g, i);

		// removing resources from the map tile
		m.resources_p[get_pos_x()][get_pos_y()] -= p;
		m.resources_g[get_pos_x()][get_pos_y()] -= g;
		m.resources_i[get_pos_x()][get_pos_y()] -= i;

		// picking up the resources
		inventory[0] += p;
		inventory[1] += g;
		inventory[2] += i;

		total_p += p;
		total_g += g;
		total_i += i;

		remLoad -= (p+g+i); // decrease the robot's ability to carry resources
		remLoad = truncf(remLoad*10.0)/10.0;
	}

}

/*- -------------------------------------------------------------- -*/
float AnalysisRobot::get_total_p() { return total_p; }
float AnalysisRobot::get_total_g() { return total_g; }
float AnalysisRobot::get_total_i() { return total_i; }

/*- -------------------------------------------------------------- -*/
float AnalysisRobot::get_total_res_picked() {
	return (total_p+total_g+total_i); // returning the total resources this robot has picked up
}

/*- -------------------------------------------------------------- -*/
void AnalysisRobot::export_info(float& p, float& g, float& i, float& rem){
	// exporting the current resources this robot is carrying

	p = inventory[0];
	g = inventory[1];
	i = inventory[2];

	rem = remLoad;
}

/*- -------------------------------------------------------------- -*/
void AnalysisRobot::export_total_info(float& tp, float& tg, float& ti, float& total_res){
	// returning the totals for the resources this robot has picked up

    tp = total_p;
    tg = total_g;
    ti = total_i;

    total_res = total_p+total_g+total_i;
}

/*- -------------------------------------------------------------- -*/

