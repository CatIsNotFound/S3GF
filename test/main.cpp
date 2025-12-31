#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/Label.h"

using namespace MyEngine;

int main() {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine;
    engine.setFPS(30);
    auto win = new Window(&engine, engine.applicationName());
    win->setResizable(true);
    Widget::Label label("Test label", win);
    label.move(200, 200);
    auto font = FontDatabase::getSystemDefaultFont().front();
    label.setFont(font.font_name, font.font_path, 24.f);
    label.setFontColor(RGBAColor::MixOrangeYellow);
    label.setText(label.objectName());
    label.setBackgroundColor(RGBAColor::BlueCobalt);
    win->installPaintEvent([](Renderer* r) {
        r->fillBackground(StdColor::LightBlue);
    });

    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&label](SDL_Event e) {
        if (e.drop.type == SDL_EVENT_DROP_TEXT) {
            label.setText(e.drop.data);
        }
    });
    return engine.exec();
}