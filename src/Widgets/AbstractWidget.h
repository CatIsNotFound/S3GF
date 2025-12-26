
#ifndef MYENGINE_ABSTRACTWIDGET_H
#define MYENGINE_ABSTRACTWIDGET_H
#include "../Components.h"
#include "../Utils/Cursor.h"

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

            void setCursor(Cursor::StdCursor cursor_style);
            [[nodiscard]] Cursor::StdCursor cursor() const;

            template<typename ScanKeyCode, typename... Args>
            void setHotKey(ScanKeyCode key, Args... args) {
                addKey(key);
                if constexpr (sizeof...(args)) setHotKey(args...);
                else std::sort(_hot_key.begin(), _hot_key.end());
            }

        protected:
            virtual void loadEvent();
            virtual void unloadEvent();
            virtual void moveEvent(const Vector2& position);
            virtual void resizeEvent(const Size& size);
            virtual void enableChangedEvent(bool enabled);
            virtual void visibleChangedEvent(bool visible);
            virtual void objectNameChangedEvent(const std::string& object_name);
            virtual void paintEvent(Renderer *renderer);
            virtual void enterEvent();
            virtual void leaveEvent();
            virtual void focusInEvent();
            virtual void focusOutEvent();
            virtual void dragEvent();
            virtual void dragInEvent();
            virtual void dragOutEvent();
            virtual void dragMovedEvent();
            virtual void dropEvent();
            virtual void mouseClickedEvent();
            virtual void mouseDblClickedEvent();
            virtual void mouseDownEvent();
            virtual void mouseUpEvent();
            virtual void mouseEnteredEvent();
            virtual void mouseLeftEvent();
            virtual void customContextMenuRequestEvent();
            virtual void keyDownEvent(const std::vector<int>& key_scancode);
            virtual void keyUpEvent(const std::vector<int>& key_scancode);
            virtual void keyPressedEvent(const std::vector<int>& key_scancode);
            virtual void hotKeysPressedDownEvent();
            virtual void FingerDownEvent();
            virtual void FingerUpEvent();
            virtual void FingerTappedEvent();
            virtual void startedInputEvent();
            virtual void endedInputEvent();
        private:
            void unload();
            template<typename T>
            void addKey(T key) {
                static_assert(std::is_same_v<T, SDL_Scancode>,
                        "AbstractWidget: Some of the hotkeys is not valid! Used SDL_Scancode.");
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
        };
    }
}


#endif //MYENGINE_ABSTRACTWIDGET_H
