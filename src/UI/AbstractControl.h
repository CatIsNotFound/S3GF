
#ifndef MYENGINE_UI_ABSTRACTCONTROL_H
#define MYENGINE_UI_ABSTRACTCONTROL_H
#include "../Components.h"
#include "../Utils/Variant.h"

namespace MyEngine {
    namespace UI {
        constexpr inline size_t Status_Loaded{1};
        constexpr inline size_t Status_Visible{2};
        constexpr inline size_t Status_Enabled{3};
        constexpr inline size_t Status_Active{4};
        constexpr inline size_t Status_Focus{5};
        constexpr inline size_t Status_Lock{6};
        constexpr inline size_t Status_MouseIn{7};
        constexpr inline size_t Status_MouseDown{8};
        constexpr inline size_t Status_MouseRightDown{9};
        constexpr inline size_t Status_DefaultRightButton{10};
        constexpr inline size_t Status_FingerIn{11};
        constexpr inline size_t Status_FingerDown{12};
        constexpr inline size_t Status_KeyDown{13};
        constexpr inline size_t Status_HotkeysDown{14};
        constexpr inline size_t Status_Input{15};
        constexpr inline size_t Status_ContextMenu{16};

        enum class Alignment : uint8_t {
            LeftTop,
            LeftCenter,
            LeftBottom,
            Top,
            Center,
            Bottom,
            RightTop,
            RightCenter,
            RightBottom
        };

        enum class FilledMode : uint8_t {
            None,
            Stretch,
            Center,
            Fit,
            Fill
        };

        class AbstractControl {
            template <typename Key>
            std::enable_if_t<std::is_same_v<Key, SDL_Scancode>>
            appendKey(size_t index, Key key) {
                if (index > _hotkeys_list.size()) {
                    Logger::log(Logger::Fatal, "{}({}): The specified index is out of range!",
                            _class_name, _object_name);
                    Engine::throwCustomFatalError<OutOfRangeException>();
                }
                _hotkeys_list.at(index).emplace_back(key);
            }
            template <typename... Keys>
            void addHotKeys(size_t index, SDL_Scancode key, Keys... keys) {
                appendKey(index, key);
                if constexpr (sizeof...(keys)) {
                    addHotKeys(index, keys...);
                } else {
                    std::ranges::sort(_hotkeys_list.at(index));
                }
            }

        public:
            explicit AbstractControl(const std::string_view& class_name, Window* parent = nullptr);
            explicit AbstractControl(const std::string_view& class_name, const std::string_view& object_name, Window* parent = nullptr);
            virtual ~AbstractControl();

            void setObjectName(const std::string_view& object_name);
            [[nodiscard]] const std::string& objectName() const;

            void setGeometry(float x, float y, float width, float height);
            void setGeometry(const Vector2& position, const Size& size);
            void setGeometry(const GeometryF& geometry);
            void move(float x, float y);
            void move(const Vector2& position);
            void resize(float width, float height);
            void resize(const Size& size);
            [[nodiscard]] const GeometryF& geometry() const;
            [[nodiscard]] const Vector2& position() const;
            [[nodiscard]] const Size& size() const;

            void setEnabled(bool enable);
            [[nodiscard]] bool isEnabled() const;
            void setVisible(bool visible);
            [[nodiscard]] bool isVisible() const;
            void setFocus(bool focus);
            [[nodiscard]] bool isFocus() const;
            void setActive(bool active);
            [[nodiscard]] bool isActive() const;
            void setLockedEnabled(bool locked);
            [[nodiscard]] bool isLocked() const;

            void setCursor(const Cursor::StdCursor& cursor);
            [[nodiscard]] Cursor::StdCursor cursor() const;

            template <typename... Keys>
            void setHotkeys(SDL_Scancode key, Keys... keys) {
                _hotkeys_list.emplace_back();
                addHotKeys(_hotkeys_list.size() - 1, key, keys...);
            }
            template <typename... Keys>
            void setHotkeys(size_t index, SDL_Scancode key, Keys... keys) {
                addHotKeys(index, key, keys...);
            }
            void clearHotkeys(size_t index);
            void clearHotkeys();
            void removeHotkey(size_t index);
            [[nodiscard]] const std::vector<SDL_Scancode>& hotkeys(size_t index) const;
            void setRightButtonDefault(bool enabled);
            [[nodiscard]] bool isRightButtonDefault() const;
            [[nodiscard]] bool isLoaded() const;
            void unload();
            void setProperty(const std::string_view& property_name, const Variant& value);
            [[nodiscard]] const Variant& property(const std::string_view& property_name) const;
            void removeProperty(const std::string_view& property_name);
            void clearProperty();
            bool hasProperty(const std::string_view& property_name) const;

        protected:
            virtual void loadEvent() {}
            virtual void unloadEvent() {}
            virtual void propertyChanged(const std::string_view& key) {}
            virtual void enabledEvent() = 0;
            virtual void disabledEvent() = 0;
            virtual void showEvent() = 0;
            virtual void hideEvent() = 0;
            virtual void getFocusEvent() = 0;
            virtual void lostFocusEvent() = 0;
            virtual void activeEvent() = 0;
            virtual void inactiveEvent() = 0;
            virtual void moveEvent(const Vector2& position) = 0;
            virtual void resizeEvent(const Size& size) = 0;
            virtual void paintEvent() = 0;
            virtual void triggeredEvent() = 0;
            virtual void keyDownEvent(SDL_Scancode) = 0;
            virtual void keyUpEvent(SDL_Scancode) = 0;
            virtual void keyPressedEvent(SDL_Scancode) = 0;
            virtual void hotkeyDownEvent(size_t index) = 0;
            virtual void mouseButtonDownEvent(MouseStatus) = 0;
            virtual void mouseButtonUpEvent(MouseStatus) = 0;
            virtual void mouseMovedEvent(const Vector2& position, const Vector2& distance) = 0;
            virtual void mouseMovedInEvent() = 0;
            virtual void mouseMovedOutEvent() = 0;
            virtual void mouseClicked() = 0;
            virtual void customContextMenuRequested() = 0;
            virtual void fingerDownEvent(SDL_FingerID finger_id) = 0;
            virtual void fingerMovedEvent(const Vector2& position, const Vector2& distance) = 0;
            virtual void fingerUpEvent(SDL_FingerID finger_id) = 0;
            virtual void fingerTouchedEvent() = 0;
            virtual void fingerMovedInEvent() = 0;
            virtual void fingerMovedOutEvent() = 0;

            void setStatus(size_t id, bool on);
            bool status(size_t id);
            void setClassName(const std::string_view& class_name);
            [[nodiscard]] const std::string& className() const;
        private:
            void registerEvent();
            Window* _window;
            size_t _event_id;
            std::string _object_name{};
            std::string _class_name{"AbstractControl"};
            GeometryF _geometry{};
            std::bitset<32> _status{};
            Cursor::StdCursor _cursor{};
            bool _is_init{};
            std::vector<std::vector<SDL_Scancode>> _hotkeys_list{};
            std::unordered_map<std::string, Variant> _prop_map{};
        };
    }
}

#endif //MYENGINE_UI_ABSTRACTCONTROL_H