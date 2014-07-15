#ifndef GUI_SIM_H
#define GUI_SIM_H

#include "backend/Robot_management.h"
#include "backend/Map.h"
#include "frontend/Gui/Gui_textures.h"
#include "backend/Probability.h"
#include "frontend/Gui/Gui_vector3f.h"
#include "frontend/Gui/Gui_shapes.h"
#include "frontend/Gui/Gui_menus.h"

#include <GL/freeglut.h>

typedef Gui_vector3f vec3f;

class Gui_sim{
    public:
        Gui_sim();
        void init(Map &m);
        Map map_manip;
        Robot_management r;
        Gui_menus menu;

        // simulation runtime
        void start_new_sim();
        void runSim(bool paused, int &vel, uint &iter);
        void end_sim();

        // drawing methods
        void draw_map();
        void draw_map_dangers(float x, float y);
        void draw_custom_square(float side, float z, float hoff, float voff, vec3f colour, GLuint tex);

        // setters & getters
        void set_textures(GLuint tex[]);
        void set_alpha(float a);
        void set_tile_pressed(float world_x, float world_y, float world_z);

        float get_alpha();
        void get_selected_coord(int &sx, int &sy);

        // editing methods
        void addRobotPos(char type, int x, int y);
        void removeRobotPos(int x, int y);
        void moveRobot(int curX, int curY, int targetX, int targetY);

        // tile options
        bool selected;

        // simulation status
        bool succeded;
        bool failed;
    private:
        void loadTexturesSequence();

        // simulation variables
        Probability pr;

        int cur_robot;

        // mouse variables
        float mouseX;
        float mouseY;

        int selectedX;
        int selectedY;

        // colouring varibles
        float m_alpha;

        // shapes
        Gui_shapes shape;

        // textures
        Gui_textures t;

        GLuint tex_research;
        GLuint tex_rescue;
        GLuint tex_analysis;
        GLuint tex_flag;
};



#endif
