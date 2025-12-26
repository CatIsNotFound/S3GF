
#include "AbstractWidget.h"
#include "Algorithm/Collider.h"

namespace MyEngine::Widget {
    AbstractWidget::AbstractWidget(Window *window) : _window(window), _renderer(nullptr),
                _ev_id(IDGenerator::getNewEventID()) {
        if (!_window) {
            Logger::log("AbstractWidget: The specified window can not be null!", Logger::Fatal);
            throw InvalidArgumentException("AbstractWidget: The specified renderer can not be null!");
        }
        _engine = _window->engine();
        _renderer = _window->renderer();
        _renderer->window()->installPaintEvent([this](Renderer* r) {
            if (!_visible) return;
            paintEvent(r);
        });
        EventSystem::global()->appendEvent(_ev_id, [this](SDL_Event ev) {
            static uint64_t win_id = _window->windowID();
            if (!_engine->isWindowExist(win_id)) this->unload();
            static bool is_loaded = false;
            if (!is_loaded) {
                this->loadEvent();
                is_loaded = true;
            }
            if (!_enabled) return;

            // Hotkey Event
            auto cur_cap_keys = EventSystem::global()->captureKeyboardStatus();
            if (!_hot_key.empty()) {
                static bool is_hot_key_triggered = false;
                if (!is_hot_key_triggered && (cur_cap_keys == _hot_key)) {
                    is_hot_key_triggered = true;
                    hotKeysPressedDownEvent();
                } else if (cur_cap_keys.empty()) {
                    is_hot_key_triggered = false;
                }
            }
            // Keyboard Event
            if (_focus) {
                static bool key_down = false;
                static size_t key_pressed_num = 0;
                static std::vector<int> pressed_stack;
                if (!key_down) {
                    if (!cur_cap_keys.empty()) {
                        key_down = true;
                        key_pressed_num = cur_cap_keys.size();
                        // keyDownEvent(cur_cap_keys);
                        // pressed_stack.assign(cur_cap_keys.begin(), cur_cap_keys.end());
                    }
                } else {
                    if (cur_cap_keys.empty()) {
                        key_down = false;
                        // keyUpEvent(cur_cap_keys);
                        // keyPressedEvent(cur_cap_keys);
                    } else if (key_pressed_num < cur_cap_keys.size()) {
                        // keyDownEvent(cur_cap_keys);
                        key_pressed_num = cur_cap_keys.size();
                    } else if (key_pressed_num > cur_cap_keys.size()) {

                    }
                }
            }
            if (Cursor::global()->focusOn() != _window->windowID()) return;
            // Mouse Event
            auto cur_pos = EventSystem::global()->captureMousePosition();
            auto cur_abs_dis = EventSystem::global()->captureMouseAbsDistance();
            static bool mouse_in = false, mouse_down = false;
            auto trigger = (Algorithm::comparePosInRect(cur_pos, _trigger_area) > 0);
            if (!mouse_in) {
                if (EventSystem::global()->captureMouse(EventSystem::None)) {
                    if (trigger) {
                        mouse_in = true;
                        mouseEnteredEvent();
                        Cursor::global()->setCursor(_cur_style);
                    } else if (mouse_down) {
                        mouse_down = false;
                        mouseUpEvent();
                        Cursor::global()->setCursor(_window->cursor());
                    }
                }
            } else {
                auto left_btn = EventSystem::global()->captureMouse(EventSystem::Left);
                auto right_btn = EventSystem::global()->captureMouse(EventSystem::Right);
                if (trigger) {
                    if (left_btn && !mouse_down) {
                        mouseDownEvent();
                        mouse_down = true;
                    } else if (!left_btn && mouse_down) {
                        mouse_down = false;
                        mouseUpEvent();
                        mouseClickedEvent();
                        if (ev.button.clicks > 1) mouseDblClickedEvent();
                    }
                    if (right_btn && mouse_down) {
                        customContextMenuRequestEvent();
                    }
                } else {
                    mouse_in = false;
                    mouseLeftEvent();
                    if (!mouse_down) {
                        Cursor::global()->setCursor(_window->cursor());
                    }
                }
            }
        });
    }

    AbstractWidget::~AbstractWidget() {}

    void AbstractWidget::unload() {
        unloadEvent();
        EventSystem::global()->removeEvent(_ev_id);
    }

    void AbstractWidget::setObjectName(std::string object_name) {
        _object_name = std::move(object_name);
        objectNameChangedEvent(_object_name);
    }

    const std::string &AbstractWidget::objectName() const {
        return _object_name;
    }

    void AbstractWidget::setVisible(bool visible) {
        _visible = visible;
        visibleChangedEvent(visible);
    }

    bool AbstractWidget::visible() const {
        return _visible;
    }

    void AbstractWidget::setEnabled(bool enabled) {
        _enabled = enabled;
        enableChangedEvent(enabled);
    }

