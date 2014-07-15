#include "backend/Map.h"
#include "backend/Random.h"
#include "backend/Abbreviations.h"
#include <iostream>
#include <cmath>

/*- -------------------------------------------------------------- -*/
Map::Map(int x, int y) : width(x), height(y) {
	setup();	// allocating memory for our arrays
	init();		// initializing the above arrays with values
}

/*- -------------------------------------------------------------- -*/
// copy constructor
Map::Map(const Map &m) : width(m.width), height(m.height), av_danger(m.av_danger) {
	setup();

	// copying the base array
	for (int i=0; i<earth_el; i++)
		base_res[i] = m.base_res[i];

	// copying the 2d arrays to the copy 2d arrays
	for (int i=0; i<width; i++)
		for (int j=0; j<height; j++){
			flags[i][j] = m.flags[i][j];
			dangers[i][j] = m.dangers[i][j];			
			resources_p[i][j] = m.resources_p[i][j];
			resources_g[i][j] = m.resources_g[i][j];
			resources_i[i][j] = m.resources_i[i][j];
		}
}

/*- -------------------------------------------------------------- -*/
Map& Map::operator= (const Map& m)
{
    if (this == &m)
        return *this;

    // deleting the arryays previously created to avoid memory leak
    for (int i = 0; i <width; ++i){
        delete [] flags[i];
        delete [] dangers[i];
        delete [] resources_p[i];
        delete [] resources_g[i];
        delete [] resources_i[i];
    }

    // completely deleting the arrays
    delete [] base_res;
    delete [] flags;
    delete [] dangers;
    delete [] resources_p;
    delete [] resources_g;
    delete [] resources_i;

    // copying the map's dimension to the copy variables
    width = m.width;
    height = m.height;
        // as well as the map average danger variable
    av_danger = m.av_danger;

    setup();

  	// copying the 2d arrays to the copy 2d arrays
    for (int i=0; i<width; i++){
        for (int j=0; j<height; j++){
            dangers[i][j] = m.dangers[i][j];
            flags[i][j] = m.flags[i][j];
            resources_p[i][j] = m.resources_p[i][j];
            resources_g[i][j] = m.resources_p[i][j];
            resources_i[i][j] = m.resources_p[i][j];
        }
    }

    // copying the base_res array
    for (int i=0; i<earth_el; i++) {
        base_res[i] = m.base_res[i];
    }

    return *this;
}

/*- -------------------------------------------------------------- -*/
Map::~Map(){
	// deleting the arryays previously created to avoid memory leak 
	for (int i = 0; i<width; ++i){
		delete [] flags[i];
     	delete [] dangers[i];
     	delete [] resources_p[i];
		delete [] resources_g[i]; 
		delete [] resources_i[i];
	}

	// completely deleting the arrays
	delete [] base_res;
	delete [] flags;
	delete [] dangers;
	delete [] resources_p;
	delete [] resources_g; 
	delete [] resources_i;
}

/*- -------------------------------------------------------------- -*/
void Map::setup(){
	// creating the base array
	base_res = new float[earth_el]; 
	
	// we are creating the rows of our arrays
	flags = new bool*[width];			// we create an appropriate sized array for the flags that research robots put down
	dangers = new float*[width];		// we create an approtiate sized array for the map's dangers
	resources_p = new float*[width];	// we create an approtiate sized array for the available palladium
	resources_g = new float*[width];	// we create an approtiate sized array for the available platinum
	resources_i = new float*[width];	// we create an approtiate sized array for the available iridium

	// we are crating the columns of our arrays
	for(int i = 0; i<width; ++i){
		flags[i] = new bool[height];		// we create an appropriate sized array for the flags that research robots put down
		dangers[i] = new float[height];		// we create an approtiate sized array for the map's dangers
		resources_p[i] = new float[height];	// we create an approtiate sized array for the available palladium
		resources_g[i] = new float[height];	// we create an approtiate sized array for the available platinum
		resources_i[i] = new float[height];	// we create an approtiate sized array for the available iridium
	}
}

/*- -------------------------------------------------------------- -*/
void Map::init(){
	Random r = Random();	// making an instance of the class Random

	// initializing the base_res array with zeroes (the base doesn't have any resources yet)
	for (int i=0; i<earth_el; i++) {
		base_res[i] = 0;
	}

	// initializing the flags array with false values (no flags)
	for (int i=0; i<width; i++){
		for (int j=0; j<height; j++){
			flags[i][j] = false;
		}
	}

	// getting random values for the earth elements
    av_danger = 0; // calculating average

	for (int i=0; i<width; i++){
		for (int j=0; j<height; j++){
			// assigning random values to each cell of the dangers array
			// each value represents the how dangerous an area is
			float dangers_ = std::abs( r.random_0_to_1(-0.1) ); 
			dangers[i][j] = truncf(dangers_* 10.0f) / 10.0f; 

            av_danger += dangers_;
				
			// assigning random values to the resources array
			// generating earth elements for the map
			resources_p[i][j] = truncf( r.random_0_to_1()*10.0f ) /10.0f;
			resources_g[i][j] = truncf( r.random_0_to_1()*10.0f ) /10.0f;
			resources_i[i][j] = truncf( r.random_0_to_1()*10.0f ) /10.0f;
		}// j
	}// i

	// calculating the average danger value for this generated map
    int div = (width*height);
    if (div!=0){
        av_danger = av_danger/div;
        av_danger = truncf(av_danger*10.0f)/10.0f; // truncating the result no need to be extremely accurate
    }
}

/*- -------------------------------------------------------------- -*/
// method used to alter the values of the resources array
void Map::edit_resources_xy(int x, int y, float p, float g, float i){
	resources_p[x][y] = p;	// assigning a new palladium value in this position
	resources_g[x][y] = g;	// assigning a new platinum value in this position
	resources_i[x][y] = i;	// assigning a new iridium value in this position
}

/*- -------------------------------------------------------------- -*/
// method used to alter the values of the dangers array
void Map::edit_dangers_xy(int x, int y, float danger_){
	dangers[x][y] = danger_;	// assigning a new danger value in this position
}

/*- -------------------------------------------------------------- -*/
int Map::get_size_x() { return width;  }	// return the map's width
int Map::get_size_y() { return height; }	// return the map's height

float Map::get_danger(int x, int y) { 
	if (x>=0 && y>=0)	
		return dangers[x][y]; // returns the danger in the x, y position
	else // the robot is in the base there is no danger in the base
		return 0;
}	

/*- -------------------------------------------------------------- -*/
int Map::get_width()  {  return width;  }	// returns the map's width
int Map::get_height() {  return height; }	// returns the map's height

/*- -------------------------------------------------------------- -*/
void Map::reset(){ init(); }				// resets the map's values to new ones

/*- -------------------------------------------------------------- -*/
float Map::get_average_danger(){
    return av_danger;						// returns the average danger of the map that we calculated
}

/*- -------------------------------------------------------------- -*/
