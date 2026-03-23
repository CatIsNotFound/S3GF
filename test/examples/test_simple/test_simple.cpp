#include "../../../include/MyEngine/MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(30);
    auto window = new Window(&engine, Engine::applicationName().data());
    window->setResizable(true);
    window->installPaintEvent([](Renderer* r) {
        r->fillBackground(StdColor::DarkBlue);
        r->drawDebugFPS({20, 20}, StdColor::White);
        r->drawDebugText("Hello MyEngine!", {20, 34}, StdColor::White);
    });
    window->show();
    

    return engine.exec();
}
