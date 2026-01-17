
#ifndef MYENGINE_MYWINDOW_H
#define MYENGINE_MYWINDOW_H
#include "MyEngine"

class MyWindow : public MyEngine::Window {
public:
    explicit MyWindow(MyEngine::Engine* engine, std::string title, int w = 800, int h = 600)
            : MyEngine::Window(engine, std::move(title), w, h) {}

protected:
    void mouseDownEvent(int button) override {
        MyEngine::Logger::log("Mouse down");
    }

    void mouseUpEvent() override {
        MyEngine::Logger::log("Mouse up");
    }

    void mouseMovedEvent(const MyEngine::Vector2 & pos) override {
        MyEngine::Logger::log("Mouse moving");
    }

    void mouseClickedEvent(int button) override {
        MyEngine::Logger::log(std::format("Mouse clicked: {}", button));
    }


private:
};

#endif //MYENGINE_MYWINDOW_H
