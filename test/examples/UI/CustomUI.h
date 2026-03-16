
#ifndef MYENGINE_TEST_CUSTOM_UI_H
#define MYENGINE_TEST_CUSTOM_UI_H
#include "MyEngine"
#include "UI/AbstractControl.h"

class CustomControl : public MyEngine::UI::AbstractControl {
public:
    explicit CustomControl(const std::string_view& object_name, MyEngine::Window* window)
        : AbstractControl("CustomControl", object_name, window) { CustomControl::loadEvent(); }
    ~CustomControl() override { CustomControl::unloadEvent(); }


protected:
    void loadEvent() override {
        MyEngine::Logger::log("LoadEvent()");
    }

    void unloadEvent() override {
        MyEngine::Logger::log("UnloadEvent()");
    }

    void enabledEvent() override {
        MyEngine::Logger::log("EnabledEvent()");
    }

    void disabledEvent() override {
        MyEngine::Logger::log("DisabledEvent()");
    }

    void showEvent() override {
        MyEngine::Logger::log("ShowEvent()");
    }

    void hideEvent() override {
        MyEngine::Logger::log("HideEvent()");
    }

    void getFocusEvent() override {
        MyEngine::Logger::log("GetFocusEvent()");
    }

    void lostFocusEvent() override {
        MyEngine::Logger::log("LostFocusEvent()");
    }

    void activeEvent() override {
        MyEngine::Logger::log("ActiveEvent()");
    }

    void inactiveEvent() override {
        MyEngine::Logger::log("InactiveEvent()");
    }

    void moveEvent(const MyEngine::Vector2 &position) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "MoveEvent({})", position);
    }

    void resizeEvent(const MyEngine::Size &size) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "ResizeEvent({})", size);
    }

    void paintEvent() override {
        MyEngine::Logger::log("PaintEvent()");
    }

    void triggeredEvent() override {
        MyEngine::Logger::log("TriggeredEvent()");
    }

    void keyDownEvent(SDL_Scancode scancode) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "KeyDownEvent({})",
            SDL_GetKeyName(SDL_GetKeyFromScancode(scancode, SDL_Keymod(), false)));
    }

    void keyUpEvent(SDL_Scancode scancode) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "KeyUpEvent({})",
            SDL_GetKeyName(SDL_GetKeyFromScancode(scancode, SDL_Keymod(), false)));
    }

    void keyPressedEvent(SDL_Scancode scancode) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "KeyPressedEvent({})",
            SDL_GetKeyName(SDL_GetKeyFromScancode(scancode, SDL_Keymod(), false)));
    }

    void hotkeyDownEvent(size_t index) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "HotkeyDownEvent({})", index);
    }

    void mouseButtonDownEvent(MyEngine::MouseStatus status) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "MouseButtonDownEvent({})",
            MyEngine::EventSystem::mouseStatusName(status));
    }

    void mouseButtonUpEvent(MyEngine::MouseStatus status) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "MouseButtonUpEvent({})",
            MyEngine::EventSystem::mouseStatusName(status));
    }

    void mouseMovedEvent(const MyEngine::Vector2& position, const MyEngine::Vector2& distance) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "MouseMovedEvent(pos = {}, abs_pos = {})", position, distance);
    }

    void mouseMovedInEvent() override {
        MyEngine::Logger::log("MouseMovedInEvent()");
    }

    void mouseMovedOutEvent() override {
        MyEngine::Logger::log("MouseMovedOutEvent()");
    }

    void mouseClicked() override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "MouseClickedEvent()");
    }

    void customContextMenuRequested() override {
        MyEngine::Logger::log("CustomContextMenuRequested()");
    }

    void fingerDownEvent(SDL_FingerID finger_id) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "FingerDownEvent({})", finger_id);
    }

    void fingerMovedEvent(const MyEngine::Vector2& position, const MyEngine::Vector2& distance) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "FingerMovedEvent(pos = {}, abs_pos = {})",
            position, distance);
    }

    void fingerUpEvent(SDL_FingerID finger_id) override {
        MyEngine::Logger::log(MyEngine::Logger::Debug, "FingerUpEvent({})", finger_id);
    }

    void fingerTouchedEvent() override {
        MyEngine::Logger::log("FingerTouchedEvent()");
    }

    void fingerMovedInEvent() override {
        MyEngine::Logger::log("FingerMovedInEvent()");
    }

    void fingerMovedOutEvent() override {
        MyEngine::Logger::log("FingerMovedOutEvent()");
    }
};

#endif //MYENGINE_TEST_CUSTOM_UI_H
