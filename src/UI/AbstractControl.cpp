
#include "AbstractControl.h"
#include "Algorithm/All.h"

MyEngine::UI::AbstractControl::AbstractControl(const std::string_view &class_name, Window *parent)
        : _window(parent), _event_id(), _class_name(class_name) {
    if (!_window) {
        Logger::log(Logger::Fatal, "{}: The specified window is not valid!", class_name);
        Engine::throwCustomFatalError<InvalidArgumentException>();
    }
    registerEvent();
}

MyEngine::UI::AbstractControl::AbstractControl(const std::string_view &class_name,
                                               const std::string_view &object_name, Window *parent)
        : _window(parent), _event_id(), _object_name(object_name), _class_name(class_name) {
    if (!_window) {
        Logger::log(Logger::Fatal, "{}: The specified window is not valid!", class_name);
        Engine::throwCustomFatalError<InvalidArgumentException>();
    }
    registerEvent();
}

MyEngine::UI::AbstractControl::~AbstractControl() {
    if (_is_init && _status.test(Status_Loaded)) unload();
}

void MyEngine::UI::AbstractControl::setObjectName(const std::string_view &object_name) {
    _object_name = object_name;
}

const std::string & MyEngine::UI::AbstractControl::objectName() const {
    return _object_name;
}

void MyEngine::UI::AbstractControl::setLayerName(const std::string_view &layer_name) {
    _layer_name = layer_name;
}

const std::string & MyEngine::UI::AbstractControl::layerName() const {
    return _layer_name;
}

void MyEngine::UI::AbstractControl::setGeometry(float x, float y, float width, float height) {
    if (isLocked()) return;
    _geometry.setGeometry(x, y, width, height);
    moveEvent(_geometry.pos);
    resizeEvent(_geometry.size);
}

void MyEngine::UI::AbstractControl::setGeometry(const Vector2 &position, const Size &size) {
    if (isLocked()) return;
    _geometry.setGeometry(position, size);
    moveEvent(_geometry.pos);
    resizeEvent(_geometry.size);
}

void MyEngine::UI::AbstractControl::setGeometry(const GeometryF &geometry) {
    if (isLocked()) return;
    _geometry.setGeometry(geometry);
    moveEvent(_geometry.pos);
    resizeEvent(_geometry.size);
}

void MyEngine::UI::AbstractControl::move(float x, float y) {
    if (isLocked()) return;
    _geometry.move(x, y);
    moveEvent(_geometry.pos);
}

void MyEngine::UI::AbstractControl::move(const Vector2 &position) {
    if (isLocked()) return;
    _geometry.move(position);
    moveEvent(_geometry.pos);
}

void MyEngine::UI::AbstractControl::resize(float width, float height) {
    if (isLocked()) return;
    _geometry.resize(width, height);
    resizeEvent(_geometry.size);
}

void MyEngine::UI::AbstractControl::resize(const Size &size) {
    if (isLocked()) return;
    _geometry.resize(size);
    resizeEvent(_geometry.size);
}

const MyEngine::GeometryF & MyEngine::UI::AbstractControl::geometry() const {
    return _geometry;
}

const MyEngine::Vector2 & MyEngine::UI::AbstractControl::position() const {
    return _geometry.pos;
}

const MyEngine::Size & MyEngine::UI::AbstractControl::size() const {
    return _geometry.size;
}

void MyEngine::UI::AbstractControl::setEnabled(bool enable) {
    if (isLocked()) return;
    _status.set(Status_Enabled, enable);
    if (enable) enabledEvent(); else disabledEvent();
}

bool MyEngine::UI::AbstractControl::isEnabled() const {
    return _status.test(Status_Enabled);
}

void MyEngine::UI::AbstractControl::setVisible(bool visible) {
    if (isLocked()) return;
    _status.set(Status_Visible, visible);
    if (visible) showEvent(); else hideEvent();
}

bool MyEngine::UI::AbstractControl::isVisible() const {
    return _status.test(Status_Visible);
}

void MyEngine::UI::AbstractControl::setFocus(bool focus) {
    if (isLocked()) return;
    _status.set(Status_Focus, focus);
}

bool MyEngine::UI::AbstractControl::isFocus() const {
    return _status.test(Status_Focus);
}

void MyEngine::UI::AbstractControl::setActive(bool active) {
    if (isLocked()) return;
    _status.set(Status_Active, active);
}

bool MyEngine::UI::AbstractControl::isActive() const {
    return _status.test(Status_Active);
}

void MyEngine::UI::AbstractControl::setLockedEnabled(bool locked) {
    _status.set(Status_Lock, locked);
}

bool MyEngine::UI::AbstractControl::isLocked() const {
    return _status.test(Status_Lock);
}

void MyEngine::UI::AbstractControl::setCursor(const Cursor::StdCursor &cursor) {
    if (isLocked()) return;
    _cursor = cursor;
}

MyEngine::Cursor::StdCursor MyEngine::UI::AbstractControl::cursor() const {
    return _cursor;
}

