#include <SOIL/SOIL.h>

#include "frontend/Gui/Gui.h"
#include "frontend/Gui/Gui_textures.h"

#include <iostream>
#include <fstream>

#include <ctime>
#include <unistd.h>

#include <cmath>

/********************************/
    // Project initial values
    #define TITLE "RobSim ~ bp"
    const int WINDOW_WIDTH = 1280;
    const int WINDOW_HEIGHT = 720;
    const int WINDOW_X = 300;
    const int WINDOW_Y = 150;

    int Gui::mainWindow;

    // Simulation
    int SIM_SPEED = 200;

    // Visual
    Gui_textures Gui::t;
    GLuint Gui::robot_tex[4];
    GLuint Gui::tex_terrain;
/********************************/

// sim variables
Gui_sim Gui::sim;
bool Gui::paused;
int unsigned Gui::iter;
int Gui::vel;

// Gui classes
Gui_shapes Gui::shape;
Gui_menus Gui::menu;
Gui_print Gui::guiPrint;

// camera
float Gui::angle;
vec3f Gui::CAMERA_POSITION;
vec3f Gui::CAMERA_ROTATION;

// Key actions
bool Gui::fullscreen = false;
bool Gui::keys[256];

// interaction
bool Gui::showMenus;
int Gui::clickX;
int Gui::clickY;
int Gui::selectedX;
int Gui::selectedY;
int Gui::dragX;
int Gui::dragY;

// bonus functionality
bool Gui::unbound;

//-------------------

void Gui::init(int argc, char** &argv, Map m) {
    // initializing the simulations manager
    sim.init(m);
    sim.start_new_sim();

    // ++++++++++++++++++++++++++++++++++++++++++

    // Setting up the camera
    angle = 45;

    CAMERA_POSITION.x = 0;
    CAMERA_POSITION.y = 0;
    CAMERA_POSITION.z = 25;

    CAMERA_ROTATION.x = 0;
    CAMERA_ROTATION.y = 0;
    CAMERA_ROTATION.z = 0;


    for(int i=0; i<256; i++) {
        keys[i] = false;
    }

    // initial ui state
    showMenus = false;
    paused = true;
    unbound = false;
    iter = 0;
    vel = 0;

    // Initializing glut and opengl
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

    // Window specifications
    glutInitWindowPosition(WINDOW_X, WINDOW_Y);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mainWindow = glutCreateWindow(TITLE);

    initPopMenu(); // popup menus

    std::cout << "Creating window at resolution: " << WINDOW_WIDTH << "x" << WINDOW_HEIGHT << std::endl;

    // Enable features
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    loadBackground();

    // setting up glut
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // blend settings
    glutDisplayFunc(render);                            // renderer
    glutReshapeFunc(reshape);                           // handling window resizing

    glutKeyboardFunc(keyboardDown);                     // keyboard down func
    glutKeyboardUpFunc(keyboardUp);                     // keyboars up func
    glutSpecialFunc(specialIn);                         // special keyboard buttons handler

    glutMouseFunc(mouse);                               // mouse func
    glutMotionFunc(mouseActs);                          // active mouse function
    glutPassiveMotionFunc(mouseActs);                   // passive mouse function


    /* setting a timed function so that the execution of             *
     * the simulation is pretty much independent from the frame rate */
    glutTimerFunc(SIM_SPEED, &timedSim, 0);

    // Main loop for our window
    glutMainLoop();
}

