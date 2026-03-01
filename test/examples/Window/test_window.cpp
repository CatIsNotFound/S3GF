#include "MyWindow.h"

int main() {
    Engine engine;
    engine.setFPS(30);
    auto window = new MyWindow(&engine, "Test window");
    auto tool = new Window(window, "Test tool", 200, 400, Window::WindowType::Tool);
    auto tooltip = new Window(window, "Test tip", 300, 120, Window::WindowType::Tooltip);
    window->renderer()->setVSyncMode(Renderer::Adaptive);
    window->setResizable(true);
    window->setDragDropEnabled(true);
    window->show();
    tool->show();
    tooltip->installPaintEvent([](Renderer* r) {
        r->drawDebugText("Here is a tooltip!", {20, 20});
    });
    tooltip->show();
    return engine.exec();
}
