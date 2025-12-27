
#ifndef MYENGINE_WIDGETS_ABSTRACTWIDGET_H
#define MYENGINE_WIDGETS_ABSTRACTWIDGET_H
#include "../Components.h"
#include "../Utils/Cursor.h"
#include "../Utils/Variant.h"
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

            void setProperty(std::string name, bool value);
            void setProperty(std::string name, uint8_t value);
            void setProperty(std::string name, uint16_t value);
            void setProperty(std::string name, uint32_t value);
            void setProperty(std::string name, uint64_t value);
            void setProperty(std::string name, int8_t value);
            void setProperty(std::string name, int16_t value);
            void setProperty(std::string name, int32_t value);
            void setProperty(std::string name, int64_t value);
            void setProperty(std::string name, float value);
            void setProperty(std::string name, double value);
            void setProperty(std::string name, const char* value);
            void setProperty(std::string name, std::string& value);
            void setProperty(std::string name, std::string&& value);
            void setProperty(std::string name, void* value);
            void setProperty(std::string name);
            const Variant *const property(std::string name) const;

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
            virtual void FingerDownEvent();
            virtual void FingerUpEvent();
            virtual void FingerTappedEvent();
            virtual void startedInputEvent();
            virtual void endedInputEvent();
            virtual void inputEvent(const char* string);
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
            std::string _object_name{};
            uint64_t _ev_id{0};
            std::vector<int> _hot_key;
            bool _visible{true}, _enabled{true}, _focus{false};
            Graphics::Rectangle _trigger_area;
            Cursor::StdCursor _cur_style{Cursor::Default};
            struct Status {
                bool is_loaded{};
                bool mouse_in{};
                bool mouse_down{};
                bool r_mouse_down{};
                bool key_down{};
                bool is_hot_key_triggered{};
                bool input_mode{};
                bool dragging{};
            };
            Status _status{};
            std::string _cur_ch{};
            std::unordered_map<std::string, Variant> _prop_map{};
        };
    }
}


#endif //MYENGINE_WIDGETS_ABSTRACTWIDGET_H
