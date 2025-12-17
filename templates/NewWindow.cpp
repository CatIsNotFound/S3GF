
#include "NewWindow.h"

NewWindow::NewWindow(MyEngine::Engine *engine, const std::string &title, int width, int height)
        : MyEngine::Window(engine, title, width, height) {
    /// TODO: Add some codes here.


}

NewWindow::~NewWindow() = default;

void NewWindow::paintEvent() {
    MyEngine::Window::paintEvent();
    /// TODO: Add some codes here (how to render the window?)


}

void NewWindow::resizeEvent() {
    MyEngine::Window::resizeEvent();
    /// TODO: Add some codes here (When the window size is changed)

}

void NewWindow::moveEvent() {
    MyEngine::Window::moveEvent();
    /// TODO: Add some codes here (When the window is moved)

}

void NewWindow::getFocusEvent() {
    MyEngine::Window::getFocusEvent();
    /// TODO: Add some codes here (When the window gets focus)

}

void NewWindow::lostFocusEvent() {
    MyEngine::Window::lostFocusEvent();
    /// TODO: Add some codes here (When the window lost focus)

}

void NewWindow::unloadEvent() {
    /// Note: Don't remove this line, the window will not be unload safely when user want to close this window.
    MyEngine::Window::unloadEvent();
    /// TODO: Add some codes here (When the window closed)

}

/// TODO: Add more functions to implement your custom window.


