
#ifndef MYENGINE_TEST_MY_TRIGGER_AREA_H
#define MYENGINE_TEST_MY_TRIGGER_AREA_H
#include "MyEngine"

using namespace MyEngine;

class MyTriggerArea : public TriggerArea {
public:
    explicit MyTriggerArea(const GeometryF& geo, Window* window) : TriggerArea(geo, window) {}
    ~MyTriggerArea() = default;

protected:
    void mouseDownEvent(MouseStatus button) override {
        Logger::log(Logger::Info, "Mouse down: {}", EventSystem::mouseStatusName(button));
    }

    void mouseUpEvent(MouseStatus button) override {
        Logger::log(Logger::Info, "Mouse up: {}", EventSystem::mouseStatusName(button));
    }

    void mouseMovedEvent(const Vector2& pos, const Vector2& dis) override {
        Logger::log(Logger::Info, "Mouse move: {}, {}", pos, dis);
    }

    void mouseMovedInEvent() override {
        Logger::log(Logger::Info, "Mouse moved in");
    }

    void mouseMovedOutEvent() override {
        Logger::log(Logger::Info, "Mouse moved out");
    }

    void mouseClickedEvent(MouseStatus button) override {
        Logger::log(Logger::Info, "Mouse clicked: {}", EventSystem::mouseStatusName(button));
    }

    void keyPressedEvent(SDL_Scancode keycode) override {
        Logger::log(Logger::Info, "Key pressed: {}", static_cast<int>(keycode));
    }

    void keyDownEvent(SDL_Scancode keycode) override {
        Logger::log(Logger::Info, "key down: {}", static_cast<int>(keycode));
    }

    void keyUpEvent(SDL_Scancode keycode) override {
        Logger::log(Logger::Info, "key up: {}", static_cast<int>(keycode));
    }

    void fingerDownEvent(SDL_FingerID id) override {
        Logger::log(Logger::Info, "finger down: {}", id);
    }

    void fingerUpEvent(SDL_FingerID id) override {
        Logger::log(Logger::Info, "finger up: {}", id);
    }

    void fingerTouchedEvent(SDL_FingerID id) override {
        Logger::log(Logger::Info, "finger touched: {}", id);
    }

    void fingerMovedEvent(SDL_FingerID id, const Vector2& pos, const Vector2& dis) override {
        auto pre = window()->getFingerEventByID(id).value().pressure;
        Logger::log(Logger::Info, "finger moved: {} {} {} {:.2f}", id, pos, dis, pre);
    }

    void fingerMovedInEvent() override {
        Logger::log(Logger::Info, "finger moved in");
    }

    void fingerMovedOutEvent() override {
        Logger::log(Logger::Info, "finger moved out");
    }

};


#endif //MYENGINE_TEST_MY_TRIGGER_AREA_H
