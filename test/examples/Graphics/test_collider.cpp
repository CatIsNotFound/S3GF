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
    bool is_check_collider = false;
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
            is_check_collider = false;
        }
        std::array<bool, 6> is_triggered;

        if (!is_check_collider) {
            is_check_collider = true;
            if (Algorithm::compareRects(rect1, rect2) >= 0) {
                rect1.setBackgroundColor(RGBAColor::BlueBaby);
                rect2.setBackgroundColor(RGBAColor::BlueBaby);
                is_triggered[0] = true;
                is_triggered[1] = true;
            } else {
                if (!is_triggered[0]) rect1.setBackgroundColor(StdColor::Gray);
                if (!is_triggered[1]) rect2.setBackgroundColor(StdColor::Gray);
            }
            if (Algorithm::compareRotatedEllipses(ellipse1, ellipse2) >= 0) {
                ellipse1.setBackground(RGBAColor::BlueBaby);
                ellipse2.setBackground(RGBAColor::BlueBaby);
                is_triggered[2] = true;
                is_triggered[3] = true;
            } else {
                if (!is_triggered[2]) ellipse1.setBackground(StdColor::Gray);
                if (!is_triggered[3]) ellipse2.setBackground(StdColor::Gray);
            }
            if (Algorithm::compareRotatedEllipses(ellipse3, ellipse4) >= 0) {
                ellipse3.setBackground(RGBAColor::BlueBaby);
                ellipse4.setBackground(RGBAColor::BlueBaby);
                is_triggered[4] = true;
                is_triggered[5] = true;
            } else {
                if (!is_triggered[4]) ellipse3.setBackground(StdColor::Gray);
                if (!is_triggered[5]) ellipse4.setBackground(StdColor::Gray);
            }
            if (Algorithm::compareRotatedEllipses(ellipse1, ellipse3) >= 0) {
                ellipse1.setBackground(RGBAColor::BlueBaby);
                ellipse3.setBackground(RGBAColor::BlueBaby);
                is_triggered[2] = true;
                is_triggered[4] = true;
            } else {
                if (!is_triggered[2]) ellipse1.setBackground(StdColor::Gray);
                if (!is_triggered[4]) ellipse3.setBackground(StdColor::Gray);
            }
            if (Algorithm::compareRotatedEllipses(ellipse2, ellipse4) >= 0) {
                ellipse2.setBackground(RGBAColor::BlueBaby);
                ellipse4.setBackground(RGBAColor::BlueBaby);
                is_triggered[3] = true;
                is_triggered[5] = true;
            } else {
                if (!is_triggered[3]) ellipse2.setBackground(StdColor::Gray);
                if (!is_triggered[5]) ellipse4.setBackground(StdColor::Gray);
            }
            if (Algorithm::compareRotatedEllipses(ellipse1, ellipse4) >= 0) {
                ellipse1.setBackground(RGBAColor::BlueBaby);
                ellipse4.setBackground(RGBAColor::BlueBaby);
                is_triggered[2] = true;
                is_triggered[5] = true;
            } else {
                if (!is_triggered[2]) ellipse1.setBackground(StdColor::Gray);
                if (!is_triggered[5]) ellipse4.setBackground(StdColor::Gray);
            }
            if (Algorithm::compareRotatedEllipses(ellipse2, ellipse3) >= 0) {
                ellipse2.setBackground(RGBAColor::BlueBaby);
                ellipse3.setBackground(RGBAColor::BlueBaby);
                is_triggered[3] = true;
                is_triggered[4] = true;
            } else {
                if (!is_triggered[3]) ellipse2.setBackground(StdColor::Gray);
                if (!is_triggered[4]) ellipse3.setBackground(StdColor::Gray);
            }
            if (Algorithm::compareRectEllipse(rect1, ellipse1) >= 0) {
                rect1.setBackgroundColor(RGBAColor::BlueBaby);
                ellipse1.setBackground(RGBAColor::BlueBaby);
                is_triggered[0] = true;
                is_triggered[2] = true;
            } else {
                if (is_triggered[0]) rect1.setBackgroundColor(StdColor::Gray);
                if (is_triggered[2]) ellipse1.setBackground(StdColor::Gray);
            }
            if (Algorithm::compareRectEllipse(rect1, ellipse2) >= 0) {
                rect1.setBackgroundColor(RGBAColor::BlueBaby);
                ellipse2.setBackground(RGBAColor::BlueBaby);
                is_triggered[0] = true;
                is_triggered[3] = true;
            } else {
                if (is_triggered[0]) rect1.setBackgroundColor(StdColor::Gray);
                if (is_triggered[3]) ellipse2.setBackground(StdColor::Gray);
            }
        }
    });
    window->show();
    return engine.exec();
}