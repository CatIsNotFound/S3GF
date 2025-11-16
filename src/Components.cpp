#include "Components.h"

namespace S3GF {
    Texture::Texture(const std::string &path, Renderer *renderer) : _renderer(renderer) {
        _surface = IMG_Load(path.c_str());
        if (!_surface) {
            Logger::log(std::format("The image path '{}' is not found!", path), Logger::ERROR);
            _property = std::make_unique<Property>();
            return;
        }
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<Property>();
        _property->resize(_surface->w, _surface->h);
        _property->clip_mode = false;
        _property->color_alpha = StdColor::White;
        _property->setScale(1.0f);
        Logger::log(std::format("Texture created from image path '{}'", path), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
    }

    Texture::Texture(SDL_Surface* surface, Renderer *renderer, bool deep_copy) : _renderer(renderer) {
        if (!_surface) {
            Logger::log(std::format("The surface is not valid!\nException: {}", SDL_GetError()), Logger::ERROR);
            _property = std::make_unique<Property>();
            return;
        }
        _surface = (deep_copy ? SDL_DuplicateSurface(surface) : surface);
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<Property>();
        _property->resize(_surface->w, _surface->h);
        _property->clip_mode = false;
        _property->color_alpha = StdColor::White;
        _property->setScale(1.0f);
        Logger::log(std::format("Texture created from surface"), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
    }

    Texture::Texture(Renderer* renderer, SDL_PixelFormat format, int width, int height, SDL_TextureAccess access)
        : _renderer(renderer), _surface(nullptr) {
        _texture = SDL_CreateTexture(renderer->self(), format, access, width, height);
        if (!_texture) {
            Logger::log(std::format("Created texture failed!\nException: {}", SDL_GetError()), Logger::ERROR);
            _property = std::make_unique<Property>();
            return;
        }
        _property = std::make_unique<Property>();
        _property->resize(width, height);
        _property->setScale(1.0f);
        _property->clip_mode = false;
        _property->color_alpha = StdColor::White;
        Logger::log(std::format("Texture created from custom"), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", width, height), Logger::DEBUG);
    }

    Texture::~Texture() {
        if (_texture) {
            SDL_DestroyTexture(_texture);
        }
        if (_surface) {
            SDL_DestroySurface(_surface);
        }
    }

    bool Texture::setImagePath(const std::string& path) {
        auto img = IMG_Load(path.c_str());
        if (!img) {
            Logger::log(std::format("The image path '{}' is not found!", path), Logger::ERROR);
            return false;
        }
        if (_texture) {
            SDL_DestroyTexture(_texture);
        }
        if (_surface) {
            SDL_DestroySurface(_surface);
        }
        _surface = img;
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property->resize(_surface->w, _surface->h);
        Logger::log(std::format("Texture image changed to '{}'", path), Logger::DEBUG);
        Logger::log(std::format("Texture size updated to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
        return true;
    }

    SDL_Texture* Texture::self() const {
        if (!_texture) {
            Logger::log("The texture is not created!", Logger::ERROR);
        }
        return _texture;
    }

    Property* Texture::property() {
        return _property.get();
    }

    void Texture::draw() const {
        if (!_texture) {
            Logger::log("The texture is not created!", Logger::ERROR);
            return;
        }
        _renderer->drawTexture(_texture, _property.get());
    }

}