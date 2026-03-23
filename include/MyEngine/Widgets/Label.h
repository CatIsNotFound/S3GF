
#ifndef MYENGINE_WIDGETS_LABEL_H
#define MYENGINE_WIDGETS_LABEL_H
#include "AbstractWidget.h"

#define ENGINE_SIGNAL_LABEL_TEXT_CHANGED                         0b1
#define ENGINE_SIGNAL_LABEL_TEXT_COLOR_CHANGED                   0b10
#define ENGINE_SIGNAL_LABEL_FONT_CHANGED                         0b100
#define ENGINE_SIGNAL_LABEL_FONT_SIZE_CHANGED                    0b1000
#define ENGINE_SIGNAL_LABEL_AUTO_RESIZED_TEXT_CHANGED            0b10000
#define ENGINE_SIGNAL_LABEL_TEXT_ALIGNMENT_CHANGED               0b100000
#define ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_CHANGED             0b1000000
#define ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_FILLED_CHANGED      0b10000000
#define ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_NEED_DELETE         0b100000000
#define ENGINE_SIGNAL_LABEL_SIZE_CHANGED                         0b1000000000

namespace MyEngine {
    namespace Widget {
        class Label : public AbstractWidget {
        public:
            explicit Label(Window* window);
            explicit Label(std::string object_name, Window* window);
            ~Label();

            void setFont(const std::string &font_name, const std::string& font_path, float font_size = 9.f);
            void setFont(const std::string &font_name);
            [[nodiscard]] std::string_view fontName() const;
            [[nodiscard]] std::string_view fontPath() const;

            void setText(const std::string& text);
            [[nodiscard]] const std::string& text() const;
            void appendText(const std::string& text);
            void setTextColor(const SDL_Color& color);
            void setTextColor(uint64_t hex_code, bool alpha = false);
            void setFontSize(float size);
            [[nodiscard]] const SDL_Color& textColor() const;
            [[nodiscard]] float fontSize() const;

            void setTextAlignment(Alignment alignment);
            [[nodiscard]] Alignment textAlignment() const;

            void setAutoResizedByTextEnabled(bool enabled);
            [[nodiscard]] bool isAutoResizedByTextEnabled() const;

            void setBackgroundVisible(bool visible);
            void setBackgroundColor(const SDL_Color& back_color);
            void setBackgroundColor(uint64_t hex_code, bool alpha = false);
            [[nodiscard]] bool backgroundVisible() const;
            [[nodiscard]] const SDL_Color& backgroundColor() const;

            void setBackgroundImage(SDL_Surface* surface, bool delete_later = false);
            void setBackgroundImage(Texture* texture, bool delete_later = false);
            void setBackgroundImage(const std::string& image_path);
            void setBackgroundImageFillMode(ImageFilledMode filled_mode);
            void clearBackgroundImage();
            void setBackgroundImageVisible(bool visible);
            [[nodiscard]] const Texture* const backgroundImage() const;
            [[nodiscard]] ImageFilledMode backgroundImageFilledMode() const;
            [[nodiscard]] bool backgroundImageVisible() const;

        protected:
            void loadEvent() override;
            void unloadEvent() override;
            void paintEvent(MyEngine::Renderer *renderer) override;
            void moveEvent(const Vector2 &position) override;
            void resizeEvent(const MyEngine::Size &size) override;
            void visibleChangedEvent(bool visible) override;
            void enableChangedEvent(bool enabled) override;

        private:
            void updateBgIMGGeometry();
            void updateTextGeometry();
        private:
            uint64_t _text_id{0};
            bool _visible_bg{true};
            bool _visible_text{};
            bool _visible_img{};
            bool _auto_resize_by_text{};
            Font* _font{};
            TextSystem::Text* _text{};
            std::unique_ptr<Texture> _bg_img{};
            std::string _string{};
            Vector2 _text_pos{};
            uint16_t _changer_signal{};
            ImageFilledMode _fill_mode{};
            Alignment _alignment{};
        };
    }
}

#endif //MYENGINE_WIDGETS_LABEL_H
