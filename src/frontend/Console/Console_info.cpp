#include "frontend/Console/Console_info.h"

#include "backend/Robot_management.h"
#include "backend/Abbreviations.h"
#include <iostream>

#include <ctime>
#include <unistd.h>
#include <cstdlib>

using std::cout;
using std::endl;
using std::string;

Console_info::Console_info(Map m) : _m(m), _width(_m.get_width()), _height(_m.get_height()) {
	
}

/*- -------------------------------------------------------------- -*/
void Console_info::robotMovedTo(char type, int id, int x, int y){
	if(x>=0 && y>=0)
		cout << "\t\t* The robot " << type << id << " moved to position " << x << ", " << y << endl;
	else
		cout << "\t\t* The robot returned to the base " << endl;
}

/*- -------------------------------------------------------------- -*/
void Console_info::map_tile_info(int x, int y) {
	Abbreviations a;
    Robot_management r = Robot_management(_width, _height);

	// printing danger and resource information about this tile
	cout << "Info for the tile in position (" << x << ", " << y << "):" << endl;
    cout << "\t Danger Meter: " << _m.dangers[x][y] << endl;
    cout << "\t Palladium: " << _m.resources_p[x][y] << endl;
    cout << "\t Platinum: " << _m.resources_g[x][y] << endl;
    cout << "\t Iridium: " << _m.resources_i[x][y] << endl;

	a.newLine(1);

	// telling the user whether there is a robot in the given position
	cout << "Robots in current position: ";
	if (r.checkForRobots(x, y))
		cout << "Yes" << endl;
	else
		cout << "No" << endl;

	// telling the user if this position is marked with a flag or not
	cout << "Flag: ";
    if (_m.flags[x][y])
		cout << "Yes" << endl;
	else
		cout << "No" << endl;		
}

/*- -------------------------------------------------------------- -*/
void Console_info::robot_info(int id) {
    Robot_management r(_width, _height);

    char type = r.roblist[id]->get_type();
	type = tolower(type);
	
	// general robot information
	cout << "Robot velocity: " << Robot_management::roblist[id]->get_velocity() << endl;
	cout << "Robot status: " << Robot_management::roblist[id]->get_status() << endl;
	cout << "Distance travelled: " << Robot_management::roblist[id]->get_travelled() << endl;
	cout << "Damages taken: " << Robot_management::roblist[id]->total_damages << endl;


	switch(type){
		case 'a':
			// specific information about the analysis robots
			cout << "This robot has picked up: " << endl;


            float tp, tg, ti, tres;

            r.roblist[id]->export_total_info(tp, tg, ti, tres);

             cout << "\t Palladium: " << tp << endl;
             cout << "\t Platinum: " << tg << endl;
             cout << "\t Iridium: " << ti << endl;
             cout << "\t Total: " << tres << endl;
			break;

		case 'r':
			// specific information about the rescue robot
            int treps;

            r.roblist[id]-> export_total_info(treps);
            cout << "Repairs: " << treps << endl;
			break;
		case 's':
			// specific information about the research robot    
            int tflags;

            r.roblist[id]-> export_total_info(tflags);
             cout << "Flags Deployed: " << tflags << endl;
			break;
        default:
            cout << "No robot was found with this id" << endl;
            break;

	}

}

/*- -------------------------------------------------------------- -*/
// printing info about a specific robot
void Console_info::robot_info(uint x, uint y) {
	Robot_management r = Robot_management(_width, _height); 

	char type;
	int num;

	if (r.checkForRobots(x, y, num, type)) { 
		robot_info(type, num);
	}else
		cout << "No robot was found in the given position" << endl;
}

/*- -------------------------------------------------------------- -*/
//printing info for the analysis robot given its id
void Console_info::analysis_info(int id){
	Robot_management r = Robot_management(_width, _height);

	float p, g, i, remload;
	Robot_management::roblist[id]->export_info(p, g, i, remload);

	cout << "\t\t* Robot status: " << r.roblist[id]->get_status() << endl;
	cout << "\t\t* The robot A" << id << " is currently carrying ->"; 
	cout << " p: " << p;
	cout << " g: " << g;
	cout << " i: " << i << endl; 
	cout << "\t\t* Remaining load: " << remload << endl;
}

// /*- -------------------------------------------------------------- -*/
//printing info for the rescue robot given its id
void Console_info::rescue_info(int id){
	Robot_management r = Robot_management(_width, _height);

	bool repaired;
	r.roblist[id]->export_info(repaired);

	cout << "\t\t* Robot status: " << r.roblist[id]->get_status() << endl;
	if (repaired){
		cout << "\t\t* R" << id << " repaired a robot !" << endl;
	}else {
		cout << "\t\t* No robots repaired in this round" << endl;
	}

}

/*- -------------------------------------------------------------- -*/
//printing info for the research robot given its id
void Console_info::research_info(int id){
	Robot_management r = Robot_management(_width, _height);

	bool depFlag;
	r.roblist[id]->export_info(depFlag);

	cout << "\t\t* Robot status: " << r.roblist[id]->get_status() << endl;
	if (depFlag){
		cout << "\t\t* S" << id << " deployed a flag !" << endl;
	}else {
		cout << "\t\t* No flags deployed in this round" << endl;
	}

}

/*- -------------------------------------------------------------- -*/
// checking if robot took damages this round and printing the appropiate info
void Console_info::took_damage(int damage){
	if(damage>0) {
		cout << "\t\t* Robot took " << damage << " damage" << endl;
	}else{
		cout << "\t\t* Robot survived this round without a scratch !!!" << endl;
	}
}

/*- -------------------------------------------------------------- -*/
// printing messages for success status
void Console_info::success(){
	Abbreviations a;
	a.skip_page();

	cout << "\t\t\tThe simulation was successful and all targets were met" << endl;

    general_info();

    sleep(30);
}

/*- -------------------------------------------------------------- -*/
// printing messages for fail status
void Console_info::fail(){
	Abbreviations a;
	a.skip_page();
	cout << "\t\t\tAll robots are down and the simulation can no longer continue " << endl;

    general_info();

    sleep(30);
}

/*- -------------------------------------------------------------- -*/
// priting general info concerning the map and the robots
void Console_info::general_info(){
    Robot_management r = Robot_management(_width, _height);
    Abbreviations a;

    cout << "\t Map average danger: " << _m.get_average_danger() << endl;
    cout << "\t\t Totals: " << endl;

    for(uint i=0; i<r.roblist.size(); i++){
        cout << "Robot " << r.roblist[i]->get_type() << r.roblist[i]->get_num_id() << " :" << endl;
        robot_info(r.roblist[i]->get_num_id());
        a.newLine(2);
    }

}
/*- -------------------------------------------------------------- -*/
