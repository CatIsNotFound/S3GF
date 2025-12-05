
#ifndef S3GF_SPRITE_H
#define S3GF_SPRITE_H
#include "../Components.h"

namespace S3GF {
    class Sprite {
    public:
        explicit Sprite(Texture* texture = nullptr);
        ~Sprite();

        void setTexture(Texture* new_texture);
        [[nodiscard]] Texture* texture() const;

        void setPosition(float x, float y);
        void setPosition(const Vector2& position);
        [[nodiscard]] const Vector2& position() const;

        void setSize(float w, float h);
        void setSize(const Size& size);
        [[nodiscard]] const Size& size() const;

        void setScale(float scale);
        [[nodiscard]] float scale() const;

        void setRotation(double rotation);
        [[nodiscard]] double rotation() const;

        void setOpacity(float opcacity);
        [[nodiscard]] float opacity() const;

        void setVisible(bool visible);
        [[nodiscard]] bool visible() const;

        void setFlipMode(SDL_FlipMode flip_mode);
        [[nodiscard]] SDL_FlipMode flipMode() const;

        void draw() const;
    private:
        Texture* _texture;
        std::unique_ptr<TextureProperty> _property;
        bool _visible{true};
    };
}

#endif //S3GF_SPRITE_H
