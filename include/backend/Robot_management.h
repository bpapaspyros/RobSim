#ifndef ROBOT_MANAGEMENT_H
#define ROBOT_MANAGEMENT_H

#include "AnalysisRobot.h"
#include "ResearchRobot.h"
#include "RescueRobot.h"
#include "Map.h"
#include <vector>

typedef unsigned uint;

class Robot_management{
	public:
		Robot_management(const int width, const int height);	// initializing basic variables
		bool checkForRobots(unsigned int x, unsigned int y);	// returns a boolean value if robot is found
		bool checkForRobots(unsigned int x, unsigned int y, int &num, char& type);	// returns a boolean value if robot is found and its num_id, type
		void addRobot(char type);					// adds a robot. A for analysis, S for research, R for rescue
		void addRobot(char type, int x, int y);	    // adds a robot in a specific location
        void delete_robot(int id);	// deletes a specific robot by its id
		bool listEmpty();	// if all lists are empty returns true
		void emptyList();	// empties the robot lists
		static std::vector<Vehicle *> roblist;
	private:
		int width;				// map's width		
		int height;				// map's height
};

#endif
