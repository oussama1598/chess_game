#include "launcher.h"

//int main(int argc, char **argv) {
//    Launcher app(argc, argv);
//
//    return app.exec();
//}


int main() {
    Game game;
    Renderer_3D renderer{&game};

    while (renderer.is_running()) {
        renderer.render();
    }
}

