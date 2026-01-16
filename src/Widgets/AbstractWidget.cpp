
#include "AbstractWidget.h"
#include "Algorithm/Collider.h"

namespace MyEngine::Widget {
    AbstractWidget::AbstractWidget(Window *window) : _window(window), _renderer(nullptr),
                _ev_id(IDGenerator::getNewEventID()) {
        load();
    }

    AbstractWidget::AbstractWidget(std::string object_name, MyEngine::Window *window)
        : _window(window), _renderer(nullptr), _object_name(std::move(object_name)),
          _ev_id(IDGenerator::getNewEventID()) {
        load();
    }

    AbstractWidget::~AbstractWidget() {  }

    void AbstractWidget::load() {
        if (!_window) {
            auto err = std::format("AbstractWidget ({}): The specified window can not be null!", _object_name);
            Logger::log(err, Logger::Fatal);
            throw InvalidArgumentException(err);
        }
        _engine = _window->engine();
        _renderer = _window->renderer();
        _renderer->window()->installPaintEvent([this](Renderer* r) {
            if (!_visible) return;
            paintEvent(r);
        }, true);
        _trigger_area.setGeometry(0, 0, 200, 50);
        uint64_t win_id = _window->windowID();
        EventSystem::global()->appendEvent(_ev_id, [this, win_id](SDL_Event ev) {
            if (!_engine->isWindowExist(win_id)) {
                unload();
                return;
            }
            if (!_status.is_loaded) {
                this->loadEvent();
                _status.is_loaded = true;
            }

            if (!_enabled) {
                if (_status.input_mode) {
                    setInputModeEnabled(false);
                }
                return;
            }

            // Hotkey Event
            auto cur_cap_keys = EventSystem::global()->captureKeyboardStatus();
            if (_status.hot_keys && !_hot_key.empty() && !ev.key.repeat) {
                if (!_status.is_hot_key_triggered && (cur_cap_keys == _hot_key)) {
                    _status.is_hot_key_triggered = true;
                    hotKeysPressedEvent();
                } else if (_status.is_hot_key_triggered) {
                    if (cur_cap_keys == _hot_key) {
                        hotKeysPressedEvent();
                    } else {
                        _status.is_hot_key_triggered = false;
                    }
                }
            }
            // Input Event
            if (_status.input_mode) {
                // Set the keys to cope with different events
                if (ev.key.down) {
                    switch (ev.key.key) {
                        case SDLK_RETURN:
                        case SDLK_KP_ENTER:
                        case SDLK_ESCAPE:
                            setInputModeEnabled(false);
                            break;
                        case SDLK_BACKSPACE:
                            keyDownEvent(SDL_SCANCODE_BACKSPACE);
                        case SDLK_LEFT:
                            keyDownEvent(SDL_SCANCODE_LEFT);
                        case SDLK_RIGHT:
                            keyDownEvent(SDL_SCANCODE_RIGHT);
                        case SDLK_DELETE:
                            keyDownEvent(SDL_SCANCODE_DELETE);
                    }
                }

                if (ev.text.type == SDL_EVENT_TEXT_INPUT) {
                    char text[16] = {};
                    strncpy(text, ev.text.text, strlen(ev.text.text));
                    inputEvent(text);
                }
            }
            // Keyboard Event
            if (_focus) {
                if (!_status.key_down) {
                    if (!cur_cap_keys.empty()) {
                        _status.key_down = true;
                        uint64_t scancode = ev.key.scancode;
                        if (scancode) {
                            keyDownEvent(static_cast<SDL_Scancode>(scancode));
                        }
                    }
                } else {
                    if (cur_cap_keys.empty()) {
                        _status.key_down = false;
                        uint64_t scancode = ev.key.scancode;
                        if (scancode) {
                            keyUpEvent(static_cast<SDL_Scancode>(scancode));
                        }
                        keyPressedEvent(static_cast<SDL_Scancode>(scancode));
                    } else if (!ev.key.repeat) {
                        uint64_t scancode = ev.key.scancode;
                        if (scancode) {
                            if (std::find(cur_cap_keys.begin(), cur_cap_keys.end(),
                                          scancode) != cur_cap_keys.end()) {
                                keyDownEvent(static_cast<SDL_Scancode>(scancode));
                            } else {
                                keyUpEvent(static_cast<SDL_Scancode>(scancode));
                            }
                        }
                    }
                }
            }
            if (Cursor::global()->focusOn() != _window->windowID()) return;
            // Mouse Event
            auto cur_pos = EventSystem::global()->captureMousePosition();
            auto trigger = (Algorithm::comparePosInRect(cur_pos, _trigger_area) > 0);
            // Add the input event when the mouse moved out and clicked
            if (_status.input_mode && !trigger && EventSystem::global()->captureMouse(EventSystem::Left)) {
                setInputModeEnabled(false);
            }
            if (!_status.mouse_in) {
                if (EventSystem::global()->captureMouse(EventSystem::None)) {
                    if (trigger) {
                        _status.mouse_in = true;
                        mouseEnteredEvent();
                        Cursor::global()->setCursor(_cur_style);
                    } else if (_status.mouse_down) {
                        _status.mouse_down = false;
                        mouseUpEvent();
                        Cursor::global()->setCursor(_window->cursor());
                    }
                }
            } else {
                auto left_btn = EventSystem::global()->captureMouse(EventSystem::Left);
                auto right_btn = EventSystem::global()->captureMouse(EventSystem::Right);
                if (trigger) {
                    if (left_btn && !_status.mouse_down) {
                        mouseDownEvent();
                        _status.mouse_down = true;
                    } else if (!left_btn && _status.mouse_down) {
                        _status.mouse_down = false;
                        mouseUpEvent();
                        mouseClickedEvent();
                        if (ev.button.clicks > 1) mouseDblClickedEvent();
                    }
                    if (right_btn && !_status.r_mouse_down) {
                        _status.r_mouse_down = true;
                    }
                    if (!right_btn && _status.r_mouse_down) {
                        customContextMenuRequestEvent(cur_pos);
                        _status.r_mouse_down = false;
                    }
                } else {
                    _status.mouse_in = false;
                    mouseLeftEvent();
                    if (!_status.mouse_down) {
                        Cursor::global()->setCursor(_window->cursor());
                    }
                }
            }
            // TouchScreen Event
            auto finger_type = ev.tfinger.type;
            if (!_status.finger_down) {
                if (trigger) {
                    auto w = static_cast<float>(_window->geometry().width);
                    auto h = static_cast<float>(_window->geometry().height);
                    Vector2 pos(ev.tfinger.x * w, ev.tfinger.y * h);
                    if (finger_type == SDL_EVENT_FINGER_DOWN) {
                        _status.finger_down = true;
                        _status.finger_down_pos.reset(pos);
                        _status.finger_id = ev.tfinger.fingerID;
                        fingerDownEvent(pos);
                    } else if (finger_type == SDL_EVENT_FINGER_MOTION) {
                        _status.finger_down = true;
                        _status.finger_down_pos.reset(pos);
                        _status.finger_id = ev.tfinger.fingerID;
                        _status.finger_move_in = true;
                        fingerMoveInEvent();
                    }
                }
            } else {
                auto w = static_cast<float>(_window->geometry().width);
                auto h = static_cast<float>(_window->geometry().height);
                Vector2 pos(ev.tfinger.x * w, ev.tfinger.y * h);
                bool fin_tri = (Algorithm::comparePosInRect(pos, _trigger_area) > 0);
                if (fin_tri && finger_type == SDL_EVENT_FINGER_DOWN) {
                    _status.finger_id = ev.tfinger.fingerID;
                    fingerUpEvent(pos);
                    fingerDownEvent(pos);
                    _status.finger_down_pos.reset(pos);
                } else if ((_status.finger_id == ev.tfinger.fingerID) && (finger_type == SDL_EVENT_FINGER_UP)) {
                    _status.finger_down = false;
                    _status.finger_move_out = false;
                    fingerUpEvent(pos);
                    if (trigger && !_status.finger_move_in) fingerTappedEvent();
                    _status.finger_move_in = false;
                } else if ((_status.finger_id == ev.tfinger.fingerID) && (finger_type == SDL_EVENT_FINGER_MOTION)) {
                    fingerMovedEvent(pos, pos - _status.finger_down_pos);
                    if (!_status.finger_move_out && !fin_tri) {
                        _status.finger_move_out = true;
                        fingerMoveOutEvent();
                    } else if (_status.finger_move_out && fin_tri) {
                        _status.finger_move_out = false;
                        fingerMoveInEvent();
                    }
                }
            }
        });
    }

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

