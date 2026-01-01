
#ifndef MYENGINE_WIDGETS_BUTTON_H
#define MYENGINE_WIDGETS_BUTTON_H
#include "Label.h"

#define BACKGROUND_IMAGE_STATUS_NORMAL    "BackgroundImage.status.normal"
#define BACKGROUND_IMAGE_STATUS_ACTIVE    "BackgroundImage.status.active"
#define BACKGROUND_IMAGE_STATUS_DISABLED  "BackgroundImage.status.disabled"
#define BACKGROUND_IMAGE_STATUS_HOVERED   "BackgroundImage.status.hovered"
#define BACKGROUND_IMAGE_STATUS_PRESSED   "BackgroundImage.status.pressed"
#define BACKGROUND_IMAGE_STATUS_CHECKED   "BackgroundImage.status.checked"

#define TEXT_COLOR_STATUS_NORMAL          "TextColor.status.normal"
#define TEXT_COLOR_STATUS_ACTIVE          "TextColor.status.active"
#define TEXT_COLOR_STATUS_DISABLED        "TextColor.status.disabled"
#define TEXT_COLOR_STATUS_HOVERED         "TextColor.status.hovered"
#define TEXT_COLOR_STATUS_PRESSED         "TextColor.status.pressed"
#define TEXT_COLOR_STATUS_CHECKED         "TextColor.status.checked"

#define BACKGROUND_COLOR_STATUS_NORMAL      "BackgroundColor.status.normal"
#define BACKGROUND_COLOR_STATUS_ACTIVE      "BackgroundColor.status.active"
#define BACKGROUND_COLOR_STATUS_DISABLED    "BackgroundColor.status.disabled"
#define BACKGROUND_COLOR_STATUS_HOVERED     "BackgroundColor.status.hovered"
#define BACKGROUND_COLOR_STATUS_PRESSED     "BackgroundColor.status.pressed"
#define BACKGROUND_COLOR_STATUS_CHECKED     "BackgroundColor.status.checked"


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
            void setBackgroundImage(WidgetStatus status, Texture* texture, bool delete_later = false);
            void setBackgroundColor(WidgetStatus status, const SDL_Color& color);
            void setTextColor(WidgetStatus status, const SDL_Color& color);
            [[nodiscard]] const Texture* backgroundImage(WidgetStatus status) const;
            [[nodiscard]] const SDL_Color& backgroundColor(WidgetStatus status) const;
            [[nodiscard]] const SDL_Color& textColor(WidgetStatus status) const;

        protected:
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
            void keyPressedEvent() override;
            void hotKeysPressedEvent() override;
            void fingerDownEvent(const MyEngine::Vector2 &position) override;
            void fingerUpEvent(const MyEngine::Vector2 &position) override;
            void fingerMoveInEvent() override;
            void fingerMoveOutEvent() override;
            void fingerTappedEvent() override;
            void checkedChanged(bool checked);
        private:
            void initStatus();
            void updateStatus();
        private:
            WidgetStatus _wid_status{};
            bool _checkable{}, _checked{};
            std::function<void()> _event{};
        };
    }
}

#endif //MYENGINE_WIDGETS_BUTTON_H
