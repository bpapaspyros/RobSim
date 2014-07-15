#ifndef VEHICLE_H
#define VEHICLE_H

#include "Map.h"

#include <vector>
class Vehicle {
	public:
		Vehicle(const int width, const int height);	// costructor -> initializes important variables
		virtual ~Vehicle();
		
		int move(Map m);				  // basic robot operation for moving, returns the dirextion
		virtual void operate(Map& m) = 0; // operate method for every robot
		// getters
		int get_pos_x();		// return robot x axis position
		int get_pos_y();		// return robot y axis position
		int get_velocity();		// return robot velocity
		int get_status();		// return robot remaining life
		int get_num_id();		// get the robot's unique num id
		float get_ability();	// return robot's ability to overcome obstacles
		char get_type();		// returns the type of the robot (A, S, R)
		int get_travelled();	// returns total distance travelled
        bool get_brokeQ();       // is the robot broken ?

		// setters
		void set_type(char type);		// sets the robot's type
		void set_num_id(int num);		// sets the unique num id
		void set_velocity(int num);		// sets the velocity for the current robot
		void set_pos_x(int x);			// implicitly sets the position of a robot
		void set_pos_y(int y);			// implicitly sets the position of a robot
		void set_ability(float ab);		// implicitly sets the ability of a robot 
		void set_status(int n_status);	// set the status of a robot
		void returnToMap();		// gets a robot that is in base back to map	
		
		bool checkRobotDeath();	// if a robot is damaged for too long it dies
		bool gotoBase;			// checks if the robot has to go back to the base
		int def_velocity;	// robot's default velocity
		int def_status;		// robot's default status
		int total_damages;	// total damages taken by the robot

		virtual void export_info(float& p, float& g, float& i, float& remLoad);
		virtual void export_info(bool& successfulOperation);

        virtual void export_total_info(float& tp, float& tg, float& ti, float& total_res);
        virtual void export_total_info(int& total_successfullOps);
	private:
		// Basic robot variables
		void init_vars();	// initializes varibles
		int pos_x;			// robot's x axis position
		int pos_y;			// robot's y axis position
		int velocity;		// robot's velocity
		int status;			// robot's life status (up to 15)
		float ability;		// robot's ability to overcome obstacles
		int num_id;			// each robot has a num id 
		char type;			// each robot has a type
		int travelled;		// distance that a robot has travelled	
		int stayed_broken;	// rounds that the robot has remained broken
		int _width;			// map's width
		int _height;		// map's height
};

#endif
