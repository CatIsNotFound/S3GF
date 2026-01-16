
#ifndef MYENGINE_WIDGETS_BUTTON_H
#define MYENGINE_WIDGETS_BUTTON_H
#include "Label.h"

#define ENGINE_PROP_BACKGROUND_IMAGE_STATUS_NORMAL    "backgroundImage.status.normal"
#define ENGINE_PROP_BACKGROUND_IMAGE_STATUS_ACTIVE    "backgroundImage.status.active"
#define ENGINE_PROP_BACKGROUND_IMAGE_STATUS_DISABLED  "backgroundImage.status.disabled"
#define ENGINE_PROP_BACKGROUND_IMAGE_STATUS_HOVERED   "backgroundImage.status.hovered"
#define ENGINE_PROP_BACKGROUND_IMAGE_STATUS_PRESSED   "backgroundImage.status.pressed"
#define ENGINE_PROP_BACKGROUND_IMAGE_STATUS_CHECKED   "backgroundImage.status.checked"

#define ENGINE_PROP_TEXT_COLOR_STATUS_NORMAL          "textColor.status.normal"
#define ENGINE_PROP_TEXT_COLOR_STATUS_ACTIVE          "textColor.status.active"
#define ENGINE_PROP_TEXT_COLOR_STATUS_DISABLED        "textColor.status.disabled"
#define ENGINE_PROP_TEXT_COLOR_STATUS_HOVERED         "textColor.status.hovered"
#define ENGINE_PROP_TEXT_COLOR_STATUS_PRESSED         "textColor.status.pressed"
#define ENGINE_PROP_TEXT_COLOR_STATUS_CHECKED         "textColor.status.checked"

#define ENGINE_PROP_BACKGROUND_COLOR_STATUS_NORMAL      "backgroundColor.status.normal"
#define ENGINE_PROP_BACKGROUND_COLOR_STATUS_ACTIVE      "backgroundColor.status.active"
#define ENGINE_PROP_BACKGROUND_COLOR_STATUS_DISABLED    "backgroundColor.status.disabled"
#define ENGINE_PROP_BACKGROUND_COLOR_STATUS_HOVERED     "backgroundColor.status.hovered"
#define ENGINE_PROP_BACKGROUND_COLOR_STATUS_PRESSED     "backgroundColor.status.pressed"
#define ENGINE_PROP_BACKGROUND_COLOR_STATUS_CHECKED     "backgroundColor.status.checked"


namespace MyEngine {
    namespace Widget {
        class Button : public Label {
        public:
            explicit Button(std::string object_name, Window* window);
            explicit Button(Window* window);
            ~Button();

            void setChecked(bool checked);
            [[nodiscard]] bool isChecked() const;
            void setCheckable(bool checkable);
            [[nodiscard]] bool isCheckable() const;

            void setTriggerEvent(const std::function<void()>& event);
            void setTriggerEvent(TriggerAction action, std::function<void()> event);
            void setBackgroundImage(WidgetStatus status, Texture* texture, bool delete_later = false);
            void setBackgroundColor(WidgetStatus status, const SDL_Color& color);
            void setTextColor(WidgetStatus status, const SDL_Color& color);
            [[nodiscard]] const Texture* backgroundImage(WidgetStatus status) const;
            [[nodiscard]] const SDL_Color& backgroundColor(WidgetStatus status) const;
            [[nodiscard]] const SDL_Color& textColor(WidgetStatus status) const;

        protected:
            void loadEvent() override;
            void focusInEvent() override;
            void focusOutEvent() override;
            void mouseEnteredEvent() override;
            void mouseLeftEvent() override;
            void mouseDownEvent() override;
            void mouseUpEvent() override;
            void mouseClickedEvent() override;
            void mouseDblClickedEvent() override;
            void keyDownEvent(SDL_Scancode scancode) override;
            void keyUpEvent(SDL_Scancode scancode) override;
            void keyPressedEvent(SDL_Scancode scancode) override;
            void hotKeysPressedEvent() override;
            void fingerDownEvent(const Vector2 &position) override;
            void fingerUpEvent(const Vector2 &position) override;
            void fingerMoveInEvent() override;
            void fingerMoveOutEvent() override;
            void fingerTappedEvent() override;
            void checkedChanged(bool checked);
            void enableChangedEvent(bool enabled) override;
        private:
            void init();
            void updateStatus(WidgetStatus status);
            bool isConfirmKey(SDL_Scancode scancode) {
                return scancode == SDL_SCANCODE_SPACE ||
                       scancode == SDL_SCANCODE_RETURN ||
                       scancode == SDL_SCANCODE_RETURN2 ||
                       scancode == SDL_SCANCODE_KP_ENTER;
            }

            [[nodiscard]] std::string getBackgroundImagePropertyKey(WidgetStatus status) const;
            [[nodiscard]] static std::string getBackgroundColorPropertyKey(WidgetStatus status) ;
            [[nodiscard]] static std::string getTextColorPropertyKey(WidgetStatus status) ;
            [[nodiscard]] SDL_Color getBackgroundColorForStatus(WidgetStatus status) const;
            [[nodiscard]] SDL_Color getTextColorForStatus(WidgetStatus status) const;
            [[nodiscard]] Texture* getBackgroundImageForStatus(WidgetStatus status) const;

            bool _checkable{}, _checked{};
            std::function<void()> _event{};
            std::unordered_map<TriggerAction, std::function<void()>> _triggers;
            std::array<TriggerAction, 8> _m_triggers;
        };
    }
}

#endif //MYENGINE_WIDGETS_BUTTON_H
