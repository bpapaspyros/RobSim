#include "backend/Robot_management.h"
#include "backend/Vehicle.h"

#include <iostream>
#include <vector>


std::vector<Vehicle *> Robot_management::roblist;

Robot_management::Robot_management(int width, int height) {
	this->width = width;
	this->height = height;
}

/*- -------------------------------------------------------------- -*/
void Robot_management::addRobot(char type){
	type = tolower(type);

	switch (type){
		case 'a': {					// user chose to add an analysis robot
			roblist.push_back(new AnalysisRobot(roblist.size(), width, height));	// adding the robot
			break;
		}
		case 'r': {					// user chose to add a rescue robot
			roblist.push_back(new RescueRobot(roblist.size(), width, height));	// adding the robot
			break;
		}
		case 's':					// user chose to add a research robot
		default: {					// defaulting in research robots			implicitly setting the robot's position
			roblist.push_back(new ResearchRobot(roblist.size(), width, height));	// adding the robot
			break;
		}
	}

}

/*- -------------------------------------------------------------- -*/
void Robot_management::addRobot(char type, int x, int y){
	type = tolower(type);

	switch (type){
		case 'a': {					// user chose to add an analysis robot
			// implicitly setting the robot's position
			roblist.push_back(new AnalysisRobot(roblist.size(), width, height));	// adding the robot
			break;
		}
		case 'r': {					// user chose to add a rescue robot
			// implicitly setting the robot's position
			roblist.push_back(new RescueRobot(roblist.size(), width, height));	// adding the robot
			break;
		}
		case 's':					// user chose to add a research robot
		default: {					// defaulting in research robots
			roblist.push_back(new ResearchRobot(roblist.size(), width, height));	// adding the robot
			break;
		}
	}

    roblist[roblist.size()-1]->set_pos_x(x);
    roblist[roblist.size()-1]->set_pos_y(y);

}

/*- -------------------------------------------------------------- -*/
void Robot_management::delete_robot(int id){
	// deleting a robot based on its unique id

    uint position;

    for (uint i=0; i<roblist.size(); i++){
        if(roblist[i]->get_num_id() == id){
            position = i;

            delete roblist[position];					// deleting the object
            roblist.erase(roblist.begin()+position);	// removing from the vector


            for(uint i=position; i<roblist.size(); i++){
                roblist[i]-> set_num_id(roblist[i]->get_num_id() - 1);	// correcting the id numbering
            }
        }
    }
}

/*- -------------------------------------------------------------- -*/
bool Robot_management::checkForRobots(uint x, uint y) {

	/* checking the vector containing the active robots *
	 * to see whether there is a robot in the area with *
	 * x, y coordinates and returning 1 if one is found */ 

	for (uint i=0; i<roblist.size(); i++){
		uint pos_x = roblist[i]-> get_pos_x();
		uint pos_y = roblist[i]-> get_pos_y();

		if (pos_x==x && pos_y==y)
			return true;
	}

	return false;	
}

/*- -------------------------------------------------------------- -*/
bool Robot_management::checkForRobots(uint x, uint y, int &num, char &type) {

	/* checking the vector containing the active robots *
	 * to see whether there is a robot in the area with *
	 * x, y coordinates and returning 1 if one is found */ 

	for (uint i=0; i<roblist.size(); i++){
		uint pos_x = roblist[i]->get_pos_x();
		uint pos_y = roblist[i]->get_pos_y();

		if (pos_x==x && pos_y==y){
			num = roblist[i] -> get_num_id();
			type = roblist[i]-> get_type();
			return true;
		}
	}

	type = 'U';
	num = -1;
	return false;
	
}

/*- -------------------------------------------------------------- -*/
bool Robot_management::listEmpty(){
	// returns true if the robot list is empty

	if ( roblist.empty() )	
		return true;
	else
		return false;
}

/*- -------------------------------------------------------------- -*/
void Robot_management::emptyList(){
	// emptying the list of all robots 

	uint lim = roblist.size();
	for (uint i=0; i<lim; i++){
		delete roblist[i]; 
	}

	roblist.clear();
}

/*- -------------------------------------------------------------- -*/
