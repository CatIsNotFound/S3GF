#ifndef S3GF_CONTROL_H
#define S3GF_CONTROL_H
#include <utility>

#include "Core.h"
#include "Utils/Cursor.h"
#include "Algorithm/Collider.h"
namespace S3GF {
    using GT = std::variant<std::monostate, Graphics::Point, Graphics::Rectangle>;
    inline uint64_t _next_id = 0;

    class ClickArea {
    public:
        explicit ClickArea(uint64_t window_id, GT graphic)
            : _base(std::move(graphic)), _winID(window_id) {
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
                        tri = Algorithm::comparePosInPoint(cur_pos, std::get<Graphics::Point>(_base));
                        break;
                    case 2:
                        tri = Algorithm::comparePosInRect(cur_pos, std::get<Graphics::Rectangle>(_base));
                        break;
                    default:
                        break;
                }
                if (tri > 0) {
                    if (!_is_hovered) {
                        _is_hovered = true;
                    }
                    if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN || ev.key.type == SDL_EVENT_KEY_DOWN) {
                        _is_down = true;
                    } else if (ev.button.type == SDL_EVENT_MOUSE_BUTTON_UP || ev.key.type == SDL_EVENT_KEY_UP) {
                        _is_down = false;
                        _is_pressed = true;
                    }
                } else {
                    _is_hovered = false;
                    _is_pressed = false;
                    _is_entered = false;
                }
                if (_is_hovered && !_is_entered) _is_entered = true;
                if (_is_pressed && _func) {
                    _func();
                    _is_pressed = false;
                }
            });
        }

        void setEnabled(bool enabled) {
            _enabled = enabled;
        }

        bool enabled() {
            return _enabled;
        }

        void setPressedEvent(std::function<void()> function) {
            _func = std::move(function);
        }

        void resetPress() { _is_pressed = false; }
        void setHover(bool enabled) { _is_hovered = enabled; }

        void setGraphic(GT graphic) {
            _base = std::move(graphic);

        }

        bool setPoint(const Vector2& position, uint16_t size) {
            if (_base.index() != 1) return false;
            auto& pt = std::get<Graphics::Point>(_base);
            pt.reset(position, size, pt.color());
            return true;
        }

        bool setRect(const GeometryF& geometry) {
            if (_base.index() != 2) return false;
            auto& rect = std::get<Graphics::Rectangle>(_base);
            rect.setGeometry(geometry);
            return true;
        }

        Graphics::Point& point() {
            if (_base.index() != 1) {
                throw std::runtime_error("ClickArea: The graphic is not point!");
            }
            return std::get<Graphics::Point>(_base);
        }

        Graphics::Rectangle& rectangle() {
            if (_base.index() != 2) {
                throw std::runtime_error("ClickArea: The graphic is not rectangle!");
            }
            return std::get<Graphics::Rectangle>(_base);
        }

        bool getPoint(Graphics::Point& point) {
            if (_base.index() != 1) return false;
            point = std::get<Graphics::Point>(_base);
            return true;
        }

        bool getRect(Graphics::Rectangle& rect) {
            if (_base.index() != 2) return false;
            auto o_rect = std::get<Graphics::Rectangle>(_base);
            rect.reset(o_rect.geometry(), o_rect.borderSize(), o_rect.borderColor(), o_rect.backgroundColor());
            return true;
        }

        [[nodiscard]] bool isPressed() const { return _is_pressed; }
        [[nodiscard]] bool isHovered() const { return _is_hovered; }
        [[nodiscard]] bool isEntered() const { return _is_entered; }
        [[nodiscard]] bool isDown() const { return _is_down; }
        [[nodiscard]] size_t index() const { return _base.index(); }
    private:
        GT _base;
        uint64_t _winID;
        bool _is_pressed{false}, _is_hovered{false}, _is_entered{false}, _is_down{false}, _is_triggered{false};
        bool _enabled{true};
        std::function<void()> _func;
    };

    class AbstractControl {
    public:
        enum State {
            Normal,
            Hover,
            Pressed,
            Disabled,
            Selected
        };

        explicit AbstractControl(const std::string& name, Renderer* renderer,
                                 GT click_area = Graphics::Rectangle(),
                                 AbstractControl* parent = nullptr)
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
                if (_click_area.isHovered()) onHovered();
                bool _button_down_changed = (_is_down != _click_area.isDown());
                if (_button_down_changed) {
                    _is_down = _click_area.isDown();
                    if (_is_down) onPressed(); else onReleased();
                }
                if (_click_area.isPressed()) {
                    onTriggered();
                    _click_area.resetPress();
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
            loadEvent();
        }

        ~AbstractControl() { unloadEvent(); };

        void setName(const std::string& name) {
            _name = name;
        }

        [[nodiscard]] const std::string& name() const {
            return _name;
        }

        void move(float x, float y) {
            _geometry.resetPos({x, y});
            _update_click_area();
            moveEvent();
        }

        void move(const Vector2& pos) {
            _geometry.resetPos(pos);
            _update_click_area();
            moveEvent();
        }

        void move(Vector2&& pos) {
            _geometry.resetPos(pos);
            _update_click_area();
            moveEvent();
        }

        void resize(float w, float h) {
            _geometry.resetSize({w, h});
            _update_click_area();
            resizeEvent();
        }

        void resize(const Size& size) {
            _geometry.resetSize(size);
            _update_click_area();
            resizeEvent();
        }

        void resize(Size&& size) {
            _geometry.resetSize(size);
            _update_click_area();
            resizeEvent();
        }

        void setGeometry(float x, float y, float w, float h) {
            _geometry.reset(x, y, w, h);
            _update_click_area();
            moveEvent();
            resizeEvent();
        }

        void setGeometry(const Vector2& pos, const Size& size) {
            _geometry.reset(pos, size);
            _update_click_area();
            moveEvent();
            resizeEvent();
        }

        void setGeometry(Vector2&& pos, Size&& size) {
            _geometry.reset(pos, size);
            _update_click_area();
            moveEvent();
            resizeEvent();
        }

        void setGeometry(const GeometryF& geometry) {
            _geometry.reset(geometry);
            _update_click_area();
            moveEvent();
            resizeEvent();
        }

        void setGeometry(GeometryF&& geometry) {
            _geometry.reset(geometry);
            _update_click_area();
            moveEvent();
            resizeEvent();
        }

        [[nodiscard]] const GeometryF& geometry() const { return _geometry; }
        [[nodiscard]] const Vector2& position() const { return _geometry.pos; }
        [[nodiscard]] const Size& size() const { return _geometry.size; }

        void setVisible(bool visible) {
            _visible = visible;
            visibleChanged();
        }
        [[nodiscard]] bool visible() const { return _visible; }
        void setEnabled(bool enabled) {
            _enabled = enabled;
            enabledChanged();
        }
        [[nodiscard]] bool enabled() const { return _enabled; }

    protected:
        virtual void paintEvent(Renderer*) {}
        virtual void loadEvent() {}
        virtual void unloadEvent() {}
        virtual void moveEvent() {}
        virtual void resizeEvent() {}
        virtual void onPressed() {}
        virtual void onReleased() {}
        virtual void onTriggered() {}
        virtual void onHovered() {}
        virtual void onEntered() {}
        virtual void onLeft() {}
        virtual void enabledChanged() {}
        virtual void visibleChanged() {}

        void _update_click_area() {
            if (_click_area.index() == 1) {
                auto min = std::min(_geometry.size.width, _geometry.size.height);
                auto real_size = std::min(static_cast<int>(min), UINT16_MAX);
                _click_area.setPoint(_geometry.pos, real_size);
            } else if (_click_area.index() == 2) {
                _click_area.setRect(_geometry);
                Logger::log(std::format("Changed: {}, {} {}x{}",
                        _geometry.pos.x, _geometry.pos.y, _geometry.size.width, _geometry.size.height));
            }
        }

    protected:
        uint64_t _id{0};
        std::string _name;
        Renderer* _renderer;
        AbstractControl* _parent;
        GeometryF _geometry;
        ClickArea _click_area;
        bool _is_down{false};
        bool _visible{true}, _enabled{true}, _entered{false};
    };

//    class Button : public AbstractControl {
//    public:
//        explicit Button(const std::string& object_name, Renderer* renderer, AbstractControl* parent = nullptr)
//            : AbstractControl(object_name, renderer, _click_area, parent) {
//            auto g = geometry();
//            _base = Graphics::Rectangle(g.pos.x, g.pos.y, _default_size.width, _default_size.height,
//                                        1, StdColor::Black, StdColor::White);
//            setGeometry(0, 0, _default_size.width, _default_size.height);
//        }
//
//        void setText(const std::string& text) { _text = text; }
//        const std::string& text() const { return _text; }
//        void setFontName(const std::string& font_name) { _font_name = font_name; }
//        const std::string& fontName() const { return _font_name; }
//
//    protected:
//        virtual void drawEvent() override {
//            if (!_visible) return;
//            if (!_renderer) return;
//            _renderer->drawRectangle(std::get<Graphics::Rectangle>(_base));
//        }
//
//    private:
//        GT _base;
//        Size _min_size{0, 0};
//        Size _max_size{0, 0};
//        Size _default_size{60, 40};
//        std::string _font_name;
//        std::string _text{"Button"};
//        std::function<void()> _function;
//    };

} // S3GF

#endif //S3GF_CONTROL_H
