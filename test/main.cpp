#include "Core.h"
#include "Utils/Logger.h"
#include "Controls/Control.h"
#include "MultiThread/Components.h"
#include "Utils/Random.h"

using namespace S3GF;

class TestControl : public AbstractControl {
public:
    explicit TestControl(const std::string& name, Renderer* renderer, const Vector2& position = {0, 0},
                          AbstractControl* parent = nullptr)
          : AbstractControl(name, renderer, Graphics::Point(), parent) {
        _geometry.reset(position.x, position.y, 80, 60);
        _rect.reset(_geometry, 1, StdColor::Black, StdColor::MixGrayLight);
        _click_area.setGraphic(_rect);
    }
protected:
    void loadEvent() override {
        Logger::log(std::format("Loaded: {}", _name), S3GF::Logger::INFO);
    }

    void unloadEvent() override {
        Logger::log(std::format("Unloaded: {}", _name), S3GF::Logger::INFO);
    }

    void onEntered() override {
        Logger::log(std::format("Entered: {}", _name), S3GF::Logger::INFO);
    }

    void onHovered() override {
        Cursor::global()->setCursor(Cursor::Hand);
        _rect.setBackgroundColor(StdColor::BlueLight);
    }

    void onLeft() override {
        Logger::log(std::format("Left: {}", _name), S3GF::Logger::INFO);
        Cursor::global()->setCursor(Cursor::Default);
        _rect.setBackgroundColor(StdColor::MixGrayLight);
    }

    void onPressed() override {
        Logger::log(std::format("Pressed: {}", _name), S3GF::Logger::INFO);
        _rect.setBackgroundColor(StdColor::MixGrayDark);
    }

    void resizeEvent() override {
        AbstractControl::resizeEvent();
        _click_area.getRect(_rect);
    }

    void moveEvent() override {
        _click_area.getRect(_rect);
    }

    void paintEvent(Renderer* ren) override {
        ren->drawRectangle(_rect);
    }

private:
    Graphics::Rectangle _rect;
};

int main() {
//    Logger::setBaseLogLevel(S3GF::Logger::DEBUG);
    Engine engine;
    Window* main_win = new Window(&engine, "我的第一个窗口");
//    Window* sec_win = new Window(&engine, "第二个");
    Graphics::Point pt(300, 300, 20);
    Graphics::Rectangle rect1(100, 100, 100, 100, 1, StdColor::BlueDark);
    Graphics::Rectangle rect2(150, 150, 100, 100);

    TestControl con1("con1", main_win->renderer(), {100, 200});
    TestControl con2("con2", main_win->renderer(), {200, 300});

    Logger::log(std::format("Created {}: ({}, {}), ({} x {})",
                            con1.name(), con1.position().x, con1.position().y,
                            con1.size().width, con1.size().height));
    Logger::log(std::format("Created {}: ({}, {}), ({} x {})",
                            con2.name(), con2.position().x, con2.position().y,
                            con2.size().width, con2.size().height));

    main_win->installPaintEvent([&pt, &rect1, &rect2](Renderer* r) {
        r->fillBackground(StdColor::MixGray);
    });

    Timer timer(5000, [&con1, &rect1] {
        auto f1 = RandomGenerator::randFloat(20.f, 760.f);
        auto f2 = RandomGenerator::randFloat(20.f, 560.f);
        Logger::log(std::format("Rand: {} {}", f1, f2), S3GF::Logger::INFO);
        con1.move(f1, f2);
        rect1.move(f1, f2);
    });
    timer.start(0);
    return engine.exec();
}