    bool AbstractWidget::isFocusEnabled() const {
        return _focus;
    }

    bool AbstractWidget::isHovered() const {
        return _status.mouse_in;
    }

    void AbstractWidget::setInputModeEnabled(bool enabled) {
        if (enabled) {
            if (SDL_StartTextInput(_window->self())) {
                _status.input_mode = true;
                startedInputEvent();
            }
        } else {
            if (SDL_StopTextInput(_window->self())) {
                _status.input_mode = false;
                endedInputEvent();
            }
        }
    }

    bool AbstractWidget::isInputModeEnabled() const {
        return _status.input_mode;
    }

    const std::string& AbstractWidget::getInputChar() const {
        return _cur_ch;
    }

    void AbstractWidget::setCursor(Cursor::StdCursor cursor_style) {
        _cur_style = cursor_style;
    }

    Cursor::StdCursor AbstractWidget::cursor() const {
        return _cur_style;
    }

    void AbstractWidget::setHotKeyEnabled(bool enabled) {
        _status.hot_keys = enabled;
    }

    bool AbstractWidget::hotKeyEnabled() const {
        return _status.hot_keys;
    }

    void AbstractWidget::setProperty(const std::string& name, bool value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }
    
    void AbstractWidget::setProperty(const std::string& name, int8_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, int16_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, int32_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, int64_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, uint8_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, uint16_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, uint32_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, uint64_t value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, float value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, double value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, const char* value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, const std::string& value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value.c_str());
        } else {
            _prop_map.try_emplace(name, value.c_str());
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, std::string&& value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, void* value) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
        } else {
            _prop_map.try_emplace(name, value);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name, void* value, std::function<void(void*)> deleter) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).setValue(value);
            _prop_map.at(name).setDeleter(std::move(deleter));
        } else {
            _prop_map.try_emplace(name, value, std::move(deleter));
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::setProperty(const std::string& name) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).clearValue();
        } else {
            _prop_map.try_emplace(name);
        }
        propertyChanged(name, _prop_map.at(name));
    }

    void AbstractWidget::eraseProperty(const std::string& name) {
        if (_prop_map.contains(name)) {
            _prop_map.at(name).clearValue();
            _prop_map.erase(name);
        }
    }

    void AbstractWidget::clearProperties() {
        for (auto& [name, value] : _prop_map) {
            value.clearValue();
        }
        _prop_map.clear();
    }

    bool AbstractWidget::hasProperty(const std::string &name) const {
        return _prop_map.contains(name);
    }
    
    const Variant *const AbstractWidget::property(const std::string& name) const {
        if (_prop_map.contains(name)) {
            return &_prop_map.at(name);
        } else {
            auto err = std::format("AbstractWidget ({}): Property '{}' is not found!", _object_name, name);
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
        }
    }

    void AbstractWidget::loadEvent() {}

    void AbstractWidget::unloadEvent() {
        clearProperties();
    }

    void AbstractWidget::moveEvent(const Vector2 &position) {}

    void AbstractWidget::resizeEvent(const Size &size) {}

    void AbstractWidget::enableChangedEvent(bool enabled) {}

    void AbstractWidget::visibleChangedEvent(bool visible) {}

    void AbstractWidget::objectNameChangedEvent(const std::string &object_name) {}

    void AbstractWidget::paintEvent(Renderer *renderer) {}

    void AbstractWidget::focusInEvent() {}

    void AbstractWidget::focusOutEvent() {}

    void AbstractWidget::mouseClickedEvent() {}

    void AbstractWidget::mouseDblClickedEvent() {}

    void AbstractWidget::mouseDownEvent() {}

    void AbstractWidget::mouseUpEvent() {}

    void AbstractWidget::mouseEnteredEvent() {}

    void AbstractWidget::mouseLeftEvent() {}

    void AbstractWidget::customContextMenuRequestEvent(const Vector2 &position) {}

    void AbstractWidget::keyDownEvent(SDL_Scancode scancode) {}

    void AbstractWidget::keyUpEvent(SDL_Scancode scancode) {}

    void AbstractWidget::keyPressedEvent(SDL_Scancode scancode) {}

    void AbstractWidget::hotKeysPressedEvent() {}

    void AbstractWidget::fingerDownEvent(const Vector2& position) {}

    void AbstractWidget::fingerUpEvent(const Vector2& position) {}

    void AbstractWidget::fingerMovedEvent(const Vector2 &position, const Vector2 &distance) {}

    void AbstractWidget::fingerMoveInEvent() {}

    void AbstractWidget::fingerMoveOutEvent() {}

    void AbstractWidget::fingerTappedEvent() {}

    void AbstractWidget::startedInputEvent() {}

    void AbstractWidget::endedInputEvent() {}

    void AbstractWidget::inputEvent(const char *string) {
        _cur_ch = string;
    }

    void AbstractWidget::propertyChanged(const std::string &property, const Variant &variant) {}
}