void MyEngine::UI::AbstractControl::clearHotkeys(size_t index) {
    if (isLocked()) return;
    if (index > _hotkeys_list.size()) {
        Logger::log(Logger::Fatal, "{}({}): The specified index is out of range!",
                _class_name, _object_name);
        Engine::throwCustomFatalError<OutOfRangeException>();
    }
    _hotkeys_list.at(index).clear();
}

void MyEngine::UI::AbstractControl::clearHotkeys() {
    if (isLocked()) return;
    _hotkeys_list.clear();
}

void MyEngine::UI::AbstractControl::removeHotkey(size_t index) {
    if (isLocked()) return;
    if (index > _hotkeys_list.size()) {
        Logger::log(Logger::Fatal, "{}({}): The specified index is out of range!",
                _class_name, _object_name);
        Engine::throwCustomFatalError<OutOfRangeException>();
    }
    _hotkeys_list.erase(_hotkeys_list.begin() + index);
}

const std::vector<SDL_Scancode> & MyEngine::UI::AbstractControl::hotkeys(size_t index) const {
    if (index > _hotkeys_list.size()) {
        Logger::log(Logger::Fatal, "{}({}): The specified index is out of range!",
                _class_name, _object_name);
        Engine::throwCustomFatalError<OutOfRangeException>();
    }
    return _hotkeys_list.at(index);
}

void MyEngine::UI::AbstractControl::setRightButtonDefault(bool enabled) {
    if (isLocked()) return;
    _status.set(Status_DefaultRightButton, enabled);
}

bool MyEngine::UI::AbstractControl::isRightButtonDefault() const {
    return _status.test(Status_DefaultRightButton);
}

bool MyEngine::UI::AbstractControl::isLoaded() const {
    return _status.test(Status_Loaded);
}

void MyEngine::UI::AbstractControl::unload() {
    _status.reset(Status_Loaded);
    unloadEvent();
}

void MyEngine::UI::AbstractControl::setProperty(const std::string_view &property_name, const Variant &value) {
    if (_prop_map.contains(property_name.data())) {
        _prop_map.at(property_name.data()) = value;
    } else {
        _prop_map.emplace(property_name, value);
    }
}

const MyEngine::Variant& MyEngine::UI::AbstractControl::property(const std::string_view &property_name) const {
    if (_prop_map.contains(property_name.data())) {
        return _prop_map.at(property_name.data());
    }
    Logger::log(Logger::Fatal, "{}({}): The specified property name '{}' is not exist!",
                _class_name, _object_name, property_name.data());
    Engine::throwCustomFatalError<InvalidArgumentException>();
}

void MyEngine::UI::AbstractControl::removeProperty(const std::string_view &property_name) {
    if (_prop_map.contains(property_name.data())) {
        _prop_map.erase(property_name.data());
    }
}

void MyEngine::UI::AbstractControl::clearProperty() {
    _prop_map.clear();
}

bool MyEngine::UI::AbstractControl::hasProperty(const std::string_view &property_name) const {
    return _prop_map.contains(property_name.data());
}

void MyEngine::UI::AbstractControl::setStatus(size_t id, bool on) {
    _status.set(id, on);
}

bool MyEngine::UI::AbstractControl::status(size_t id) {
    return _status.test(id);
}

void MyEngine::UI::AbstractControl::setClassName(const std::string_view &class_name) {
    _class_name = class_name;
}

const std::string &MyEngine::UI::AbstractControl::className() const {
    return _class_name;
}

MyEngine::Window *MyEngine::UI::AbstractControl::window() const {
    return _window;
}

