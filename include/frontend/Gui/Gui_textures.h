#ifndef GUI_TEXTURES_H
#define GUI_TEXTURES_H

#include <iostream>

#include <GL/freeglut.h>

using std::string;

typedef unsigned uint;
typedef unsigned short ushort;

class Gui_textures{
    public:
        Gui_textures(); // setting the defaults

        GLuint loadEpicTex(std::string filename); // textures for the background
        void drawEpicBackground(); // drawing on the screen

    private:
        string background; // filename for the background

};

#endif
