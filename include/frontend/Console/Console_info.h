#ifndef CONSOLE_INFO_H
#define CONSOLE_INFO_H

#include "backend/Map.h"

typedef unsigned uint;

class Console_info{
	public:
        Console_info(Map m);

		// prints where the current robot moved
		void robotMovedTo(char type, int id, int x, int y);	

		// info about a tile of the map
        void map_tile_info(int x, int y);

		// robot info
		void robot_info(int id); 			// by type & id
		void robot_info(uint x, uint y);	// by coordinates

		// specific info about the robots
		void analysis_info(int id);
		void rescue_info(int id);
		void research_info(int id);
		void took_damage(int damage);
		void success();
		void fail();
        void general_info();
	private:
        Map _m;

		int _width;
		int _height;
};

#endif
