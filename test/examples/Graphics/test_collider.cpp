#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Hello world!");
    window->setResizable(true);
    Graphics::Rectangle rect1(0, 0, 100, 100);
    Graphics::Rectangle rect2(120, 0, 100, 100);
    Graphics::Ellipse ellipse1(290, 50, 50, 50);
    Graphics::Ellipse ellipse2(410, 50, 50, 50);
    Graphics::Ellipse ellipse3(120, 240, 100, 50);
    Graphics::Ellipse ellipse4(270, 240, 50, 100);
    window->installPaintEvent([&](Renderer* r) {
        r->drawRectangle(&rect1);
        r->drawRectangle(&rect2);
        r->drawEllipse(&ellipse1);
        r->drawEllipse(&ellipse2);
        r->drawEllipse(&ellipse3);
        r->drawEllipse(&ellipse4);
    });
    EventSystem::global()->appendEvent([&](SDL_Event ev) {
        auto mouse_cur = EventSystem::global()->captureMousePosition();
        static uint8_t is_moving = 0;
        static Vector2 old_pos;
        bool is_left_button_down = EventSystem::global()->captureMouse(MouseStatus::Left);
        if (is_left_button_down) {
            if (Algorithm::comparePosInRect(mouse_cur, rect1) >= 0) {
                if (is_moving == 0) {
                    is_moving = 1;
                    old_pos = rect1.geometry().pos;
                } else if (is_moving == 1) {
                    rect1.move(old_pos + EventSystem::global()->captureMouseAbsDistance());
                }
            }
            if (Algorithm::comparePosInRect(mouse_cur, rect2) >= 0) {
                if (!is_moving) {
                    is_moving = 2;
                    old_pos = rect2.geometry().pos;
                } else if (is_moving == 2) {
                    rect2.move(old_pos + EventSystem::global()->captureMouseAbsDistance());
                }
            }
            if (Algorithm::comparePosInRotatedEllipse(mouse_cur, ellipse1) >= 0) {
                if (!is_moving) {
                    is_moving = 3;
                    old_pos = ellipse1.centerPosition();
                } else if (is_moving == 3) {
                    ellipse1.move(old_pos + EventSystem::global()->captureMouseAbsDistance());
                }
            }
            if (Algorithm::comparePosInRotatedEllipse(mouse_cur, ellipse2) >= 0) {
                if (!is_moving) {
                    is_moving = 4;
                    old_pos = ellipse2.centerPosition();
                } else if (is_moving == 4) {
                    ellipse2.move(old_pos + EventSystem::global()->captureMouseAbsDistance());
                }
            }
            if (Algorithm::comparePosInRotatedEllipse(mouse_cur, ellipse3) >= 0) {
                if (!is_moving) {
                    is_moving = 5;
                    old_pos = ellipse3.centerPosition();
                } else if (is_moving == 5) {
                    ellipse3.move(old_pos + EventSystem::global()->captureMouseAbsDistance());
                }
            }
            if (Algorithm::comparePosInRotatedEllipse(mouse_cur, ellipse4) >= 0) {
                if (!is_moving) {
                    is_moving = 6;
                    old_pos = ellipse4.centerPosition();
                } else if (is_moving == 6) {
                    ellipse4.move(old_pos + EventSystem::global()->captureMouseAbsDistance());
                }
            }
        } else {
            is_moving = false;
        }
    });
    window->show();
    return engine.exec();
}