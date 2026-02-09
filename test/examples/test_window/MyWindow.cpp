#include "MyWindow.h"

MyWindow::MyWindow(MyEngine::Engine *engine, const std::string &title, int width, int height)
        : MyEngine::Window(engine, title, width, height) {
    _debug = "Loaded";
    _debug_mouse = "No buttons pressed down";
    _debug_keyboard = "No keys pressed down";
}

MyWindow::~MyWindow() = default;

void MyWindow::paintEvent() {
    Window::paintEvent();
    renderer()->fillBackground(_bg_color);
    renderer()->drawDebugText(_debug, {20, 20}, _font_color);
    renderer()->drawDebugText(_debug_keyboard, {20, 30}, _font_color);
    renderer()->drawDebugText(_debug_mouse, {20, 40}, _font_color);
    renderer()->drawDebugText(FMT::format("Fingers count: {}", getFingersCount()), {20, 50}, _font_color);
    renderer()->drawDebugText(_debug_finger, {20, 60}, _font_color);
}

void MyWindow::resizeEvent() {
    Window::resizeEvent();
    _debug = FMT::format("Resized window: {}x{}", this->geometry().width, this->geometry().height);
}

void MyWindow::moveEvent() {
    Window::moveEvent();
    _debug = FMT::format("Moved window: ({}, {})", this->geometry().x, this->geometry().y);
}

void MyWindow::getFocusEvent() {
    Window::getFocusEvent();
    _bg_color = StdColor::LightBlue;
    _font_color = StdColor::Black;
    _is_focus = true;
}

void MyWindow::lostFocusEvent() {
    Window::lostFocusEvent();
    _bg_color = StdColor::Gray;
    _font_color = StdColor::White;
    _is_focus = false;
}

void MyWindow::unloadEvent() {
    Window::unloadEvent();
    Logger::log("Unloaded window", Logger::Info);
}

void MyWindow::showEvent() {
    Window::showEvent();
}

void MyWindow::hideEvent() {
    Window::hideEvent();
}

void MyWindow::windowMinimizedEvent() {
    Window::windowMinimizedEvent();
    Logger::log("Windows is minimized!", Logger::Info);
}

void MyWindow::windowMaximizedEvent() {
    Window::windowMaximizedEvent();
    _debug = "Maximized window";
    Logger::log("Windows is maximized!", Logger::Info);
}

void MyWindow::windowRestoredEvent() {
    Window::windowRestoredEvent();
    _debug = "Restored window";
    Logger::log("Windows is restored!", Logger::Info);
}

void MyWindow::enteredFullscreenEvent() {
    Window::enteredFullscreenEvent();
    Logger::log("Windows is entered fullscreen!", Logger::Info);
}

void MyWindow::leaveFullscreenEvent() {
    Window::leaveFullscreenEvent();
    Logger::log("Windows is left fullscreen!", Logger::Info);
}

void MyWindow::mouseEnteredEvent() {
    Window::mouseEnteredEvent();
    Logger::log("Mouse is moved in the window!", Logger::Info);
}

void MyWindow::mouseLeftEvent() {
    Window::mouseLeftEvent();
    Logger::log("Mouse is moved out from window!", Logger::Info);
}

void MyWindow::mouseUpEvent() {
    Window::mouseUpEvent();
    _btn = static_cast<MouseStatus>(EventSystem::global()->captureMouseStatus());
    if (_btn != MyEngine::MouseStatus::None) {
        _debug_mouse = FMT::format("Mouse is pressing down: {}", mouseButtonString(_btn));
    } else {
        _debug_mouse = FMT::format("No mouse pressed down");
    }
}

void MyWindow::mouseDownEvent(MyEngine::MouseStatus button) {
    Window::mouseDownEvent(button);
    _btn = static_cast<MouseStatus>(EventSystem::global()->captureMouseStatus());
    _debug_mouse = FMT::format("Mouse is pressing down: {}", mouseButtonString(button));
}

