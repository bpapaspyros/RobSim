#include "frontend/Console/Console_graphics.h"
#include "frontend/Console/Console_info.h"
#include "backend/Abbreviations.h"
#include "backend/Robot_management.h"
#include "backend/Map.h"
#include "backend/Looper.h"
#include "backend/Probability.h"
#include "backend/SimulationStatus.h"

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <cstdlib>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;


// simulation speed
int REFRESH = 200000;

Console_graphics::Console_graphics(Map& m) : r(m.get_size_x(), m.get_size_y()), map_manip(m) {
    width = map_manip.get_size_x();
    height = map_manip.get_size_y();

    // initialize our bool variable
    exitLoop = false;

    // printing welcome message
    Abbreviations a;

    // clearing the screen
    a.skip_page();
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::start_new_sim(){
    // if the list has objects from a previous sim we delete them
    if ( !r.listEmpty() ) {
        r.emptyList();
    }

    map_manip.reset();

    // flags showing that the simulation ended
    failed = false;
    succeded = false;

    // we spawn 3 robots, one of each kind to begin with
    r.addRobot('A');
    r.addRobot('R');
    r.addRobot('S');

    // we also start the view switcher toolbar
    print_view_switcher('p');
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::run_sim(char mode){
    Abbreviations a;	// making an instance of the class Abbreviations
    Looper l;			// making an instance of the class Looper
    Probability pr;			// making an instance of the probability class
    SimulationStatus sim;	// making an instance of the SimulationStatus class
    Console_info c = Console_info(map_manip);	// making an instance of the class Console_info

    refresh(mode);

    while(!l.kbhit() && !failed && !succeded){	// while the user hasn't pressed a button keep looping
        // clearing the screen
        a.skip_page();


        for (uint i=0; i<r.roblist.size(); i++){
            int vel = 0;

            if (r.roblist[i]->get_velocity() == 0){
                // robot is broken and is waiting in the same position until it is repaired
                r.roblist[i]->move(map_manip);
            }

            while(vel++ < r.roblist[i]->get_velocity()){
                // getting the current robot's type
                char r_type = r.roblist[i]->get_type();
                r_type = tolower(r_type);

                // moving
                r.roblist[i]->move(map_manip);	// invoke move method

                // Handling damage
                int cur_x = r.roblist[i]->get_pos_x();	// getting current x position
                int cur_y = r.roblist[i]->get_pos_y();	// getting current y position

                    // take damage
                int damage = pr.takeDamage(r.roblist[i], map_manip.get_danger(cur_x, cur_y));

                // Handling robot operation
                r.roblist[i]->operate(map_manip);

                // refreshing the map
                refresh(mode);

                // printing action info
                c.robotMovedTo(toupper(r_type), r.roblist[i]->get_num_id(), r.roblist[i]->get_pos_x(), r.roblist[i]->get_pos_y());
                if (r_type == 'a'){
                    c.analysis_info(i);
                }else if (r_type == 'r'){
                    c.rescue_info(i);
                }else if (r_type == 's'){
                    c.research_info(i);
                }
                c.took_damage(damage);

                // refresh time for the simulation
                usleep(REFRESH);
            }

            // checking for simulations success or fail conditions
            failed = sim.fail();	// checking if all robots are damaged
            succeded = sim.success(map_manip);	// checking if all resources of the map are picked up
        }
    }

    if (failed){
        c.fail();
        r.emptyList();
    }else if (succeded){
        c.success();
        r.emptyList();
    }else
        cout << "\t > The simulation was paused by the user" << endl;
}
/*- -------------------------------------------------------------- -*/
void Console_graphics::refresh(char mode){
    Abbreviations a; // making an instance for the Abbreviations

    a.skip_page(); 	 // clearing the screen

    // refreshing map in each round
    if (mode == 'p')
        draw_plex_map();		// drawing the plex map
    else if (mode == 'r')
        draw_resources_map();	// drawing resources map
    else if (mode == 'd')
        draw_danger_map();		// drawing dangers map

    draw_base();		// draw the base as well

    // tell the user how to pause the loop
    a.newLine(1);
    cout << "To pause the simulation and print the menu press any button" << endl;
    cout << "* The simulation will pause when the robots have finished moving for this round" << endl;
}

/*- -------------------------------------------------------------- -*/



    // ******************************************************************** //
    // *--------------------- General Visual Methods ---------------------* //
    // ******************************************************************** //

char Console_graphics::max(float p, float g, float i){
    if (p>g && p>i)
        return 'p';
    else if (g>p && g>i)
        return 'g';
    else if (i>g && i>p)
        return 'i';
    else if (p==0 && g==0 && i==0)
        return '-';
    else
        return '$';
}

/*- -------------------------------------------------------------- -*/
float Console_graphics::maxV(float p, float g, float i){
    float max_value = (p>g) ? p:g ;
    return (max_value>i)? max_value:i ;
}

/*- -------------------------------------------------------------- -*/
char Console_graphics::checkForRobots(int x, int y){
    /* checking the vector containing the active robots *
     * to see whether there is a robot in the area with *
     * x, y coordinates and returning 1 if one is found */

    int pos_x = 0;
    int pos_y = 0;

    for (uint i=0; i<r.roblist.size(); i++){
        pos_x = r.roblist[i]->get_pos_x();
        pos_y = r.roblist[i]->get_pos_y();

        if (pos_x==x && pos_y==y)
            return r.roblist[i]->get_type();
    }

    if (map_manip.flags[x][y])
        return '#';

    return ' ';
}

/*- -------------------------------------------------------------- -*/
char Console_graphics::checkForRobotsMarker(int x, int y){
    char ch = checkForRobots(x, y);

    if (ch == '#'){
        return '*';
    }else if (ch != ' ')
        return '>';
    else
        return ' ';
}

/*- -------------------------------------------------------------- -*/
bool Console_graphics::choice(){ return exitLoop; }	// returns the user choice
/*- -------------------------------------------------------------- -*/





    // ********************************************************************* //
    // *------------------------- Print Methods -------------------------* //
    // ********************************************************************* //

void Console_graphics::print_toolbar(){
    Abbreviations a;

    // clearing the console
    a.skip_page();
    using std::cin;

    // welcoming the user
    cout << "~ Welcome to RoboSim";
    a.newLine(2);

    // asks user for input and act accordingly
    get_choice();
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::get_choice(){
    Abbreviations a;
    char ch;	// stores the user's choice

    do{
        // printing the toolbar with options about the runtime and the map
        cout << "\t| Start new Simulation  (n) | " << endl;
        cout << "\t| View Switcher         (v) | " << endl;
        cout << "\t| Edit Map              (m) | " << endl;
        cout << "\t| Exit                  (e) | " << endl;

        a.newLine(2);

        cout << "Choice: ";		// notify the user that we are waiting for input
        cin >> ch;				// read input

        ch = tolower(ch);		// convert to lower (case insensitive)

        switch (ch){
            case 'e':
                r.emptyList();		// deleting left overs from the memory to avoid memory leak
                exitLoop = true;					// user chose to exit
                cout << "Goodbye !" << endl;		// goodbye :(
                break;
            case 'm':
                if (r.listEmpty())
                    cout << "No active simulation. Press 'n' to start a new simulation"  << std::endl;
                else
                    print_edit_menu();				// printing the edit menu
                break;
            case 'c':
                if (r.listEmpty())
                    cout << "No active simulation. Press 'n' to start a new simulation"  << std::endl;
                else
                    cout << "c" << endl;			// loading sim control
                break;
            case 'n':
                start_new_sim();
                break;
            case 'v':	// user chose to see the view switcher
                if (r.listEmpty())
                    cout << "No active simulation. Start one by pressing (n)" << endl;
                else
                    print_view_switcher('p');

                break;
            default:
                cout << "Invalid input. Try again ..." << endl;

        }

    }while(exitLoop == false);
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::print_view_switcher(char view){
    Abbreviations a;	// make an instance of the class Abbreviations
    a.newLine(1);

    do{
        switch (view){
            case 'p':				// user chose plex map
                a.skip_page(); 		// clearing the screen
                run_sim('p');		// running the simulation
                a.newLine(1);
                view = 'b';			// when we return from the run sim we exit the menu
                cout << " -> Plex View         (p) " << endl;
                cout << "    Danger View       (d) " << endl;
                cout << "    Resources View    (r) " << endl;
                cout << "Simoulation settings  (s) " << endl;
                cout << "General information   (g) " << endl;
                cout << "Go back               (b) " << endl;
                cout << "To resume the simulation just select a view" << endl;
                break;
            case 'd':				// user chose danger map
                a.skip_page(); 		// clearing the screen
                run_sim('d');		// running the simulation
                a.newLine(1);
                view = 'b';			// when we return from the run sim we exit the menu
                cout << "    Plex View         (p) " << endl;
                cout << " -> Danger View       (d) " << endl;
                cout << "    Resources View    (r) " << endl;
                cout << "Simoulation settings  (s) " << endl;
                cout << "General information   (g) " << endl;
                cout << "Go back               (b) " << endl;
                cout << "To resume the simulation just select a view" << endl;
                break;
            case 'r':				// user chose resources map
                a.skip_page();		// clearing the screen
                run_sim('r');		// running the simulation
                a.newLine(1);
                view = 'b';			// when we return from the run sim we exit the menu
                cout << "    Plex View         (p) " << endl;
                cout << "    Danger View       (d) " << endl;
                cout << " -> Resources View    (r) " << endl;
                cout << "Simoulation settings  (s) " << endl;
                cout << "General information   (g) " << endl;
                cout << "Go back               (b) " << endl;
                cout << "To resume the simulation just select a view" << endl;
                break;
            case 's':
                sim_settings();
                run_sim('p');		// running the simulation
                view = 'b';			// when we return from the run sim we exit the menu
                cout << " -> Plex View         (p) " << endl;
                cout << "    Danger View       (d) " << endl;
                cout << "    Resources View    (r) " << endl;
                cout << "Simoulation settings  (s) " << endl;
                cout << "General information   (g) " << endl;
                cout << "Go back               (b) " << endl;
                cout << "To resume the simulation just select a view" << endl;
                break;
            case 'g':
                {
                    Abbreviations a;
                    Console_info c(map_manip);

                    a.skip_page();
                    c.general_info();
                    sleep(30);
                }
                break;
            case 'b':
                a.skip_page();		// skip a console page
                // print_toolbar();
                break;
            default:	// wrong input
                cout << "Invalid input. Try again .." << endl;
        }

        a.newLine(1);
        if (!failed && !succeded) {
            cout << "Choice: ";	// inform user we are waiting for his input
            cin >> view;		// read input
            view = tolower(view); // case insensitive
        }
    }while(view != 'b');

    a.skip_page();
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::print_edit_menu(){
    // making an instance of the class Anbreviations
    Abbreviations a;

    // printing the new menu
    cout << "~ Map & Robot Editor" << endl;
    a.newLine(1);

    cout << "\t 1 - Edit Resources    	 								 " << endl;
    cout << "\t 2 - Edit Danger Areas 	 								 " << endl;
    cout << "\t 3 - Edit Robots  	  	 								 " << endl;
    cout << "\t 4 - Edit Robot Variables 								 " << endl;
    cout << "\t Go back  (b) 		  	 								 " << endl;

    a.newLine(1);

    char ch;

    cout << "Choice: ";
    cin >> ch;

    while(ch != '1' && ch != '2' && ch != '3' && ch != '4' && ch != 'b'){
        cout << "Invalid input. Try again ..." << endl;
        cout << "Choice: ";
        cin >> ch;
    }

    if (ch == '1'){
        edit_resources();
    }
    else if (ch == '2'){
        edit_dangers();
    }
    else if (ch == '3'){
        edit_robots();
    }
    else if (ch == '4'){
        edit_robot_variables();
    }
    else if (ch == 'b')
        print_toolbar();	// going back
    else
        cout << "Invalid input. Try again ..." << endl;
}





    // ********************************************************************* //
    // *------------------------- Drawing Methods -------------------------* //
    // ********************************************************************* //

void Console_graphics::draw_plex_map(){
    // make an instance for s Abbreviations class
    Abbreviations a;

    // Current map
    std::cout << "Current Map: Plex";
    a.newLine(1);

    // printing a plex, a map without any info about the resources or
    // the dangers, just the robot positions
    for (uint i=0; i<height; i++) {
        a.tabLeft(2);
        std::cout << " |";
        for (uint j=0; j<width; j++) {
            std::cout << checkForRobots(j, i) <<"|";
        }

        a.newLine(1);
    }// i
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::draw_danger_map(){
    // make an instance for the Abbreviations class
    Abbreviations a;

    // Current map
    std::cout << "Current Map: Dangerous Areas";
    a.newLine(2);

    // setting precision of the floats we are printing to 1 decimal point
    a.set_precision(1);

    // printing the dangers map and the robots' position on it
    for (uint i=0; i<height; i++){
        a.tabLeft(1);
        for (uint j=0; j<width; j++){
            char robotFound = checkForRobotsMarker(j, i);
            if (robotFound != ' ')
                std::cout << " " <<robotFound << map_manip.dangers[j][i];
            else
                std::cout << "  " << map_manip.dangers[j][i];
        }

        a.newLine(1);
    }// i
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::draw_resources_map(){
    // make an instance for the Abbreviations class
    Abbreviations a;

    // Current map
    std::cout << "Current Map: Resources";
    a.newLine(2);

    // taking some distance from the edge of the console
    a.tabLeft(2);
    std::cout << "Elements";
    a.tabLeft(5);
    std::cout << "Quantity" << std::endl;

    // printing 2 maps side by side, one showing the element
    // and the other the quantity of this element in a specific area
    for (uint i=0; i<height; i++){
        a.tabLeft(1);

        for (uint j=0; j<width; j++){

            char robotFound = checkForRobotsMarker(j, i);

            if (robotFound != ' ')
                std::cout << robotFound << max(map_manip.resources_p[j][i], map_manip.resources_g[j][i], map_manip.resources_i[j][i]) << "  ";
            else
                std::cout << " " << max(map_manip.resources_p[j][i], map_manip.resources_g[j][i], map_manip.resources_i[j][i]) << "  ";
        }

        //setting precision of the floats we are printing to 1 decimal point
        a.set_precision(1);
        a.tabLeft(1);

        for (uint j=0; j<width; j++){
                        char robotFound = checkForRobotsMarker(j, i);

            if (robotFound != ' ')
                std::cout << robotFound << maxV(map_manip.resources_p[j][i], map_manip.resources_g[j][i], map_manip.resources_i[j][i]) << "  ";
            else
                std::cout << " " << maxV(map_manip.resources_p[j][i], map_manip.resources_g[j][i], map_manip.resources_i[j][i]) << "  ";
        }

        a.newLine(1); // new line

    }// i
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::draw_base(){
    Abbreviations a;	// instance of the class Abbreviations
    a.newLine(1);

    bool none = true;	// checks if there is no robot at all in the bas
    a.newLine(1);

    cout << "In base:";	// informing the user which robots are in base

    for (uint i=0; i<r.roblist.size(); i++){
        if (r.roblist[i]->gotoBase){
            // at least one robot is in base
            none = false;

            // printing the robots that are in base and their id
            cout << " " << r.roblist[i]->get_type() << r.roblist[i]->get_num_id();

            if (r.roblist[i]->get_brokeQ()){
                cout << "(broken)";
            }
        }
    }

    if (none == true){
        cout << " All robots are on the field !";	// no robot in the base
    }

    cout << "\n * Base resources: p[" << map_manip.base_res[0] << "] g[" << map_manip.base_res[1] << "] i[" << map_manip.base_res[2]
         << "] total[" << map_manip.base_res[0]+map_manip.base_res[1]+map_manip.base_res[2] << "]";

    a.newLine(2);
}

/*- -------------------------------------------------------------- -*/






    // ******************************************************************** //
    // *-------------------------- Edit Methods --------------------------* //
    // ******************************************************************** //

void Console_graphics::edit_dangers(){
    int x, y;
    float danger;

    // getting coordinates from the user
    get_robot_coordinates(x, y);

    // printing the current value in this position
    cout << "Current value for the position (" << x << ", " << y << ") -> ";
    cout << "Danger level: " << map_manip.dangers[x][y] << endl;
    // getting a value for the new danger level
    cout << "Enter a new value for the danger level: ";
    danger = truncf(get_input(0, 0.9)*10.0f)/10.0f;

    // registering the changes
    map_manip.dangers[x][y] = danger;

    // printing the map
    print_view_switcher('d');
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::edit_resources(){
    Abbreviations a; 	// making an instance of the class abbreviations

    // variables that will store the coordinates
    int x, y;
    float p, g, i;

    cout << "**All values will be truncated to 1 decimal point" << endl;

    // getting the coordinates of the area tha we will edit
    get_robot_coordinates(x, y);

    a.newLine(1);
    // printing the current values so that the user doesn't have to search for them
    cout << "Current values for the position (" << x << ", " << y << ") -> ";
    cout << "p:" << map_manip.resources_p[x][y] << " ";
    cout << "g:" << map_manip.resources_g[x][y] << " ";
    cout << "i:" << map_manip.resources_i[x][y] << " " << endl;

    // getting the new values for the resources of the specified area
    cout << "Enter new values for the palladium: ";		// palladium
    p = trunc(get_input(0, 1)*10.0f)/10.0f;

    cout << "Enter new values for the platinum: ";		// platinum
    g = trunc(get_input(0, 1)*10.0f)/10.0f;

    cout << "Enter new values for the iridium: ";		// iridium
    i = trunc(get_input(0, 1)*10.0f)/10.0f;

    // register the changes to the map
    map_manip.edit_resources_xy(x, y, p, g, i);

    // reload the maps
    print_view_switcher('r');
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::edit_robots(){
    // coordinates on the map
    int x, y;
    int num_id;	// robot's num id
    char type;	// robot's type

    // getting the coordinates of the area tha we will edit
    get_robot_coordinates(x, y);


    if (r.checkForRobots(x, y, num_id, type)){

        // inform the user that a robot was found and tell him its type
        cout << "A robot of type " << type;
        cout << " with id " << num_id << " was found in this position" << endl;

        // Ask him if he wants to delete it
        cout << "Do you want to delete it ? (y/n) ";

        if (checkForYes()){	// user chose to delete the robot
            r.delete_robot(num_id);
            print_view_switcher('p');
        }else // user chose not to delete the robot
                print_view_switcher('p');
    }else{	// no robot was found so we ask the user if he wants to add one
        cout << "Do you want to add a robot in the position ";
        cout << "(" << x << ", " << y << ") ? (y/n) ";

        if (checkForYes()){
            // Asking the user what kind of robot they want to add
            cout << "What type of robot do you want to add ? (A, S, R) " << endl;

            // getting the user's answer
            char answer;
            cout << "Type: ";
            cin >> answer;
            answer = tolower(answer);	// case insensitive

            switch (answer){
                case 'a':	// user chose to add an analysis robot
                    r.addRobot('A', x, y);
                    break;
                case 'r':	// user chose to add a rescue robot
                    r.addRobot('R', x, y);
                    break;
                case 's':	// user chose to add a research robot
                    r.addRobot('S', x, y);
                    break;
                default:	// user gave invalid input
                    cout << "Unknown type, no robot added !" << endl;
                    print_view_switcher('p');
            }// switch
        }// if

        print_view_switcher('p');

    }// else


}

/*- -------------------------------------------------------------- -*/
void Console_graphics::edit_robot_variables(){
    int x, y;	// robot coordinates
    int id;		// robot's id
    char type; 	// robot's type

    cout << "1 - Change robot velocity " << endl;
    cout << "2 - Change robot position " << endl;
    cout << "3 - Change robot ability  " << endl;
    cout << "4 - Change robot status   " << endl;

    char ch;
    cout << "Choice: ";
    cin >> ch;

    switch (ch){
        case '1':	// user chose to change the velocity
            int vel;	// variable for the new velocity value
            get_robot_coordinates(x, y);	// asking the user for coordinates
            if ( r.checkForRobots(x, y, id, type) ){
                do{	// ask the user for his input in a specific range

                    cout << "Current velocity: " << r.roblist[id]->get_velocity() << endl;

                    cout << "Give a value in the range [0, 4] for the new robot velocity: " << endl;
                    cin >> vel;
                }while( vel<0 || vel>4 );

                r.roblist[id]->set_velocity(vel);	// change the robot's velocity

                print_view_switcher('p');	// print the plex map
            }else{
                cout << "Invalid input. Going back ..." << endl;
                print_edit_menu();
            }


            break;
        case '2':	// user chose to change the position
            cout << "Give coordinates for the robot you want to move" << endl;
            get_robot_coordinates(x, y);

            if (r.checkForRobots(x, y, id, type) ){
                int new_x, new_y;	// variables for the new robot coordinates
                cout << "Give new coordinates for the robot" << endl;
                get_robot_coordinates(new_x, new_y);

                r.roblist[id]->set_pos_x(new_x);
                r.roblist[id]->set_pos_y(new_y);

                print_view_switcher('p');
            }else{
                cout << "Invalid input. Going back ..." << endl;
                print_edit_menu();
            }
            break;
        case '3':	// user chose to change the ability
            cout << "Give coordinates for the robot you want to edit" << endl;
            get_robot_coordinates(x, y);

            if ( r.checkForRobots(x, y, id, type) ){
                float ability;

                do{	// get a valid number for the robot's ability

                    cout << "Current ability: " << r.roblist[id]->get_ability() << endl;

                    cout << "Give a number for the robot's ability to overcome obstacles";
                    cout << " in the range [0.1, 1.0]" << endl;
                    cin >> ability;
                    ability = truncf(ability*10.0f/10.0f);
                }while(ability<0.1 || ability>1.0);

                r.roblist[id]->set_ability(ability);		// set the robot's ability

                print_view_switcher('p');
            }else{
                cout << "Invalid input. Going back ..." << endl;
                print_edit_menu();
            }

            break;
        case '4':	// user chose to change the status
            cout << "Give coordinates for the robot you want to edit" << endl;
            get_robot_coordinates(x, y);

            if ( r.checkForRobots(x, y, id, type) ){
                int status;

                do{
                    cout << "Culrrent status: " << r.roblist[id]->get_status() << endl;

                    cout << "Give a value for the new robot's status" << endl;
                    cin >> status;
                }while(status<1);

                r.roblist[id]->set_status(status);

                print_view_switcher('p');
            }else{
                cout << "Invalid input. Going back ..." << endl;
                print_edit_menu();
            }


            break;
        default:
            cout << "Invalid input. Going back ..." << endl;
            print_edit_menu();
    }
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::sim_settings() {
    // loop while the user doesn't give a valid value
    bool loop = false;

    do{
        // print the choices to the user
        cout << "\t Current speed: " << REFRESH << endl;
        cout << "\t Choose the desired speed: " << endl;
        cout << "\t\t - 1 (0.2 s)" << endl;
        cout << "\t\t - 2 (0.5 s)" << endl;
        cout << "\t\t - 3 (0.8 s)" << endl;
        cout << "\t\t - 4 (1.0 s)" << endl;
        cout << "\t\t - 5 (2.0 s)" << endl;

        // get the input
        int ch;
        cin >> ch;

        switch (ch) {
            case 1:
                REFRESH = 200000;
                loop = false;
                break;
            case 2:
                REFRESH = 500000;
                loop = false;
                break;
            case 3:
                REFRESH = 800000;
                loop = false;
                break;
            case 4:
                REFRESH = 1000000;
                loop = false;
                break;
            case 5:
                REFRESH = 2000000;
                loop = false;
                break;
            default:
                cout << "Invalid input. Try again ... " << endl;
        }
    }while(loop);
}

/*- -------------------------------------------------------------- -*/



    // ******************************************************************** //
    // *----------------------- User Input Methods -----------------------* //
    // ******************************************************************** //

int Console_graphics::checkForYes(){
    char answer;

    // checking if answer is yes
    do{
        cin >> answer;
        answer = tolower(answer);

        if (answer == 'y')	// if it is we return 1
            return 1;
        else if (answer == 'n')	// else we return 0
            return 0;
        else 	// inform the user that his input is invalid
            cout << "Invalid input. Try again ..." << endl;
    }while(1);
}

/*- -------------------------------------------------------------- -*/
float Console_graphics::get_input(float lower_lim, float upper_lim){
    float x;

    cin >> x;

    // checking if the input is in the given range
    while(x>upper_lim || x<lower_lim){	// if not we ask for input again
        cout << "Invalid input. Try again" << endl;
        cout << "Value [" << lower_lim << ", " << upper_lim << "]" << endl;
        cin >> x;
    }

    return x;
}

/*- -------------------------------------------------------------- -*/
void Console_graphics::get_robot_coordinates(int &x, int &y){
    // getting the coordinates of the area tha we will edit
    cout << "Enter the coordinate x: ";
    x = (int)get_input(0, width-1);
    cout << "Enter the coordinate y: ";
    y = (int)get_input(0, height-1);
}

/*- -------------------------------------------------------------- -*/
