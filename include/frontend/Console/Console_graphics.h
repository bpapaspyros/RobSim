#ifndef CONSOLE_GRAPHICS_H
#define CONSOLE_GRAPHICS_H

#include "backend/Map.h"
#include "backend/Robot_management.h"

typedef unsigned uint;

class Console_graphics{
	public:
		Console_graphics(Map& m);	// initializes important variables
		bool choice();				// returns the user's choice for the oprions given in the toolbar 
		void print_toolbar();		// prints a toolbar on the bottom of the page
	private:
		void start_new_sim();		// start the simulation
		void run_sim(char mode);	// runs the simulation, drawing the chosen (mode) map
		
		// drawing methods
		void draw_danger_map();		// printing a map showing the dangers
		void draw_resources_map();	// printing a map showing the resources
		void draw_plex_map();		// printing a map whithout any info, just a plex
		void draw_base();			// printing the base

		// printing methods
		void print_view_switcher(char view); // lets the user change map views
		void print_edit_menu();				 // printing a menu with various variables of the program for the user to edit

		// edit methods 
		void edit_dangers();		// edit the danger level in a position of the map
		void edit_resources();		// edit the resources in a position of the map
		void edit_robots();			// add/delete robots
        void edit_robot_variables();// edit the variables of a robot
        void sim_settings();
	
	
		// User input
		// reads the user's input and checks so that it is between lower_lim and upper_lim
		void get_choice();			// reads the user's choice and checks for correct input
		float get_input(float lower_lim, float upper_lim);	
		void get_robot_coordinates(int &x, int &y);	// gives back a robot's coordinates that the user entered 
		int checkForYes();			// checks if the user answeres yes 
		
		// Checking for visual markers
		char checkForRobots(int x, int y);		// checks to see if there 
												// any robots in a given position
		char checkForRobotsMarker(int x, int y);// checks for robots in this area 
												// and returns a marker	
		char max(float p, float g, float i);	// returns p, g, i depending on which element 
												// is more frequent in this area 
		float maxV(float p, float g, float i);	// return the value of the most frequent element
												// int this area
		void refresh(char mode);	// refreshing the map
	
		// simulation status flags
		bool failed;
		bool succeded;

		bool exitLoop;				// if true then we end the program
		unsigned int width;			// map's width
		unsigned int height;		// map's height
		Robot_management r;			// instance for the class Robot_management
		Map map_manip;				// a copy of the initial Map instance
};

#endif
