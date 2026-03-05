#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    auto win = new Window(&engine, "Test file dialog");
    auto font_db = FontDatabase::getSystemDefaultFont();
    const auto& FONT = font_db.front();
    TextSystem::global()->addFont(FONT.font_name, FONT.font_path, win->renderer(), 24.f);
    Widget::Label label("result", win);
    bool is_opened_file_dialog = false;
    label.setFont(FONT.font_name);
    label.setText("It will show the URL here...");
    label.setTextColor(RGBAColor::Black);
    label.setBackgroundVisible(false);
    label.setAutoResizedByTextEnabled(true);
    label.move(100, 100);
    Widget::Button button1("openFile", win);
    button1.setFont(FONT.font_name);
    button1.setText("Open File...");
    button1.setTextAlignment(Widget::CenterMiddle);
    button1.move(100, 140);
    button1.setTriggerEvent([&] {
        engine.openFileDialog({"All files(*.*)", "Text file(*.txt;*.json;*.xml)"},
                              FileSystem::homePath(), win);
        is_opened_file_dialog = true;
    });
    Widget::Button button2("openDir", win);
    button2.setFont(FONT.font_name);
    button2.setText("Open Directory...");
    button2.setTextAlignment(Widget::CenterMiddle);
    button2.move(300, 140);
    button2.setTriggerEvent([&] {
        engine.openDirectoryDialog(FileSystem::homePath(), win);
        is_opened_file_dialog = true;
    });
    EventSystem::global()->appendGlobalEvent([&] {
        if (is_opened_file_dialog) {
            bool is_cancelled = false, is_finished = false;
            auto url = engine.getFileDialogURL(&is_cancelled, &is_finished);
            if (is_finished) {
                label.setText(is_cancelled ? "User cancelled!" : url);
                is_opened_file_dialog = false;
            }
        }
    });
    win->show();
    return engine.exec();
}
