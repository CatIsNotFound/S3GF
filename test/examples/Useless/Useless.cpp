#include "MyEngine"

using namespace MyEngine;

#ifdef __USED_VLD_LIB__
#include <vld.h>
#endif

int main() {
    Engine engine;
    engine.setFPS(60);
    auto win2 = new Window(&engine, "2nd window", 800, 600, Window::Vulkan);
    auto rect = Algorithm::drawFilledRoundedRectangleSurface(800, 600, 20);
    // SDL_SetWindowShape(win2->self(), rect);
    delay(1);
    win2->show();
    win2->setResizable(true);
    win2->setBorderless(true);
    win2->setWindowShape(rect);
    win2->installPaintEvent([&](Renderer* r) {
        r->fillBackground(StdColor::LightBlue);
        r->drawDebugFPS({40, 40}, StdColor::DarkBlue);
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_P)) {
            auto p = r->capture();
            IMG_SavePNG(p, "./output_window.png");
            SDL_DestroySurface(p);
        }
        static bool is_moving = false;
        if (EventSystem::global()->captureMouse(MouseStatus::Left)) {
            static Vector2 old_pos, old_mouse_cur;
            if (!is_moving) {
                old_pos = Vector2{static_cast<float>(r->window()->geometry().x), static_cast<float>(r->window()->geometry().y)};
                old_mouse_cur = Cursor::global()->globalPosition();
                is_moving = true;
                Cursor::global()->setCursor(Cursor::Move);
            } else {
                auto new_distance = Cursor::global()->globalPosition() - old_mouse_cur;
                auto new_pos = old_pos + new_distance;
                r->window()->move(new_pos.x, new_pos.y);
            }
        } else {
            if (is_moving) {
                is_moving = false;
                Cursor::global()->setCursor(Cursor::Default);
                if (r->window()->geometry().y < 0) r->window()->move(r->window()->geometry().x, 0);
            }
        }
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_ESCAPE)) engine.exit();
    });
    Logger::log(Logger::Info, "Window created: {}", win2->windowTitle());
    return engine.exec();
}
