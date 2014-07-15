#include "backend/Vehicle.h"
#include "backend/Random.h"
#include "backend/Robot_management.h"
#include "backend/Probability.h"

#include <ctime>
#include <cstdlib>
#include <iostream>

using std::string;

Vehicle::~Vehicle(){
}

/*- -------------------------------------------------------------- -*/
Vehicle::Vehicle(int width, int height) {
	Random r; 						// make an instance for the class Random

	// storing the map's size
	_width = width;
	_height = height;

	pos_x = r.random_int(_width);	// x axis
	pos_y = r.random_int(_height);	// y axis

	gotoBase = false;				// check if the robot has to return to the base
	
	def_status = 10;				// life remaining
	status = def_status;			// storing the default status of the robot so that we can restore it when it is repaired

    def_velocity = r.random_int(3)+1;	// velocity for the robot (default value) (zero not included in the defaults)
    velocity = def_velocity;	 		// stores the robots initial velocity so that we can restore it after it is damaged
	
	travelled = 0;					// counter for the total moves a robot has made
	stayed_broken = 0;				// rounds that the robot has remained broken
	total_damages = 0;				// counter for the damages that the robot has taken

	ability = r.random_0_to_1();	// ability for the robot to overcome obstacles
}

/*- -------------------------------------------------------------- -*/
int Vehicle::get_pos_x()   	 {  return pos_x;    }	// return the robot's x position	
int Vehicle::get_pos_y()   	 {  return pos_y;    }	// return the robot's y position
int Vehicle::get_velocity()	 {  return velocity; }	// return the robot's velocity
int Vehicle::get_status()  	 {  return status;   }	// return the robot's status
float Vehicle::get_ability() {  return ability;  }	// return the robot's ability
int Vehicle::get_travelled() {  return travelled;}	// returns total distance travelled

bool Vehicle::get_brokeQ()   {
    if (stayed_broken > 14){
        return true;	// returns true if the robot has stayed broken for too many rounds
    }

    return false;
}

// getters for robot values
char Vehicle::get_type()  { return type;   }	// return the robot's type
int Vehicle::get_num_id() { return num_id; }	// return the robot's unique num_id

//Setters for robot values
void Vehicle::set_type(char robot_type) { type = robot_type; }	// set the type of the robot
void Vehicle::set_num_id(int num)		{ num_id = num;      }	// set the num_id for the robot
void Vehicle::set_velocity(int num)		{ velocity = num;    }	// set the velocity for the current robot
void Vehicle::set_pos_x(int x)			{ pos_x = x;		 }	// implicitly sets the position of a robot
void Vehicle::set_pos_y(int y)			{ pos_y = y; 		 }	// implicitly sets the position of a robot
void Vehicle::set_ability(float ab)		{ ability = ab;		 }	// implicitly sets the ability of a robot

void Vehicle::set_status(int n_status)	{ 
	// set the status of the robot
	status = n_status; 

	if (status <=0) 
		set_velocity(0);
	else
		set_velocity(def_velocity);

}	

/*- -------------------------------------------------------------- -*/
void Vehicle::returnToMap(){
	// gets the robot back in the map and puts it in the middle
	set_pos_x(_width/2);
	set_pos_y(_height-1);
}
/*- -------------------------------------------------------------- -*/
bool Vehicle::checkRobotDeath() {
	// making an instance of the Robot_management class
	Robot_management r = Robot_management(_width, _height);

	if (stayed_broken >= 15) {	// robot stayed damaged for way too long and is deleted permenantly
        r.roblist[num_id] -> set_pos_x(-1);
        r.roblist[num_id] -> set_pos_y(-1);

        gotoBase = true;

		return true;	
	}

	return false;
}