void MyEngine::UI::AbstractControl::registerEvent() {
    _status.set(Status_Loaded);
    _status.set(Status_Visible);
    _status.set(Status_Enabled);
    _is_init = true;
    _event_id = EventSystem::global()->appendEvent([&] (SDL_Event event) {
        if (!_is_init || !isEnabled() || !isVisible()) return;
        // Mouse Event
        if (SDL_HasMouse()) {
            auto default_button = isRightButtonDefault() ? MouseStatus::Right : MouseStatus::Left;
            auto default_cxt_button = isRightButtonDefault() ? MouseStatus::Left : MouseStatus::Right;
            auto is_mouse_down = EventSystem::global()->captureMouse(default_button);
            auto is_cxt_mouse_down = EventSystem::global()->captureMouse(default_cxt_button);
            auto mouse_pos = EventSystem::global()->captureMousePosition();
            auto is_on_area = Algorithm::comparePosInGeometry(mouse_pos, _geometry) >= 0;
            if (is_on_area) {
                if (!_status.test(Status_MouseIn)) {
                    _status.set(Status_MouseIn);
                    mouseMovedInEvent();
                    Cursor::global()->setCursor(_cursor);
                }
                if (!_status.test(Status_MouseDown) && is_mouse_down) {
                    _status.set(Status_MouseDown);
                    mouseButtonDownEvent(default_button);
                } else if (!_status.test(Status_MouseRightDown) && is_cxt_mouse_down) {
                    _status.set(Status_MouseRightDown);
                    mouseButtonDownEvent(default_cxt_button);
                } else if (!is_mouse_down && _status.test(Status_MouseDown)) {
                    _status.reset(Status_MouseDown);
                    mouseButtonUpEvent(default_button);
                    mouseClicked();
                } else if (!is_cxt_mouse_down && _status.test(Status_MouseRightDown)) {
                    _status.reset(Status_MouseRightDown);
                    mouseButtonUpEvent(default_cxt_button);
                    customContextMenuRequested();
                } else if (_status.test(Status_MouseDown) || _status.test(Status_MouseRightDown)) {
                    mouseMovedEvent(mouse_pos, EventSystem::global()->captureMouseAbsDistance());
                }
            } else {
                if (_status.test(Status_MouseDown) && !is_mouse_down) {
                    _status.reset(Status_MouseIn);
                    _status.reset(Status_MouseDown);
                    mouseButtonUpEvent(default_button);
                } else if (_status.test(Status_MouseRightDown) && !is_cxt_mouse_down) {
                    _status.reset(Status_MouseIn);
                    _status.reset(Status_MouseRightDown);
                    mouseButtonUpEvent(default_cxt_button);
                }
                if (_status.test(Status_MouseIn)) {
                    _status.reset(Status_MouseIn);
                    mouseMovedOutEvent();
                    Cursor::global()->setCursor(_window->cursor());
                }
                if (_status.test(Status_MouseDown) || _status.test(Status_MouseRightDown)) {
                    mouseMovedEvent(mouse_pos, EventSystem::global()->captureMouseAbsDistance());
                }
            }
        }
        // Touch Screen Event
        size_t fin_cnt = _window->getFingersCount();
        auto fingers_list = _window->getFingersIDList();
        if (event.type == SDL_EVENT_FINGER_DOWN || event.type == SDL_EVENT_FINGER_UP ||
            event.type == SDL_EVENT_FINGER_MOTION || event.type == SDL_EVENT_FINGER_CANCELED) {
            auto fin = _window->getFingerEventByID(event.tfinger.fingerID);
            if (fin.has_value()) {
                bool is_fin_on_area = Algorithm::comparePosInGeometry(fin->finger_down_pos + fin->distance_pos,
                                                                        _geometry) >= 0;
                if (!_status.test(Status_FingerDown) && is_fin_on_area) {
                    _status.set(Status_FingerDown);
                    _status.set(Status_FingerIn);
                    fingerDownEvent(event.tfinger.fingerID);
                } else if (_status.test(Status_FingerDown)) {
                    if (!_status.test(Status_FingerIn) && is_fin_on_area) {
                        _status.set(Status_FingerIn);
                        fingerMovedInEvent();
                    } else if (_status.test(Status_FingerIn) && !is_fin_on_area) {
                        _status.reset(Status_FingerIn);
                        fingerMovedOutEvent();
                    } else {
                        fingerMovedEvent(fin->finger_down_pos, fin->distance_pos);
                    }
                }
            } else if (_status.test(Status_FingerDown)) {
                fingerUpEvent(event.tfinger.fingerID);
                if (_status.test(Status_FingerIn )) fingerTouchedEvent();
            }

            if (fin_cnt == 0) {
                _status.reset(Status_FingerIn);
                _status.reset(Status_FingerDown);
            }
        }

        // Keyboard Event
        if ((event.type != SDL_EVENT_KEY_DOWN && event.type != SDL_EVENT_KEY_UP) || !SDL_HasKeyboard()) return;
        auto keys_status = EventSystem::global()->captureKeyboardStatus();
        if (_status.test(Status_Active)) {
            if (!keys_status.empty()) {
                if (event.key.type == SDL_EVENT_KEY_DOWN) {
                    if (!_status.test(Status_KeyDown)) {
                        _status.set(Status_KeyDown);
                    }
                    if (!event.key.repeat) keyDownEvent(event.key.scancode);
                } else if (_status.test(Status_KeyDown)) {
                    auto scancode = event.key.scancode;
                    if (std::ranges::find(keys_status, scancode) == keys_status.end()) {
                        keyUpEvent(scancode);
                        keyPressedEvent(scancode);
                    }
                }
            } else if (event.key.type == SDL_EVENT_KEY_UP) {
                _status.reset(Status_KeyDown);
                keyUpEvent(event.key.scancode);
                keyPressedEvent(event.key.scancode);
            }
        }

        if (!keys_status.empty() && !event.key.repeat) {
            for (size_t i = 0; i < keys_status.size(); i++) {
                if (_hotkeys_list[i] == keys_status) {
                    if (!_status.test(Status_HotkeysDown)) {
                        _status.set(Status_HotkeysDown);
                        hotkeyDownEvent(i);
                    }
                    break;
                }
                if (_status.test(Status_HotkeysDown)) {
                    _status.reset(Status_HotkeysDown);
                }
            }
        }
    });
}