/*- -------------------------------------------------------------- -*/
void Gui::render() {  
    // handling input actions
    inputActions();

    // clearing the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background colour (clearing colour)
    glLoadIdentity();       // loading identity

    // printing usefull messages
    glEnable(GL_RGB);
    // setting camera position
    glTranslatef(-CAMERA_POSITION.x, -CAMERA_POSITION.y, -CAMERA_POSITION.z);
    glRotatef(angle, CAMERA_ROTATION.x, CAMERA_ROTATION.y, CAMERA_ROTATION.z);

    // getting the simulation going visually
        // drawing background
    glBindTexture(GL_TEXTURE_2D, tex_terrain);
    t.drawEpicBackground();
    glDisable(GL_TEXTURE_2D);

    guiPrint.bmpString(vec3f(-10, 5.2, -14), "Press 'H' for help", GLUT_BITMAP_HELVETICA_18, vec3f(0.5, 0, 0));

    // check if simulation is running
    if ( !Robot_management::roblist.empty()){
        sim.draw_map(); // drawing the map in the window

        // if the simulations has either succeded or failed we terminate it
        if (sim.failed || sim.succeded){
            sim.end_sim();
        }

    }else {
        // printing messages on the screen to justify why the simulation has ended

        if (sim.failed){
            guiPrint.bmpString(vec3f(1.5, 0, -14), "Simulation failed :(", GLUT_BITMAP_TIMES_ROMAN_24, vec3f(1, 0, 0));
        }else if (sim.succeded){
            guiPrint.bmpString(vec3f(1.5, 0, -14), "Simulation succeded !!!", GLUT_BITMAP_TIMES_ROMAN_24, vec3f(0, 1, 0));
        }

        guiPrint.bmpString(vec3f(0, -1, -14), "No active simulation. Press 'n' to start one !", GLUT_BITMAP_TIMES_ROMAN_24, vec3f(0.5, 0, 0));

    }

    // toggle the help menu on/off
    if (showMenus){
        menu.controls(CAMERA_POSITION);
    }

    // telling the user which image stands for which robot
    texInfo();

    // swapping the buffers
    glutSwapBuffers();
}
/*- -------------------------------------------------------------- -*/
void Gui::timedSim(int value) {
    (void) value;

    // running the sim in a timed way
    sim.runSim(paused, vel, iter);

    // getting the selected mouse coordinates on the map
    sim.get_selected_coord(selectedX, selectedY);

    glutTimerFunc(SIM_SPEED, &timedSim, 0);
    glutPostRedisplay();    // force render
}

/*- -------------------------------------------------------------- -*/
void Gui::reshape(int w, int h) {
    /* reshape method used when the user resizes the window so that * 
     * the shapes in it keep their aspect ratio                     */

    if(h==0)
        h=1;

    // calculating the ratio
    float ratio = (float)w/h;

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 0.01, 1000);

    glMatrixMode(GL_MODELVIEW);

}

/*- -------------------------------------------------------------- -*/
void Gui::mouse(int button, int state, int x, int y){
    (void) x;
    (void) y;


    // mouse actions when in free flight mode
    if (unbound && button == 3){
        CAMERA_POSITION.z -= 0.5;
    }else if (unbound && button == 4){
        CAMERA_POSITION.z += 0.5;
    }

    // getting drag location
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        dragX = selectedX;
        dragY = selectedY;
    }

    // getting drop location
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP){
        sim.moveRobot(dragX, dragY, selectedX, selectedY);  // move robot to drop location

        dragX = -1;
        dragY = -1;

        glutPostRedisplay(); // force render
    }
}

/*- -------------------------------------------------------------- -*/
void Gui::mouseActs(int x, int y){
    // getting the current mouse coordinates
    clickX = x;
    clickY = y;

    // translating these coordinates to array indexes
    mouseToMap();

    glutPostRedisplay(); // force render
}

/*- -------------------------------------------------------------- -*/
void Gui::keyboardDown(unsigned char key, int x, int y){
    key = tolower(key);

    keys[key] = true;

    // simulation specific controls
    switch(key){
        case 'n':
            sim.start_new_sim();
            break;
        case 'p':
            paused = !paused;
            break;
        case 'h':
            showMenus = !showMenus;
            break;
    }

    // toggle free flight mode
    if (keys['u'] && keys['=']){
        unbound = !unbound;
    }

    // suppress warnings
    (void) x;
    (void) y;
}

/*- -------------------------------------------------------------- -*/
void Gui::keyboardUp(unsigned char key, int x, int y){
    keys[key] = false;

    // suppress warnings
    (void) x;
    (void) y;
}

