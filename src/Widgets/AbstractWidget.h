
#ifndef MYENGINE_WIDGETS_ABSTRACTWIDGET_H
#define MYENGINE_WIDGETS_ABSTRACTWIDGET_H
#include "../Components.h"
#include "../Utils/Cursor.h"
#include "../Utils/Variant.h"

#define NEW_PROPERTY_PTR(POINTER, NAME, CLASS)                                   \
POINTER->setProperty(NAME, static_cast<void*>(new CLASS()), [](void* v) {        \
    delete static_cast<CLASS*>(v);                                               \
});

#define GET_PROPERTY_PTR(POINTER, NAME, CLASS)                                   \
static_cast<CLASS*>(POINTER->property(NAME)->toPointer());


namespace MyEngine {
    namespace Widget {
        class AbstractWidget {
        public:
            explicit AbstractWidget(Window* window);
            virtual ~AbstractWidget();

            void setObjectName(std::string object_name);
            [[nodiscard]] const std::string& objectName() const;

            void setVisible(bool visible);
            [[nodiscard]] bool visible() const;

            void setEnabled(bool enabled);
            [[nodiscard]] bool enabled() const;

            void setGeometry(float x, float y, float w, float h);
            void setGeometry(const Vector2& position, const Size& size);
            void setGeometry(const GeometryF& geometry);
            [[nodiscard]] const GeometryF& geometry() const;

            void move(float x, float y);
            void move(const Vector2& position);
            [[nodiscard]] const Vector2& position() const;

            void resize(float w, float h);
            void resize(const Size& size);
            [[nodiscard]] const Size& size() const;

            void setFocusEnabled(bool enabled);
            [[nodiscard]] bool focusEnabled() const;

            void setInputModeEnabled(bool enabled);
            [[nodiscard]] bool inputModeEnabled() const;
            [[nodiscard]] const std::string& getInputChar() const;

            void setCursor(Cursor::StdCursor cursor_style);
            [[nodiscard]] Cursor::StdCursor cursor() const;

            template<typename ScanKeyCode, typename... Args>
            void setHotKey(ScanKeyCode key, Args... args) {
                addKey(key);
                if constexpr (sizeof...(args)) setHotKey(args...);
                else std::sort(_hot_key.begin(), _hot_key.end());
            }

            void setHotKeyEnabled(bool enabled);
            [[nodiscard]] bool hotKeyEnabled() const;

            void setProperty(const std::string& name, bool value);
            void setProperty(const std::string& name, uint8_t value);
            void setProperty(const std::string& name, uint16_t value);
            void setProperty(const std::string& name, uint32_t value);
            void setProperty(const std::string& name, uint64_t value);
            void setProperty(const std::string& name, int8_t value);
            void setProperty(const std::string& name, int16_t value);
            void setProperty(const std::string& name, int32_t value);
            void setProperty(const std::string& name, int64_t value);
            void setProperty(const std::string& name, float value);
            void setProperty(const std::string& name, double value);
            void setProperty(const std::string& name, const char* value);
            void setProperty(const std::string& name, std::string& value);
            void setProperty(const std::string& name, std::string&& value);
            void setProperty(const std::string& name, void* value);
            void setProperty(const std::string& name, void* value, std::function<void(void*)> deleter);
            void setProperty(const std::string& name);
            const Variant *const property(const std::string& name) const;

        protected:
            Renderer* render() { return _renderer; }
            virtual void loadEvent();
            virtual void unloadEvent();
            virtual void moveEvent(const Vector2& position);
            virtual void resizeEvent(const Size& size);
            virtual void enableChangedEvent(bool enabled);
            virtual void visibleChangedEvent(bool visible);
            virtual void objectNameChangedEvent(const std::string& object_name);
            virtual void paintEvent(Renderer *renderer);
            virtual void focusInEvent();
            virtual void focusOutEvent();
            virtual void mouseClickedEvent();
            virtual void mouseDblClickedEvent();
            virtual void mouseDownEvent();
            virtual void mouseUpEvent();
            virtual void mouseEnteredEvent();
            virtual void mouseLeftEvent();
            virtual void customContextMenuRequestEvent(const Vector2 &position);
            virtual void keyDownEvent(SDL_Scancode scancode);
            virtual void keyUpEvent(SDL_Scancode scancode);
            virtual void keyPressedEvent();
            virtual void hotKeysPressedEvent();
            virtual void fingerDownEvent(const Vector2& position);
            virtual void fingerUpEvent(const Vector2& position);
            virtual void fingerMovedEvent(const Vector2& position, const Vector2& distance);
            virtual void fingerMoveOutEvent();
            virtual void fingerMoveInEvent();
            virtual void fingerTappedEvent();
            virtual void startedInputEvent();
            virtual void endedInputEvent();
            virtual void inputEvent(const char* string);
            virtual void propertyChanged(const std::string& property, const Variant& variant);
        protected:
            std::string _object_name{};
            Graphics::Rectangle _trigger_area;
        private:
            void unload();
            template<typename T>
            void addKey(T key) {
                static_assert(std::is_same_v<T, SDL_Scancode>,
                        "AbstractWidget: Some of the hot keys is not valid! Used SDL_Scancode instead.");
                _hot_key.emplace_back(key);
            }
            Window* _window;
            Renderer* _renderer;
            Engine* _engine{nullptr};
            uint64_t _ev_id{0};
            std::vector<int> _hot_key;
            std::vector<std::vector<int>> _hot_key_list;
            bool _visible{true}, _enabled{true}, _focus{false};
            Cursor::StdCursor _cur_style{Cursor::Default};
            struct Status {
                bool is_loaded{};
                bool mouse_in{};
                bool mouse_down{};
                bool r_mouse_down{};
                bool key_down{};
                bool is_hot_key_triggered{};
                bool input_mode{};
                bool hot_keys{};
                bool finger_down{};
                bool finger_move_in{};
                bool finger_move_out{};
                uint64_t finger_id{};
                Vector2 finger_down_pos{};
            };
            Status _status{};
            std::string _cur_ch{};
            std::unordered_map<std::string, Variant> _prop_map{};
        };
    }
}


#endif //MYENGINE_WIDGETS_ABSTRACTWIDGET_H
