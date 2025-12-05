#include "Components.h"
#include "Utils/Logger.h"
#include "Utils/FileSystem.h"
#include "Utils/Random.h"
#include "Utils/RGBAColor.h"
#include "Utils/DateTime.h"
#define MAX_AUDIO_FILE_SIZE 2 * 1024 * 1024 /// Defined the larger audio file

namespace S3GF {
    Font::Font(const std::string& font_path, float font_size)
            : _font_size(font_size){
        _font = TTF_OpenFont(font_path.c_str(), font_size);
        if (!_font) {
            Logger::log(std::format("Can't load font from path '{}'.", font_path), Logger::ERROR);
        }
        _font_is_loaded = true;
    }

    Font::~Font() {
        if (_font) {
            TTF_CloseFont(_font);
        }
    }

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

    const SDL_Color Font::fontColor() const {
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

    const SDL_Color Font::outlineColor() const {
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
            Logger::log("Font is not loaded! Did you forgot to load font?", Logger::ERROR);
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
            Logger::log(std::format("Can't drawEvent the current text!\nException: {}", SDL_GetError()), Logger::ERROR);
        }
        return surface;
    }

    SDL_Surface* Font::toImage(const std::string& text, const SDL_Color& backgrond_color) {
        SDL_Surface* surface = nullptr;
        if (!_font_is_loaded) {
            Logger::log("Font is not loaded! Did you forget to load font?", Logger::ERROR);
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
            Logger::log(std::format("Can't drawEvent the current text!\nException: {}", SDL_GetError()), Logger::ERROR);
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
            Logger::log("FontDatabase: Get Font files from system!", Logger::DEBUG);
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
        std::vector<FontDatabase::FontInfo> default_fonts;
#ifdef _WIN32
        StringList common_fonts = { "arial", "segoeui", "tahoma", "verdana", "calibri" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) default_fonts.push_back({font_name, path});
        }
#endif
#ifdef __linux__
        StringList common_fonts = { "AdwaitaSans-Regular", "DejaVuSans", "Roboto-Regular", "Ubuntu-R" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) default_fonts.push_back({font_name, path});
        }
#endif
#ifdef __APPLE__
        StringList common_fonts = { "SanFrancisco-Regular", "HelveticaNeue", "ArialMT", "TimesNewRomanPSMT" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) default_fonts.push_back({font_name, path});
        }