/*- -------------------------------------------------------------- -*/
void Gui::inputActions(){
    // exit program button
    if (keys[27]){
        exit(0);
    }

    // simulation speed controls
    if (keys['>']){
        if (SIM_SPEED >100){
            SIM_SPEED -= 100;
        }
    }

    if (keys['<']){
        if (SIM_SPEED < 2000){
            SIM_SPEED += 100;
        }
    }

    // free flight mode controls
    if (unbound){
        if (keys['w']) {
            CAMERA_POSITION.y += 0.5;
        }

        if (keys['s']) {
            CAMERA_POSITION.y -= 0.5;
        }


        if (keys['d']) {
            CAMERA_POSITION.x += 0.5;
        }

        if (keys['a']) {
            CAMERA_POSITION.x -= 0.5;
        }


        if (keys['q']) {
            CAMERA_POSITION.z -= 0.5;
        }

        if (keys['e']) {
            CAMERA_POSITION.z += 0.5;
        }

        // lock and reset camera
        if (keys[32]){
            CAMERA_POSITION.x = 0;
            CAMERA_POSITION.y = 0;
            CAMERA_POSITION.z = 25;

            unbound = false;
        }

        glutPostRedisplay(); // force render
    }
}

/*- -------------------------------------------------------------- -*/
// managing special keyboard keys
void Gui::specialIn(int key, int x, int y) {

    // fullscreen mode toggle
    if (key == GLUT_KEY_F11){
        if (!fullscreen){
            fullscreen = true;
            glutFullScreen();
        }else {
            glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
            fullscreen = false;
        }
    }else if (key == GLUT_KEY_END) {
        sim.end_sim();  // ending simulation
    
    // tile opacity setting
    }else if (key == GLUT_KEY_PAGE_UP){
        float a = sim.get_alpha();

        if (a < 1){
            sim.set_alpha(a+=0.1);
        }

    }else if (key == GLUT_KEY_PAGE_DOWN){
        float a = sim.get_alpha();

        if (a > 0.0f){
            sim.set_alpha(a-=0.1);
        }
    }

    // suppress warnings
    (void) x;
    (void) y;
}

/*- -------------------------------------------------------------- -*/
void Gui::texInfo(){
    // drawing the helpfull images in the bottom left of the screen at each round

    vec3f pos(CAMERA_POSITION.x-14, CAMERA_POSITION.y-5, CAMERA_POSITION.z-20);

    sim.draw_custom_square(0.4, pos.z, pos.x, pos.y, vec3f(1, 1, 1), robot_tex[0]);
    guiPrint.bmpString(vec3f(-9.2, -3.5, -14), std::string("Research Robot"), GLUT_BITMAP_9_BY_15, vec3f(1, 1, 1));
    pos.y -=1;

    sim.draw_custom_square(0.4, pos.z, pos.x, pos.y, vec3f(1, 1, 1), robot_tex[2]);
    guiPrint.bmpString(vec3f(-9.2, -4.2, -14), std::string("Analysis Robot"), GLUT_BITMAP_9_BY_15, vec3f(1, 1, 1));
    pos.y -=1;

    sim.draw_custom_square(0.4, pos.z, pos.x, pos.y, vec3f(1, 1, 1), robot_tex[1]);
    guiPrint.bmpString(vec3f(-9.2, -4.9, -14), std::string("Rescue Robot"), GLUT_BITMAP_9_BY_15, vec3f(1, 1, 1));
}

/*- -------------------------------------------------------------- -*/
void Gui::loadBackground() {
    // loading the textures that we are going to use

    tex_terrain = t.loadEpicTex("../res/world/snow.jpg");

    robot_tex[0] = t.loadEpicTex("../res/robots/research.png");
    robot_tex[1] = t.loadEpicTex("../res/robots/polymechanon.png");
    robot_tex[2] = t.loadEpicTex("../res/robots/analysis.png");
    robot_tex[3] = t.loadEpicTex("../res/objects/flag3.png");

    // sending the robot textures to the simulation class were they will be drawn
    sim.set_textures(robot_tex);
}

/*- -------------------------------------------------------------- -*/
void Gui::mouseToMap(){
    // matrices
    GLdouble model_view[16];
    GLdouble projection[16];
    GLint viewport[4];
    GLfloat depth[2];

    // getting the current world state matrices
    glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // targeted pixel coordinates (not normalised)
    double target_x;
    double target_y;
    double target_z;

    // getting the coordinates of the pixel clicked and unprojecting them
    glReadPixels (clickX, clickY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, depth);
    gluUnProject(clickX, clickY, depth[0], model_view, projection, viewport, &target_x, &target_y, &target_z);

    // normalising variables
    float world_x = float(target_x);
    float world_y = float(target_y);
    float world_z = float(target_z);

    // marking the tiled pressed
    sim.set_tile_pressed(world_x, -world_y, world_z);
}

