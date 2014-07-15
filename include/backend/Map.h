#ifndef MAP_H
#define MAP_H

#define earth_el 3

class Map{
	public:
		Map(int x, int y);			// initializes basic values to generate map
		Map(const Map &v);			// copy constructor 
		~Map();						// deconstructor
		Map& operator= (const Map &m);	// assign operator overload
        int get_size_x();               // return the map's width
        int get_size_y();               // return the map's height
		float get_danger(int x, int y);	// returns the danger in the x, y position
        float get_average_danger();     // returns the average danger for this generated map
		int get_width();	// returns the map's width
		int get_height();	// returns the map's height

		// editing the resources of a specific map area
		void edit_resources_xy(int x, int y, float p, float g, float i); 
		// editing the danger level of a specific area on the map	
		void edit_dangers_xy(int x, int y, float danger_);	 
			
		void reset();			// resets map and reinitializes it with new values

		bool **flags;			// array containing the flags that research robot put down
		float **dangers;		// array containing the dangers
		float **resources_p;	// array containing the quantity of palladium		
		float **resources_g;	// array containing the quantity of gold
		float **resources_i;	// array containing the quantity of iridium
		float *base_res;		// array containing the resources stored in the base
	private:
		void setup();			// setting up basic variables
		void init();			// initializing the variables
		int width;				// width
		int height;				// height
        float av_danger;        // average map danger
};

#endif
