
#ifndef MYENGINE_WIDGETS_LABEL_H
#define MYENGINE_WIDGETS_LABEL_H
#include "AbstractWidget.h"
namespace MyEngine {
    namespace Widget {
        class Label : public AbstractWidget {
        public:
            explicit Label(Window* window);
            ~Label();

            void setFont(const std::string& font_path, float font_size = 9.f);
            void setFontFromTextSystem(const std::string &font_name);
            [[nodiscard]] const std::string& fontPath() const;
            [[nodiscard]] Font* font();

            void setText(const std::string& text);
            [[nodiscard]] const std::string& text() const;

            void setImage(SDL_Surface* surface, bool delete_later = true);
            void setImage(Texture* texture, bool delete_later = false);
            void setImageAnimation(TextureAnimation* animation, bool delete_later = false);
            [[nodiscard]] const Texture* const image() const;
            [[nodiscard]] const TextureAnimation* const imageAnimation() const;

        protected:
            void loadEvent() override;
            void unloadEvent() override;
            void paintEvent(MyEngine::Renderer *renderer) override;
            void moveEvent(const MyEngine::Vector2 &position) override;
            void resizeEvent(const MyEngine::Size &size) override;
            void visibleChangedEvent(bool visible) override;
            void enableChangedEvent(bool enabled) override;
        private:
            std::string _text{};
            std::string _font_path{};
            uint64_t _text_id{0};
            Font* _font{};
            std::unique_ptr<Texture> _text_self{};
            Texture* _texture{};
            TextureAnimation* _texture_ani{};
            SDL_Color _none_color{};
            bool _delete_font{};
            bool _delete_later{};
        };
    }
}

#endif //MYENGINE_WIDGETS_LABEL_H
