#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/AbstractWidget.h"
using namespace MyEngine;

int main() {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, engine.applicationName(), 1024, 800);
    window->setResizable(true);
    window->setDragDropEnabled(true);
    Widget::AbstractWidget wid(window);
    Widget::AbstractWidget wid2(window);
    wid.setGeometry(100, 200, 600, 400);
    wid.setCursor(MyEngine::Cursor::Hand);
    wid.setHotKey(SDL_SCANCODE_LCTRL, SDL_SCANCODE_LALT, SDL_SCANCODE_SPACE);
    wid.setFocusEnabled(true);
    Graphics::Rectangle rect(300, 300, 100, 200, 32, RGBAColor::GreenDark, RGBAColor::MixOrangeYellow);
    Graphics::Rectangle rect2(200, 100, 200, 100, 32, RGBAColor::RedDark, RGBAColor::RedLightPink);
    Graphics::Rectangle rect3(100, 200, 600, 400);
    TextSystem::global()->addFont("simsun", R"(C:\Windows\Fonts\simsun.ttc)", window->renderer());
//    TextSystem::global()->font("simsun")->setFontSize(64.f);
    uint64_t id = IDGenerator::getNewTextID();
    TextSystem::global()->addText(id, "simsun", "Use Ctrl+Alt+Space to enabled input mode!");
    TextSystem::global()->setTextColor(id, StdColor::Blue);
    window->installPaintEvent([&rect, &rect2, &rect3, &id, &wid](Renderer* r) {
        auto ro = rect.rotate();
        if (ro + 1.f >= 360.f) ro = 0; else ro += 1.f;
        rect.setRotate(ro);
        rect2.setRotate(360.f - ro);
        r->drawRectangle(&rect3);
        r->drawRectangle(&rect);
        r->drawRectangle(&rect2);
        r->drawDebugFPS();

        if (wid.inputModeEnabled()) {
            TextSystem::global()->setText(id, wid.getInputChar());
        } else {
            TextSystem::global()->setText(id, "Press Ctrl+Alt+Space to enabled input mode!");
        }
        TextSystem::global()->drawText(id, {100, 100}, r);
        auto mouse_pos = EventSystem::global()->captureMousePosition();
        r->drawDebugText(std::format("Mouse pos: ({}, {})", mouse_pos.x, mouse_pos.y), {20, 30});
        r->drawDebugText(std::format("Button: {}", EventSystem::global()->captureMouseStatus()), {20, 40});
    });

    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&window, &rect, &rect2](SDL_Event e) {
        if (Cursor::global()->focusOn() != window->windowID()) return;
        auto mouse_pos = Cursor::global()->position();
        static bool trigger = false;
        static bool drag_1 = false, drag_2 = false;
        static Vector2 old_pos_1, old_pos_2;

        if (EventSystem::global()->captureMouse(EventSystem::Left)) {
            if (!trigger) {
                trigger = true;
                if (Algorithm::comparePosInRotatedRect(mouse_pos, rect) > 0) {
                    drag_1 = true;
                    old_pos_1 = rect.geometry().pos;
                    Cursor::global()->setCursor(Cursor::Move);
                    return;
                } else if (Algorithm::comparePosInRotatedRect(mouse_pos, rect2) > 0) {
                    drag_2 = true;
                    old_pos_2 = rect2.geometry().pos;
                    Cursor::global()->setCursor(Cursor::Move);
                    return;
                }
            }
            auto moved_distance = EventSystem::global()->captureMouseAbsDistance();
            if (drag_1) {
                rect.move(old_pos_1 + moved_distance);
            } else if (drag_2) {
                rect2.move(old_pos_2 + moved_distance);
            }
        } else {
            if (trigger) {
                if (drag_1 || drag_2) Cursor::global()->setCursor(window->cursor());
                trigger = false;
                drag_1 = false;
                drag_2 = false;
                if (rect.geometry().pos < Vector2(0, 0)) rect.move(200, 200);
                if (rect2.geometry().pos < Vector2(0, 0)) rect2.move(400, 200);
            }
        }

    });

    return engine.exec();
}