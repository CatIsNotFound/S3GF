
#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/Button.h"

using namespace MyEngine;

int main(int argc, const char* argv[]) {
//    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine;
    FileSystem::setCurrentPath(FileSystem::getDirectoryFromFile(argv[0]));
     engine.setFPS(60);
    auto win = new Window(&engine, engine.applicationName());
     auto win2 = new Window(&engine, "Second window");
    win->setResizable(true);
//    win->renderer()->setVSyncMode(Renderer::HalfRate);
    engine.setLimitMaxMemorySize(100'000);

    Texture texture1(FileSystem::getAbsolutePath("./back_button_1.png"), win->renderer());
    Texture texture2(FileSystem::getAbsolutePath("./back_button_2.png"), win->renderer());
    Texture texture3(FileSystem::getAbsolutePath("./back_button_3.png"), win->renderer());
    Texture texture4(FileSystem::getAbsolutePath("./back_button_4.png"), win->renderer());
    auto def_fonts = FontDatabase::getSystemDefaultFont();
    auto first_font = def_fonts.front();
    auto second_font = def_fonts.at(1);
    TextSystem::global()->addFont(first_font.font_name, first_font.font_path, win->renderer(), 32.f);
    TextSystem::global()->addFont(second_font.font_name, second_font.font_path, win->renderer(), 32.f);

    Widget::Button button1("button1", win);
    Widget::Button button2("button2", win);
    Widget::Button button3("button3", win);
    Widget::Label label("label", win);
    Timer timer(10, [&label, &engine, &win, &timer] {
        if (!engine.isWindowExist(win->windowID())) {
            if (win) {
                Logger::log("The window pointer is still not null!");
            } else {
                timer.stop();
            }
        }

        static float dx = 1, dy = 1;
        auto pos = label.position();
        if (pos.x >= 700 || pos.x <= 0) dx = -dx;
        if (pos.y >= 570 || pos.y <= 0) dy = -dy;
        label.move(pos.x + dx, pos.y + dy);
    });
    button1.setGeometry(20, 20, 180, 60);
    button2.setGeometry(20, 90, 180, 60);
    button3.setGeometry(20, 160, 180, 60);
    label.setGeometry(20, 240, 180, 60);

    button1.setBackgroundVisible(false);
    button1.setBackgroundImageVisible(true);
    button1.setBackgroundImage(Widget::WidgetStatus::Normal, &texture1);
    button1.setBackgroundImage(Widget::WidgetStatus::Hovered, &texture2);
    button1.setBackgroundImage(Widget::WidgetStatus::Pressed, &texture3);
    button1.setBackgroundImage(Widget::WidgetStatus::Disabled, &texture4);
    button1.setBackgroundImage(Widget::WidgetStatus::Checked, &texture3);
    button1.setBackgroundImageFillMode(Widget::Label::Stretch);
    button1.setFont(first_font.font_name, first_font.font_path, 32.f);
    button2.setFont(first_font.font_name);
    button3.setFont(first_font.font_name);
    label.setFont(first_font.font_name);

    label.setBackgroundImage(&texture1, false);
    label.setBackgroundImageFillMode(Widget::Label::Fit);
    label.setTextColor(StdColor::Black);
    label.setTextAlignment(Widget::Label::CenterMiddle);

    button1.setText("Test 1");
    button2.setText("Test 2");
    button3.setText("Test 3");
    label.setText("Output");

    button1.setTextAlignment(Widget::Label::CenterMiddle);
    button2.setTextAlignment(Widget::Label::CenterMiddle);
    button3.setTextAlignment(Widget::Label::CenterMiddle);

    button2.setFocusEnabled(true);
    button2.setHotKeyEnabled(true);
    button2.setHotKey(SDL_SCANCODE_LCTRL, SDL_SCANCODE_LALT, SDL_SCANCODE_SPACE);

    button1.setTriggerEvent([&label, &button1]{
        label.setText("Hello world");
        button1.setFontSize(24.f);
    });
    button2.setTriggerEvent([&label, &button1, &timer]{
        label.setText("Active");
        button1.setFontSize(32.f);
        if (timer.enabled()) timer.stop(); else timer.start(0);
    });
    button3.setTriggerEvent([&label, &second_font, &first_font]{
        static bool tri = false;
        if (!tri) {
            label.resize(100, 50);
            label.setFont(second_font.font_name);
            tri = true;
        } else {
            label.resize(180, 60);
            label.setFont(first_font.font_name);
            tri = false;
        }
        label.setTextAlignment(MyEngine::Widget::Label::CenterMiddle);
    });

    win->installPaintEvent([&win](Renderer* r) {
        r->fillBackground(StdColor::LimeGreen);
        r->drawDebugFPS({20, (float)(win->geometry().height) - 20});
        r->drawDebugText(std::format("Memory: {} kB", SysMemory::getCurProcUsedMemSize()), {20, (float)(win->geometry().height - 30)});
        r->drawDebugText(std::format("Render counts: {}", r->renderCountInSec()), {20, (float)(win->geometry().height - 40)});
    });

    win2->installPaintEvent([&win2](Renderer* r) {
        r->fillBackground(StdColor::LightPink);
        r->drawDebugFPS({20, (float)(win2->geometry().height) - 20});
        r->drawDebugText(std::format("Memory: {} kB", SysMemory::getCurProcUsedMemSize()), {20, (float)(win2->geometry().height - 30)});
    });

    timer.start(0);
    engine.exec();
    return 0;
}