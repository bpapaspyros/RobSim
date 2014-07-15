#include "frontend/Gui/Gui_sim.h"

#include <iostream>
#include <cmath>

#include "frontend/Gui/Gui_print.h"
#include "backend/SimulationStatus.h"

Gui_sim::Gui_sim() : map_manip(0, 0), r(1, 1){}

/*- -------------------------------------------------------------- -*/
void Gui_sim::init(Map &m){
    map_manip = m;
    r = Robot_management(map_manip.get_width(), map_manip.get_height());

    m_alpha = 0.3;

    selected = false;
}

/*- -------------------------------------------------------------- -*/
void Gui_sim::start_new_sim(){
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
}

/*- -------------------------------------------------------------- -*/
void Gui_sim::runSim(bool paused, int &vel, uint &iter) {
    if (!paused && !failed && !succeded) {    // check if there is a simulation running

        if (vel < r.roblist[iter]->get_velocity()) {    // check if this robot has to move multiple times
            cur_robot = iter;

            r.roblist[iter]->move(map_manip);              // invoke move method

            // Handling damage
            int cur_x = r.roblist[iter]->get_pos_x();	// getting current x position
            int cur_y = r.roblist[iter]->get_pos_y();	// getting current y position

                // take damage
            int damage = pr.takeDamage(r.roblist[iter], map_manip.get_danger(cur_x, cur_y));

            (void) damage;

            // Handling robot operation
            r.roblist[iter]->operate(map_manip);

            vel++;
        }else {
            vel = 0;
            iter++;

            if (iter == r.roblist.size()){
                iter = 0;
            }
        }// velocity if

        SimulationStatus stat;
        failed = stat.fail();
        succeded = stat.success(map_manip);
    }// active sim if
}

/*- -------------------------------------------------------------- -*/
void Gui_sim::end_sim(){
    r.emptyList(); // emptying list

    map_manip.reset();  // resetting the map
}

/*- -------------------------------------------------------------- -*/
// draw a square customized so that its bind to our texture
void Gui_sim::draw_custom_square(float side, float z, float hoff, float voff, vec3f colour, GLuint tex){
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex);
        glColor3f(colour.x, colour.y, colour.z);
        shape.square(side, z, hoff, voff);
    glDisable(GL_TEXTURE_2D);
}

/*- -------------------------------------------------------------- -*/
void Gui_sim::draw_map() {
    float side = 0.4;
    float zfar = 6;
    float initX = -2.5;
    float initY = -6.5;

    vec3f baseC(1, 1, 1);
    vec3f backC(0, 0, 0);

    // robot characteristics
    int num;
    char type;

    selectedX = -1;
    selectedY = -1;

    // drawing the map square by square and draqing the robots/flags in the appropriate tile
    for (int i=0; i<map_manip.get_height(); i++) {
        for (int j=0; j<map_manip.get_width(); j++) {
            if (r.checkForRobots(j, i, num, type) ){
                switch (type) {
                    case 'R':
                        draw_custom_square(side, zfar, initX, initY, baseC, tex_rescue);
                        break;
                    case 'A':
                        draw_custom_square(side, zfar, initX, initY, baseC, tex_analysis);
                        break;
                    case 'S':
                        draw_custom_square(side, zfar, initX, initY, baseC, tex_research);
                        break;
                }
            }else {
                if (map_manip.flags[j][i]){

                    draw_custom_square(side, zfar-0.005, initX, initY, baseC, tex_flag);

                    glColor4f(0, 0, 0, m_alpha);
                    shape.square(side, zfar, initX, initY);
                }
            } // checkForRobots if-else

            if (selected && shape.chechInsideSq(side, initX, initY, mouseX, mouseY)){
                glColor4f(1, 1, 1, m_alpha);
                shape.square(side, zfar-0.002, initX, initY);

                selectedX = j;
                selectedY = i;
            }else{
                glColor4f(0, 0, 0, m_alpha);
                shape.square(side, zfar-0.002, initX, initY);
            }// selected tile if-else

            initX += 0.9;
        }// j loop

        initX = -2.5;
        initY += 0.9;
    }// i loop

    // printing info on the screen
    menu.round_info(cur_robot, map_manip.get_width(), map_manip.get_height());
    menu.base_info(map_manip);
}

/*- -------------------------------------------------------------- -*/
// checks if a tile is selected
void Gui_sim::set_tile_pressed(float world_x, float world_y, float world_z){
    selected = true;

    if(world_z < 3|| world_z >7){
        selected = false;
    }

    mouseX = world_x;
    mouseY = world_y;
}

/*- -------------------------------------------------------------- -*/
// setting the textures that we till use for the robots
void Gui_sim::set_textures(GLuint tex[]){
    tex_research = tex[0];
    tex_rescue   = tex[1];
    tex_analysis = tex[2];
    tex_flag     = tex[3];
}

/*- -------------------------------------------------------------- -*/
// tile alpha value
void Gui_sim::set_alpha(float a){
    m_alpha = a;
}

/*- -------------------------------------------------------------- -*/
float Gui_sim::get_alpha(){
    return m_alpha; // returning alpha value
}

/*- -------------------------------------------------------------- -*/
// add robots
void Gui_sim::addRobotPos(char type, int x, int y){
    type = tolower(type);

    r.addRobot(type, x, y);
}

// remove robots
void Gui_sim::removeRobotPos(int x, int y){
    char type;
    int id;

    if (r.checkForRobots(x, y, id, type)){
        r.delete_robot(id);
    }
}

// moving robots
void Gui_sim::moveRobot(int curX, int curY, int targetX, int targetY){
    char type;
    int id;

    if(targetX !=-1 && r.checkForRobots(curX, curY, id, type)){
        r.roblist[id]->set_pos_x(targetX);
        r.roblist[id]->set_pos_y(targetY);
    }
}

/*- -------------------------------------------------------------- -*/
// returning the selected tile's coordinates
void Gui_sim::get_selected_coord(int &sx, int &sy) {
    sx = selectedX;
    sy = selectedY;
}