/*- -------------------------------------------------------------- -*/
void Gui::initPopMenu(){
    // Dangers sub menu
    int dangerM = glutCreateMenu(dangerMenu);
    glutAddMenuEntry(" Increase danger by 0.1 ", 1);
    glutAddMenuEntry(" Decrease danger by 0.1 ", 2);

    // Flags sub menu
    int flagM = glutCreateMenu(flagMenu);
    glutAddMenuEntry(" Add/Remove Flag ", 1);

    // Robot Variables
        // robot velocity
    int robotVelM = glutCreateMenu(robotVelMenu);
    glutAddMenuEntry(" Increase velocity by 1 ", 1);
    glutAddMenuEntry(" Decrease velocity by 1 ", 2);

        // robot status
    int robotStatusM = glutCreateMenu(robotStatusMenu);
    glutAddMenuEntry(" Increase status by 10 ", 1);
    glutAddMenuEntry(" Decrease status by 10 ", 2);

    int robotM = glutCreateMenu(robotMenu);
    glutAddSubMenu(" Robot Velocity ", robotVelM);
    glutAddSubMenu(" Robot Status ", robotStatusM);

    // main pop up menu
    glutCreateMenu(popMenu);

    glutAddSubMenu(" Dangers ", dangerM);
    glutAddSubMenu(" Flags ", flagM);
    glutAddMenuEntry(" --------------- ", -1);

    glutAddSubMenu(" Robot Variables ", robotM);
    glutAddMenuEntry(" --------------- ", -1);

    glutAddMenuEntry(" Add Rescue Robot ", 1);
    glutAddMenuEntry(" Add Analysis Robot ", 2);
    glutAddMenuEntry(" Add Research Robot ", 3);
    glutAddMenuEntry(" --------------- ", -1);

    glutAddMenuEntry(" Remove robot ", 4);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

/*- -------------------------------------------------------------- -*/
/* Handling the various pop up menus we created so that the user *
 * can add elements to the simulation                            */


void Gui::popMenu(int item){

    switch (item) {
        case 1:
            sim.addRobotPos('R', selectedX, selectedY);
            break;
        case 2:
            sim.addRobotPos('A', selectedX, selectedY);
            break;
        case 3:
            sim.addRobotPos('S', selectedX, selectedY);
            break;
        case 4:
            sim.removeRobotPos(selectedX, selectedY);
            break;
    }
}


void Gui::dangerMenu(int item){
    switch(item){
        case 1:
            if (selectedX!=-1 && sim.map_manip.dangers[selectedX][selectedY] < 0.9){
                sim.map_manip.dangers[selectedX][selectedY] += 0.1;
            }
            break;
        case 2:
            if (selectedX!=-1 && sim.map_manip.dangers[selectedX][selectedY] > 0.0){
                sim.map_manip.dangers[selectedX][selectedY] -= 0.1;
            }
            break;
    }
}

void Gui::flagMenu(int item){
    switch(item){
        case 1:
            if (selectedX !=-1)
                sim.map_manip.flags[selectedX][selectedY] = !sim.map_manip.flags[selectedX][selectedY];
            break;
    }

}

void Gui::robotMenu(int item){ (void) item; }

void Gui::robotStatusMenu(int item){
    char type;
    int id;

    switch(item){
        case 1:
            if (selectedX!=-1 && sim.r.checkForRobots(selectedX, selectedY, id, type)){
                sim.r.roblist[id]->set_status(sim.r.roblist[id]->get_status() + 10);
            }
            break;
        case 2:
            if (selectedX!=-1 && sim.r.checkForRobots(selectedX, selectedY, id, type)){
                sim.r.roblist[id]->set_status(sim.r.roblist[id]->get_status() - 10);
            }
            break;
    }
}

void Gui::robotVelMenu(int item){
    char type;
    int id;

    switch(item){
        case 1:
            if (selectedX!=-1 && sim.r.checkForRobots(selectedX, selectedY, id, type)){
                int v = sim.r.roblist[id]->get_velocity();
                sim.r.roblist[id]->set_velocity(v + 1);
            }
            break;
        case 2:
            if (selectedX!=-1 && sim.r.checkForRobots(selectedX, selectedY, id, type)){
                sim.r.roblist[id]->set_velocity(sim.r.roblist[id]->get_velocity() - 1);
            }
            break;
    }

}
/*- -------------------------------------------------------------- -*/


