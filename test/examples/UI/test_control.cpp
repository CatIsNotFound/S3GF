#include "CustomUI.h"

using namespace MyEngine;

class TT {
public:
    explicit TT() {
        std::cout << "TT()" << std::endl;
    }

    ~TT() {
        std::cout << "~TT()" << std::endl;
    }
};

int main() {
    Logger::setBaseLogLevel(Logger::Debug);
    Engine engine;
    engine.setFPS(60);
    auto win = new Window(&engine, "Test Window", 880, 660);
    CustomControl control("custom_control", win);
    control.setGeometry(0, 0, 150, 150);
    control.setCursor(Cursor::Hand);
    control.setHotkeys(SDL_SCANCODE_LCTRL, SDL_SCANCODE_SPACE, SDL_SCANCODE_S);
    control.setHotkeys(SDL_SCANCODE_RCTRL, SDL_SCANCODE_W);
    control.setActive(true);
    control.setLockedEnabled(true);
    control.move(100, 100);
    control.resize(100, 100);

    control.setProperty("hello", Variant(new TT(), [](void* d) { delete static_cast<TT*>(d); }));
    win->show();
    Graphics::Rectangle rect(control.geometry());
    win->installPaintEvent([&rect](Renderer* r) {
        r->drawRectangle(&rect);
    });
    return engine.exec();
}