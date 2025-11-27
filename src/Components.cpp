#include "Components.h"
#include "Utils/Logger.h"

namespace S3GF {
    Texture::Texture(const std::string &path, Renderer *renderer) : _renderer(renderer), _texture(nullptr), _path(path) {
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
        _property->color_alpha = RGBAColor::White;
        _property->setScale(1.0f);
        Logger::log(std::format("Texture created from image path '{}'", path), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
    }

    Texture::Texture(SDL_Surface* surface, Renderer *renderer, bool deep_copy) : _renderer(renderer), _texture(nullptr) {
        if (!surface) {
            Logger::log(std::format("The surface is not valid!\nException: {}", SDL_GetError()), Logger::ERROR);
            _property = std::make_unique<Property>();
            return;
        }
        _surface = (deep_copy ? SDL_DuplicateSurface(surface) : surface);
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<Property>();
        _property->resize(_surface->w, _surface->h);
        _property->clip_mode = false;
        _property->color_alpha = RGBAColor::White;
        _property->setScale(1.0f);
        Logger::log(std::format("Texture created from surface"), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
    }

    Texture::Texture(Renderer* renderer, SDL_PixelFormat format, int width, int height, SDL_TextureAccess access)
        : _renderer(renderer), _surface(nullptr), _texture(nullptr) {
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
        _property->color_alpha = RGBAColor::White;
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

    Renderer* Texture::render() const {
        return _renderer;
    }

    bool Texture::setImagePath(const std::string& path) {
        auto img = IMG_Load(path.c_str());
        _path = path;
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

    const std::string& Texture::imagePath() const {
        return _path;
    }

    bool Texture::setImageFromSurface(SDL_Surface *surface, bool deep_copy) {
        if (!surface) {
            Logger::log(std::format("The surface is not valid!\nException: {}", SDL_GetError()), Logger::ERROR);
            _property = std::make_unique<Property>();
            return false;
        }
        _surface = (deep_copy ? SDL_DuplicateSurface(surface) : surface);
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<Property>();
        _property->resize((float)_surface->w, (float)_surface->h);
        _property->clip_mode = false;
        _property->color_alpha = RGBAColor::White;
        _property->setScale(1.0f);
        Logger::log(std::format("Texture created from surface"), Logger::DEBUG);
        Logger::log(std::format("Texture size set to {}x{}", _surface->w, _surface->h), Logger::DEBUG);
        return true;
    }

    SDL_Texture* Texture::self() const {
        if (!_texture) {
            Logger::log("Texture: The current texture is not created or not valid!", Logger::ERROR);
        }
        return _texture;
    }

    bool Texture::isValid() const {
        return (_texture != nullptr);
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

    TextureAtlas::TextureAtlas(const std::string &path, Renderer *renderer) : Texture(path, renderer) {
    }

    TextureAtlas::TextureAtlas(SDL_Surface *surface, Renderer *renderer, bool deep_copy)
        : Texture(surface, renderer, deep_copy) {}

    TextureAtlas::~TextureAtlas() {}

    void TextureAtlas::setTiles(const std::string &tiles_name, Property &&tiles_property) {
        if (_tiles_map.contains(tiles_name)) {
            _tiles_map[tiles_name]->reset(tiles_property);
        } else {
            _tiles_map.emplace(tiles_name, std::shared_ptr<Property>(new Property(tiles_property)));
        }
    }

    Property *TextureAtlas::tilesProperty(const std::string &tiles_name) {
        if (_tiles_map.contains(tiles_name)) {
            return _tiles_map[tiles_name].get();
        } else {
            Logger::log(std::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                        "Did you forget to use `TextureAtlas::setTiles()`?", tiles_name), Logger::ERROR);
            return nullptr;
        }
    }

    void TextureAtlas::setCurrentTiles(const std::string &tiles_name) {
        if (_tiles_map.contains(tiles_name)) {
            _current_tiles = tiles_name;
        } else {
            Logger::log(std::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                        "Did you forget to use `TextureAtlas::setTiles()`?", tiles_name), Logger::ERROR);
        }
    }

    const std::string &TextureAtlas::currentTiles() const {
        return _current_tiles;
    }

    void TextureAtlas::draw() const {
        if (_tiles_map.contains(_current_tiles)) {
            render()->drawTexture(self(), _tiles_map.at(_current_tiles).get());
        } else {
            Logger::log(std::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                         "Did you forget to use `TextureAtlas::setTiles()`?", _current_tiles), Logger::ERROR);
        }
    }

    void TextureAtlas::draw(const std::string &tiles_name) const {
        if (_tiles_map.contains(tiles_name)) {
            render()->drawTexture(self(), _tiles_map.at(tiles_name).get());
        } else {
            Logger::log(std::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                                    "Did you forget to use `TextureAtlas::setTiles()`?", tiles_name), Logger::ERROR);
        }
    }
}