#endif
        return default_fonts;
    }

    Texture::Texture(const std::string &path, Renderer *renderer) : _renderer(renderer), _texture(nullptr), _path(path) {
        _surface = IMG_Load(path.c_str());
        if (!_surface) {
            Logger::log(std::format("The image path '{}' is not found!", path), Logger::ERROR);
            _property = std::make_unique<TextureProperty>();
            return;
        }
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<TextureProperty>();
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
            _property = std::make_unique<TextureProperty>();
            return;
        }
        _surface = (deep_copy ? SDL_DuplicateSurface(surface) : surface);
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<TextureProperty>();
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
            _property = std::make_unique<TextureProperty>();
            return;
        }
        _property = std::make_unique<TextureProperty>();
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
            _property = std::make_unique<TextureProperty>();
            return false;
        }
        _surface = (deep_copy ? SDL_DuplicateSurface(surface) : surface);
        _texture = SDL_CreateTextureFromSurface(_renderer->self(), _surface);
        _property = std::make_unique<TextureProperty>();
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

    TextureProperty* Texture::property() {
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

    void TextureAtlas::setTiles(const std::string &tiles_name, TextureProperty &&tiles_property) {
        if (_tiles_map.contains(tiles_name)) {
            _tiles_map[tiles_name]->reset(tiles_property);
        } else {
            _tiles_map.emplace(tiles_name, std::shared_ptr<TextureProperty>(new TextureProperty(tiles_property)));
        }
    }

    TextureProperty *TextureAtlas::tilesProperty(const std::string &tiles_name) {
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

    StringList TextureAtlas::tilesNameList() const {
        StringList out;
        for (auto& [name, t] : _tiles_map) {
            out.emplace_back(name);
        }
        return out;
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

    BGM::BGM(MIX_Mixer *mixer, const std::string &path) : _mixer(mixer), _path(path) {
        if (!_mixer) {
            Logger::log("BGM: The specified mixer can not be null!", Logger::FATAL);
            Engine::throwFatalError();
        }
        _global_ev_id = IDGenerator::getID(NewGlobalEventID);
        EventSystem::global()->appendGlobalEvent(_global_ev_id, [this]() {
            if (_play_status == FadingOut && !MIX_TrackPlaying(_track)) {
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
        load();
    }

    BGM::~BGM() {
        if (_play_status > Invalid) unload();
        EventSystem::global()->removeGlobalEvent(_global_ev_id);
    }

    void BGM::setPath(const std::string &path) {
        _path = path;
    }

    const std::string &BGM::path() const {
        return _path;
    }

    bool BGM::isLoaded() const {
        return _play_status >= Loaded;
    }

    bool BGM::play(int64_t start_position, bool loop, int64_t fade_in_duration) {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't play current audio! Current audio is not valid!", Logger::ERROR);
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
                Logger::log(std::format("BGM: Play audio failed! The file path '{}' is not valid! "
                                        "Exception: {}", _path, SDL_GetError()), Logger::ERROR);
                _play_status = Invalid;
                return false;
            }
            _play_status = (fade_in_duration ? FadingIn : Playing);
        }
        return true;
    }

    void BGM::stop(int64_t fade_out_duration) {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't stop current audio! Current audio is not valid!", Logger::ERROR);
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
            Logger::log("BGM: Can't pause current audio! Current status is not valid!", Logger::ERROR);
            return;
        }
        if (_play_status != Playing) {
            Logger::log("BGM: Current audio is not playing!", Logger::WARN);
            return;
        }
        MIX_PauseTrack(_track);
        _play_status = Paused;
    }

    bool BGM::resume() {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't resume current audio! Current status is not valid!", Logger::ERROR);
            return false;
        }
        if (_play_status != Paused) {
            Logger::log("BGM: Current audio is already playing!", Logger::WARN);
            return false;
        }
        if (position() >= duration()) playAt(0);
        auto _ret = MIX_ResumeTrack(_track);
        if (!_ret) {
            Logger::log("BGM: Can't resume current audio! Current status is not valid!", Logger::ERROR);
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
            Logger::log("BGM: Failed to set playback position!", Logger::WARN);
            return false;
        }
        return true;
    }

    bool BGM::backward(int64_t ms) {
        auto pos = position();
        auto new_pos = std::min(pos - ms, duration());
        if (new_pos < 0) new_pos = 0;
        if (!MIX_SetTrackPlaybackPosition(_track, MIX_TrackMSToFrames(_track, new_pos))) {
            Logger::log("BGM: Failed to set playback position!", Logger::WARN);
            return false;
        }
        return true;
    }

    bool BGM::playAt(int64_t position) {
        auto new_pos = std::min(position, duration());
        if (new_pos < 0) new_pos = 0;
        if (!MIX_SetTrackPlaybackPosition(_track, MIX_TrackMSToFrames(_track, new_pos))) {
            Logger::log("BGM: Failed to set playback position!", Logger::WARN);
            return false;
        }
        return true;
    }

    int64_t BGM::position() const {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't pause current audio! Current audio is not valid!", Logger::ERROR);
            return 0;
        }
        return MIX_TrackFramesToMS(_track, MIX_GetTrackPlaybackPosition(_track));
    }

    int64_t BGM::duration() const {
        if (_play_status < Loaded) {
            Logger::log("BGM: Can't pause current audio! Current audio is not valid!", Logger::ERROR);
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
        auto new_vol = volume < 0 ? 0 : std::min(volume, 10.f);
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
        _stereo_gains.left = (_stereo_gains.left > 0 ? std::min(left, 10.f) : 0.f);
        _stereo_gains.right = (_stereo_gains.right > 0 ? std::min(right, 10.f) : 0.f);
        return MIX_SetTrackStereo(_track, &_stereo_gains);
    }

    bool BGM::set3DPosition(float x, float y, float z) {
        _mix_3d = { .x = x, .y = y, .z = z };
        if (!MIX_SetTrack3DPosition(_track, &_mix_3d)) {
            Logger::log(std::format("BGM::set3DPosition: Failed to set 3D position! Exception: {}", SDL_GetError()), Logger::WARN);
            return false;
        }
        return true;
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

    void BGM::load() {
        _play_status = Loading;
        _audio = MIX_LoadAudio(_mixer, _path.c_str(), false);
        if (!_audio) {
            Logger::log(std::format("BGM: The specified file path '{}' is not valid! Exception: {}",
                                    _path, SDL_GetError()), Logger::ERROR);
            _play_status = Invalid;
            return;
        }
        _track = MIX_CreateTrack(_mixer);
        if (!_track) {
            Logger::log(std::format("BGM: Create audio track failed! Exception: {}"
                    , SDL_GetError()), Logger::ERROR);
            _play_status = Invalid;
            return;
        }
        if (!MIX_SetTrackAudio(_track, _audio)) {
            Logger::log(std::format("BGM: The specified file path '{}' can not set as audio track! Exception: {}",
                                    _path, SDL_GetError()), Logger::ERROR);
            _play_status = Invalid;
            return;
        }
        _play_status = Loaded;
    }

    void BGM::unload() {
        if (_track) {
            MIX_DestroyTrack(_track);
        }
        if (_audio) {
            MIX_DestroyAudio(_audio);
        }
        SDL_DestroyProperties(_prop_id);
        _play_status = Invalid;
    }

    SFX::SFX(MIX_Mixer *mixer, const std::string &path) : _mixer(mixer), _path(path) {
        if (!_mixer) {
            Logger::log("BGM: The specified mixer can not be null!", Logger::FATAL);
            Engine::throwFatalError();
        }
        load();
    }

    SFX::~SFX() {
        if (_is_load) unload();
    }

    void SFX::setPath(const std::string &path) {
        _path = path;
    }

    const std::string &SFX::path() const {
        return _path;
    }

    bool SFX::isLoaded() const {
        return _is_load;
    }

    bool SFX::play(bool loop, int64_t fade_in_duration) {
        if (!_is_load) {
            Logger::log("BGM: Can't play current audio! Current audio is not valid!", Logger::ERROR);
            _is_playing = false;
            return false;
        }
        _prop_id = SDL_CreateProperties();
        SDL_SetNumberProperty(_prop_id, MIX_PROP_PLAY_LOOPS_NUMBER, (loop ? -1 : 0));
        SDL_SetNumberProperty(_prop_id, MIX_PROP_PLAY_FADE_IN_MILLISECONDS_NUMBER, fade_in_duration);
        if (!MIX_PlayTrack(_track, _prop_id)) {
            Logger::log(std::format("BGM: Play audio failed! The file path '{}' is not valid! "
                                    "Exception: {}", _path, SDL_GetError()), Logger::ERROR);
            return false;
        }
        return true;
    }

    void SFX::stop(int64_t fade_out_duration) {
        if (!_is_load) {
            Logger::log("BGM: Can't stop current audio! Current audio is not valid!", Logger::ERROR);
        }
        auto ms = (fade_out_duration > 0 ? MIX_TrackMSToFrames(_track, fade_out_duration) : 0);
        MIX_StopTrack(_track, ms);
        _is_playing = false;
    }

    int64_t SFX::position() const {
        if (!_is_load) {
            Logger::log("BGM: Can't pause current audio! Current audio is not valid!", Logger::ERROR);
            return 0;
        }
        return MIX_TrackFramesToMS(_track, MIX_GetTrackPlaybackPosition(_track));
    }

    int64_t SFX::duration() const {
        if (!_is_load) {
            Logger::log("BGM: Can't pause current audio! Current audio is not valid!", Logger::ERROR);
            return 0;
        }
        return MIX_TrackFramesToMS(_track, MIX_GetAudioDuration(_audio));
    }

    bool SFX::isLoop() const {
        return _is_loop;
    }

    bool SFX::isPlaying() const {
        return _is_playing;
    }

    bool SFX::setVolume(float volume) {
        auto new_vol = volume < 0 ? 0 : std::min(volume, 10.f);
        if (MIX_SetTrackGain(_track, new_vol)) {
            _volume = new_vol;
            return true;
        }
        return false;
    }

    bool SFX::setMuted(bool enabled) {
        if (MIX_SetTrackGain(_track, (enabled ? 0.f : _volume))) {
            _muted = enabled;
            return true;
        }
        return false;
    }

    bool SFX::setLRChannel(float left, float right) {
        _stereo_gains.left = (_stereo_gains.left > 0 ? std::min(left, 10.f) : 0.f);
        _stereo_gains.right = (_stereo_gains.right > 0 ? std::min(right, 10.f) : 0.f);
        return MIX_SetTrackStereo(_track, &_stereo_gains);
    }

    bool SFX::set3DPosition(float x, float y, float z) {
        _mix_3d = { .x = x, .y = y, .z = z };
        if (!MIX_SetTrack3DPosition(_track, &_mix_3d)) {
            Logger::log(std::format("BGM::set3DPosition: Failed to set 3D position! Exception: {}", SDL_GetError()), Logger::WARN);
            return false;
        }
        return true;
    }

    float SFX::volume() const {
        return _volume;
    }

    bool SFX::isMuted() const {
        return _muted;
    }

    const MIX_StereoGains& SFX::getLRChannel() const {
        return _stereo_gains;
    }

    const MIX_Point3D& SFX::get3DPosition() const {
        return _mix_3d;
    }

    void SFX::load() {
        auto size = FileSystem::readableSize(_path, FileSystem::MB);
        _audio = MIX_LoadAudio(_mixer, _path.c_str(), (size >= MAX_AUDIO_FILE_SIZE));
        if (!_audio) {
            Logger::log(std::format("BGM: The specified file path '{}' is not valid! Exception: {}",
                                    _path, SDL_GetError()), Logger::ERROR);
            _is_load = false;
            return;
        }
        _track = MIX_CreateTrack(_mixer);
        if (!_track) {
            Logger::log(std::format("BGM: Create audio track failed! Exception: {}"
                    , SDL_GetError()), Logger::ERROR);
            _is_load = false;
            return;
        }
        if (!MIX_SetTrackAudio(_track, _audio)) {
            Logger::log(std::format("BGM: The specified file path '{}' can not set as audio track! Exception: {}",
                                    _path, SDL_GetError()), Logger::ERROR);
            _is_load = false;
            return;
        }
        _is_load = true;
    }

    void SFX::unload() {
        if (_track) {
            MIX_DestroyTrack(_track);
        }
        if (_audio) {
            MIX_DestroyAudio(_audio);
        }
        SDL_DestroyProperties(_prop_id);
        _is_load = false;
    }


}