#ifndef GUI_VECTOR3F_H
#define GUI_VECTOR3F_H


class Gui_vector3f{
    public:
        Gui_vector3f();
        Gui_vector3f(float nx, float ny, float nz);
        Gui_vector3f(float nx, float ny, float nz, float nalpha);

        float x;
        float y;
        float z;
        float alpha;

};

#endif
