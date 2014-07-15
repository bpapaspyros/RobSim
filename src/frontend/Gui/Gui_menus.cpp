#include "frontend/Gui/Gui_menus.h"

#include "frontend/Gui/Gui_print.h"
#include "frontend/Gui/Gui_shapes.h"

#include "backend/Robot_management.h"

/*- -------------------------------------------------------------- -*/
// printing the help menu
void Gui_menus::controls(vec3f CAMERA_POSITION) {
    Gui_shapes shape;
    Gui_print out;
    vec3f pos(-2, -1, -9);
    vec3f colour(1, 1, 0, 0.8);

    float line_space = 0.3;

    // draw surface to print controls
    glColor4f(0, 0, 0, 0.8);
    shape.square(8, CAMERA_POSITION.z-10, CAMERA_POSITION.x, CAMERA_POSITION.y);

    // printing info
    out.bmpString(vec3f(-2.5, pos.y-1.5, pos.z), "                q, e       (zoom in/out)", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(vec3f(-2.5, pos.y-1.5, pos.z), "* Free flight : w, a, s, d (up/right/down/left)", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(vec3f(pos.x, pos.y-0.75, pos.z), "- Edit Map & Robots    : Right click on a map tile", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(vec3f(pos.x, pos.y-0.75, pos.z), "- Move Robots around   : Drag and Drop a selected robot", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(pos, "- Slower simulation    : '<' (shift + ',')", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(pos, "- Faster simulation    : '>' (shift + '.')", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(pos, "- Reset Camera & lock  : space", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(pos, "- Free flight Mode     : 'u' + '='", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(pos, "- End Simulation       : End", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(pos, "- Pause simulation     : P", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    out.bmpString(pos, "- New simulation       : N", GLUT_BITMAP_9_BY_15, colour);
    pos.y += line_space;

    pos.y += 1.5*line_space;
    out.bmpString(pos, "* Hide this menu by pressing H", GLUT_BITMAP_9_BY_15, colour);

}

/*- -------------------------------------------------------------- -*/
// handling the printing of the base
void Gui_menus::base_info(Map m){
    Gui_print out;
    vec3f pos(-7.0, 0, -10);
    vec3f colour(0, 0, 0, 0);

    out.bmpString(pos, "Base: ", GLUT_BITMAP_9_BY_15, colour);

    int bp = m.base_res[0]*100;
    int bg = m.base_res[1]*100;
    int bi = m.base_res[2]*100;

    // palladium
    int ph, pd, pf;
    convert(ph, pd, pf, bp);

    out.bmpString(vec3f(-7, -0.5, -10), "Palladium: ", GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.8, -0.5, -10), '0'+ph, GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.7, -0.5, -10), '0'+pd, GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.6, -0.5, -10), '.', GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.5, -0.5, -10), '0'+pf, GLUT_BITMAP_9_BY_15, colour);

    // platinum
    int gh, gd, gf;
    convert(gh, gd, gf, bg);

    out.bmpString(vec3f(-7, -0.75, -10), "Platinum: ", GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.8, -0.75, -10), '0'+gh, GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.7, -0.75, -10), '0'+gd, GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.6, -0.75, -10), '.', GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.5, -0.75, -10), '0'+gf, GLUT_BITMAP_9_BY_15, colour);

    // iridium
    int ih, id, ifl;
    convert(ih, id, ifl, bi);

    out.bmpString(vec3f(-7, -1, -10), "Iridium: ", GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.8, -1, -10), '0'+ih, GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.7, -1, -10), '0'+id, GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.6, -1, -10), '.', GLUT_BITMAP_9_BY_15, colour);
    out.bmpChar(vec3f(-5.5, -1, -10), '0'+ifl, GLUT_BITMAP_9_BY_15, colour);

}

/*- -------------------------------------------------------------- -*/
// can't print numbers in glut so we are converting the digits to single characters
void Gui_menus::convert(int &dec, int &sing, int &flo, int target){
    dec = target/1000;
    sing = (target%1000)/100;
    flo = target%100/10;
}

