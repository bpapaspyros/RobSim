#include "frontend/Gui/Gui_shapes.h"

#include <cmath>


#include <SOIL/SOIL.h>

/*- -------------------------------------------------------------- -*/
void Gui_shapes::rect(vec3f pos, float hoffset, float voffset) {
    // creating a rectangle and attaching the texture

    glPushMatrix();
        // translate the objects below according to the vertical and horizontal offsets given
        glTranslatef(hoffset, voffset, 0);

        // drawing the polygon
        glBegin(GL_POLYGON);
            // vertices
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-pos.x,  pos.y, pos.z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-pos.x, -pos.y, pos.z);
            glTexCoord2f(0.0f, 0.0f); glVertex3f( pos.x, -pos.y, pos.z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f( pos.x,  pos.y, pos.z);
        glEnd();
    glPopMatrix();

    glFlush();
}

/*- -------------------------------------------------------------- -*/
// square shape using the rectangle above
void Gui_shapes::square(float side, float z, float hoffset, float voffset) {
    vec3f pos(side, side, z);

    rect( pos, hoffset, voffset);
}

/*- -------------------------------------------------------------- -*/
// sphere shape
void Gui_shapes::sphere(float radius, int slices, int stacks, float hoffset, float voffset, float z) {
    glPushMatrix();
        glTranslatef(hoffset, voffset, z);
        glutSolidSphere(radius, slices, stacks);
    glPopMatrix();
}

/*- -------------------------------------------------------------- -*/
// circle shape
void Gui_shapes::draw_circle(vec3f pos, float r, int num_segments) {
    float theta = 2 * 3.1415926 / float(num_segments);
    float c = cosf(theta);
    float s = sinf(theta);
    float t;

    float x = r;
    float y = 0;

    glBegin(GL_LINE_LOOP);


    for(int ii = 0; ii < num_segments; ii++)
    {
        glVertex3f(x + pos.x, y + pos.y, pos.x);

        //apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
    glFlush();
}
/*- -------------------------------------------------------------- -*/
// method that checks whether the given coordinates are inside a square
bool Gui_shapes::chechInsideSq(float side, float x, float y, float pX, float pY){
    if ( (pX < x+side) && (pX > x-side) && (pY < y+side) && (pY > y-side) ){
        return true;
    }

    return false;
}
