#include "../../../include/MyEngine/MyEngine"

#ifdef __USED_VLD_LIB__
#include <vld.h>
#endif

using namespace MyEngine;

int main(int argc, char *argv[]) {
    Engine engine;
    engine.setFPS(60);

    auto win = new Window(&engine, "Test Window");
    auto win2 = new Window(&engine, "Test Window2");
    win2->show();
    win->show();
    auto layer_manager = new LayerManager(win);

    return engine.exec();
}

