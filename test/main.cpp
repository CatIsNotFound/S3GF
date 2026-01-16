
#include "FilledWin.h"
#include "MyEngine"
#include "Widgets/Button.h"
#include "Widgets/LineEdit.h"
#include "MyWindow.h"

using namespace MyEngine;

int main(int argc, const char* argv[]) {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    FileSystem::setCurrentPath(FileSystem::getDirectoryFromFile(argv[0]));
    Engine engine;
    engine.setFPS(30);
    engine.setLimitMaxMemorySize(FileSystem::translateSize(10.f, MyEngine::FileSystem::MB));
    auto win = new MyWindow(&engine, "Test");
    win->setResizable(true);
    win->installPaintEvent([&](Renderer* r){
        r->fillBackground(RGBAColor::RedLightPink);
        static bool _t = false;
        if (!_t) {
            _t = true;
            r->setBlendMode(SDL_BLENDMODE_BLEND);
        }
        r->drawDebugFPS({60, 20});
    });

    Widget::LineEdit user_name("user_name", win),
                     user_passwd("user_passwd", win);
    auto sys_db = FontDatabase::getFontDatabaseFromSystem();
    user_name.setGeometry(80, 100, 240, 40);
    user_passwd.setGeometry(80, 150, 240, 40);
    user_name.setPlaceHolderText("User name");
    user_passwd.setPlaceHolderText("Password");
    user_name.setFont("simsun", sys_db.at("simsun"), 20.f);
    user_name.setTextCursorStyle(4, RGBAColor::BlueDark);
    user_name.setPadding(8, 8);
    user_passwd.setFont("simsun");
    user_passwd.setPasswordEnabled(true);
    user_passwd.setTextCursorStyle(2, RGBAColor::MixGrayDark);
    user_passwd.setText("123456");
    user_passwd.setBackgroundVisible(false);
    user_passwd.setPadding(8, 8);
//    user_passwd.setBorderVisible(false);
    Widget::Button button("show_passwd", win);
    button.setGeometry(120, 200, 140, 24);
    button.setFont("simsun");
    button.setText("Show password");
    button.setTextAlignment(MyEngine::Widget::CenterMiddle);
    button.setTriggerEvent([&button, &user_passwd] {
        if (user_passwd.passwordEnabled()) {
            user_passwd.setPasswordEnabled(false);
            button.setText("Hide password");
        } else {
            user_passwd.setPasswordEnabled(true);
            button.setText("Show password");
        }
    });
    Widget::Button login("login", win);
    login.setGeometry(120, 230, 140, 24);
    login.setFont("simsun");
    login.setText("Login");
    login.setTextAlignment(MyEngine::Widget::CenterMiddle);

    Widget::Label label("label", win);
    label.move(80, 70);
    label.setAutoResizedByTextEnabled(true);
    label.setFont("simsun");
    label.setTextColor(StdColor::Black);
    label.setText("Welcome");
    label.setBackgroundColor(RGBAColor::BlueIce);
    login.setTriggerEvent([&] {
        if (user_name.text() == "admin" && user_passwd.text() == "123456") {
            label.setText("Login Successful!");
        } else {
            label.setText("User name or password is incorrect!");
        }
    });
    Widget::Button enabled_trigger("enabled_trigger", win);
    enabled_trigger.setGeometry(120, 260, 140, 24);
    enabled_trigger.setFont("simsun");
    enabled_trigger.setText("Click me");
    enabled_trigger.setTextAlignment(MyEngine::Widget::CenterMiddle);
    enabled_trigger.setTriggerEvent([&] {
        user_name.setEnabled(!user_name.enabled());
    });
    return engine.exec();
}