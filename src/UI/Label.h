
#ifndef MYENGINE_UI_LABEL_H
#define MYENGINE_UI_LABEL_H
#include "AbstractControl.h"

namespace MyEngine::UI {
    inline constexpr const char* LabelProp_TextID{"__Label_TextID__"};
    inline constexpr const char* LabelProp_TextPos{"__Label_TextPosition__"};
    inline constexpr const char* LabelProp_Font_Ptr{"__Label_Font_Ptr__"};
    inline constexpr const char* LabelProp_Text_Alignment{"__Label_Text_Alignment__"};
    inline constexpr const char* LabelProp_Image_FilledMode{"__Label_Image_FillMode__"};

    class Label : public AbstractControl {
    public:
        explicit Label(const std::string_view& object_name, Window* parent = nullptr);
        explicit Label(Window* parent = nullptr);
        void setFont(Font* font);
        void setFont(const std::string_view& font_name);
        void setFont(const std::string_view& font_name, const std::string_view& path, float size = 9.f);
        [[nodiscard]] std::optional<Font*> font() const;
        void setText(const std::string_view& text);
        void appendText(const std::string_view& text);
        [[nodiscard]] const std::string& text() const;
        void setTextColor(const SDL_Color& color);
        [[nodiscard]] const SDL_Color& textColor() const;
        void setBackgroundColor(const SDL_Color& color);
        void setBackgroundColor(uint64_t hex_code, bool alpha = false);
        void setBorderColor(const SDL_Color& color);
        void setBorderColor(uint64_t hex_code, bool alpha = false);
        [[nodiscard]] const SDL_Color& backgroundColor() const;
        [[nodiscard]] const SDL_Color& borderColor() const;
        void setBackgroundImage(SDL_Surface *surface, bool delete_later = false);
        void setBackgroundImage(const std::shared_ptr<Texture>& texture);
        [[nodiscard]] std::optional<Texture*> backgroundImage() const;
        [[nodiscard]] std::optional<TextureProperty*> backgroundImageProperty() const;
        void setImageFilledMode(FilledMode mode);
        [[nodiscard]] FilledMode imageFilledMode() const;
        void setTextAlignment(Alignment alignment);
        [[nodiscard]] Alignment textAlignment() const;
    protected:
        void loadEvent() override;
        void unloadEvent() override;
        void enabledEvent() override;
        void disabledEvent() override;
        void showEvent() override;
        void hideEvent() override;
        void getFocusEvent() override;
        void lostFocusEvent() override;
        void activeEvent() override;
        void inactiveEvent() override;
        void moveEvent(const Vector2& position) override;
        void resizeEvent(const Size& size) override;
        void paintEvent() override;
        void triggeredEvent() override;
        void keyDownEvent(SDL_Scancode) override;
        void keyUpEvent(SDL_Scancode) override;
        void keyPressedEvent(SDL_Scancode) override;
        void hotkeyDownEvent(size_t index) override;
        void mouseButtonDownEvent(MouseStatus) override;
        void mouseButtonUpEvent(MouseStatus) override;
        void mouseMovedEvent(const Vector2& position, const Vector2& distance) override;
        void mouseMovedInEvent() override;
        void mouseMovedOutEvent() override;
        void mouseClicked() override;
        void customContextMenuRequested() override;
        void fingerDownEvent(SDL_FingerID finger_id) override;
        void fingerMovedEvent(const Vector2& position, const Vector2& distance) override;
        void fingerUpEvent(SDL_FingerID finger_id) override;
        void fingerTouchedEvent() override;
        void fingerMovedInEvent() override;
        void fingerMovedOutEvent() override;
    private:
        std::shared_ptr<Texture> _texture{};
        std::shared_ptr<Graphics::Rectangle> _rectangle{};
        std::string _text{};
    };

    inline constexpr uint32_t Label_Font_Type_ID{76979865};
}

#endif //MYENGINE_UI_LABEL_H