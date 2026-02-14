#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(30);
    auto window = new Window(&engine, "Circle", 1024, 800);
    Texture tex(Algorithm::drawFilledCircleSurface(80, StdColor::Red),
            window->renderer(), false);
    tex.property()->move(100, 100);
    Texture tex2(Algorithm::drawFilledRoundedRectangleSurface(500, 240, 80, StdColor::Blue),
            window->renderer(), false);
    tex2.property()->move(240, 100);
    window->renderer()->setVSyncMode(Renderer::HalfRate);
    window->installPaintEvent([&tex, &tex2] (Renderer* r) {
        tex.draw();
        tex2.draw();
    });
    window->show();
    return engine.exec();
}