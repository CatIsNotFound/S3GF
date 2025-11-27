#pragma once
#ifndef S3GF_COMPONETS_H
#define S3GF_COMPONETS_H
#include "Basic.h"
#include "Utils/Random.h"

namespace S3GF {
    class Renderer;
    struct Property {
        bool clip_mode;
        SDL_FRect clip_area;
        SDL_Color color_alpha;
        double rotate_angle;
        void reset(const Property& property) {
            _position = property._position;
            _size = property._size;
            _scale = property._scale;
            _scaled_position = property._scaled_position;
            _scaled_size = property._scaled_size;
            clip_mode = property.clip_mode;
            clip_area = property.clip_area;
            color_alpha = property.color_alpha;
            rotate_angle = property.rotate_angle;
        }
        void reset(Property&& property) {
            _position = property._position;
            _size = property._size;
            _scale = property._scale;
            _scaled_position = property._scaled_position;
            _scaled_size = property._scaled_size;
            clip_mode = property.clip_mode;
            clip_area = property.clip_area;
            color_alpha = property.color_alpha;
            rotate_angle = property.rotate_angle;
        }

        void move(const Vector2& pos) {
            _position.reset(pos);
            setScale(_scale);
        }
        void move(float x, float y) {
            _position.reset(x, y);
            setScale(_scale);
        }
        [[nodiscard]] const Vector2 position() const {
            return _position;
        }
        void resize(const Size& size) {
            _size.reset(size);
            setScale(_scale);
        }
        void resize(float width, float height) {
            _size.reset(width, height);
            setScale(_scale);
        }
        [[nodiscard]] const Size size() const {
            return _size;
        }
        void setGeomentry(const Vector2& pos, const Size& size) {
            _position.reset(pos);
            _size.reset(size);
            setScale(_scale);
        }
        void setGeomentry(float x, float y, float width, float height) {
            _position.reset(x, y);
            _size.reset(width, height);
            setScale(_scale);
        }
        [[nodiscard]] const GeometryF geomentry() const {
            return GeometryF{_position, _size};
        }
        void setScale(float scale = 1.0f) {
            _scale = scale;
            Vector2 global_center_pos = {
                _position.x + _size.width / 2, _position.y + _size.height / 2
            };
            _scaled_position.reset((_position.x - global_center_pos.x) * _scale + global_center_pos.x, 
                                    (_position.y - global_center_pos.y) * _scale + global_center_pos.y);
            _scaled_size.reset(_size.width * _scale, _size.height * _scale);
        }
        [[nodiscard]] float scale() const {
            return _scale;
        }
        [[nodiscard]] GeometryF scaledGeometry() const {
            return GeometryF{_scaled_position, _scaled_size};
        }
    private:
        Vector2 _position;
        Size _size;
        float _scale;
        Vector2 _scaled_position;
        Size _scaled_size;
    };

    class Texture {
    public:
        Texture(const Texture &) = delete;
        Texture(Texture &&) = delete;
        Texture &operator=(const Texture &) = delete;
        Texture &operator=(Texture &&) = delete;
        explicit Texture(const std::string &path, Renderer *renderer);
        explicit Texture(SDL_Surface* surface, Renderer *renderer, bool deep_copy = false);
        explicit Texture(Renderer* renderer, SDL_PixelFormat format, int width, int height, SDL_TextureAccess access);
        ~Texture();

        Renderer* render() const;

        bool setImagePath(const std::string& path);
        [[nodiscard]] const std::string& imagePath() const;
        bool setImageFromSurface(SDL_Surface* surface, bool deep_copy = false);

        [[nodiscard]] SDL_Texture* self() const;
        [[nodiscard]] bool isValid() const;
        Property* property();

        virtual void draw() const;
    private:
        SDL_Surface* _surface;
        SDL_Texture* _texture;
        std::string _path;
        std::unique_ptr<Property> _property;
        Renderer* _renderer;
    };

    class TextureAtlas : public Texture {
    public:
        TextureAtlas(const TextureAtlas &) = delete;
        TextureAtlas(TextureAtlas &&) = delete;
        TextureAtlas &operator=(const TextureAtlas &) = delete;
        TextureAtlas &operator=(TextureAtlas &&) = delete;

        explicit TextureAtlas(const std::string &path, Renderer *renderer);
        explicit TextureAtlas(SDL_Surface* surface, Renderer *renderer, bool deep_copy = false);
        ~TextureAtlas();

        void setTiles(const std::string& tiles_name, Property&& tiles_property);
        Property* tilesProperty(const std::string& tiles_name);
        void setCurrentTiles(const std::string& tiles_name);
        [[nodiscard]] const std::string& currentTiles() const;

        void draw() const override;
        void draw(const std::string& tiles_name) const;
    private:
        std::unordered_map<std::string, std::shared_ptr<Property>> _tiles_map;
        std::string _current_tiles;
    };
}
#include "Core.h"
#endif // !S3GF_COMPONETS_H