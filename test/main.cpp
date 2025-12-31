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
    Texture texture(FileSystem::getAbsolutePath("./back_button_1.png"), win->renderer());
    texture.property()->color_alpha = RGBAColor::GreenLight;
    std::array<std::unique_ptr<Widget::Label>, 9> _test_labels;
    int i = 0;
    auto def_font = FontDatabase::getSystemDefaultFont().front();
    for (auto& label : _test_labels) {
        label = std::make_unique<Widget::Label>(std::format("Label #{}", i++), win);
        if (i == 1) {
            label->setFont(def_font.font_name, def_font.font_path, 24.f);
        } else {
            label->setFont(def_font.font_name);
        }
        label->setText(label->objectName());
        label->resize(300, 100);

        label->setTextAlignment(static_cast<Widget::Label::Alignment>(i - 1));
        auto b = (i - 1) / 3;
        label->setBackgroundColor(i % 2 ? 0x22d819 : 0x1922d8);
        Logger::log(std::format("IG: {}", b));
        label->setBackgroundImageVisible(true);
        label->setBackgroundImage(&texture, false);
        label->setBackgroundImageFillMode(static_cast<Widget::Label::ImageFilledMode>(b + 2));
        label->setFontColor(0x902020);
        label->setBackgroundVisible(false);
        label->move((label->size().width) * (float)((i - 1) % 3), (label->size().height) * (float)b);
    }
    win->installPaintEvent([](Renderer* r) {
        r->fillBackground(StdColor::LightBlue);
    });

    return engine.exec();
}