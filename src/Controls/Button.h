
#ifndef S3GF_BUTTON_H
#define S3GF_BUTTON_H
#include "../Basic.h"
#include "Control.h"
namespace S3GF {

    class Button : public AbstractControl {
    public:
        struct ButtonSkin {
            ColorStatus border;
            ColorStatus background;
            ColorStatus font;
        };
        explicit Button(const std::string& name, Renderer* renderer,
                        AbstractControl* parent = nullptr);

        void setFont(const std::string &font_name, const std::string& font_path = {});
        [[nodiscard]] const std::string& fontName() const;
        void setText(const std::string &text);
        [[nodiscard]] const std::string& text() const;

        [[nodiscard]] Size textSize() const;
        ButtonSkin& buttonSkin();
        void setEvent(const std::function<void()>& event);

    protected:
        void onEntered() override;
        void onHovered() override;
        void onLeft() override;
        void onPressed() override;
        void onReleased() override;
        void onTriggered() override;
        void onKeyPressed() override;
        void onKeyReleased() override;
        void resizeEvent() override;
        void moveEvent() override;
        void onChecked(bool checked) override;
        void enabledChanged(bool enabled) override;
        void paintEvent(Renderer* r) override;

    private:
        Graphics::Rectangle _rect;
        std::string _font_name;
        std::string _btn_text;
        float _font_size{0};
        SDL_Color _font_color{StdColor::Black};
        ButtonSkin _btn_skin;
        Vector2 _text_pos{0, 0};
        bool _is_valid;
        uint64_t id{0};
        std::function<void()> _event;
    };

    class TextureButton : public AbstractControl {
    public:
        struct TextureStatus {
            Texture* normal;
            Texture* active;
            Texture* pressed;
            Texture* invalid;
            Texture* checked;
        };

        explicit TextureButton(const std::string& name, Renderer* renderer, AbstractControl* parent = nullptr);
        ~TextureButton();

        void setTextures(Texture* normal, Texture* active = nullptr,
                         Texture* pressed = nullptr, Texture* invalid = nullptr, Texture* checked = nullptr);

        void setFont(const std::string &font_name, const std::string& font_path = {});
        [[nodiscard]] const std::string& fontName() const;
        void setText(const std::string &text);
        [[nodiscard]] const std::string& text() const;
        [[nodiscard]] Size textSize() const;
        void setTextVisible(bool visible);
        [[nodiscard]] bool isTextVisible() const;
        void setTextPosition(const Vector2& pos);
        [[nodiscard]] const Vector2& textPosition() const;
        [[nodiscard]] ColorStatus& textColor();
        void setEvent(const std::function<void()>& event);

    protected:
        void onEntered() override;
        void onHovered() override;
        void onLeft() override;
        void onPressed() override;
        void onReleased() override;
        void onTriggered() override;
        void onKeyPressed() override;
        void onKeyReleased() override;
        void resizeEvent() override;
        void moveEvent() override;
        void onChecked(bool checked) override;
        void enabledChanged(bool enabled) override;
        void paintEvent(Renderer* r) override;

    private:
        Graphics::Rectangle _rect;
        std::string _font_name;
        std::string _btn_text;
        float _font_size{0};
        bool _textures_loaded{false}, _font_loaded{false};
        Vector2 _text_pos{};
        bool _text_visible{false};
        ColorStatus _font{StdColor::Black, StdColor::Black,
                          StdColor::Black, StdColor::LightGray};
        std::unique_ptr<TextureStatus> _textures;
        std::function<void()> _event;
        Texture* _cur_texture{nullptr};
    };

} // S3GF

#endif //S3GF_BUTTON_H
