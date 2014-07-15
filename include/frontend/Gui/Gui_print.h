#ifndef GUI_PRINT_H
#define GUI_PRINT_H

#include <iostream>

#include <GL/freeglut.h>
#include "Gui_vector3f.h"

class Gui_print{
    public:
        void bmpString(Gui_vector3f position, std::string str, void *font, Gui_vector3f ColourRGB); // method for string printing on the screen
        void bmpChar(Gui_vector3f position, char ch, void *font, Gui_vector3f ColourRGB);			// method for character printing on the screen
};

#endif
