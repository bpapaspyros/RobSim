#ifndef RESEARCHROBOT_H
#define RESEARCHROBOT_H

#include "Vehicle.h"
#include "Map.h"

class ResearchRobot : public Vehicle {
	public:
		// initializing variables
		ResearchRobot(int id, const int width, const int height);

		virtual void operate(Map& m);	// basic operation fucntion
		int get_flagsDeployed();		// returns the number of flags deployed
		bool get_flagDeployedThisRound(); // true if it deployed a flag this round
	
		virtual void export_info(bool& succesfulOperation);			// true if it deployed a flag this round
        virtual void export_total_info(int& total_successfullOps);	// times it deployed a flag
	private:
		int flagsDeployed;	// total flags deployed by this robot

		// flag showing whether a flag was deployed in this round
		static bool flagDeployedThisRound;
};

#endif