/*- -------------------------------------------------------------- -*/
// printing some general info that all robots have and some specific depending on the type
void Gui_menus::round_info(int id, int w, int h){
    Robot_management r = Robot_management(w, h);
    Gui_print out;
    vec3f pos(-7.0, 3, -10);
    vec3f colour(0, 0, 0, 0);

    // Robot status
    out.bmpString(pos, "Robot: ", GLUT_BITMAP_9_BY_15, colour);
    pos.x += 0.8;
    out.bmpChar(pos, r.roblist[id]->get_type(), GLUT_BITMAP_9_BY_15, colour);
    pos.x += 0.1;

    out.bmpChar(pos, '0'+id, GLUT_BITMAP_9_BY_15, colour);

    pos.y -= 0.5;
    pos.x -= 0.9;

    out.bmpString(pos, "Robot Status: ", GLUT_BITMAP_9_BY_15, colour);

    float off = 1.5;
    for(int i=0; i<r.roblist[id]->get_status(); i++){
        out.bmpChar(vec3f(pos.x+off, pos.y, pos.z), '|', GLUT_BITMAP_9_BY_15, vec3f(0, 1, 0));
        off+=0.01;
    }

    switch(r.roblist[id]->get_type()){
        case 'A':
            analysis_info(id);
            break;
        case 'R':
            rescue_info(id);
            break;
        case 'S':
            research_info(id);
            break;
    }
}

/*- -------------------------------------------------------------- -*/
// research robot info
void Gui_menus::research_info(int id){
    Gui_print out;
    vec3f pos(-6.0, 2, -10);

    bool depFlag;
    Robot_management::roblist[id]->export_info(depFlag);

    if (depFlag){
        out.bmpString(pos, "Deployed a flag ! ", GLUT_BITMAP_9_BY_15, vec3f(0, 1, 0));
    }else {
        out.bmpString(pos, "No flags deployed", GLUT_BITMAP_9_BY_15, vec3f(1, 0, 0));
    }
}

/*- -------------------------------------------------------------- -*/
// rescue robot info
void Gui_menus::rescue_info(int id){
    Gui_print out;
    vec3f pos(-6.0, 2, -10);

    bool repaired;
    Robot_management::roblist[id]->export_info(repaired);

    if (repaired){
        out.bmpString(pos, "Repaired a robot ! ", GLUT_BITMAP_9_BY_15, vec3f(0, 1, 0));
    }else {
        out.bmpString(pos, "No repairs", GLUT_BITMAP_9_BY_15, vec3f(1, 0, 0));
    }
}

/*- -------------------------------------------------------------- -*/
// analysis robot info
void Gui_menus::analysis_info(int id){
    Gui_print out;
    vec3f pos(-7.0, 2, -10);
    vec3f colour(0, 0, 0, 0);

    float p, g, i, remload;
    Robot_management::roblist[id]->export_info(p, g, i, remload);

    int decP = (int)(p*10)%10;
    int decG = (int)(g*10)%10;
    int decI = (int)(i*10)%10;

    out.bmpString(pos, "Carrying: ", GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));

    // palladium
    pos.y-=0.3;
    pos.x += 0.5;

    out.bmpString(pos, "P: ", GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.3, pos.y, pos.z), '0'+p, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.4, pos.y, pos.z), '.', GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.5, pos.y, pos.z), '0'+decP, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));

    // Platinum
    pos.y-=0.3;

    out.bmpString(pos, "G: ", GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.3, pos.y, pos.z), '0'+g, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.4, pos.y, pos.z), '.', GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.5, pos.y, pos.z), '0'+decG, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));

    // Iridium
    pos.y-=0.3;

    out.bmpString(pos, "I: ", GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.3, pos.y, pos.z), '0'+i, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.4, pos.y, pos.z), '.', GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    out.bmpChar(vec3f(pos.x+0.5, pos.y, pos.z), '0'+decI, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));

    // Remaining Load
    pos.y-=0.3;

    out.bmpString(pos, "Remaining Load: ", GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    if (remload!=10){
        int decR = (int)(remload*10)%10;

        out.bmpChar(vec3f(pos.x+1.8, pos.y, pos.z), '0'+remload, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
        out.bmpChar(vec3f(pos.x+1.9, pos.y, pos.z), '.', GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
        out.bmpChar(vec3f(pos.x+2.0, pos.y, pos.z), '0'+decR, GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    }else{
        out.bmpChar(vec3f(pos.x+1.8, pos.y, pos.z), '1', GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
        out.bmpChar(vec3f(pos.x+1.9, pos.y, pos.z), '0', GLUT_BITMAP_9_BY_15, vec3f(0, 0, 0));
    }
}
/*- -------------------------------------------------------------- -*/
