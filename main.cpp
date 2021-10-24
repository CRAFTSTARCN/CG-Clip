#include "Window.h"

int main(int, char**) {
    initGLFW(3,3);

    Window win(900,900,"Clip by Liang-Barsky algorithm, press C to show, left mouse to clear");
    win.initWindow();
    win.setUpRendProp();
    win.mainLoop();

    return 0;
}