#ifndef GUI_SHAPES_H
#define GUI_SHAPES_H

#include <frontend/Gui/Gui_vector3f.h>
#include <GL/freeglut.h>

typedef Gui_vector3f vec3f;

class Gui_shapes{
    public:
        void rect(vec3f pos, float hoffset, float voffset);											// rectancgle
        void square(float side, float z, float hoffset, float voffset);								// square

        void sphere(float radius, int slices, int stacks, float hoffset, float voffset, float z);	// sphere
        void draw_circle(vec3f pos, float r, int num_segments);										// circle

        bool chechInsideSq(float side, float x, float y, float pX, float pY); // checking if the given coordinates are located inside a square
};

#endif
