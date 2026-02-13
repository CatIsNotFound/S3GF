
#ifndef MYENGINE_TEST_MYWINDOW_H
#define MYENGINE_TEST_MYWINDOW_H
#include "MyEngine"

using namespace MyEngine;

class MyWindow : public MyEngine::Window {
public:
    explicit MyWindow(MyEngine::Engine* engine, const std::string& title, int width = 800, int height = 600);
    ~MyWindow() override;

protected:
    void paintEvent() override;
    void resizeEvent() override;
    void moveEvent() override;
    void getFocusEvent() override;
    void lostFocusEvent() override;
    void unloadEvent() override;
    void showEvent() override;
    void hideEvent() override;
    void windowMinimizedEvent() override;
    void windowMaximizedEvent() override;
    void windowRestoredEvent() override;
    void enteredFullscreenEvent() override;
    void leaveFullscreenEvent() override;
    void mouseEnteredEvent() override;
    void mouseLeftEvent() override;
    void mouseUpEvent() override;
    void mouseDownEvent(MyEngine::MouseStatus button) override;
    void mouseClickedEvent(MyEngine::MouseStatus button) override;
    void mouseMovedEvent(const MyEngine::Vector2& position, const MyEngine::Vector2& distance) override;
    void keyUpEvent(SDL_Scancode keycode) override;
    void keyDownEvent(SDL_Scancode keycode) override;
    void keyPressedEvent(SDL_Scancode keycode) override;
    void fingerDownEvent(SDL_FingerID id, const MyEngine::Vector2 &position) override;
    void fingerUpEvent(SDL_FingerID id, const MyEngine::Vector2 &position) override;
    void fingerMovedEvent(SDL_FingerID id, const MyEngine::Vector2 &position, const MyEngine::Vector2 &distance) override;
    void fingerMoveOutEvent(SDL_FingerID id) override;
    void fingerMoveInEvent(SDL_FingerID id) override;
    void fingerTappedEvent(SDL_FingerID id, const MyEngine::Vector2 &position) override;
    void dragInEvent() override;
    void dragOutEvent() override;
    void dragMovedEvent(const MyEngine::Vector2 &position, const char *data) override;
    void dropEvent(const char *url) override;

private:
    static std::string_view mouseButtonString(MouseStatus status);
    std::string _debug{}, _debug_mouse{}, _debug_keyboard{}, _debug_finger{};
    MouseStatus _btn{};
    SDL_Scancode _key{};
    bool _is_focus{};
    SColor _bg_color{StdColor::White}, _font_color{StdColor::Black};
};


#endif //MYENGINE_TEST_MYWINDOW_H
