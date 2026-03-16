
#ifndef MYENGINE_UI_LABEL_H
#define MYENGINE_UI_LABEL_H
#include "AbstractControl.h"

namespace MyEngine::UI {
    class Label : public AbstractControl {
    public:
        explicit Label(const std::string_view& object_name, Window* parent = nullptr)
                : AbstractControl(object_name, parent) { Label::loadEvent(); }
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
        [[nodiscard]] const SDL_Color& backgroundColor() const;
        void setBackgroundImage(const SDL_Surface* surface, bool delete_later = false);
        void setBackgroundImage(const std::shared_ptr<Texture>& texture);
        [[nodiscard]] const std::optional<Texture*> backgroundImage() const;
        [[nodiscard]] const std::optional<TextureProperty*> backgroundImageProperty() const;
        void setImageFilledMode(FilledMode mode);

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
        uint64_t _text_id{};
        Font* _font{};
        std::shared_ptr<Texture> _texture{};
        std::shared_ptr<Graphics::Rectangle> _rectangle{};
        Alignment _text_alignment{};
        FilledMode _filled_mode{};
    };
}

#endif //MYENGINE_UI_LABEL_H