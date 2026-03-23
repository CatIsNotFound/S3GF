#include "../../../include/MyEngine/MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Test Point");
    window->show();
    Graphics::Point point(window->geometry().width / 2.f, window->geometry().height / 2.f, 20, StdColor::Black, 32);
    window->installPaintEvent([&point] (Renderer* r) {
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_EQUALS)) {
            point.resize(std::clamp(point.size() + 1, 1, 255));
        } else if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_MINUS)) {
            point.resize(std::clamp(point.size() - 1, 1, 255));
        }
        r->drawDebugText(FMT::format("Point: pos = {}, size = {}, indices = {}",
            point.position(), point.size(), point.indicesCount()), {20, 20});
        r->drawPoint(&point);
    });

    return engine.exec();
}