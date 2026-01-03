
#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/Button.h"

using namespace MyEngine;

int main() {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine;
    engine.setFPS(20);
    auto win = new Window(&engine, engine.applicationName());
    win->setResizable(true);
    Texture texture(FileSystem::getAbsolutePath("./back_button_1.png"), win->renderer());
    texture.property()->color_alpha = RGBAColor::GreenLight;
    int i = 0;
    auto def_font = FontDatabase::getSystemDefaultFont().front();

    Widget::Button button1("button1", win);
    Widget::Button button2("button2", win);
    Widget::Button button3("button3", win);
    Widget::Label label("label", win);

    button1.setGeometry(20, 20, 180, 60);
    button2.setGeometry(20, 90, 180, 60);
    button3.setGeometry(20, 160, 180, 60);
    label.setGeometry(20, 240, 180, 60);

    button1.setFont(def_font.font_name, def_font.font_path, 32.f);
    button2.setFont(def_font.font_name);
    button3.setFont(def_font.font_name);
    label.setFont(def_font.font_name);

    button1.setText("Test 1");
    button2.setText("Test 2");
    button3.setText("Test 3");
    label.setText("Output");

    button1.setTextAlignment(Widget::Label::CenterMiddle);
    button2.setTextAlignment(Widget::Label::CenterMiddle);
    button3.setTextAlignment(Widget::Label::CenterMiddle);
    label.setTextAlignment(Widget::Label::CenterMiddle);

    button2.setFocusEnabled(true);
    button2.setHotKeyEnabled(true);
    button2.setHotKey(SDL_SCANCODE_LCTRL, SDL_SCANCODE_LALT, SDL_SCANCODE_SPACE);

    button1.setTriggerEvent([&label, &button1, &button2, &button3]{
        label.setText("Hello world");
        button1.setFontSize(24.f);
    });
    button2.setTriggerEvent([&label, &button1, &button2, &button3]{
        label.setText("Active");
        button1.setFontSize(32.f);
    });
    button3.setTriggerEvent([&label]{
        static bool tri = false;
        if (!tri) {
            label.resize(100, 50);
            tri = true;
        } else {
            label.resize(180, 60);
            tri = false;
        }
    });

    win->installPaintEvent([&win](Renderer* r) {
        r->fillBackground(StdColor::LimeGreen);
        r->drawDebugFPS({20, (float)(win->geometry().height) - 20});
    });

    engine.exec();

    return 0;
}