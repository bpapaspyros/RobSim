#ifndef RESCUEROBOT_H
#define RESCUEROBOT_H

#include "Vehicle.h"
#include "Map.h"

class RescueRobot : public Vehicle {
	public:
		RescueRobot(int id, const int width, const int height);

		virtual void operate(Map& m);
		int get_repaired();				// returns the times this robot repaired another robot
		bool get_repairedThisRound();	// checks if this robot made a repair this round
	
		virtual void export_info(bool& successfulOperation);		// true if it repaired this round
        virtual void export_total_info(int& total_successfullOps);	// returns number of repairs
	private:
		void repair(int num_id);	// repairs a robot given its id and type
		int repaired;				// times this robot has repaired another robot
		
		// map size
		int _width;
		int _height;

		// flag that shows whether a robot was repaired in this round
		static bool repairedThisRound;
};

#endif
