#include <iostream>

#include <SOIL/SOIL.h>

#include "frontend/Gui/Gui_textures.h"


Gui_textures::Gui_textures(){
    background = "../res/world/moon.jpg";
}

/*- -------------------------------------------------------------- -*/
GLuint Gui_textures::loadEpicTex(std::string filename){

    GLuint tex;




    tex = SOIL_load_OGL_texture(
                filename.c_str(),
                SOIL_LOAD_AUTO,
                SOIL_CREATE_NEW_ID,
                SOIL_FLAG_POWER_OF_TWO
                | SOIL_FLAG_MIPMAPS
                | SOIL_FLAG_MULTIPLY_ALPHA
                | SOIL_FLAG_COMPRESS_TO_DXT
                | SOIL_FLAG_DDS_LOAD_DIRECT
                | SOIL_FLAG_INVERT_Y
                );

        if( !tex )
        {
            std::cerr << "Could not load the texture from file: " << filename << std::endl;
        }

        return tex;
}

/*- -------------------------------------------------------------- -*/
void Gui_textures::drawEpicBackground() {
    glEnable(GL_TEXTURE_2D);

    glBegin(GL_QUADS);
        glColor3f(1, 1, 1);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-30, -30,  0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f( 30, -30,  0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f( 30,  30,  0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-30,  30,  0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

/*- -------------------------------------------------------------- -*/

