#include <MyEngine>
#include "NewWindow.h"
using namespace MyEngine;

int main(int argc, char* argv[]) {
    Engine engine("your.application.name", "v1.0.0", "your.application.id");
    engine.setFPS(30);
    auto my_window = new NewWindow(&engine, engine.applicationName(), 1024, 800);
    my_window->show();
    return engine.exec();
}

