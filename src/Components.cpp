#include "Components.h"

#include <memory>
#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Utils/RGBAColor.h"
#include "Algorithm/All.h"
#include "Utils/WavWriter.h"

namespace MyEngine {
    constexpr size_t MAX_AUDIO_FILE_SIZE = 2 * 1024 * 1024;
    Font::Font(const std::string& font_path, float font_size)
            : _font_path(font_path), _font_size(font_size) {
        _font = TTF_OpenFont(font_path.c_str(), font_size);
        if (!_font) {
            Logger::log(FMT::format("Font: Can't load font from path '{}'.", font_path),
                        Logger::Error);
        }
        _font_is_loaded = true;
    }

    Font::~Font() {
        if (_font) {
            TTF_CloseFont(_font);
        }
    }

    void Font::setFontPath(const std::string &font_path) {
        auto _new_font = TTF_OpenFont(font_path.c_str(), _font_size);
        if (!_new_font) {
            Logger::log(FMT::format("Font: Can't load font from path '{}'.", font_path),
                        Logger::Error);
            return;
        }
        if (_font) {
            TTF_CloseFont(_font);
        }
        _font = _new_font;
    }

    const std::string& Font::fontPath() const { return _font_path; }

    void Font::setFontSize(float size) {
        auto _ret = TTF_SetFontSize(_font, size);
        if (_ret) {
            _font_size = size;
        }
    }

    float Font::fontSize() const {
        return _font_size;
    }

    void Font::setFontColor(const SDL_Color& color) {
        _font_color = color;
    }

    const SDL_Color& Font::fontColor() const {
        return _font_color;
    }

    void Font::setStyle(uint32_t flags) {
        TTF_SetFontStyle(_font, static_cast<TTF_FontStyleFlags>(flags));
        _font_style_flags = flags;
    }

    void Font::setOutline(uint32_t value) {
        _font_outline = value;
    }

    uint32_t Font::outline() const {
        return _font_outline;
    }

    void Font::setOutlineColor(const SDL_Color& color) {
        _outline_color = color;
    }

    SDL_Color Font::outlineColor() const {
        return _outline_color;
    }

    void Font::setFontDirection(Direction direction) {
        auto _ret = TTF_SetFontDirection(_font, static_cast<TTF_Direction>(direction));
        if (_ret) {
            _font_direction = direction;
        }
    }

    Font::Direction Font::fontDirection() const {
        return _font_direction;
    }

    void Font::setFontHinting(uint32_t flags) {
        TTF_SetFontHinting(_font, static_cast<TTF_HintingFlags>(flags));
        _font_hinting = flags;
    }

    void Font::setFontKerning(bool enabled) {
        TTF_SetFontKerning(_font, enabled);
        _font_kerning = enabled;
    }

    bool Font::fontKerning() const {
        return _font_kerning;
    }

    void Font::setLineSpacing(uint32_t spacing) {
        TTF_SetFontLineSkip(_font, spacing);
        _line_spacing = spacing;
    }

    uint32_t Font::lineSpacing() const {
        return _line_spacing;
    }

    SDL_Surface* Font::toImage(const std::string& text) {
        SDL_Surface* surface;
        if (!_font_is_loaded) {
            Logger::log("Font is not loaded! Did you forgot to load font?", Logger::Error);
            return nullptr;
        }
        if (_font_outline) {
            if (_font_color.a > 0) {
                auto filled_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
                TTF_SetFontOutline(_font, _font_outline);
                auto bordered_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _outline_color);
                TTF_SetFontOutline(_font, 0);
                int real_width = bordered_surface->w, real_height = bordered_surface->h;
                surface = SDL_CreateSurface(real_width, real_height, bordered_surface->format);

                SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0));
                SDL_BlitSurface(bordered_surface, nullptr, surface, nullptr);
                SDL_Rect rect(real_width / 2 - filled_surface->w / 2,
                              real_height / 2 - filled_surface->h / 2,
                              filled_surface->w, filled_surface->h);
                SDL_BlitSurface(filled_surface, nullptr, surface, &rect);

                SDL_DestroySurface(filled_surface);
                SDL_DestroySurface(bordered_surface);
            } else {
                TTF_SetFontOutline(_font, _font_outline);
                surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _outline_color);
                TTF_SetFontOutline(_font, 0);
            }
        } else {
            surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
        }
        if (!surface) {
            Logger::log(FMT::format("Can't drawEvent the current text!\nException: {}", SDL_GetError()), Logger::Error);
        }
        return surface;
    }

    SDL_Surface* Font::toImage(const std::string& text, const SDL_Color& backgrond_color) {
        SDL_Surface* surface = nullptr;
        if (!_font_is_loaded) {
            Logger::log("Font is not loaded! Did you forget to load font?", Logger::Error);
            return nullptr;
        }
        if (_font_outline) {
            if (_font_color.a > 0) {
                auto filled_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
                TTF_SetFontOutline(_font, _font_outline);
                auto bordered_surface = TTF_RenderText_LCD(_font, text.c_str(), 0, _outline_color, backgrond_color);
                TTF_SetFontOutline(_font, 0);
                int real_width = bordered_surface->w, real_height = bordered_surface->h;
                surface = SDL_CreateSurface(real_width, real_height, bordered_surface->format);

                SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0));
                SDL_BlitSurface(bordered_surface, nullptr, surface, nullptr);
                SDL_Rect rect(real_width / 2 - filled_surface->w / 2,
                              real_height / 2 - filled_surface->h / 2,
                              filled_surface->w, filled_surface->h);
                SDL_BlitSurface(filled_surface, nullptr, surface, &rect);

                SDL_DestroySurface(filled_surface);
                SDL_DestroySurface(bordered_surface);
            } else {
                TTF_SetFontOutline(_font, _font_outline);
                surface = TTF_RenderText_LCD(_font, text.c_str(), 0, _outline_color, backgrond_color);
                TTF_SetFontOutline(_font, 0);
            }
        } else {
            surface = TTF_RenderText_LCD(_font, text.c_str(), 0, _font_color, backgrond_color);
        }
        if (!surface) {
            Logger::log(FMT::format("Can't drawEvent the current text!\nException: {}", SDL_GetError()), Logger::Error);
        }
        return surface;
    }

    TTF_Font* Font::self() const {
        return _font;
    }

    FontMap FontDatabase::getFontDatabaseFromSystem() {
        if (!_is_loaded) {
            StringList find_font_dir;
#ifdef _WIN32
            find_font_dir.emplace_back("C:/Windows/Fonts");
#endif
#ifdef __linux__
            find_font_dir.emplace_back("/usr/share/fonts");
            if (FileSystem::isDir("~/.fonts")) {
                find_font_dir.emplace_back("~/.fonts");
            }
            if (FileSystem::isDir("~/.local/share/fonts")) {
                find_font_dir.emplace_back("~/.local/share/fonts");
            }
#endif
#ifdef __APPLE__
            if (FileSystem::isDir("/System/Library/Fonts")) {
                find_font_dir.emplace_back("/System/Library/Fonts");
            }
            if (FileSystem::isDir("/Library/Fonts")) {
                find_font_dir.emplace_back("/Library/Fonts");
            }
            if (FileSystem::isDir("~/Library/Fonts")) {
                find_font_dir.emplace_back("~/Library/Fonts");
            }
#endif

            for (auto& font_dir : find_font_dir) {
                StringList font_files = FileSystem::listFilesRecursively(font_dir,
                                                         {".ttf", ".otf", ".ttc", ".woff", ".eot"});
                for (auto& file : font_files) {
                    auto short_file = FileSystem::getShortFileName(file, true);
                    if (!_font_db.contains(short_file)) {
                        _font_db.insert({FileSystem::getShortFileName(file, true), file});
                    }
                }
            }
            Logger::log("FontDatabase: Get Font files from system!", Logger::Debug);
            _is_loaded = true;
        }
        return _font_db;
    }

    std::string FontDatabase::findFontFromSystem(const std::string &font_name) {
        if (!_is_loaded) getFontDatabaseFromSystem();
        if (!_font_db.contains(font_name)) return {};
        return _font_db[font_name];
    }

    std::vector<FontDatabase::FontInfo> FontDatabase::getSystemDefaultFont() {
        if (!_is_loaded) getFontDatabaseFromSystem();
        if (!_def_fonts.empty()) return _def_fonts;
#ifdef _WIN32
        StringList common_fonts = { "arial", "segoeui", "tahoma", "verdana", "calibri" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) _def_fonts.push_back({font_name, path});
        }
#endif
#ifdef __linux__
        StringList common_fonts = { "AdwaitaSans-Regular", "DejaVuSans", "Roboto-Regular", "Ubuntu-R" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) _def_fonts.push_back({font_name, path});
        }
#endif
#ifdef __APPLE__
        StringList common_fonts = { "SanFrancisco-Regular", "HelveticaNeue", "ArialMT", "TimesNewRomanPSMT" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) _def_fonts.push_back({font_name, path});
        }
