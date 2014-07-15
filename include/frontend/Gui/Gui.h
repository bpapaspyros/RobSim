#ifndef GUI_H
#define GUI_H

#include <GL/freeglut.h>

#include "frontend/Gui/Gui_menus.h"
#include "frontend/Gui/Gui_shapes.h"
#include "frontend/Gui/Gui_vector3f.h"
#include "frontend/Gui/Gui_textures.h"
#include "frontend/Gui/Gui_print.h"

#include "backend/Map.h"
#include "backend/Probability.h"
#include "frontend/Gui/Gui_sim.h"

typedef Gui_vector3f vec3f;


class Gui{
	public:
        // initializing glut and opengl
        static void init(int argc, char** &argv, Map m); // initializing glut, opengl and the simulation
    private:
        // window management
        static int mainWindow;  // main window id

        // hidden functionality
        static bool unbound;    // flag for unbound movement

        // Simulation
        static Gui_sim sim;     // instance of the simulation management class for gui
        static bool paused;     // flag for pausing the simulation

        static int vel;            // taking into acount the robots velocity
        static unsigned int iter;  // for every robot

        static void timedSim(int value);    // refreshing the simulation with a specific refresh rate

        // graphics handling
        static void render();               // renderer
        static void renderSidebar();        // renderer for sidebar
        static void reshape(int w, int h);  // reshape method to be called when resizing

        static void initPopMenu();            // popup menu initialisation
        static void popMenu(int item);        // popup menu selection actions

            // more menus
        static void dangerMenu(int item);
        static void flagMenu(int item);
        static void robotMenu(int item);
        static void robotVelMenu(int item);
        static void robotStatusMenu(int item);

        // user interaction
        static void mouse(int button, int state, int x, int y);       // mouse click management
        static void mouseActs(int x, int y);                          // active mouse function
        static void mouseToMap();                                     // getting the window equivalent coordinates for the mouse


        // storing mouse coordinates
        static int clickX;
        static int clickY;

        static int dragX;
        static int dragY;

            // Map array counters pointing to the clicked array cell
        static int selectedX;   // selected tile x coordinate
        static int selectedY;   // selected tile y coordinate

        static void specialIn(int key, int x, int y);                  // special key manipulation
        static void keyboardDown(unsigned char key, int x, int y);     // keyboard listener
        static void keyboardUp(unsigned char key, int x, int y);       // keyboard up
        static void inputActions();                                    // defines actions for key presses

        static bool keys[256];  // keyboard buttons down array
        static bool fullscreen; // fullscreen flag

        // handling the map from backend
        static void draw_map();         // drawing the simulation map

        // Instanciating Classes
        static Gui_shapes shape;        // instanciating the shape class
        static Gui_menus menu;          // instanciating the menu class
        static Gui_print guiPrint;      // instanciating the printing class

        // values for camera and menus
        static bool showMenus;

        static float angle;              // camera angle
        static vec3f CAMERA_POSITION;    // camera position
        static vec3f CAMERA_ROTATION;    // camera rotation

        // textures
        static Gui_textures t;
        static GLuint robot_tex[4];
        static GLuint tex_terrain;

        static void texInfo();

        static void loadBackground();     // loads textures that will be used

};


#endif
