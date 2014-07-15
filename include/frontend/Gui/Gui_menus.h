#ifndef GUI_MENUS_H
#define GUI_MENUS_H

#include "frontend/Gui/Gui_vector3f.h"
#include "backend/Map.h"

typedef Gui_vector3f vec3f;

class Gui_menus{
    public:
        void controls(vec3f CAMERA_POSITION);		// help menu
        void round_info(int id, int w, int h);		// info for this round depending on the current robot's type
        void base_info(Map m);						// base info including the resources on base
    private:
    	// robot info printing methods
        void analysis_info(int id);
        void rescue_info(int id);
        void research_info(int id);

        // convering numbers to single characters
        void convert(int &dec, int &sin, int &flo, int target);
};

#endif
