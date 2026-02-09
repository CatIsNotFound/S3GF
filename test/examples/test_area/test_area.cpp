#include "MyTriggerArea.h"

int main() {
    Engine engine;
    engine.setFPS(30);
    auto window = new Window(&engine, Engine::applicationName().data());
    window->show();
    MyTriggerArea area(GeometryF{100.f, 100.f, 100.f, 100.f}, window);
    area.setTriggerKey(SDL_SCANCODE_SPACE);
    area.setTriggerEvent([] {
        Logger::log("Area is triggered", Logger::Info);
    });
    Graphics::Rectangle rect(area.geometry(), 0, {}, StdColor::Yellow);
    window->installPaintEvent([&rect, &area, &window] (Renderer* r) {
        r->fillBackground(StdColor::White);
        if (!area.isEnabled()) {
            rect.setBackgroundColor(StdColor::LightGray);
        } else if (area.isTriggeredOnArea()) {
            rect.setBackgroundColor(StdColor::LightPink);
        } else if (area.isOnArea()) {
            rect.setBackgroundColor(StdColor::LightBlue);
        } else {
            rect.setBackgroundColor(StdColor::Yellow);
        }
        r->drawRectangle(&rect);
        r->drawDebugText(FMT::format("E: {:08b}", area.events()), {20, 20});
        r->drawDebugText(FMT::format("Finger Count: {}", window->getFingersCount()), {20, 30});
    });
    // Timer timer(3000, [&area] {
    //     area.setEnabled(!area.isEnabled());
    // });
    // timer.start(0);

    return engine.exec();
}
