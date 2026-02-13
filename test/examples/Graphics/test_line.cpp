#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Test Line");
    window->show();
    Graphics::Line line(window->geometry().width / 4.f, window->geometry().height / 4.f,
                        window->geometry().width * 0.75f, window->geometry().height * 0.75f,
                        20, StdColor::LightBlue);
    window->installPaintEvent([&line] (Renderer* r) {
        r->drawLine(&line);
        r->drawDebugText(FMT::format("Line: start_pos = {}, end_pos = {}, size = {}, indices = {}",
            line.startPosition(), line.endPosition(), line.size(), line.indicesCount()), {20, 20});
    });

    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&] (SDL_Event e) {
        if (e.type != SDL_EVENT_KEY_DOWN) return;
        if (e.key.key == SDLK_W) {
            line.setStartPosition(line.startPosition().x, line.startPosition().y - 1.f);
        } else if (e.key.key == SDLK_S) {
            line.setStartPosition(line.startPosition().x, line.startPosition().y + 1.f);
        } else if (e.key.key == SDLK_UP) {
            line.setEndPosition(line.endPosition().x, line.endPosition().y - 1.f);
        } else if (e.key.key == SDLK_DOWN) {
            line.setEndPosition(line.endPosition().x, line.endPosition().y + 1.f);
        } else if (e.key.key == SDLK_EQUALS) {
            line.setSize(std::clamp(line.size() + 1, 1, 255));
        } else if (e.key.key == SDLK_MINUS) {
            line.setSize(std::clamp(line.size() - 1, 1, 255));
        }
    });

    return engine.exec();
}