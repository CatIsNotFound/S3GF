#include "Core.h"
#include "Algorithm/All"
#include "MultiThread/Components.h"

using namespace S3GF;
int testGraphic(int argc, char* argv[]) {
    Engine engine;
    engine.newWindow(new Window(&engine, "Hello world!"));
    auto win = engine.window();
    win->setResizable(true);
    engine.setFPS(120);

    Timer timer(1000, [&engine, &win] {
        win->setWindowTitle(std::format("FPS: {}", engine.fps()));
    });

    Graphics::Rectangle rect1(0, 0, 100, 100, 2, RGBAColor::RedDark, RGBAColor::RedLightTrans);
    Graphics::Rectangle rect2(300, 200, 200, 150, 4, RGBAColor::BlueDark, RGBAColor::BlueSeaTrans);
    Graphics::Ellipse ellipse(400, 300, 100, 50, 6, RGBAColor::RedLightPink, RGBAColor::MixOrangeYellow);
    Geometry clip_area(0, 0, 300, 300);
    engine.window()->installPaintEvent([&rect1, &rect2, &ellipse](Renderer* r) {
        // r->setClipView(clip_area);
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        r->fillBackground({142, 198, 128, 128});
        r->drawRectangle(rect1);
        r->drawRectangle(rect2);
        auto pos = rect2.geometry().pos;
        auto size = rect2.geometry().size;
        static float fx = 0.5f, fy = 0.5f;
        float winW = static_cast<float>(r->window()->geometry().width);
        float winH = static_cast<float>(r->window()->geometry().height);
        if (pos.x >= winW - size.width) {
            fx = -0.5f;
        }
        if (pos.y >= winH - size.height) {
            fy = -0.5f;
        }
        if (pos.x <= 0) {
            fx = 0.5f;
        }
        if (pos.y <= 0) {
            fy = 0.5f;
        }
        rect2.move(pos.x + fx, pos.y + fy);
        auto degree = ellipse.rotateDegree();
        ellipse.setRotate((degree >= 360.f) ? 0.f : degree + 0.5f);
        r->drawEllipse(ellipse);
    });
    EventSystem::global()->appendEvent(10, [&win, &rect1, &rect2](SDL_Event ev) {
        // auto cur_pos = Cursor::global()->position();
        // rect1.setGeometry(cur_pos, rect1.geometry().size);
        if (Algorithm::compareRects(rect1, rect2) >= 0) {
            rect1.setBackgroundColor(RGBAColor::MixYellowTrans);
            rect2.setBackgroundColor(RGBAColor::MixPurpleTrans);
        } else {
            rect1.setBackgroundColor(RGBAColor::RedLightTrans);
            rect2.setBackgroundColor(RGBAColor::BlueSeaTrans);
        }
//        if (Algorithm::comparePosInRect(cur_pos, rect2) >= 0) {
//            Cursor::global()->setCursor(Cursor::Hand);
//        } else {
//            Cursor::global()->setCursor(Cursor::Default);
//        }
        if (ev.key.down) {
            if (ev.key.key == SDLK_F) {
                win->setFullScreen(!win->fullScreen(), false);
            } else if (ev.key.key == SDLK_L) {
                win->setBorderless(!win->borderless());
            } else if (ev.key.key == SDLK_H) {
                // Cursor::global()->setVisible(false);
            } else if (ev.key.key == SDLK_S) {
                // Cursor::global()->setVisible(true);
            }
        }
    });

    timer.start(0);
    return engine.exec();
}
