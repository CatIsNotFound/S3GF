#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Test Rectangle");
    window->show();
    Graphics::Rectangle rect(window->geometry().width / 4.f, window->geometry().height / 4.f,
                        window->geometry().width * 0.5f, window->geometry().height * 0.5f,
                        1, StdColor::DarkBlue, StdColor::LightBlue, 0);
    window->installPaintEvent([&rect] (Renderer* r) {
        r->drawRectangle(&rect);
        r->drawDebugText(FMT::format("Rect: geometry = {}, size = {}, rotation = {:.2f} degrees",
            rect.geometry(), rect.borderSize(), rect.rotation()), {20, 20});
    });

    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&] (SDL_Event e) {
        if (e.type != SDL_EVENT_KEY_DOWN) return;
        if (e.key.key == SDLK_W) {
            rect.move(rect.geometry().pos.x, rect.geometry().pos.y - 1.f);
        } else if (e.key.key == SDLK_S) {
            rect.move(rect.geometry().pos.x, rect.geometry().pos.y + 1.f);
        } else if (e.key.key == SDLK_A) {
            rect.move(rect.geometry().pos.x - 1.f, rect.geometry().pos.y);
        } else if (e.key.key == SDLK_D) {
            rect.move(rect.geometry().pos.x + 1.f, rect.geometry().pos.y);
        } else if (e.key.key == SDLK_UP) {
            rect.resize(rect.geometry().size.width, rect.geometry().size.height - 1.f);
        } else if (e.key.key == SDLK_DOWN) {
            rect.resize(rect.geometry().size.width, rect.geometry().size.height + 1.f);
        } else if (e.key.key == SDLK_LEFT) {
            rect.resize(rect.geometry().size.width - 1.f, rect.geometry().size.height);
        } else if (e.key.key == SDLK_RIGHT) {
            rect.resize(rect.geometry().size.width + 1.f, rect.geometry().size.height);
        } else if (e.key.key == SDLK_EQUALS) {
            rect.setBorder(std::clamp(rect.borderSize() + 1, 1, 255), rect.borderColor());
        } else if (e.key.key == SDLK_MINUS) {
            rect.setBorder(std::clamp(rect.borderSize() - 1, 1, 255), rect.borderColor());
        } else if (e.key.key == SDLK_Q) {
            rect.setRotate(rect.rotation() - 1.f);
        } else if (e.key.key == SDLK_E) {
            rect.setRotate(rect.rotation() + 1.f);
        }
    });

    return engine.exec();
}