void MyWindow::mouseClickedEvent(MyEngine::MouseStatus button) {
    Window::mouseClickedEvent(button);
    Logger::log(Logger::Info, "Mouse clicked: {}", mouseButtonString(button));
}

void MyWindow::mouseMovedEvent(const Vector2 &position, const Vector2 &distance) {
    Window::mouseMovedEvent(position, distance);
    _debug_mouse = FMT::format("Mouse moved: position {}, distance {}",
                               position, distance);
}

void MyWindow::keyUpEvent(SDL_Scancode keycode) {
    Window::keyUpEvent(keycode);
    _debug_keyboard = FMT::format("Key up: {}", static_cast<int>(keycode));
}

void MyWindow::keyDownEvent(SDL_Scancode keycode) {
    Window::keyDownEvent(keycode);
    _debug_keyboard = FMT::format("Key down: {}", static_cast<int>(keycode));
    if (keycode == SDL_SCANCODE_RETURN) {
        this->setFullScreen(!fullScreen());
    }
}

void MyWindow::keyPressedEvent(SDL_Scancode keycode) {
    Window::keyPressedEvent(keycode);
    Logger::log(Logger::Info, "Key clicked: {}", static_cast<int>(keycode));
}

void MyWindow::fingerDownEvent(SDL_FingerID id, const MyEngine::Vector2 &position) {
    Window::fingerDownEvent(id, position);
    _debug_finger = FMT::format("Finger down: FID: {}, POS: {}", id, position);
}

void MyWindow::fingerUpEvent(SDL_FingerID id, const MyEngine::Vector2 &position) {
    _debug_finger = "No fingers down!";
}

void MyWindow::fingerMovedEvent(SDL_FingerID id, const MyEngine::Vector2 &position, const MyEngine::Vector2 &distance) {
    _debug_finger = FMT::format("Finger Moved {}: FID: {}, POS: {}, DIS: {}",
                                this->getFingerEventByID(id).value().is_in_window ? "in" : "out",
                                id, position, distance);

}

void MyWindow::fingerMoveOutEvent(SDL_FingerID id) {
    Logger::log(Logger::Info, "Finger Moved out: FID: {}", id);
}

void MyWindow::fingerMoveInEvent(SDL_FingerID id) {
    Logger::log(Logger::Info, "Finger Moved in: FID: {}", id);
}

void MyWindow::fingerTappedEvent(SDL_FingerID id, const MyEngine::Vector2 &position) {
    Logger::log(Logger::Info, "Finger Tapped: FID: {}, POS: {}", id, position);
}

void MyWindow::dragInEvent() {
    Window::dragInEvent();
    _bg_color = StdColor::DarkBlue;
    Logger::log(Logger::Info, "Dragged in");
}

void MyWindow::dragOutEvent() {
    Window::dragOutEvent();
    Logger::log(Logger::Info, "Dragged out");
}

void MyWindow::dragMovedEvent(const Vector2 &position, const char *data) {
    Window::dragMovedEvent(position, data);
    _debug_mouse = FMT::format("Dragging: pos {}, data ({})", position, data ? data : "Unknown");
}

void MyWindow::dropEvent(const char *url) {
    Window::dropEvent(url);
    if (_is_focus) {
        _bg_color = StdColor::LightBlue;
        _font_color = StdColor::Black;
    } else {
        _bg_color = StdColor::Gray;
        _font_color = StdColor::White;
    }
    _debug_mouse = FMT::format("Dropped: {}", url);
}

std::string_view MyWindow::mouseButtonString(MouseStatus status) {
    switch (status) {
        case MouseStatus::None:
            return "None";
        case MouseStatus::Left:
            return "Left";
        case MouseStatus::Middle:
            return "Middle";
        case MouseStatus::LeftMiddle:
            return "Left Middle";
        case MouseStatus::Right:
            return "Right";
        case MouseStatus::LeftRight:
            return "Left Right";
        case MouseStatus::MiddleRight:
            return "Middle Right";
        case MouseStatus::LeftMiddleRight:
            return "Left Middle Right";
    }
    return "";
}
