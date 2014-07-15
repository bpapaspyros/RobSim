#include "frontend/Gui/Gui.h"
#include "backend/Map.h"

int main(int argc, char** argv) {
    Map m(0, 0);

    if (argc == 3){
        m = Map(*argv[1], *argv[2]);
    }else {
        m = Map(16, 16);
    }

    Gui::init(argc, argv, m);

	return 0;
}
