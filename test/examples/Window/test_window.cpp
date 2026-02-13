#include "MyWindow.h"

int main() {
    Engine engine;
    engine.setFPS(30);
    auto window = new MyWindow(&engine, "Test window");
    window->renderer()->setVSyncMode(Renderer::Adaptive);
    window->setResizable(true);
    window->setDragDropEnabled(true);
    window->show();
    return engine.exec();
}