/*- -------------------------------------------------------------- -*/
int Vehicle::move(Map m){
	// robot is damaged and can't move until it is repaired 
	if (velocity==0) {
		stayed_broken++;	// if the robot stays still for a set amount of rounds then it dies

        checkRobotDeath();

		return -1;
	}

	// the robot is in base and must not move
	if (get_pos_x()==-1 || get_pos_y()==-1)
		return -1;	// therefore returning -1 to inform the programmer that the robot didn't move


	int random_dir; // random number for the robot's direction

	// checks if we need to get another random number
	bool loop = true;

	Random r; 		// making an instance for the random class
	Probability pr;	// making an instance for the probability class

	// getting this robot's current coordinates
	int cur_x;	// current x position
	int cur_y;	// current y position

	cur_x = Robot_management::roblist[get_num_id()]->get_pos_x();
	cur_y = Robot_management::roblist[get_num_id()]->get_pos_y();

	// the new coordinates initialized with the current x, y values
	int new_x = cur_x;	// new x
	int new_y = cur_y;	// new y

	while(loop==true){
		// getting a random direction
		random_dir = r.random_int(4);	// north, south, west, east 
		switch (random_dir){
			/* * * * * * * * * * *
			 *	  Moving north   *
 			 * * * * * * * * * * */
			case 0:	// north
				if ( cur_y==0  )
					continue;
				else if (m.flags[cur_x][cur_y-1]) {
					if (pr.calc_probability(get_ability(), m.get_danger(cur_x, cur_y-1))<0.5){
						new_y = cur_y-1;
						loop = false;
					}else
						continue;
				}
				else {
					new_y = cur_y-1;		
					loop = false;
				}
				break;
			/* * * * * * * * * * *
			 *	  Moving south   *
 			 * * * * * * * * * * */
			case 1:	// south
				if ( cur_y==(_height-1) ) // invalid because it will go out of bounds
					continue;
				else if (m.flags[cur_x][cur_y+1]) {
					if (pr.calc_probability(get_ability(), m.get_danger(cur_x, cur_y+1))<0.5){
						new_y = cur_y+1;
						loop = false;
					}else
						continue;
				}else{
					new_y = cur_y+1;
					loop = false;
				}
				break;
			/* * * * * * * * * * *
			 *	  Moving west    *
 			 * * * * * * * * * * */
			case 2:	// west
				if ( cur_x==0 ) // invalid because it will go out of bounds
					continue;
				else if (m.flags[cur_x-1][cur_y]) {
					if (pr.calc_probability(get_ability(), m.get_danger(cur_x-1, cur_y))<0.5){
						new_x = cur_x-1;
						loop = false;
					}else
						continue;
				}else{
					new_x = cur_x-1;
					loop = false;
				}
				break;
			/* * * * * * * * * * *
			 *	  Moving east    *
 			 * * * * * * * * * * */
			case 3:	// east
				if ( cur_x==(_width-1) ) // invalid because it will go out of bounds
					continue;
				else if (m.flags[cur_x+1][cur_y]) {
					if (pr.calc_probability(get_ability(), m.get_danger(cur_x, cur_x+1))<0.5){
						new_x = cur_x+1;
						loop = false;
					}else
						continue;
				}else{
					new_x = cur_x+1;
					loop = false;
				}
				break;
		}// switch
	}// while 

	// setting the new random positions
	Robot_management::roblist[get_num_id()]->set_pos_x(new_x);
	Robot_management::roblist[get_num_id()]->set_pos_y(new_y);

	travelled++;	// robot moved one position

	return random_dir;
}

/*- -------------------------------------------------------------- -*/
// export methods for usefull variables that need to be displayed in the UI

void Vehicle::export_info(float& p, float& g, float& i, float& remLoad){
    p = 0;
    g = 0;
    i = 0;
    remLoad = 0;
}

void Vehicle::export_info(bool& successfulOperation){
    successfulOperation = false;
}

void Vehicle::export_total_info(float& tp, float& tg, float& ti, float& total_res){
    tp = 0;
    tg = 0;
    ti = 0;
    total_res = 0;
}

void Vehicle::export_total_info(int& total_successfullOps){
    total_successfullOps = 0;
}

/*- -------------------------------------------------------------- -*/
