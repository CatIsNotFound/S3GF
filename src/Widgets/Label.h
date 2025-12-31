
#ifndef MYENGINE_WIDGETS_LABEL_H
#define MYENGINE_WIDGETS_LABEL_H
#include "AbstractWidget.h"
#define LABEL_TEXT_POSITION "Label.textPosition"
#define LABEL_IMAGE_GEOMETRY "Label.imageGeometry"
namespace MyEngine {
    namespace Widget {
        class Label : public AbstractWidget {
        public:
            enum ImageFilledMode {
                Stretch,
                Center,
                Fill
            };
            enum Alignment {
                LeftTop,
                CenterTop,
                RightTop,
                LeftMiddle,
                CenterMiddle,
                RightMiddle,
                LeftBottom,
                CenterBottom,
                RightBottom
            };
            explicit Label(Window* window);
            explicit Label(std::string object_name, Window* window);
            ~Label();

            void setFont(const std::string &font_name, const std::string& font_path, float font_size = 9.f);
            void setFont(const std::string &font_name);
            [[nodiscard]] const std::string& fontName() const;
            [[nodiscard]] const std::string& fontPath() const;

            void setText(const std::string& text);
            [[nodiscard]] const std::string& text() const;
            void appendText(const std::string& text);
            void setFontColor(const SDL_Color& color);
            void setFontColor(uint64_t hex_code, bool alpha = false);
            void setFontSize(float size);
            [[nodiscard]] const SDL_Color& fontColor() const;
            [[nodiscard]] float fontSize() const;

            void setBackgroundVisible(bool visible);
            void setBackgroundColor(const SDL_Color& back_color);
            void setBackgroundColor(uint64_t hex_code, bool alpha = false);
            [[nodiscard]] bool backgroundVisible() const;
            [[nodiscard]] const SDL_Color& backgroundColor() const;

            void setBackgroundImage(SDL_Surface* surface, bool delete_later = true);
            void setBackgroundImage(Texture* texture, bool delete_later = false);
            void setBackgroundImage(const std::string& image_path);
            void setBackgroundImageFillMode(ImageFilledMode filled_mode);
            [[nodiscard]] const Texture* const backgroundImage() const;
            [[nodiscard]] ImageFilledMode backgroundImageFilledMode() const;

        protected:
            void loadEvent() override;
            void unloadEvent() override;
            void paintEvent(MyEngine::Renderer *renderer) override;
            void moveEvent(const MyEngine::Vector2 &position) override;
            void resizeEvent(const MyEngine::Size &size) override;
            void visibleChangedEvent(bool visible) override;
            void enableChangedEvent(bool enabled) override;
            void imageFillModeChangedEvent(ImageFilledMode filled_mode);
            void alignmentChangedEvent(Alignment alignment);
        private:
            void updateBgIMGGeometry();
            void updateTextGeometry();
        private:
            uint64_t _text_id{0};
            bool _visible_bg{true};
            bool _visible_text{};
            Font* _font{};
            TextSystem::Text* _text{};
            std::shared_ptr<Texture> _bg_img{}, _img{};
            SDL_Color _none_color{};
            std::string _none_str{}, _string{};
            ImageFilledMode _fill_mode{Stretch};
            Alignment _alignment{LeftTop};
            bool _delete_font{};
            bool _delete_bg_img{};
            bool _delete_img{};
            bool _remove_text{};
        };
    }
}

#endif //MYENGINE_WIDGETS_LABEL_H
