#include "frontend/Gui/Gui_print.h"

// printing charecters on the screen
void Gui_print::bmpChar(Gui_vector3f position, char ch, void *font, Gui_vector3f ColourRGB){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glColor3d(ColourRGB.x, ColourRGB.y, ColourRGB.z);
        glRasterPos3f(position.x, position.y, position.z);
        glutBitmapCharacter(font, ch);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// printing strings on the screen
void Gui_print::bmpString(Gui_vector3f position, std::string str, void *font, Gui_vector3f ColourRGB){
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        glLoadIdentity();
        glColor3d(ColourRGB.x, ColourRGB.y, ColourRGB.z);
        glRasterPos3f(position.x, position.y, position.z);
        for (std::string::iterator i = str.begin(); i != str.end(); ++i){
            glutBitmapCharacter(font, *i);
        }
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}
