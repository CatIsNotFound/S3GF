#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    auto win = new Window(&engine, "Hello world!");
    win->installPaintEvent([](Renderer* r) {
        r->fillBackground(StdColor::DarkBlue);
        r->drawPixelText("Hello world!", {20, 20});
    });
    return engine.exec();
}