#endif
        return _def_fonts;
    }

    Texture::Texture(const std::string &path, Renderer *renderer)
                : _renderer(renderer), _texture(nullptr), _path(path) {
        _surface = IMG_Load(path.c_str());
        if (!_surface) {
            Logger::log(Logger::Error, "Texture: The image path '{}' is not valid!\nException: {}",
                        path, SDL_GetError());
            _property = std::make_unique<TextureProperty>();
            return;
        }
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<TextureProperty>();
        _property->resize((float)_surface->w, (float)_surface->h);
        _property->clip_mode = false;
        _property->color_alpha = RGBAColor::White;
        _property->setScale(1.0f);
        Logger::log(FMT::format("Texture: Created from image path '{}'", path));
        Logger::log(FMT::format("Texture: Size set to {}x{}", _surface->w, _surface->h));
    }

    Texture::Texture(SDL_Surface*&& surface, Renderer *renderer, bool deep_copy)
                : _renderer(renderer), _texture(nullptr) {
        if (!surface) {
            Logger::log(FMT::format("Texture: The surface is not valid!\n"
                                    "Exception: {}", SDL_GetError()), Logger::Error);
            _property = std::make_unique<TextureProperty>();
            return;
        }
        _surface = deep_copy ? SDL_DuplicateSurface(surface) : std::move(surface);
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<TextureProperty>();
        _property->resize((float)_surface->w, (float)_surface->h);
        _property->clip_mode = false;
        _property->color_alpha = RGBAColor::White;
        _property->setScale(1.0f);
        Logger::log(FMT::format("Texture: Created from surface"));
        Logger::log(FMT::format("Texture: Size set to {}x{}", _surface->w, _surface->h));
    }

    Texture::Texture(Renderer* renderer, SDL_PixelFormat format, int width, int height, SDL_TextureAccess access)
        : _renderer(renderer), _surface(nullptr), _texture(nullptr) {
        _texture = SDL_CreateTexture(renderer->self(), format, access, width, height);
        if (!_texture) {
            Logger::log(FMT::format("Texture: Created texture failed!\n"
                                    "Exception: {}", SDL_GetError()), Logger::Error);
            _property = std::make_unique<TextureProperty>();
            return;
        }
        _property = std::make_unique<TextureProperty>();
        _property->resize((float)width, (float)height);
        _property->setScale(1.0f);
        _property->clip_mode = false;
        _property->color_alpha = RGBAColor::White;
        Logger::log(FMT::format("Texture: Created texture by renderer"));
        Logger::log(FMT::format("Texture: Size set to {}x{}", width, height));
    }

    Texture::Texture(const Texture& texture) : _renderer(texture.renderer()), _surface(), _texture() {
        _surface = SDL_DuplicateSurface(texture.surface());
        if (!_surface) {
            Logger::log(Logger::Error, "Texture: The specified texture is not valid! "
                                       "Exception: {}", SDL_GetError());
            return;
        }
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        if (!_texture) {
            Logger::log(FMT::format("Texture: Created texture failed!\n"
                                    "Exception: {}", SDL_GetError()), Logger::Error);
            _property = std::make_unique<TextureProperty>();
            return;
        }
        _property = std::make_unique<TextureProperty>(*texture._property);
        Logger::log(FMT::format("Texture: Created from another texture"));
        Logger::log(FMT::format("Texture: Size set to {}x{}", _surface->w, _surface->h));
    }

    Texture::~Texture() {
        if (_texture) {
            SDL_DestroyTexture(_texture);
        }
        if (_surface) {
            SDL_DestroySurface(_surface);
        }
    }

    Renderer* Texture::renderer() const {
        return _renderer;
    }

    bool Texture::setImagePath(const std::string& path) {
        auto img = IMG_Load(path.c_str());
        _path = path;
        if (!img) {
            Logger::log(FMT::format("The image path '{}' is not found!", path), Logger::Error);
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
        _property->resize((float)_surface->w, (float)_surface->h);
        Logger::log(FMT::format("Texture: Image changed to '{}'", path));
        Logger::log(FMT::format("Texture Size updated to {}x{}", _surface->w, _surface->h));
        return true;
    }

    const std::string& Texture::imagePath() const {
        return _path;
    }

    bool Texture::setImageFromSurface(SDL_Surface*&& surface, bool deep_copy) {
        if (!surface) {
            Logger::log(FMT::format("The surface is not valid!\n"
                                    "Exception: {}", SDL_GetError()), Logger::Error);
            _property = std::make_unique<TextureProperty>();
            return false;
        }
        _surface = (deep_copy ? SDL_DuplicateSurface(surface) : std::move(surface));
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<TextureProperty>();
        _property->resize((float)_surface->w, (float)_surface->h);
        _property->clip_mode = false;
        _property->color_alpha = RGBAColor::White;
        _property->setScale(1.0f);
        Logger::log(FMT::format("Texture: Created from surface"));
        Logger::log(FMT::format("Texture: Size set to {}x{}", _surface->w, _surface->h));
        return true;
    }

    SDL_Texture* Texture::self() const {
        if (!_texture) {
            auto err = "Texture: The current texture is not created or not valid!";
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
        }
        return _texture;
    }

    SDL_Surface* Texture::surface() const {
        if (!_surface) {
            auto err = "Texture: The current surface is not valid or is null!";
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
        }
        return _surface;
    }

    bool Texture::isValid() const {
        return (_texture != nullptr);
    }

    TextureProperty* Texture::property() {
        return _property.get();
    }

    void Texture::draw() {
        if (!_texture) {
            Logger::log("Texture: The texture is not created or not valid!", Logger::Fatal);
            throw NullPointerException("Texture: The texture is not created or not valid!");
        }
        _renderer->drawTexture(_texture, _property.get());
    }

    TextureAtlas::TextureAtlas(const std::string &path, Renderer *renderer) : Texture(path, renderer) {
    }

    TextureAtlas::TextureAtlas(SDL_Surface *surface, Renderer *renderer, bool deep_copy)
        : Texture(std::move(surface), renderer, deep_copy) {}

    TextureAtlas::~TextureAtlas() {}

    bool TextureAtlas::addTiles(const std::string &tiles_name, const MyEngine::GeometryF &clip_geometry) {
        if (_tiles_map.contains(tiles_name)) {
            Logger::log(FMT::format("TextureAtlas: Tiles '{}' is already in tiles map! "
                                    , tiles_name), Logger::Error);
            return false;
        }
        std::vector<std::unique_ptr<TextureProperty>> new_list;
        new_list.emplace_back(std::make_unique<TextureProperty>(property()));
        auto* ptr = new_list.back().get();
        ptr->resize(clip_geometry.size);
        ptr->clip_mode = true;
        ptr->clip_area = { clip_geometry.pos.x, clip_geometry.pos.y,
                           clip_geometry.size.width, clip_geometry.size.height };

        _tiles_map.insert({tiles_name, Tile(tiles_name, std::move(new_list))});
        return true;
    }

    bool TextureAtlas::addTilesProperty(const std::string &tiles_name) {
        if (_tiles_map.contains(tiles_name)) {
            auto& temp = _tiles_map.at(tiles_name).properties.front();
            _tiles_map.at(tiles_name).properties.push_back(std::make_unique<TextureProperty>(temp.get()));
            return true;
        } else {
            Logger::log(FMT::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                                    "Did you forget to use `TextureAtlas::addTiles()`?", tiles_name), Logger::Error);
            return false;
        }
    }

    bool TextureAtlas::eraseTiles(const std::string &tiles_name) {
        if (_tiles_map.contains(tiles_name)) {
            _tiles_map.erase(tiles_name);
            return true;
        }
        return false;
    }

    TextureProperty *TextureAtlas::tilesProperty(const std::string &tiles_name, size_t index) {
        if (_tiles_map.contains(tiles_name)) {
            if (index >= _tiles_map[tiles_name].properties.size()) {
                Logger::log(FMT::format("TextureAtlas: The index of the tiles '{}' is out of range! "
                            "Try to use `TextureAtlas::tilesPropertyCount()`?", tiles_name), Logger::Error);
                return nullptr;
            }
            return _tiles_map[tiles_name].properties[index].get();
        } else {
            Logger::log(FMT::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                        "Did you forget to use `TextureAtlas::addTiles()`?", tiles_name), Logger::Error);
            return nullptr;
        }
    }

    size_t TextureAtlas::tilesPropertyCount(const std::string& tiles_name) const {
        if (_tiles_map.contains(tiles_name)) {
            return _tiles_map.at(tiles_name).properties.size();
        } else {
            Logger::log(FMT::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                        "Did you forget to use `TextureAtlas::addTiles()`?", tiles_name), Logger::Error);
            return 0;
        }
    }

    void TextureAtlas::setCurrentTiles(const std::string &tiles_name) {
        if (_tiles_map.contains(tiles_name)) {
            _current_tiles = tiles_name;
        } else {
            Logger::log(FMT::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                        "Did you forget to use `TextureAtlas::addTiles()`?", tiles_name), Logger::Error);
        }
    }

    const std::string &TextureAtlas::currentTiles() const {
        return _current_tiles;
    }

    StringList TextureAtlas::tilesNameList() const {
        StringList out;
        for (auto& [name, t] : _tiles_map) {
            out.emplace_back(name);
        }
        return out;
    }

    bool TextureAtlas::isTilesNameExist(const std::string& tiles_name) const {
        return _tiles_map.contains(tiles_name);
    }

    void TextureAtlas::draw() {
        if (_tiles_map.contains(_current_tiles)) {
            renderer()->drawTexture(self(), _tiles_map[_current_tiles].properties[0].get());
        } else {
            auto err = FMT::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                                   "Did you forget to use `TextureAtlas::addTiles()`?", _current_tiles);
            Logger::log(err, Logger::Fatal);
            throw OutOfRangeException(err);
        }
    }

    void TextureAtlas::draw(size_t index) {
        if (_tiles_map.contains(_current_tiles)) {
            if (index >= _tiles_map[_current_tiles].properties.size()) {
                auto err = FMT::format("TextureAtlas: The index of the tiles '{}' is out of range! "
                                       "Try to use `TextureAtlas::tilesPropertyCount()`?", _current_tiles);
                Logger::log(err, Logger::Fatal);
                throw OutOfRangeException(err);
            }
            renderer()->drawTexture(self(), _tiles_map[_current_tiles].properties[index].get());
        } else {
            auto err = FMT::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                                   "Did you forget to use `TextureAtlas::addTiles()`?", _current_tiles);
            Logger::log(err, Logger::Fatal);
            throw OutOfRangeException(err);
        }
    }

    void TextureAtlas::draw(const std::string &tiles_name, size_t index) {
        if (_tiles_map.contains(tiles_name)) {
            if (index >= _tiles_map[tiles_name].properties.size()) {
                auto err = FMT::format("TextureAtlas: The index of the tiles '{}' is out of range! "
                                       "Try to use `TextureAtlas::tilesPropertyCount()`?", _current_tiles);
                Logger::log(err, Logger::Fatal);
                throw OutOfRangeException(err);
            }
            renderer()->drawTexture(self(), _tiles_map[tiles_name].properties[index].get());
        } else {
            auto err = FMT::format("TextureAtlas: Tiles '{}' is not in tiles map! "
                                   "Did you forget to use `TextureAtlas::addTiles()`?", tiles_name);
            Logger::log(err, Logger::Fatal);
            throw OutOfRangeException(err);
        }
    }


    TextureAnimation::TextureAnimation(const std::string &file_path, Renderer* renderer)
                        : _file_path(file_path), _renderer(renderer), _null(true), _img_ani(nullptr) {
        _property = std::make_unique<TextureProperty>();
        loadAnimation(file_path);
        EventSystem::global()->appendGlobalEvent(IDGenerator::getNewGlobalEventID(), [this] {
            if (!_playing) return;
            auto now = SDL_GetTicks();
            if (now - _start_time >= _textures[_cur_frame]->duration) {
                _cur_frame = (_cur_frame + 1 >= _textures.size() ? 0 : _cur_frame + 1);
                _start_time = SDL_GetTicks();
            }
        });
    }

    TextureAnimation::~TextureAnimation() {
        if (!_null) {
            for (int i = 0; i < _img_ani->count; ++i) {
                SDL_DestroyTexture(_textures[i]->texture);
            }
            IMG_FreeAnimation(_img_ani);
        }
    }

    void TextureAnimation::setDurationPerFrame(size_t duration) {
        for (const auto & texture : _textures) {
            texture->duration = duration;
        }
    }

    void TextureAnimation::setDurationInFrame(size_t index, size_t duration) {
        if (index >= _textures.size()) {
            Logger::log("TextureAnimation: The specified index is out of range!", Logger::Error);
            return;
        }
        _textures[index]->duration = duration;
    }

    size_t TextureAnimation::durationInFrame(size_t index) const {
        if (index >= _textures.size()) {
            Logger::log("TextureAnimation: The specified index is out of range!", Logger::Error);
            return 0;
        }
        return _textures[index]->duration;
    }

    size_t TextureAnimation::currentFrame() const {
        return _cur_frame;
    }

    size_t TextureAnimation::framesCount() const {
        return _textures.size();
    }

    const TextureAnimation::Frame *TextureAnimation::indexOfFrame(size_t index) const {
        if (!_null) {
            Logger::log("TextureAnimation: Current textures are null! "
                        "Use `TextureAnimation::loadAnimation()` at first.", Logger::Error);
            return nullptr;
        }
        if (index >= _textures.size()) {
            Logger::log("TextureAnimation: The specified index is out of range!", Logger::Error);
            return nullptr;
        }
        return _textures.at(index).get();
    }

    bool TextureAnimation::isNull() const {
        return _null;
    }

    TextureProperty* TextureAnimation::property() {
        return _property.get();
    }

    bool TextureAnimation::loadAnimation(const std::string &path) {
        if (_playing) _playing = false;
        if (!_textures.empty()) _textures.clear();
        _img_ani = IMG_LoadAnimation(path.c_str());
        if (!_img_ani) {
            Logger::log(FMT::format("TextureAnimation: The image file '{}' is not the animation image file "
                                    "(*.gif, *.webp) or it is not valid!", path), Logger::Error);
            _null = false;
            return false;
        }
        for (int i = 0; i < _img_ani->count; ++i) {
            _textures.emplace_back(new Frame(_img_ani->frames[i],
                     SDL_CreateTextureFromSurface(_renderer->self(), _img_ani->frames[i]),
                     _img_ani->delays[i]));
        }
        _property->setAnchor(1, 1);
        _property->resize(static_cast<float>(_img_ani->w), static_cast<float>(_img_ani->h));
        _null = true;
        Logger::log(FMT::format("TextureAnimation: Loaded image file '{}', "
                                "get image size: {}x{}.", path, _img_ani->w, _img_ani->h));
        return true;
    }

    void TextureAnimation::draw() {
        if (_null) {
            Logger::log("TextureAnimation: No image loaded or it is not valid!", Logger::Fatal);
            Engine::throwCustomFatalError<NullPointerException>();
        }
        if (_textures.size() >= _cur_frame) {
            auto err = FMT::format("TextureAnimation: Current frame is out of range (at frame {})!", _cur_frame);
            Logger::log(err,Logger::Fatal);
            Engine::throwCustomFatalError<BadValueException>();
        }
        _renderer->drawTexture(_textures.at(_cur_frame)->texture, _property.get());
    }

    void TextureAnimation::play(size_t frame) {
        _cur_frame = frame;
        _start_time = SDL_GetTicks();
        _playing = true;
    }

    void TextureAnimation::stop() {
        _playing = false;
        _start_time = 0;
    }

    BGM::BGM(MIX_Mixer *mixer, const std::string &path) : _mixer(mixer), _path(path), _play_status() {
        if (!_mixer) {
            Logger::log("BGM: The specified mixer can not be null!", Logger::Fatal);
            throw InvalidArgumentException("BGM: The specified mixer can not be null!");
        }
        init();
        load();
    }

    BGM::BGM(MIX_Mixer *mixer, MIX_Audio *audio) : _mixer(mixer), _audio(audio), _path(), _play_status() {
        if (!_mixer) {
            Logger::log("BGM: The specified mixer can not be null!", Logger::Fatal);
            throw InvalidArgumentException("BGM: The specified mixer can not be null!");
        }
        if (!_audio) {
            Logger::log("BGM: The specified audio is not valid!", Logger::Fatal);
            throw InvalidArgumentException("BGM: The specified audio is not valid!");
        }
        init();
        load();
    }

    BGM::~BGM() {
        if (_play_status > Invalid) unload();
        EventSystem::global()->removeGlobalEvent(_global_ev_id);
    }

    void BGM::setPath(const std::string &path) {
        _path = path;
        unload();
        load();
    }

    const std::string &BGM::path() const {
        return _path;
    }

    bool BGM::isLoaded() const {
        return _play_status >= Loaded;
    }

    bool BGM::play(int64_t start_position, bool loop, int64_t fade_in_duration) {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't play current audio! Current audio is not valid!", Logger::Error);
            _play_status = Invalid;
            return false;
        }
        if (!_prop_id) { _prop_id = SDL_CreateProperties(); }
        SDL_SetNumberProperty(_prop_id, MIX_PROP_PLAY_START_MILLISECOND_NUMBER, start_position);
        SDL_SetNumberProperty(_prop_id, MIX_PROP_PLAY_LOOPS_NUMBER, (loop ? -1 : 0));
        SDL_SetNumberProperty(_prop_id, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fade_in_duration);
        if (_play_status == Paused) {
            return resume();
        } else if (_play_status == Loaded) {
            if (!MIX_PlayTrack(_track, _prop_id)) {
                Logger::log(FMT::format("BGM: Play audio failed! The file path '{}' is not valid! "
                                        "Exception: {}", _path, SDL_GetError()), Logger::Error);
                _play_status = Invalid;
                return false;
            }
            _play_status = (fade_in_duration ? FadingIn : Playing);
        }
        return true;
    }

    void BGM::stop(int64_t fade_out_duration) {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't stop current audio! Current audio is not valid!", Logger::Error);
        }
        if (_play_status == FadingOut || _play_status == Loaded) return;

        auto ms = (fade_out_duration > 0 ? MIX_TrackMSToFrames(_track, fade_out_duration) : 0);
        MIX_StopTrack(_track, ms);
        if (!fade_out_duration) {
            _play_status = Loaded;
            return;
        }
        _play_status = FadingOut;
    }

    void BGM::pause() {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't pause current audio! Current status is not valid!", Logger::Error);
            return;
        }
        if (_play_status != Playing) {
            Logger::log("BGM: Current audio is not playing!", Logger::Warn);
            return;
        }
        MIX_PauseTrack(_track);
        _play_status = Paused;
    }

    bool BGM::resume() {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't resume current audio! Current status is not valid!", Logger::Error);
            return false;
        }
        if (_play_status != Paused) {
            Logger::log("BGM: Current audio is already playing!", Logger::Warn);
            return false;
        }
        if (position() >= duration()) playAt(0);
        auto _ret = MIX_ResumeTrack(_track);
        if (!_ret) {
            Logger::log("BGM: Can't resume current audio! Current status is not valid!", Logger::Error);
            return false;
        }
        _play_status = Playing;
        return true;
    }

    bool BGM::forward(int64_t ms) {
        auto pos = position();
        auto new_pos = std::min(pos + ms, duration());
        if (new_pos < 0) new_pos = 0;
        if (!MIX_SetTrackPlaybackPosition(_track, MIX_TrackMSToFrames(_track, new_pos))) {
            Logger::log("BGM: Failed to set playback position!", Logger::Warn);
            return false;
        }
        return true;
    }

    bool BGM::backward(int64_t ms) {
        auto pos = position();
        auto new_pos = std::min(pos - ms, duration());
        if (new_pos < 0) new_pos = 0;
        if (!MIX_SetTrackPlaybackPosition(_track, MIX_TrackMSToFrames(_track, new_pos))) {
            Logger::log("BGM: Failed to set playback position!", Logger::Warn);
            return false;
        }
        return true;
    }

    bool BGM::playAt(int64_t position) {
        auto new_pos = std::min(position, duration());
        if (new_pos < 0) new_pos = 0;
        if (!MIX_SetTrackPlaybackPosition(_track, MIX_TrackMSToFrames(_track, new_pos))) {
            Logger::log("BGM: Failed to set playback position!", Logger::Warn);
            return false;
        }
        return true;
    }

    int64_t BGM::position() const {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't pause current audio! Current audio is not valid!", Logger::Error);
            return 0;
        }
        return MIX_TrackFramesToMS(_track, MIX_GetTrackPlaybackPosition(_track));
    }

    int64_t BGM::duration() const {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't pause current audio! Current audio is not valid!", Logger::Error);
            return 0;
        }
        return MIX_TrackFramesToMS(_track, MIX_GetAudioDuration(_audio));
    }

    BGM::PlayStatus BGM::playStatus() const {
        return _play_status;
    }

    std::string BGM::playStatusText() const {
        switch (_play_status) {
            case Loading:
                return "Loading";
            case Loaded:
                return "Loaded";
            case Playing:
                return "Playing";
            case Paused:
                return "Paused";
            case FadingIn:
                return "Fading in";
            case FadingOut:
                return "Fading out";
            default:
                return "Invalid";
        }
    }

    bool BGM::setVolume(float volume) {
        auto new_vol = std::clamp(volume, 0.f, 10.f);
        if (MIX_SetTrackGain(_track, new_vol)) {
            _volume = new_vol;
            return true;
        }
        return false;
    }

    bool BGM::setMuted(bool enabled) {
        if (MIX_SetTrackGain(_track, (enabled ? 0.f : _volume))) {
            _muted = enabled;
            return true;
        }
        return false;
    }

    bool BGM::setLRChannel(float left, float right) {
        _stereo_gains.left = std::clamp(left, 0.f, 10.f);
        _stereo_gains.right = std::clamp(right, 0.f, 10.f);
        return MIX_SetTrackStereo(_track, &_stereo_gains);
    }

    bool BGM::set3DPosition(float x, float y, float z) {
        _mix_3d = { .x = x, .y = y, .z = z };
        if (!MIX_SetTrack3DPosition(_track, &_mix_3d)) {
            Logger::log(FMT::format("BGM::set3DPosition: Failed to set 3D position! Exception: {}", SDL_GetError()), Logger::Warn);
            return false;
        }
        return true;
    }

    bool BGM::setSpeedAndPitch(float value) {
        return MIX_SetTrackFrequencyRatio(_track, value);
    }

    float BGM::volume() const {
        return _volume;
    }

    bool BGM::isMuted() const {
        return _muted;
    }

    const MIX_StereoGains& BGM::getLRChannel() const {
        return _stereo_gains;
    }

    const MIX_Point3D& BGM::get3DPosition() const {
        return _mix_3d;
    }

    float BGM::speedAndPitch() const {
        return MIX_GetTrackFrequencyRatio(_track);
    }

    const MIX_Audio* BGM::audio() const {
        return _audio;
    }

    const MIX_Track* BGM::track() const {
        return _track;
    }

    MIX_Track* BGM::getTrack() const {
        return _track;
    }

    void BGM::init() {
        _global_ev_id = IDGenerator::getNewGlobalEventID();
        EventSystem::global()->appendGlobalEvent(_global_ev_id, [this]() {
            if ((_play_status == Playing || _play_status == FadingOut) && !MIX_TrackPlaying(_track)) {
                _play_status = Loaded;
            } else if (_play_status == FadingIn) {
                auto start_pos = SDL_GetNumberProperty(_prop_id,
                                                       MIX_PROP_PLAY_START_MILLISECOND_NUMBER, 0);
                auto fade_in_dur = SDL_GetNumberProperty(_prop_id,
                                                         MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, 0);
                auto end_point = std::min(start_pos + fade_in_dur, duration());
                if (position() >= end_point) {
                    _play_status = Playing;
                }
            }
        });
    }

    void BGM::load() {
        _play_status = Loading;
        if (!_audio) {
            _audio = MIX_LoadAudio(_mixer, _path.c_str(), false);
            if (!_audio) {
                Logger::log(FMT::format("BGM: The specified file path '{}' is not valid! Exception: {}",
                                        _path, SDL_GetError()), Logger::Error);
                _play_status = Invalid;
                return;
            }
        }
        _track = MIX_CreateTrack(_mixer);
        if (!_track) {
            Logger::log(FMT::format("BGM: Create audio track failed! Exception: {}"
                    , SDL_GetError()), Logger::Error);
            _play_status = Invalid;
            return;
        }
        if (!MIX_SetTrackAudio(_track, _audio)) {
            Logger::log(FMT::format("BGM: The specified file path '{}' can not set as audio track! Exception: {}",
                                    _path, SDL_GetError()), Logger::Error);
            _play_status = Invalid;
            return;
        }
        _play_status = Loaded;
    }

    void BGM::unload() {
        if (_track) {
            MIX_DestroyTrack(_track);
            _track = nullptr;
        }
        if (_audio) {
            MIX_DestroyAudio(_audio);
            _audio = nullptr;
        }
        SDL_DestroyProperties(_prop_id);
        _prop_id = 0;
        _play_status = Invalid;
    }

    SFX::SFX(MIX_Mixer *mixer, const std::string &path) : _path(path), _mixer(mixer) {
        if (!_mixer) {
            Logger::log("SFX: The specified mixer can not be null!", Logger::Fatal);
            throw InvalidArgumentException("SFX: The specified mixer can not be null!");
        }
        load();
    }

    SFX::SFX(MIX_Mixer *mixer, int hz, float wave_volume, int64_t ms) : _path(), _mixer(mixer) {
        if (!_mixer) {
            Logger::log("SFX: The specified mixer can not be null!", Logger::Fatal);
            throw InvalidArgumentException("SFX: The specified mixer can not be null!");
        }
        loadSineWave(hz, wave_volume, ms);
    }

    SFX::~SFX() {
        if (_is_load) unload();
    }

    void SFX::setPath(const std::string &path) {
        _path = path;
        unload();
        load();
    }

    const std::string &SFX::path() const {
        return _path;
    }

    void SFX::loadSineWave(int hz, float wave_volume, int64_t ms) {
        if (_is_load) unload();
        _audio = MIX_CreateSineWaveAudio(_mixer, hz, wave_volume, ms);
        if (!_audio) {
            Logger::log(FMT::format("SFX: The specified sine wave {}Hz is not valid! Exception: {}",
                                    hz, SDL_GetError()), Logger::Error);
            _is_load = false;
            return;
        }
        if (_tracks.empty()) _tracks.emplace_back();
        if (_tracks[0].track) MIX_DestroyTrack(_tracks[0].track);
        _tracks[0].track = MIX_CreateTrack(_mixer);
        if (!_tracks[0].track) {
            Logger::log(Logger::Error, "SFX: Create audio track failed! Exception: {}", SDL_GetError());
            _is_load = false;
            _tracks.clear();
            return;
        }
        if (!MIX_SetTrackAudio(_tracks[0].track, _audio)) {
            Logger::log(FMT::format("SFX: The specified sine wave can not set as audio track! Exception: {}",
                                    SDL_GetError()), Logger::Error);
            _is_load = false;
            MIX_DestroyTrack(_tracks[0].track);
            _tracks[0].track = nullptr;
            return;
        }
        _is_load = true;
    }

    bool SFX::isLoaded() const {
        return _is_load;
    }

    void SFX::setDefaultSFX(float volume, MIX_StereoGains &&stereo_gains) {
        _default_track.volume = volume;
        _default_track.stereo_gains = std::move(stereo_gains);
    }

    size_t SFX::findFreeIndex() {
        return findFreeTrackIndex();
    }

    size_t SFX::lastIndex() const {
        return _last_index;
    }

    bool SFX::play(bool loop, int64_t fade_in_duration) {
        if (!_is_load) {
            Logger::log("BGM: Can't play current audio! Current audio is not valid!", Logger::Error);
            return false;
        }
        if (_prop_id == 0) _prop_id = SDL_CreateProperties();
        SDL_SetNumberProperty(_prop_id, MIX_PROP_PLAY_LOOPS_NUMBER, (loop ? -1 : 0));
        SDL_SetNumberProperty(_prop_id, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fade_in_duration);

        _last_index = findFreeTrackIndex();
        if (_last_index >= _tracks.size()) {
            _tracks.emplace_back(_default_track);
            _tracks.back().track = MIX_CreateTrack(_mixer);
            MIX_SetTrackAudio(_tracks.back().track, _audio);
            setVolume(_default_track.volume, _tracks.size() - 1);
            setLRChannel(_default_track.stereo_gains.left, _default_track.stereo_gains.right,
                    _tracks.size() - 1);
        }
        if (!MIX_PlayTrack(_tracks[_last_index].track, _prop_id)) {
            Logger::log(FMT::format("SFX: Play audio failed! The file path '{}' is not valid! "
                                    "Exception: {}", _path, SDL_GetError()), Logger::Error);
            return false;
        }
        return true;
    }

    bool SFX::play(size_t index) {
        _last_index = index;
        if (index >= _tracks.size()) {
            Logger::log(Logger::Error, "SFX: Play audio failed! Index '{}' is out of range!", index);
            return false;
        }
        if (!MIX_PlayTrack(_tracks[index].track, _prop_id)) {
            Logger::log(FMT::format("SFX: Play audio failed! The file path '{}' is not valid! "
                                    "Exception: {}", _path, SDL_GetError()), Logger::Error);
            return false;
        }
        return true;
    }

    void SFX::stop(int64_t fade_out_duration) {
        if (!_is_load) {
            Logger::log("BGM: Can't stop current audio! Current audio is not valid!", Logger::Error);
            return;
        }
        _last_index = findBusyTrackIndex();
        if (_last_index >= _tracks.size()) return;
        auto ms = (fade_out_duration > 0 ? MIX_TrackMSToFrames(_tracks[_last_index].track,
                                                                        fade_out_duration) : 0);
        MIX_StopTrack(_tracks[_last_index].track, ms);
    }

    void SFX::stop(size_t index, int64_t fade_out_duration) {
        if (!_is_load) {
            Logger::log("BGM: Can't stop current audio! Current audio is not valid!", Logger::Error);
            return;
        }
        _last_index = index;
        if (index >= _tracks.size()) return;
        auto ms = (fade_out_duration > 0 ? MIX_TrackMSToFrames(_tracks[index].track, fade_out_duration) : 0);
        MIX_StopTrack(_tracks[index].track, ms);
    }

    void SFX::stopAll(int64_t fade_out_duration) {
        if (!_is_load) {
            Logger::log("BGM: Can't stop current audio! Current audio is not valid!", Logger::Error);
            return;
        }
        _last_index = 0;
        for (auto & track: _tracks) {
            auto ms = (fade_out_duration > 0 ? MIX_TrackMSToFrames(track.track, fade_out_duration) : 0);
            MIX_StopTrack(track.track, ms);
        }
    }

    void SFX::resetAll() {
        if (!_is_load) return;
        size_t i = 0;
        for (auto& track: _tracks) {
            MIX_StopTrack(track.track, 0);
            if (i > 0) MIX_DestroyTrack(track.track);
            i++;
        }
        _tracks.erase(_tracks.begin() + 1, _tracks.end());
        _last_index = 0;
    }

    std::optional<int64_t> SFX::position(size_t index) const {
        if (!_is_load) {
            Logger::log("BGM: Can't get current audio! Current audio is not valid!", Logger::Error);
            return {};
        }
        return _tracks.size() <= index ? std::optional<uint64_t>() :
                    MIX_TrackFramesToMS(_tracks[index].track, MIX_GetTrackPlaybackPosition(_tracks[index].track));
    }

    int64_t SFX::duration() const {
        if (!_is_load) {
            Logger::log("BGM: Can't pause current audio! Current audio is not valid!", Logger::Error);
            return 0;
        }
        return MIX_TrackFramesToMS(_tracks[0].track, MIX_GetAudioDuration(_audio));
    }

    std::optional<bool> SFX::isLoop(size_t index) const {
        return index >= _tracks.size() ? std::optional<bool>() : MIX_GetTrackLoops(_tracks[index].track) > 0;
    }

    std::optional<bool> SFX::isPlaying(size_t index) const {
        return index >= _tracks.size() ? std::optional<bool>() : MIX_TrackPlaying(_tracks[index].track);
    }

    bool SFX::setVolume(float volume, size_t index) {
        auto new_vol = std::clamp(volume, 0.f, 10.f);
        if (_tracks.size() <= index) return false;
        if (MIX_SetTrackGain(_tracks[index].track, new_vol)) {
            _tracks[index].volume = new_vol;
            return true;
        }
        return false;
    }

    bool SFX::setLRChannel(float left, float right, size_t index) {
        if (_tracks.size() <= index) return false;
        MIX_StereoGains _stereo_gains;
        _stereo_gains.left  = std::clamp(left, 0.f, 10.f);
        _stereo_gains.right = std::clamp(right, 0.f, 10.f);
        if (MIX_SetTrackStereo(_tracks[index].track, &_stereo_gains)) {
            _tracks[index].stereo_gains = _stereo_gains;
            return true;
        }
        return false;
    }

    bool SFX::set3DPosition(float x, float y, float z, size_t index) {
        MIX_Point3D _mix_3d = { .x = x, .y = y, .z = z };
        if (_tracks.size() <= index) return false;
        if (MIX_SetTrack3DPosition(_tracks[index].track, &_mix_3d)) {
            _tracks[index].point_3d = _mix_3d;
            return true;
        }
        return false;
    }

    bool SFX::setSpeedAndPitch(float value, size_t index) {
        if (_tracks.size() <= index) return false;
        return MIX_SetTrackFrequencyRatio(_tracks[index].track, value);
    }

    std::optional<float> SFX::volume(size_t index) const {
        return _tracks.size() <= index ? std::optional<float>() : _tracks[index].volume;
    }

    const std::optional<MIX_StereoGains> SFX::getLRChannel(size_t index) const {
        return _tracks.size() <= index ? std::optional<MIX_StereoGains>() : _tracks.at(index).stereo_gains;
    }

    const std::optional<MIX_Point3D> SFX::get3DPosition(size_t index) const {
        return _tracks.size() <= index ? std::optional<MIX_Point3D>() : _tracks.at(index).point_3d;
    }

    std::optional<float> SFX::speedAndPitch(size_t index) const {
        return _tracks.size() <= index ? std::optional<float>() : MIX_GetTrackFrequencyRatio(_tracks[index].track);
    }

    const MIX_Audio* SFX::audio() const {
        return _audio;
    }

    const std::optional<MIX_Track*> SFX::track(size_t index) const {
        return _tracks.size() <= index ? std::optional<MIX_Track*>() : _tracks.at(index).track;
    }

    std::optional<MIX_Track *> SFX::getTrack(size_t index) const {
        return _tracks.size() <= index ? std::optional<MIX_Track*>() : _tracks.at(index).track;
    }

    size_t SFX::count() const {
        return _tracks.size();
    }

    size_t SFX::playingCount() const {
        size_t cnt = 0;
        for (auto& track: _tracks) {
            if (MIX_TrackPlaying(track.track)) cnt++;
        }
        return cnt;
    }

    void SFX::load() {
        auto size = FileSystem::readableSize(_path, FileSystem::MB);
        _audio = MIX_LoadAudio(_mixer, _path.c_str(), (size >= MAX_AUDIO_FILE_SIZE));
        if (!_audio) {
            Logger::log(FMT::format("BGM: The specified file path '{}' is not valid! Exception: {}",
                                    _path, SDL_GetError()), Logger::Error);
            _is_load = false;
            return;
        }
        if (_tracks.empty()) _tracks.emplace_back();
        if (_tracks[0].track) MIX_DestroyTrack(_tracks[0].track);
        _tracks[0].track = MIX_CreateTrack(_mixer);
        if (!_tracks[0].track) {
            Logger::log(Logger::Error, "BGM: Create audio track failed! Exception: {}", SDL_GetError());
            _is_load = false;
            _tracks.clear();
            return;
        }
        if (!MIX_SetTrackAudio(_tracks[0].track, _audio)) {
            Logger::log(FMT::format("BGM: The specified file path '{}' can not set as audio track! Exception: {}",
                                    _path, SDL_GetError()), Logger::Error);
            _is_load = false;
            MIX_DestroyTrack(_tracks[0].track);
            _tracks[0].track = nullptr;
            return;
        }
        _is_load = true;
    }

    void SFX::unload() {
        for (auto& track: _tracks) {
            if (track.track) {
                MIX_DestroyTrack(track.track);
                track.track = nullptr;
            }
        }
        _tracks.clear();
        if (_audio) {
            MIX_DestroyAudio(_audio);
            _audio = nullptr;
        }
        SDL_DestroyProperties(_prop_id);
        _prop_id = 0;
        _is_load = false;
    }

    size_t SFX::findBusyTrackIndex() const {
        for (size_t index = 0; index < _tracks.size(); index++) {
            if (MIX_TrackPlaying(_tracks[index].track)) {
                return index;
            }
        }
        return _tracks.size();
    }

    size_t SFX::findFreeTrackIndex() const {
        for (size_t index = 0; index < _tracks.size(); index++) {
            if (!MIX_TrackPlaying(_tracks[index].track)) {
                return index;
            }
        }
        return _tracks.size();
    }

    AudioRecorder::AudioRecorder(SDL_AudioDeviceID output_id) : _output_deviceID(output_id) {
        load();
    }

    AudioRecorder::AudioRecorder(SDL_AudioDeviceID input_id, SDL_AudioDeviceID output_id)
            : _input_deviceID(input_id), _output_deviceID(output_id) {
        load();
    }

    AudioRecorder::~AudioRecorder() {
        unload();
    }

    bool AudioRecorder::setInputDeviceID(SDL_AudioDeviceID input_id) {
        unload();
        _input_deviceID = input_id;
        return load();
    }

    bool AudioRecorder::reload() {
        unload();
        return load();
    }

    SDL_AudioDeviceID AudioRecorder::inputDeviceID() const {
        return _input_deviceID;
    }

    SDL_AudioDeviceID AudioRecorder::outputDeviceID() const {
        return _output_deviceID;
    }

    SDL_AudioSpec AudioRecorder::inputAudioSpec() const {
        SDL_AudioSpec ret;
        SDL_GetAudioDeviceFormat(_input_deviceID, &ret, nullptr);
        return ret;
    }

    SDL_AudioSpec AudioRecorder::outputAudioSpec() const {
        SDL_AudioSpec ret;
        SDL_GetAudioDeviceFormat(_output_deviceID, &ret, nullptr);
        return ret;
    }

    bool AudioRecorder::startRecord() {
        if (!SDL_ResumeAudioStreamDevice(_stream)) {
            Logger::log(Logger::Error, "AudioRecorder: Failed to start recording! Exception: {}", SDL_GetError());
            return false;
        }
        _status = Recording;

        SDL_AudioSpec spec = inputAudioSpec();
        auto bit_size = SDL_AUDIO_BITSIZE(spec.format);
        if (!_wav_writer.begin(spec.freq, spec.channels, bit_size, spec.format)) {
            Logger::log(Logger::Error, "AudioRecorder: Failed to write audio file!");
            stopRecord();
            return false;
        }

        return true;
    }

    bool AudioRecorder::stopRecord() {
        if (!SDL_PauseAudioStreamDevice(_stream)) {
            Logger::log(Logger::Error, "AudioRecorder: Failed to stop recording! Exception: {}", SDL_GetError());
            return true;
        }
        _status = Processing;
        if (_wav_writer.isOpen()) {
            bool ok = _wav_writer.end();
            if (!ok) {
                Logger::log(Logger::Error, "AudioRecorder: Failed to write audio file!");
            }
        }
        _status = Idle;
        return true;
    }

    bool AudioRecorder::isRecording() const {
        return _status == Recording;
    }

    bool AudioRecorder::isValid() const {
        return _status >= Idle;
    }

    SDL_AudioStream *AudioRecorder::audioStream() const {
        return _stream;
    }

    void AudioRecorder::setOutputFileName(const std::string &file_name) {
        if (_status == Recording) stopRecord();
        _wav_writer.setOutputPath(file_name);
    }

    void AudioRecorder::setAudioDecibalMeter(AudioDecibelMeter *decibel_meter) {
        _audio_decibel_meter = decibel_meter;
    }

    AudioDecibelMeter *AudioRecorder::audioDecibelMeter() const {
        return _audio_decibel_meter;
    }

    bool AudioRecorder::load() {
        auto real_id = _input_deviceID == 0 ? SDL_AUDIO_DEVICE_DEFAULT_RECORDING : _input_deviceID;
        _input_deviceID = SDL_OpenAudioDevice(real_id, nullptr);
        if (_input_deviceID == 0) {
            Logger::log(Logger::Error, "AudioRecorder: Failed to open audio recorder! "
                                       "Exception: {}", SDL_GetError());
            Engine::throwCustomFatalError<InvalidArgumentException>();
        }
        auto in_spec = inputAudioSpec();
        auto out_spec = outputAudioSpec();
        _stream = SDL_CreateAudioStream(&in_spec, &out_spec);
        if (!_stream) {
            Logger::log(Logger::Error, "AudioRecorder: Failed to create audio stream! "
                                       "Exception: {}", SDL_GetError());
            SDL_CloseAudioDevice(_input_deviceID);
            _input_deviceID = 0;
            _output_deviceID = 0;
            Engine::throwCustomFatalError<InvalidArgumentException>();
            return false;
        }
        bool is_load = SDL_BindAudioStream(_input_deviceID, _stream);
        if (is_load) {
            Logger::log(Logger::Debug, "AudioRecorder: Loaded the audio recorder ID: {}",
                        _input_deviceID);
            _status = Recording;
        } else {
            Logger::log(Logger::Error, "AudioRecorder: Failed to bind audio stream!");
            SDL_DestroyAudioStream(_stream);
            SDL_CloseAudioDevice(_input_deviceID);
            _status = Invalid;
        }
        is_load = SDL_SetAudioStreamPutCallback(_stream,
            SDL_AUDIO_ISFLOAT(in_spec.format) ? &AudioRecorder::onRecordingF32 : &AudioRecorder::onRecordingS16,
                                        this);
        return is_load;
    }

    void AudioRecorder::unload() {
        if (_status == Recording) stopRecord();
        if (_input_deviceID > 0) {
            if (_status >= Idle) {
                if (SDL_GetAudioStreamDevice(_stream) > 0) {
                    SDL_UnbindAudioStream(_stream);
                }
                SDL_DestroyAudioStream(_stream);
            }
            SDL_CloseAudioDevice(_input_deviceID);
        }
        _status = Invalid;
    }

    void AudioRecorder::onRecordingF32(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount) {
        auto self = static_cast<AudioRecorder*>(userdata);
        auto input_spec = self->inputAudioSpec();
        auto samples = additional_amount / sizeof(float);
        std::vector<float> pcm(samples);
        auto bytes = SDL_GetAudioStreamData(stream, pcm.data(), pcm.size() * sizeof(float));
        if (bytes < 0) {
            Logger::log(Logger::Error, "AudioRecorder: Failed to get audio stream data! (ID: {})\n"
                "Exception: {}", self->_input_deviceID, SDL_GetError());
            return;
        }
        if (bytes > 0) {
            // Using limiting to deal with plosives and noise issues.
            for (size_t i = 0; i < samples; i++) {
                if (pcm[i] > 1.0f) pcm[i] = 1.0f;
                if (pcm[i] < -1.0f) pcm[i] = -1.0f;
            }

            if (self->_wav_writer.isOpen() && !self->_wav_writer.write(pcm.data(), bytes)) {
                Logger::log(Logger::Error, "AudioRecorder: Failed to write audio data! (ID: {})",
                    self->_input_deviceID);
            }
        }
        if (!self->_audio_decibel_meter) return;
        // Calculate RMS
        float sum_L = 0.f, sum_R = 0.f;
        float peak_L = 0.f, peak_R = 0.f, peak_M = 0.f;
        auto frames_count = samples / input_spec.channels;

        for (size_t i = 0; i < samples; i += input_spec.channels) {
            const float L = fabsf(pcm[i]);
            const float R = input_spec.channels > 1 ? fabsf(pcm[i + 1]) : L;

            sum_L += L * L;
            sum_R += R * R;

            if (L > peak_L) peak_L = L;
            if (R > peak_R) peak_R = R;

            float max_sample = (input_spec.channels > 1) ? fmaxf(L, R) : L;
            if (max_sample > peak_M) peak_M = max_sample;
        }

        const float RMS_L = sqrtf(sum_L / static_cast<float>(frames_count));
        const float RMS_R = sqrtf(sum_R / static_cast<float>(frames_count));
        self->_audio_decibel_meter->_level_meters.assign(1, {});
        auto& level_meter = self->_audio_decibel_meter->_level_meters.front();
        level_meter._left_dB = self->_audio_decibel_meter->linearToDecibel(RMS_L);
        level_meter._right_dB = self->_audio_decibel_meter->linearToDecibel(RMS_R);
        level_meter._mix_dB =
                self->_audio_decibel_meter->linearToDecibel(sqrtf((sum_L + sum_R) / static_cast<float>(samples)));

        level_meter._left_peak_dB = self->_audio_decibel_meter->linearToDecibel(peak_L);
        level_meter._right_peak_dB = self->_audio_decibel_meter->linearToDecibel(peak_R);
        level_meter._mix_peak_dB = self->_audio_decibel_meter->linearToDecibel(peak_M);
    }

    void AudioRecorder::onRecordingS16(void *userdata, SDL_AudioStream *stream, int additional_amount, int total_amount) {
        auto self = static_cast<AudioRecorder*>(userdata);
        auto input_spec = self->inputAudioSpec();
        auto samples = additional_amount / sizeof(uint16_t);
        std::vector<uint16_t> pcm(samples);
        auto bytes = SDL_GetAudioStreamData(stream, pcm.data(), pcm.size() * sizeof(uint16_t));
        if (bytes < 0) {
            Logger::log(Logger::Error, "AudioRecorder: Failed to get audio stream data! (ID: {})\n"
                "Exception: {}", self->_input_deviceID, SDL_GetError());
            return;
        }
        if (bytes > 0) {
            if (self->_wav_writer.isOpen() && !self->_wav_writer.write(pcm.data(), bytes)) {
                Logger::log(Logger::Error, "AudioRecorder: Failed to write audio data! (ID: {})",
                    self->_input_deviceID);
            }
        }
        if (!self->_audio_decibel_meter) return;
        // Calculate RMS
        float sum_L = 0.f, sum_R = 0.f;
        float peak_L = 0.f, peak_R = 0.f, peak_M = 0.f;
        auto frames_count = samples / input_spec.channels;

        for (size_t i = 0; i < samples; i += input_spec.channels) {
            const float L = fabsf(pcm[i] / 32768.f);
            const float R = input_spec.channels > 1 ? fabsf(pcm[i + 1] / 32768.f) : L;

            sum_L += L * L;
            sum_R += R * R;

            if (L > peak_L) peak_L = L;
            if (R > peak_R) peak_R = R;

            float max_sample = (input_spec.channels > 1) ? fmaxf(L, R) : L;
            if (max_sample > peak_M) peak_M = max_sample;
        }

        const float RMS_L = sqrtf(sum_L / static_cast<float>(frames_count));
        const float RMS_R = sqrtf(sum_R / static_cast<float>(frames_count));
        self->_audio_decibel_meter->_level_meters.assign(1, {});
        auto& level_meter = self->_audio_decibel_meter->_level_meters.front();
        level_meter._left_dB = self->_audio_decibel_meter->linearToDecibel(RMS_L);
        level_meter._right_dB = self->_audio_decibel_meter->linearToDecibel(RMS_R);
        level_meter._mix_dB =
                self->_audio_decibel_meter->linearToDecibel(sqrtf((sum_L + sum_R) / static_cast<float>(samples)));

        level_meter._left_peak_dB = self->_audio_decibel_meter->linearToDecibel(peak_L);
        level_meter._right_peak_dB = self->_audio_decibel_meter->linearToDecibel(peak_R);
        level_meter._mix_peak_dB = self->_audio_decibel_meter->linearToDecibel(peak_M);
    }

    AudioDecibelMeter::AudioDecibelMeter(BGM *bgm) : _audio(bgm) {
        init();
    }

    AudioDecibelMeter::AudioDecibelMeter(SFX *sfx) : _audio(sfx) {
        init();
    }

    AudioDecibelMeter::AudioDecibelMeter(MIX_Mixer *mixer) : _audio(mixer) {
        init();
    }

    AudioDecibelMeter::AudioDecibelMeter(AudioRecorder *recorder) : _audio(recorder) {
        init();
    }

    void AudioDecibelMeter::viewBGM(BGM *bgm) {
        uninitialized();
        _audio = bgm;
        init();
    }

    void AudioDecibelMeter::viewSFX(SFX *sfx) {
        uninitialized();
        _audio = sfx;
        init();
    }

    void AudioDecibelMeter::viewMixer(MIX_Mixer *mixer) {
        uninitialized();
        _audio = mixer;
        init();
    }

    void AudioDecibelMeter::unload() {
        uninitialized();
        _audio = std::monostate{};
    }

    float AudioDecibelMeter::mixDecibel(size_t index) const {
        if (index >= _level_meters.size()) return MUTED_DB;
        return _level_meters.at(index)._mix_dB;
    }

    float AudioDecibelMeter::leftDecibel(size_t index) const {
        if (index >= _level_meters.size()) return MUTED_DB;
        return _level_meters.at(index)._left_dB;
    }

    float AudioDecibelMeter::rightDecibel(size_t index) const {
        if (index >= _level_meters.size()) return MUTED_DB;
        return _level_meters.at(index)._right_dB;
    }

    float AudioDecibelMeter::leftPeakDecibel(size_t index) const {
        if (index >= _level_meters.size()) return MUTED_DB;
        return _level_meters.at(index)._left_peak_dB;
    }

    float AudioDecibelMeter::rightPeakDecibel(size_t index) const {
        if (index >= _level_meters.size()) return MUTED_DB;
        return _level_meters.at(index)._right_peak_dB;
    }

    float AudioDecibelMeter::mixPeakDecibel(size_t index) const {
        if (index >= _level_meters.size()) return MUTED_DB;
        return _level_meters.at(index)._mix_peak_dB;
    }

    void AudioDecibelMeter::init() {
        MIX_Track* track = nullptr;
        if (std::holds_alternative<BGM*>(_audio)) {
            track = std::get<BGM*>(_audio)->getTrack();
            _level_meters.assign(1, {});
            if (!MIX_SetTrackCookedCallback(track, &AudioDecibelMeter::cookedBGM, &_level_meters)) {
                Logger::log(Logger::Error, "AudioLevelViewer: Failed to initialized for BGM! "
                                           "Exception: {}", SDL_GetError());
            }
        } else if (std::holds_alternative<SFX*>(_audio)) {
            auto sfx = std::get<SFX*>(_audio);
            _level_meters.assign(sfx->count(), {});
            for (size_t i = 0; i < sfx->count(); i++) {
                track = sfx->getTrack(i).value();
                if (!MIX_SetTrackCookedCallback(track, &AudioDecibelMeter::cookedSFX, &_level_meters[i])) {
                    Logger::log(Logger::Error, "AudioLevelViewer: Failed to initialized for SFX! "
                                               "Exception: {}", SDL_GetError());
                }
            }
        } else if (std::holds_alternative<MIX_Mixer*>(_audio)) {
            auto mixer = std::get<MIX_Mixer*>(_audio);
            _level_meters.assign(1, {});
            if (!MIX_SetPostMixCallback(mixer, &AudioDecibelMeter::cookedMixer, &_level_meters)) {
                Logger::log(Logger::Error, "AudioLevelViewer: Failed to initialized for Mixer! "
                                           "Exception: {}", SDL_GetError());
            }
        }
    }

    void AudioDecibelMeter::uninitialized() {
        MIX_Track* track = nullptr;
        if (std::holds_alternative<MIX_Mixer*>(_audio)) {
            MIX_SetPostMixCallback(std::get<MIX_Mixer*>(_audio), nullptr, nullptr);
            return;
        }
        if (std::holds_alternative<BGM*>(_audio)) {
            track = std::get<BGM*>(_audio)->getTrack();
            MIX_SetTrackCookedCallback(track, nullptr, nullptr);
        } else if (std::holds_alternative<SFX*>(_audio)) {
            auto sfx = std::get<SFX*>(_audio);
            for (size_t i = 0; i < sfx->count(); ++i) {
                track = sfx->getTrack(0).value();
                MIX_SetTrackCookedCallback(track, nullptr, nullptr);
            }
        }
    }

    float AudioDecibelMeter::linearToDecibel(float linear) {
        return (linear > 0) ? 20.f * log10f(linear) : MUTED_DB;
    }

    void AudioDecibelMeter::cookedBGM(void *userdata, MIX_Track *, const SDL_AudioSpec *spec,
                float *pcm, int samples) {
        auto self = static_cast<std::vector<LevelMeter>*>(userdata);

        // Calculate RMS
        float sum_L = 0.f, sum_R = 0.f;
        float peak_L = 0.f, peak_R = 0.f, peak_M = 0.f;
        auto frames_count = samples / spec->channels;

        for (size_t i = 0; i < samples; i += spec->channels) {
            const float L = fabsf(pcm[i]);
            const float R = spec->channels > 1 ? fabsf(pcm[i + 1]) : L;

            sum_L += L * L;
            sum_R += R * R;

            if (L > peak_L) peak_L = L;
            if (R > peak_R) peak_R = R;

            float max_sample = (spec->channels > 1) ? fmaxf(L, R) : L;
            if (max_sample > peak_M) peak_M = max_sample;
        }

        const float RMS_L = sqrtf(sum_L / static_cast<float>(frames_count));
        const float RMS_R = sqrtf(sum_R / static_cast<float>(frames_count));
        self->begin()->_left_dB = linearToDecibel(RMS_L);
        self->begin()->_right_dB = linearToDecibel(RMS_R);
        self->begin()->_mix_dB = linearToDecibel(sqrtf((sum_L + sum_R) / static_cast<float>(samples)));

        self->begin()->_left_peak_dB = linearToDecibel(peak_L);
        self->begin()->_right_peak_dB = linearToDecibel(peak_R);
        self->begin()->_mix_peak_dB = linearToDecibel(peak_M);
    }

    void AudioDecibelMeter::cookedSFX(void *userdata, MIX_Track *, const SDL_AudioSpec *spec,
                float *pcm, int samples) {
        auto self = static_cast<LevelMeter*>(userdata);

        // Calculate RMS
        float sum_L = 0.f, sum_R = 0.f;
        float peak_L = 0.f, peak_R = 0.f, peak_M = 0.f;
        auto frames_count = samples / spec->channels;

        for (size_t i = 0; i < samples; i += spec->channels) {
            const float L = fabsf(pcm[i]);
            const float R = spec->channels > 1 ? fabsf(pcm[i + 1]) : L;

            sum_L += L * L;
            sum_R += R * R;

            if (L > peak_L) peak_L = L;
            if (R > peak_R) peak_R = R;

            float max_sample = (spec->channels > 1) ? fmaxf(L, R) : L;
            if (max_sample > peak_M) peak_M = max_sample;
        }

        const float RMS_L = sqrtf(sum_L / static_cast<float>(frames_count));
        const float RMS_R = sqrtf(sum_R / static_cast<float>(frames_count));
        self->_left_dB = linearToDecibel(RMS_L);
        self->_right_dB = linearToDecibel(RMS_R);
        self->_mix_dB = linearToDecibel(sqrtf((sum_L + sum_R) / static_cast<float>(samples)));

        self->_left_peak_dB = linearToDecibel(peak_L);
        self->_right_peak_dB = linearToDecibel(peak_R);
        self->_mix_peak_dB = linearToDecibel(peak_M);
    }

    void AudioDecibelMeter::cookedMixer(void *userdata, MIX_Mixer *, const SDL_AudioSpec *spec,
                float *pcm, int samples) {
        auto self = static_cast<std::vector<LevelMeter>*>(userdata);

        // Calculate RMS
        float sum_L = 0.f, sum_R = 0.f;
        float peak_L = 0.f, peak_R = 0.f, peak_M = 0.f;
        auto frames_count = samples / spec->channels;

        for (size_t i = 0; i < samples; i += spec->channels) {
            const float L = fabsf(pcm[i]);
            const float R = spec->channels > 1 ? fabsf(pcm[i + 1]) : L;

            sum_L += L * L;
            sum_R += R * R;

            if (L > peak_L) peak_L = L;
            if (R > peak_R) peak_R = R;

            float max_sample = (spec->channels > 1) ? fmaxf(L, R) : L;
            if (max_sample > peak_M) peak_M = max_sample;
        }

        const float RMS_L = sqrtf(sum_L / static_cast<float>(frames_count));
        const float RMS_R = sqrtf(sum_R / static_cast<float>(frames_count));
        self->begin()->_left_dB = linearToDecibel(RMS_L);
        self->begin()->_right_dB = linearToDecibel(RMS_R);
        self->begin()->_mix_dB = linearToDecibel(sqrtf((sum_L + sum_R) / static_cast<float>(samples)));

        self->begin()->_left_peak_dB = linearToDecibel(peak_L);
        self->begin()->_right_peak_dB = linearToDecibel(peak_R);
        self->begin()->_mix_peak_dB = linearToDecibel(peak_M);
    }

    TriggerArea::TriggerArea(GeometryF geometry, Window *window) :
            _geometry(geometry), _window(window), _event_id(IDGenerator::getNewEventID()){
        EventSystem::global()->appendEvent(_event_id, [&] (SDL_Event e) {
            if (!isEnabled()) return;
            auto mouse_cur = EventSystem::global()->captureMousePosition();
            auto is_on_area = Algorithm::comparePosInGeometry(mouse_cur, _geometry);
            // Keyboard Event
            if (_key > SDL_SCANCODE_UNKNOWN) {
                auto keys = EventSystem::global()->captureKeyboard(_key);
                if (!keys) {
                    if (_events & Status_KeyboardPressedDown) {
                        _events ^= Status_KeyboardPressedDown;
                        _events |= Status_TriggeredArea;
                        if (_callback) _callback();
                        keyUpEvent(e.key.scancode);
                        keyPressedEvent(e.key.scancode);
                    }
                } else if (!e.key.repeat) {
                    _events |= Status_KeyboardPressedDown;
                    keyDownEvent(e.key.scancode);
                }
            }
            // Mouse Event
            if (e.type == SDL_EVENT_MOUSE_BUTTON_UP || e.type == SDL_EVENT_MOUSE_BUTTON_DOWN ||
                e.type == SDL_EVENT_MOUSE_MOTION) {
                auto button = EventSystem::global()->captureMouseStatus();
                if (button == MouseStatus::None) {
                    if (_events & Status_MouseButtonDown) {
                        _events ^= Status_MouseButtonDown;
                        mouseUpEvent(_last_mouse_status);
                        if (is_on_area >= 0) {
                            mouseClickedEvent(_last_mouse_status);
                            _events |= Status_TriggeredArea;
                            // if (_callback) _callback();
                        }
                        if (_events & Status_OnArea) _events ^= Status_OnArea;
                    }
                } else if (!(_events & Status_MouseButtonDown) && is_on_area >= 0) {
                    _events |= Status_MouseButtonDown;
                    _events |= Status_OnArea;
                    mouseDownEvent(button);
                    _last_mouse_status = button;
                } else if (_events & Status_MouseButtonDown) {
                    mouseMovedEvent(mouse_cur,
                                    EventSystem::global()->captureMouseAbsDistance());
                    if (is_on_area >= 0 && !(_events & Status_OnArea)) {
                        _events |= Status_OnArea;
                        mouseMovedInEvent();
                    } else if (is_on_area < 0 && (_events & Status_OnArea)) {
                        _events ^= Status_OnArea;
                        mouseMovedOutEvent();
                    }
                }
            }
            // Finger Event
            if (e.type == SDL_EVENT_FINGER_UP || e.type == SDL_EVENT_FINGER_DOWN ||
                e.type == SDL_EVENT_FINGER_MOTION || e.type == SDL_EVENT_FINGER_CANCELED) {
                auto w = static_cast<float>(_window->geometry().width);
                auto h = static_cast<float>(_window->geometry().height);
                auto finger = _window->getFingerEventByID(e.tfinger.fingerID);
                if (finger.has_value()) {
                    Vector2 pos = {w * e.tfinger.x, h * e.tfinger.y};
                    bool finger_on_area = (Algorithm::comparePosInGeometry(pos, _geometry) >= 0);
                    if (finger_on_area) {
                        if (!(_events & Status_FingerDown)) {
                            _events |= Status_FingerDown;
                            fingerDownEvent(e.tfinger.fingerID);
                        } else {
                            fingerMovedEvent(e.tfinger.fingerID, pos, finger.value().distance_pos);
                            if (!(_events & Status_OnArea)) fingerMovedInEvent();
                        }
                        _events |= Status_OnArea;
                    } else {
                        if (_events & Status_OnArea) {
                            _events ^= Status_OnArea;
                            fingerMovedOutEvent();
                        }
                    }
                } else if (_events & Status_FingerDown) {
                    _events ^= Status_FingerDown;
                    fingerUpEvent(e.tfinger.fingerID);
                    if (_events & Status_OnArea) {
                        _events |= Status_TriggeredArea;
                        if (_callback) _callback();
                        fingerTouchedEvent(e.tfinger.fingerID);
                    }
                }
            }
        });
    }

    TriggerArea::~TriggerArea() = default;

    void TriggerArea::setGeometry(float x, float y, float w, float h) {
        _geometry.setGeometry(x, y, w, h);
    }

    void TriggerArea::setGeometry(const Vector2 &pos, const Size &size) {
        _geometry.setGeometry(pos, size);
    }

    void TriggerArea::setGeometry(const MyEngine::GeometryF &geometry) {
        _geometry.setGeometry(geometry);
    }

    void TriggerArea::move(float x, float y) {
        _geometry.move(x, y);
    }

    void TriggerArea::move(const Vector2 &pos) {
        _geometry.move(pos);
    }

    void TriggerArea::resize(float w, float h) {
        _geometry.resize(w, h);
    }

    void TriggerArea::resize(const Size &size) {
        _geometry.resize(size);
    }

    const GeometryF &TriggerArea::geometry() const {
        return _geometry;
    }

    const Vector2 &TriggerArea::position() const {
        return _geometry.pos;
    }

    const Size &TriggerArea::size() const {
        return _geometry.size;
    }

    bool TriggerArea::isEnabled() const {
        return !(_events & Status_DisabledArea);
    }

    void TriggerArea::setEnabled(bool enabled) {
        if (enabled && !isEnabled()) {
            _events ^= Status_DisabledArea;
        } else {
            _events = Status_DisabledArea;
        }
    }

    bool TriggerArea::isOnArea() const {
        return _events & Status_OnArea;
    }

    bool TriggerArea::isTriggeredOnArea() const {
        return _events & Status_TriggeredArea;
    }

    uint8_t TriggerArea::events() const { return _events; }

    void TriggerArea::setTriggerKey(SDL_Scancode keycode) {
        _key = keycode;
    }

    const SDL_Scancode &TriggerArea::triggerKey() const {
        return _key;
    }

    void TriggerArea::setTriggerEvent(const std::function<void()> &callback_function) {
        _callback = callback_function;
    }

    const Window *TriggerArea::window() const {
        return _window;
    }

    void TriggerArea::mouseDownEvent(MouseStatus button) {}

    void TriggerArea::mouseUpEvent(MouseStatus button) {}

    void TriggerArea::mouseMovedEvent(const Vector2 &pos, const Vector2 &dis) {}

    void TriggerArea::mouseMovedInEvent() {}

    void TriggerArea::mouseMovedOutEvent() {}

    void TriggerArea::mouseClickedEvent(MyEngine::MouseStatus button) {
        if (button == MouseStatus::Left && _callback) _callback();
    }

    void TriggerArea::keyPressedEvent(SDL_Scancode keycode) {}

    void TriggerArea::keyDownEvent(SDL_Scancode keycode) {}

    void TriggerArea::keyUpEvent(SDL_Scancode keycode) {}

    void TriggerArea::fingerDownEvent(SDL_FingerID id) {}

    void TriggerArea::fingerUpEvent(SDL_FingerID id) {}

    void TriggerArea::fingerTouchedEvent(SDL_FingerID id) {}

    void TriggerArea::fingerMovedEvent(SDL_FingerID id, const Vector2 &pos, const Vector2 &dis) {}

    void TriggerArea::fingerMovedInEvent() {}

    void TriggerArea::fingerMovedOutEvent() {}
}
