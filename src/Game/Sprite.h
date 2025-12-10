
#ifndef MYENGINE_GAME_SPRITE_H
#define MYENGINE_GAME_SPRITE_H
#include "../Components.h"

namespace MyEngine {
    class Sprite {
    public:
        explicit Sprite(Texture* texture = nullptr);
        explicit Sprite(const std::string& path, Renderer* renderer);
        ~Sprite();

        void setTexture(Texture* new_texture);
        [[nodiscard]] Texture* texture() const;

        void move(float x, float y);
        void move(const Vector2& position);
        [[nodiscard]] const Vector2& position() const;

        void resize(float w, float h);
        void resize(const Size& size);
        [[nodiscard]] const Size& size() const;

        void setScale(float scale);
        [[nodiscard]] float scale() const;

        void setRotation(double rotation);
        [[nodiscard]] double rotation() const;

        void setOpacity(float opacity);
        [[nodiscard]] float opacity() const;

        void setColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void setColorAlpha(uint64_t hex_code);
        void setColorAlpha(const SDL_Color& color);
        [[nodiscard]] const SDL_Color& colorAlpha() const;

        void setVisible(bool visible);
        [[nodiscard]] bool visible() const;

        void setFlipMode(SDL_FlipMode flip_mode);
        [[nodiscard]] SDL_FlipMode flipMode() const;

        void draw() const;
    private:
        Texture* _texture;
        std::unique_ptr<TextureProperty> _property;
        bool _visible{true};
        bool _delete_later{false};
    };
}

#endif //MYENGINE_GAME_SPRITE_H
