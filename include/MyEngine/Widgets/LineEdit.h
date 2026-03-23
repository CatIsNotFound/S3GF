#pragma once
#ifndef MYENGINE_WIDGETS_LINEEDIT_H
#define MYENGINE_WIDGETS_LINEEDIT_H
#include "AbstractWidget.h"
#include "../Utils/RGBAColor.h"
#include "../Algorithm/String.h"

#define ENGINE_PROP_LINE_EDIT_BORDER_COLOR_INPUT                   "lineEdit.input.borderColor"
#define ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_INPUT               "lineEdit.input.backgroundColor"
#define ENGINE_PROP_LINE_EDIT_BORDER_COLOR_NORMAL                  "lineEdit.normal.borderColor"
#define ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_NORMAL              "lineEdit.normal.backgroundColor"
#define ENGINE_PROP_LINE_EDIT_BORDER_COLOR_HOVERED                 "lineEdit.normal.borderColor"
#define ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_HOVERED             "lineEdit.normal.backgroundColor"
#define ENGINE_PROP_LINE_EDIT_BORDER_COLOR_ACTIVE                  "lineEdit.active.borderColor"
#define ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_ACTIVE              "lineEdit.active.backgroundColor"
#define ENGINE_PROP_LINE_EDIT_BORDER_COLOR_DISABLED                "lineEdit.disabled.borderColor"
#define ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_DISABLED            "lineEdit.disabled.backgroundColor"
#define ENGINE_PROP_LINE_EDIT_TEXT_COLOR                           "lineEdit.text.color"
#define ENGINE_PROP_LINE_EDIT_TEXT                                 "lineEdit.text.text"
#define ENGINE_PROP_LINE_EDIT_PASSWORD                             "lineEdit.text.password"
#define ENGINE_PROP_LINE_EDIT_PASSWORD_LENGTH                      "lineEdit.text.password.length"
#define ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT                     "lineEdit.placeholder.text"
#define ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR              "lineEdit.placeholder.color"
#define ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL                   "lineEdit.paddingHorizontal.horizontal"
#define ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL                     "lineEdit.paddingHorizontal.vertical"

#define ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE             0b1
#define ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE                        0b10
#define ENGINE_BOOL_LINE_EDIT_HAS_TEXT                             0b100
#define ENGINE_BOOL_LINE_EDIT_CURSOR_VISIBLE                       0b1000
#define ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE                   0b10000
#define ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE                       0b100000

#define ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED                       0b1
#define ENGINE_SIGNAL_LINE_EDIT_PASSWORD_CHANGED                   0b10
#define ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_CHANGED           0b100
#define ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_COLOR_CHANGED     0b1000
#define ENGINE_SIGNAL_LINE_EDIT_FONT_CHANGED                       0b10000
#define ENGINE_SIGNAL_LINE_EDIT_FONT_SIZE_CHANGED                  0b100000
#define ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED                 0b1000000
#define ENGINE_SIGNAL_LINE_EDIT_TEXT_SIZE_CHANGED                  0b10000000

namespace MyEngine {
    namespace Widget {
        class LineEdit : public AbstractWidget {
        public:
            explicit LineEdit(Window* window);
            explicit LineEdit(std::string object_name, Window* window);
            ~LineEdit();

            void setFont(const std::string &font_name, const std::string& font_path, float font_size = 9.f);
            void setFont(const std::string &font_name);
            [[nodiscard]] std::string_view fontName() const;
            [[nodiscard]] std::string_view fontPath() const;
            void setFontSize(float size);
            [[nodiscard]] float fontSize() const;

            void setText(const std::string& text);
            [[nodiscard]] std::string_view text() const;
            [[nodiscard]] const StringList& textList() const;
            void setPlaceHolderText(const std::string& text);
            [[nodiscard]] std::string_view placeHolderText() const;
            void setPasswordEnabled(bool enabled, const char* secret = ((char*)u8"\u25CF"));
            [[nodiscard]] bool passwordEnabled() const;

            void setTextColor(const SDL_Color& color);
            void setPlaceHolderTextColor(const SDL_Color& color);
            [[nodiscard]] const SDL_Color& textColor() const;
            [[nodiscard]] const SDL_Color& placeHolderTextColor() const;

            void setBackgroundColor(const SDL_Color& color, WidgetStatus status = WidgetStatus::Normal);
            void setBorderColor(const SDL_Color& color, WidgetStatus status = WidgetStatus::Normal);
            [[nodiscard]] const SDL_Color& backgroundColor(WidgetStatus status = WidgetStatus::Normal) const;
            [[nodiscard]] const SDL_Color& borderColor(WidgetStatus status = WidgetStatus::Normal) const;
            void setBackgroundVisible(bool visible);
            void setBorderVisible(bool visible);
            [[nodiscard]] bool backgroundVisible() const;
            [[nodiscard]] bool borderVisible() const;

            void setHorizontalPadding(int value);
            void setVerticalPadding(int value);
            void setPadding(int value);
            void setPadding(int h, int v);
            [[nodiscard]] int horizontalPadding() const;
            [[nodiscard]] int verticalPadding() const;

            void setTextCursorStyle(uint8_t size, const SDL_Color& color = StdColor::Black);
            void setTextCursorStyle(uint8_t size, uint64_t hex_code, bool alpha = false);
            [[nodiscard]] uint8_t textCursorSize() const;
            [[nodiscard]] const SDL_Color& textCursorColor() const;
        protected:
            void loadEvent() override;
            void unloadEvent() override;
            void focusInEvent() override;
            void focusOutEvent() override;
            void paintEvent(MyEngine::Renderer *renderer) override;
            void enableChangedEvent(bool enabled) override;
            void resizeEvent(const MyEngine::Size &size) override;
            void keyDownEvent(SDL_Scancode scancode) override;
            void keyPressedEvent(SDL_Scancode scancode) override;
            void mouseEnteredEvent() override;
            void mouseLeftEvent() override;
            void mouseClickedEvent() override;
            void fingerTappedEvent() override;
            void startedInputEvent() override;
            void endedInputEvent() override;
            void inputEvent(const char *string) override;
            virtual void textChangedEvent();

        private:
            void init();
            void updateStatus(WidgetStatus status);
            void updateTextPosition();
            void updateText();
            static std::string getBackgroundColorPropertyKey(WidgetStatus status);
            static std::string getBorderColorPropertyKey(WidgetStatus status);
            SDL_Color getBackgroundColor(WidgetStatus status);
            SDL_Color getBorderColor(WidgetStatus status);
        private:
            uint64_t _text_id{}, _place_text_id{};
            Font* _font{};
            TextSystem::Text* _text{}, *_place_text{};
            char _secret_char[8]{};
            StringList _strings{};
            Vector2 _text_pos{};
            uint16_t _changer_signal{};
            WidgetStatus _wid_status{};
            Geometry _real_area{};
            uint8_t _status{};
            Graphics::Line _cursor_line{};
            uint64_t _start_tick{};
        };
    }
}

#endif //MYENGINE_WIDGETS_LINEEDIT_H