    bool AbstractWidget::enabled() const {
        return _enabled;
    }

    void AbstractWidget::setGeometry(float x, float y, float w, float h) {
        _trigger_area.setGeometry(x, y, w, h);
        moveEvent(_trigger_area.geometry().pos);
        resizeEvent(_trigger_area.geometry().size);
    }

    void AbstractWidget::setGeometry(const Vector2 &position, const Size &size) {
        _trigger_area.setGeometry(position.x, position.y, size.width, size.height);
        moveEvent(_trigger_area.geometry().pos);
        resizeEvent(_trigger_area.geometry().size);
    }

    void AbstractWidget::setGeometry(const GeometryF &geometry) {
        _trigger_area.setGeometry(geometry);
        moveEvent(_trigger_area.geometry().pos);
        resizeEvent(_trigger_area.geometry().size);
    }

    const GeometryF &AbstractWidget::geometry() const {
        return _trigger_area.geometry();
    }

    void AbstractWidget::move(float x, float y) {
        _trigger_area.move(x, y);
        moveEvent(_trigger_area.geometry().pos);
    }

    void AbstractWidget::move(const Vector2 &position) {
        _trigger_area.move(position);
        moveEvent(_trigger_area.geometry().pos);
    }

    const Vector2 &AbstractWidget::position() const {
        return _trigger_area.geometry().pos;
    }

    void AbstractWidget::resize(float w, float h) {
        _trigger_area.resize(w, h);
        resizeEvent(_trigger_area.geometry().size);
    }

    void AbstractWidget::resize(const Size &size) {
        _trigger_area.resize(size);
        resizeEvent(_trigger_area.geometry().size);
    }

    const Size &AbstractWidget::size() const {
        return _trigger_area.geometry().size;
    }

    void AbstractWidget::setFocusEnabled(bool enabled) {
        _focus = enabled;
        if (_focus) focusInEvent(); else focusOutEvent();
    }

    bool AbstractWidget::focusEnabled() const {
        return _focus;
    }

    void AbstractWidget::setCursor(Cursor::StdCursor cursor_style) {
        _cur_style = cursor_style;
    }

    Cursor::StdCursor AbstractWidget::cursor() const {
        return _cur_style;
    }


    void AbstractWidget::loadEvent() {}

    void AbstractWidget::unloadEvent() {}

    void AbstractWidget::moveEvent(const Vector2 &position) {}

    void AbstractWidget::resizeEvent(const Size &size) {}

    void AbstractWidget::enableChangedEvent(bool enabled) {}

    void AbstractWidget::visibleChangedEvent(bool visible) {}

    void AbstractWidget::objectNameChangedEvent(const std::string &object_name) {}

    void AbstractWidget::paintEvent(Renderer *renderer) {}

    void AbstractWidget::enterEvent() {}

    void AbstractWidget::leaveEvent() {}

    void AbstractWidget::focusInEvent() {}

    void AbstractWidget::focusOutEvent() {}

    void AbstractWidget::dragEvent() {}

    void AbstractWidget::dragInEvent() {}

    void AbstractWidget::dragOutEvent() {}

    void AbstractWidget::dragMovedEvent() {}

    void AbstractWidget::dropEvent() {}

    void AbstractWidget::mouseClickedEvent() { Logger::log("Mouse clicked", Logger::Info); }

    void AbstractWidget::mouseDblClickedEvent() { Logger::log("Mouse double clicked", Logger::Info); }

    void AbstractWidget::mouseDownEvent() { Logger::log("Mouse down", Logger::Info); }

    void AbstractWidget::mouseUpEvent() { Logger::log("Mouse Up", Logger::Info); }

    void AbstractWidget::mouseEnteredEvent() { Logger::log("Mouse entered", Logger::Info); }

    void AbstractWidget::mouseLeftEvent() { Logger::log("Mouse Left", Logger::Info); }

    void AbstractWidget::customContextMenuRequestEvent() { Logger::log("Custom context menu.", Logger::Info); }

    void AbstractWidget::keyDownEvent(const std::vector<int>& key_scancode) { Logger::log(std::format("Key down! Count: {}", key_scancode.size()), Logger::Info); }

    void AbstractWidget::keyUpEvent(const std::vector<int>& key_scancode) { Logger::log(std::format("Key up! Size: {}", key_scancode.size()), Logger::Info); }

    void AbstractWidget::keyPressedEvent(const std::vector<int>& key_scancode) { Logger::log("Key pressed", Logger::Info); }

    void AbstractWidget::hotKeysPressedDownEvent() { Logger::log("Hot key is triggered.", Logger::Info); }

    void AbstractWidget::FingerDownEvent() {}

    void AbstractWidget::FingerUpEvent() {}

    void AbstractWidget::FingerTappedEvent() {}

    void AbstractWidget::startedInputEvent() {}

    void AbstractWidget::endedInputEvent() {}


}
