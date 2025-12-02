#ifndef S3GF_CONTROL_H
#define S3GF_CONTROL_H
#include "../Core.h"
#include "../Utils/Cursor.h"
#include "../Algorithm/Collider.h"

namespace S3GF {
    using GT = std::variant<std::monostate, Graphics::Point, Graphics::Rectangle>;
    inline uint64_t _next_id = 0;

    class ClickArea {
    public:
        explicit ClickArea(uint64_t window_id, GT graphic);

        void setEnabled(bool enabled);
        bool enabled();

        void setViewportEnabled(bool enabled);
        void setViewportArea(int x, int y, int w, int h);
        bool viewportEnabled() const;
        const Geometry& viewportArea() const;

        void setPressedEvent(std::function<void()> function);
        void resetPress();
        void setHover(bool enabled);

        void setGraphic(GT graphic);
        bool setPoint(const Vector2& position, uint16_t size);
        bool setRect(const GeometryF& geometry);

        Graphics::Point& point();
        Graphics::Rectangle& rectangle();

        bool getPoint(Graphics::Point& point);
        bool getRect(Graphics::Rectangle& rect);

        [[nodiscard]] bool isPressed() const;
        [[nodiscard]] bool isHovered() const;
        [[nodiscard]] bool isEntered() const;
        [[nodiscard]] bool isDown() const;
        [[nodiscard]] bool isLeft() const;
        [[nodiscard]] size_t index() const;

    private:
        GT _base, _real_base;
        uint64_t _winID;
        Geometry _viewport;
        bool _viewport_enabled{false};
        bool _is_pressed{false}, _is_hovered{false}, _is_entered{false},
             _is_down{false}, _is_triggered{false}, _is_left{true};
        bool _enabled{true}, _need_triggered{false};
        std::function<void()> _func;
    };

    class HoldableArea {
    public:
        explicit HoldableArea(uint64_t window_id, GT graphic);

        void setEnabled(bool enabled);
        bool enabled();

        void setViewportEnabled(bool enabled);
        void setViewportArea(int x, int y, int w, int h);
        bool viewportEnabled() const;
        const Geometry& viewportArea() const;

        void setHover(bool enabled);
        void setRelease();

        void setGraphic(GT graphic);
        bool setPoint(const Vector2& position, uint16_t size);
        bool setRect(const GeometryF& geometry);

        Graphics::Point& point();
        Graphics::Rectangle& rectangle();

        bool getPoint(Graphics::Point& point);
        bool getRect(Graphics::Rectangle& rect);

        [[nodiscard]] bool isHovered() const;
        [[nodiscard]] bool isEntered() const;
        [[nodiscard]] bool isDown() const;
        [[nodiscard]] bool isLeft() const;
        [[nodiscard]] size_t index() const;
    private:
        GT _base;
        Geometry _viewport;
        bool _viewport_enabled{false};
        uint64_t _winID;
        bool _is_hovered{false}, _is_entered{false},
                _is_down{false}, _is_left{true};
        bool _enabled{true}, _sp_trigger{false};
    };


    struct ColorStatus {
        SDL_Color normal;
        SDL_Color active;
        SDL_Color pressed;
        SDL_Color invalid;
        SDL_Color checked;
    };

    class AbstractControl {
    public:
        explicit AbstractControl(const std::string& name, Renderer* renderer,
                                 AbstractControl* parent = nullptr,
                                 GT click_area = Graphics::Rectangle());

        ~AbstractControl() = default;

        void setName(const std::string& name);
        [[nodiscard]] const std::string& name() const;

        void move(float x, float y);
        void move(const Vector2& pos);
        void move(Vector2&& pos);

        void resize(float w, float h);
        void resize(const Size& size);
        void resize(Size&& size);

        void setGeometry(float x, float y, float w, float h);
        void setGeometry(const Vector2& pos, const Size& size);
        void setGeometry(Vector2&& pos, Size&& size);
        void setGeometry(const GeometryF& geometry);
        void setGeometry(GeometryF&& geometry);

        [[nodiscard]] const GeometryF& geometry() const { return _geometry; }
        [[nodiscard]] const Vector2& position() const { return _geometry.pos; }
        [[nodiscard]] const Size& size() const { return _geometry.size; }

        void setVisible(bool visible);
        [[nodiscard]] bool visible() const { return _visible; }
        void setEnabled(bool enabled);
        [[nodiscard]] bool enabled() const { return _enabled; }
        void setActive() { _active = true; activeChanged(true); }
        void setInactive() { _active = false; activeChanged(false); }
        [[nodiscard]] bool active() const { return _active; }
        void setKey(SDL_Keycode key);
        void setCheckable(bool checkable);
        void setChecked(bool checked);
        [[nodiscard]] bool checkable() const;
        [[nodiscard]] bool isChecked() const;
        [[nodiscard]] bool isDown() const;
        [[nodiscard]] bool isHovered() const;

    protected:
        virtual void paintEvent(Renderer*) {}
        virtual void moveEvent() {}
        virtual void resizeEvent() {}
        virtual void onPressed() {}
        virtual void onReleased() {}
        virtual void onTriggered() {}
        virtual void onHovered() {}
        virtual void onEntered() {}
        virtual void onLeft() {}
        virtual void onKeyPressed() {}
        virtual void onKeyReleased() {}
        virtual void onChecked(bool checked) {}
        virtual void activeChanged(bool active) {}
        virtual void enabledChanged(bool enabled) {}
        virtual void visibleChanged(bool visible) {}
        void _update_click_area();

    protected:
        uint64_t _id{0};
        std::string _name;
        Renderer* _renderer;
        AbstractControl* _parent;
        GeometryF _geometry;
        ClickArea _click_area;
        SDL_Keycode _key_code{SDLK_UNKNOWN};
        bool _is_down{false}, _is_hovered{false}, _is_checked{false};
        bool _visible{true}, _enabled{true}, _entered{false}, _active{false},
             _checkable{false};
    };

} // S3GF

#endif //S3GF_CONTROL_H
