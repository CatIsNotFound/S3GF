
#include "Control.h"

namespace S3GF {
    ClickArea::ClickArea(uint64_t window_id, GT graphic)
        : _base(std::move(graphic)), _real_base(std::monostate()), _winID(window_id) {
        EventSystem::global()->appendEvent(_next_id++, [this](SDL_Event ev) {
            if (ev.window.windowID != _winID) {
                _is_pressed = false;
                _is_hovered = false;
                return;
            }
            if (!_enabled) return;
            Vector2 cur_pos = Cursor::global()->position();
            int8_t tri = -1;
            switch (_base.index()) {
                case 1:
                    if (viewportEnabled()) {
                        tri = Algorithm::comparePosInPoint(cur_pos, std::get<Graphics::Point>(_real_base));
                    } else {
                        tri = Algorithm::comparePosInPoint(cur_pos, std::get<Graphics::Point>(_base));
                    }
                    break;
                case 2:
                    if (_viewport_enabled) {
                        tri = Algorithm::comparePosInRect(cur_pos, std::get<Graphics::Rectangle>(_real_base));
                    } else {
                        tri = Algorithm::comparePosInRect(cur_pos, std::get<Graphics::Rectangle>(_base));
                    }
                    break;
                default:
                    break;
            }
            // Avoid the issue of triggering prematurely
            // when the mouse moves into the clickable area before pressing the mouse button.
            if (tri > 0) {
                _is_left = false;
                if (!_is_hovered) {
                    _is_hovered = true;
                }
                if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                    _is_down = true;
                    _need_triggered = true;
                } else if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_UP) {
                    _is_down = false;
                    if (_need_triggered) { _is_pressed = true; _need_triggered = false; }
                }
            } else {
                _is_hovered = false;
                _is_pressed = false;
                _is_entered = false;
                _is_left = true;
                _need_triggered = false;
            }
            if (_is_hovered && !_is_entered) _is_entered = true;
            if (_is_pressed && _func) {
                _func();
                _is_pressed = false;
            }
        });
    }

    void ClickArea::setEnabled(bool enabled) {
        _enabled = enabled;
    }

    bool ClickArea::enabled() {
        return _enabled;
    }

    void ClickArea::setViewportEnabled(bool enabled) {
        _viewport_enabled = enabled;
        if (enabled) {
            if (std::holds_alternative<Graphics::Point>(_base)) {
                _real_base = Graphics::Point(std::get<Graphics::Point>(_base));
            } else if (std::holds_alternative<Graphics::Rectangle>(_base)) {
                _real_base = Graphics::Rectangle(std::get<Graphics::Rectangle>(_base));
            }
        } else {
            _real_base = std::monostate();
        }
    }

    bool ClickArea::viewportEnabled() const {
        return _viewport_enabled;
    }

    const Geometry& ClickArea::viewportArea() const {
        return _viewport;
    }

    void ClickArea::setViewportArea(int x, int y, int w, int h) {
        /// Must enable the viewport and specified the valid size(width, height), otherwise it does not work.
        if (!_viewport_enabled || w <= 0 || h <= 0) return;
        _viewport.setGeometry(x, y, w, h);
        if (std::holds_alternative<Graphics::Point>(_real_base)) {
            auto& base = std::get<Graphics::Point>(_real_base);
            const auto& BASE = std::get<Graphics::Point>(_base);
            Vector2 pos = BASE.position();
            base.reset((float)(x + pos.x), (float)(y + pos.y), (float)(std::min(w, h)), StdColor::Black);
        } else if (std::holds_alternative<Graphics::Rectangle>(_real_base)) {
            auto& base = std::get<Graphics::Rectangle>(_real_base);
            const auto& BASE = std::get<Graphics::Rectangle>(_base);
            Vector2 pos = BASE.geometry().pos;
            Size size = BASE.geometry().size;
            base.setGeometry((float)(x + pos.x), (float)(y + pos.y), (float)w, (float)h);
        } else {
            Logger::log("ClickArea::setViewportArea: Failed to set viewport area! The graphic is not valid!", Logger::WARN);
        }
    }

    void ClickArea::setPressedEvent(std::function<void()> function) {
        _func = std::move(function);
    }

    void ClickArea::resetPress() {
        _is_pressed = false;
    }

    void ClickArea::setHover(bool enabled) {
        _is_hovered = enabled;
    }

    void ClickArea::setGraphic(GT graphic) {
        _base = std::move(graphic);
    }

    bool ClickArea::setPoint(const Vector2& position, uint16_t size) {
        if (_base.index() != 1) return false;
        auto& pt = std::get<Graphics::Point>(_base);
        pt.reset(position, size, pt.color());
        return true;
    }

    bool ClickArea::setRect(const GeometryF& geometry) {
        if (_base.index() != 2) return false;
        auto& rect = std::get<Graphics::Rectangle>(_base);
        rect.setGeometry(geometry);
        return true;
    }

    Graphics::Point& ClickArea::point() {
        if (_base.index() != 1) {
            throw std::runtime_error("ClickArea: The graphic is not point!");
        }
        return std::get<Graphics::Point>(_base);
    }

    Graphics::Rectangle& ClickArea::rectangle() {
        if (_base.index() != 2) {
            throw std::runtime_error("ClickArea: The graphic is not rectangle!");
        }
        return std::get<Graphics::Rectangle>(_base);
    }

    bool ClickArea::getPoint(Graphics::Point& point) {
        if (_base.index() != 1) return false;
        point = std::get<Graphics::Point>(_base);
        return true;
    }

    bool ClickArea::getRect(Graphics::Rectangle& rect) {
        if (_base.index() != 2) return false;
        auto o_rect = std::get<Graphics::Rectangle>(_base);
        rect.reset(o_rect.geometry(), o_rect.borderSize(), o_rect.borderColor(), o_rect.backgroundColor());
        return true;
    }

    bool ClickArea::isPressed() const { return _is_pressed; }
    bool ClickArea::isHovered() const { return _is_hovered; }
    bool ClickArea::isEntered() const { return _is_entered; }
    bool ClickArea::isDown() const { return _is_down; }
    bool ClickArea::isLeft() const { return _is_left; }
    size_t ClickArea::index() const { return _base.index(); }

    HoldableArea::HoldableArea(uint64_t window_id, GT graphic) : _base(std::move(graphic)), _winID(window_id) {
        EventSystem::global()->appendEvent(_next_id++, [this] (SDL_Event ev) {
            if (ev.window.windowID != _winID) {
                _is_hovered = false;
                return;
            }
            if (!_enabled) return;
            Vector2 cur_pos = Cursor::global()->position();
            int8_t tri = -1;
            switch (_base.index()) {
                case 1:
                    if (viewportEnabled()) {
                        auto pt = std::get<Graphics::Point>(_base);
                        Graphics::Point real_base;
                        real_base.move(_viewport.x + pt.position().x, _viewport.y + pt.position().y);
                        real_base.resize((std::min(_viewport.width, _viewport.height)));
                        tri = Algorithm::comparePosInPoint(cur_pos, real_base);
                    } else {
                        tri = Algorithm::comparePosInPoint(cur_pos, std::get<Graphics::Point>(_base));
                    }
                    break;
                case 2:
                    if (_viewport_enabled) {
                        auto rect = std::get<Graphics::Rectangle>(_base);
                        Graphics::Rectangle real_base;
                        real_base.setGeometry(_viewport.x + rect.geometry().pos.x,
                                              _viewport.y + rect.geometry().pos.y,
                                              _viewport.width, _viewport.height);
                        tri = Algorithm::comparePosInRect(cur_pos, real_base);
                    } else {
                        tri = Algorithm::comparePosInRect(cur_pos, std::get<Graphics::Rectangle>(_base));
                    }
                    break;
                default:
                    break;
            }
            auto state = SDL_GetMouseState(nullptr, nullptr);
            if (tri > 0) {
                _is_left = false;
                if (!_is_hovered) {
                    _is_hovered = true;
                }
                if (state > 0) {
                    if (_sp_trigger) _is_down = true;
                } else {
                    _is_down = false;
                }
            } else {
                _is_hovered = false;
                _is_entered = false;
                _is_left = true;
                _sp_trigger = false;
            }
            if (_is_hovered && !_is_entered) {_is_entered = true; _sp_trigger = true; }
        });
    }

    void HoldableArea::setEnabled(bool enabled) {
        _enabled = enabled;
    }

    bool HoldableArea::enabled() {
        return _enabled;
    }

    void HoldableArea::setViewportEnabled(bool enabled) {
        _viewport_enabled = enabled;
    }

    bool HoldableArea::viewportEnabled() const {
        return _viewport_enabled;
    }

    const Geometry& HoldableArea::viewportArea() const {
        return _viewport;
    }

    void HoldableArea::setViewportArea(int x, int y, int w, int h) {
        _viewport.setGeometry(x, y, w, h);
    }

    void HoldableArea::setHover(bool enabled) {
        _is_hovered = enabled;
    }

    void HoldableArea::setRelease() { _is_down = false; _sp_trigger = false; }

    void HoldableArea::setGraphic(GT graphic) {
        _base = std::move(graphic);
    }

    bool HoldableArea::setPoint(const Vector2 &position, uint16_t size) {
        if (_base.index() != 1) return false;
        auto& pt = std::get<Graphics::Point>(_base);
        pt.reset(position, size, pt.color());
        return true;
    }

    bool HoldableArea::setRect(const GeometryF &geometry) {
        if (_base.index() != 2) return false;
        auto& rect = std::get<Graphics::Rectangle>(_base);
        rect.setGeometry(geometry);
        return true;
    }

    Graphics::Point &HoldableArea::point() {
        return std::get<Graphics::Point>(_base);
    }

    Graphics::Rectangle &HoldableArea::rectangle() {
        return std::get<Graphics::Rectangle>(_base);
    }

    bool HoldableArea::getPoint(Graphics::Point &point) {
        if (_base.index() != 1) return false;
        point = std::get<Graphics::Point>(_base);
        return true;
    }

    bool HoldableArea::getRect(Graphics::Rectangle &rect) {
        if (_base.index() != 2) return false;
        rect = std::get<Graphics::Rectangle>(_base);
        return true;
    }

    bool HoldableArea::isHovered() const { return _is_hovered;}
    bool HoldableArea::isEntered() const { return _is_entered; }
    bool HoldableArea::isDown() const { return _is_down; }
    bool HoldableArea::isLeft() const { return _is_left; }
    size_t HoldableArea::index() const { return _base.index(); }

    AbstractControl::AbstractControl(const std::string& name, Renderer* renderer,
                                     AbstractControl* parent, GT click_area)
        : _name(name), _renderer(renderer), _parent(parent), _id(++_next_id),
          _click_area(renderer->window()->windowID(), std::move(click_area)) {

        renderer->window()->installPaintEvent([this] (Renderer* r) {
            if (!_visible) return;
            paintEvent(r);
        });

        EventSystem::global()->appendEvent(_next_id++, [this](SDL_Event ev) {
            if (!_visible || !_enabled) return;

            bool _entered_changed = (_entered != _click_area.isEntered());
            if (_entered_changed) {
                _entered = _click_area.isEntered();
                if (_entered) onEntered(); else onLeft();
            }
            bool _button_hoverd_changed = (_is_hovered != _click_area.isHovered());
            if (_button_hoverd_changed) {
                _is_hovered = _click_area.isHovered();
                if (_is_hovered) onHovered();
            }
            bool _button_down_changed = (_is_down != _click_area.isDown());
            if (_button_down_changed) {
                _is_down = _click_area.isDown();
                if (_is_down) onPressed(); else onReleased();
            }
            if (_active && !ev.key.repeat && (_key_code != 0 && ev.key.key == _key_code)) {
                if (ev.key.down) {
                    onKeyPressed();
                } else {
                    onKeyReleased();
                    if (_checkable) {
                        _is_checked = !_is_checked;
                        onChecked(_is_checked);
                    } else {
                        onTriggered();
                    }
                    _click_area.resetPress();
                }
            }

            if (_click_area.isPressed() && _is_hovered) {
                if (_checkable) {
                    _is_checked = !_is_checked;
                    onChecked(_is_checked);
                } else {
                    onTriggered();
                }
                _click_area.resetPress();
            }
            if (_click_area.isLeft()) {
                _entered = false;
            }

        });

        if (_click_area.index() == 1) {
            Graphics::Point& pt = _click_area.point();
            _geometry.resetPos(pt.position());
            auto size = static_cast<float>(pt.size());
            _geometry.resetSize({size, size});
        } else if (_click_area.index() == 2) {
            Graphics::Rectangle& re = _click_area.rectangle();
            _geometry.reset(re.geometry());
        }
    }

    void AbstractControl::setName(const std::string& name) {
        _name = name;
    }

    const std::string& AbstractControl::name() const {
        return _name;
    }

    void AbstractControl::move(float x, float y) {
        _geometry.resetPos({x, y});
        _update_click_area();
        moveEvent();
    }

    void AbstractControl::move(const Vector2& pos) {
        _geometry.resetPos(pos);
        _update_click_area();
        moveEvent();
    }

    void AbstractControl::move(Vector2&& pos) {
        _geometry.resetPos(pos);
        _update_click_area();
        moveEvent();
    }

    void AbstractControl::resize(float w, float h) {
        _geometry.resetSize({w, h});
        _update_click_area();
        resizeEvent();
    }

    void AbstractControl::resize(const Size& size) {
        _geometry.resetSize(size);
        _update_click_area();
        resizeEvent();
    }

    void AbstractControl::resize(Size&& size) {
        _geometry.resetSize(size);
        _update_click_area();
        resizeEvent();
    }

    void AbstractControl::setGeometry(float x, float y, float w, float h) {
        _geometry.reset(x, y, w, h);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(const Vector2& pos, const Size& size) {
        _geometry.reset(pos, size);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(Vector2&& pos, Size&& size) {
        _geometry.reset(pos, size);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(const GeometryF& geometry) {
        _geometry.reset(geometry);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setGeometry(GeometryF&& geometry) {
        _geometry.reset(geometry);
        _update_click_area();
        moveEvent();
        resizeEvent();
    }

    void AbstractControl::setVisible(bool visible) {
        _visible = visible;
        _is_checked = false;
        visibleChanged(visible);
    }

    void AbstractControl::setEnabled(bool enabled) {
        _enabled = enabled;
        _is_checked = false;
        enabledChanged(enabled);
    }


    void AbstractControl::setKey(SDL_Keycode key) {
        _key_code = key;
    }

    void AbstractControl::setCheckable(bool checkable) {
        _checkable = checkable;
        if (!checkable) {
            _is_checked = false;
        }
    }

    void AbstractControl::setChecked(bool checked) {
        _is_checked = checked;
        onChecked(checked);
    }

    bool AbstractControl::checkable() const {
        return _checkable;
    }

    bool AbstractControl::isChecked() const {
        return _is_checked;
    }

    bool AbstractControl::isDown() const {
        return _is_down;
    }

    bool AbstractControl::isHovered() const {
        return _is_hovered;
    }

    void AbstractControl::_update_click_area() {
        if (_click_area.index() == 1) {
            auto min = std::min(_geometry.size.width, _geometry.size.height);
            auto real_size = std::min(static_cast<int>(min), 65535);
            _click_area.setPoint(_geometry.pos, real_size);
        } else if (_click_area.index() == 2) {
            _click_area.setRect(_geometry);
            Logger::log(std::format("AbstractControl Changed: ({}, {}) ({}x{})",
                    _geometry.pos.x, _geometry.pos.y, _geometry.size.width, _geometry.size.height));
        }
    }



}

