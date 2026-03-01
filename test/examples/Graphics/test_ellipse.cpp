#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Test Ellipse");
    window->show();
    Graphics::Ellipse ellipse(window->geometry().width / 2.f, window->geometry().height / 2.f,
                        window->geometry().width * 0.25f, window->geometry().height * 0.25f,
                        1, StdColor::DarkRed, StdColor::Orange, 0);
    window->installPaintEvent([&ellipse] (Renderer* r) {
        r->drawEllipse(&ellipse);
        r->drawDebugText(FMT::format("Ellipse: center_pos = {}, size = {}",
            ellipse.centerPosition(), ellipse.size()), {20, 20});
        r->drawDebugText(FMT::format("{:>9s}border = {}, indices = {}, rotation = {} degrees",
            " ", ellipse.borderSize(), ellipse.indicesCount(), ellipse.rotation()), {20, 30});
        auto mouse_cur = EventSystem::global()->captureMousePosition();
        if (Algorithm::comparePosInRotatedEllipse(mouse_cur, ellipse) >= 0) {
            ellipse.setBorderColor(RGBAColor::GreenDark);
            ellipse.setBackground(RGBAColor::GreenApple);
        } else {
            ellipse.setBorderColor(StdColor::DarkRed);
            ellipse.setBackground(StdColor::Orange);
        }
    });

    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&] (SDL_Event e) {
        if (e.type != SDL_EVENT_KEY_DOWN) return;
        if (e.key.key == SDLK_W) {
            ellipse.move(ellipse.centerPosition().x, ellipse.centerPosition().y - 1.f);
        } else if (e.key.key == SDLK_S) {
            ellipse.move(ellipse.centerPosition().x, ellipse.centerPosition().y + 1.f);
        } else if (e.key.key == SDLK_A) {
            ellipse.move(ellipse.centerPosition().x - 1.f, ellipse.centerPosition().y);
        } else if (e.key.key == SDLK_D) {
            ellipse.move(ellipse.centerPosition().x + 1.f, ellipse.centerPosition().y);
        } else if (e.key.key == SDLK_UP) {
            ellipse.resize(ellipse.size().width, ellipse.size().height - 1.f);
        } else if (e.key.key == SDLK_DOWN) {
            ellipse.resize(ellipse.size().width, ellipse.size().height + 1.f);
        } else if (e.key.key == SDLK_LEFT) {
            ellipse.resize(ellipse.size().width - 1.f, ellipse.size().height);
        } else if (e.key.key == SDLK_RIGHT) {
            ellipse.resize(ellipse.size().width + 1.f, ellipse.size().height);
        } else if (e.key.key == SDLK_EQUALS) {
            ellipse.setBorder(std::clamp(ellipse.borderSize() + 1, 1, 255), ellipse.borderColor());
        } else if (e.key.key == SDLK_MINUS) {
            ellipse.setBorder(std::clamp(ellipse.borderSize() - 1, 1, 255), ellipse.borderColor());
        } else if (e.key.key == SDLK_Q) {
            ellipse.setRotate(ellipse.rotation() - 1.f);
        } else if (e.key.key == SDLK_E) {
            ellipse.setRotate(ellipse.rotation() + 1.f);
        }

    });

    return engine.exec();
}