
#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/Button.h"
#include "Widgets/LineEdit.h"

using namespace MyEngine;

int main(int argc, const char* argv[]) {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    FileSystem::setCurrentPath(FileSystem::getDirectoryFromFile(argv[0]));
    Engine engine;
    engine.setFPS(30);
    engine.setLimitMaxMemorySize(FileSystem::translateSize(10.f, MyEngine::FileSystem::MB));
    auto win = new Window(&engine, "Test");
    Graphics::Rectangle progress(0, 0, 0, 0, 4, StdColor::Orange, StdColor::Yellow);
    BGM bgm(AudioSystem::global()->mixer(), FileSystem::getAbsolutePath("./main.mp3"));
    bgm.setVolume(0.01f);
    bgm.setSpeedAndPitch(1.25f);
    bgm.play(0, true);
    Widget::LineEdit line_edit("line_edit", win);
    win->setResizable(true);
    win->installPaintEvent([&](Renderer* r) {
        r->fillBackground(RGBAColor::BlueLake);
        r->drawRectangle(&progress);
        r->drawDebugFPS();
        r->drawDebugText(std::format("BGM: {} / {}", bgm.position(), bgm.duration()), {20, 30});
        r->drawDebugText(std::format("MEMORY: {:.2f} MB", FileSystem::translateSize(SysMemory::getCurProcUsedMemSize(), MyEngine::FileSystem::KB)), {20, 40});
    });
    auto font_map = FontDatabase::getFontDatabaseFromSystem();
    FontDatabase::FontInfo def_font = {"simsun", font_map.at("simsun")};
    line_edit.setGeometry(20, 80, 280, 48);
    line_edit.setFont(def_font.font_name, def_font.font_path, 20.f);
    line_edit.setPadding(12);
//    line_edit.setPlaceHolderText("Input some text...");
    Widget::Button button("button", win);
    button.setFont(def_font.font_name);
    button.setText("Confirm");
    button.move(50, 370);
    button.setTextAlignment(Widget::Alignment::CenterMiddle);
    button.setTriggerEvent([&] {
        bool is_empty = line_edit.placeHolderText().empty();
        line_edit.setPlaceHolderText(is_empty ? "Input here..." : "");
        if (is_empty) {
            button.setText("Hide");
        } else {
            button.setText("Show");
        }
    });
    EventSystem::global()->appendGlobalEvent(IDGenerator::getNewGlobalEventID(), [&] {
        float p = (float) (bgm.position()) / (float) (bgm.duration());
        progress.resize(win->geometry().width * p, 20);
    });
    return engine.exec();
}