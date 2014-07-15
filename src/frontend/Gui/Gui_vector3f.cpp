#include "frontend/Gui/Gui_vector3f.h"

Gui_vector3f::Gui_vector3f(){}

Gui_vector3f::Gui_vector3f(float nx, float ny, float nz){
    x = nx;
    y = ny;
    z = nz;

    alpha = 1.0;
}

Gui_vector3f::Gui_vector3f(float nx, float ny, float nz, float nalpha){
    x = nx;
    y = ny;
    z = nz;

    alpha = nalpha;
}
