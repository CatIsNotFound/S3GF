#pragma once
#ifndef MYENGINE_COMPONETS_H
#define MYENGINE_COMPONETS_H
#include "Basic.h"
#include "MultiThread/Components.h"

namespace MyEngine {
    class Font {
    public:
        enum Style {
            Regular = 0x0,
            Bold = 0x1,
            Italic = 0x2,
            Underline = 0x4,
            Strikethrough = 0x8
        };
        enum Direction {
            LeftToRight = 4,
            RightToLeft,
            TopToBottom,
            BottomToTop
        };
        enum Hinting {
            Normal,
            Light,
            Mono,
            None,
            SubPixel
        };
        Font(Font &&) = delete;
        Font(const Font &) = delete;
        Font &operator=(Font &&) = delete;
        Font &operator=(const Font &) = delete;
        explicit Font(const std::string& font_path, float font_size = 9.f);
        ~Font();

        void setFontPath(const std::string& font_path);
        [[nodiscard]] const std::string& fontPath() const;
        void setFontSize(float size);
        float fontSize() const;
        void setFontColor(const SDL_Color& color);
        const SDL_Color& fontColor() const;
        void setStyle(uint32_t flags);
        void setOutline(uint32_t value = 0);
        uint32_t outline() const;
        void setOutlineColor(const SDL_Color& color);
        SDL_Color outlineColor() const;
        void setFontDirection(Direction direction);
        Direction fontDirection() const;
        void setFontHinting(uint32_t flags);
        void setFontKerning(bool enabled);
        bool fontKerning() const;
        void setLineSpacing(uint32_t spacing);
        uint32_t lineSpacing() const;
        SDL_Surface* toImage(const std::string& text);
        SDL_Surface* toImage(const std::string& text, const SDL_Color& backgrond_color);

        TTF_Font* self() const;
    private:
        TTF_Font* _font{nullptr};
        std::string _font_path{};
        float _font_size{};
        SColor _font_color{};
        uint32_t _font_style_flags{};
        uint32_t _font_outline{};
        SColor _outline_color{};
        Direction _font_direction{};
        uint32_t _font_hinting{};
        bool _font_kerning{};
        uint32_t _line_spacing{};
        bool _font_is_loaded{false};
    };

    using FontMap = std::unordered_map<std::string, std::string>;
    class FontDatabase {
    public:
        FontDatabase() = delete;
        FontDatabase(const FontDatabase&) = delete;
        FontDatabase(FontDatabase&&) = delete;
        ~FontDatabase() = delete;
        FontDatabase& operator=(const FontDatabase&) = delete;
        FontDatabase& operator=(FontDatabase&&) = delete;
        struct FontInfo {
            std::string font_name;
            std::string font_path;
        };

        static FontMap getFontDatabaseFromSystem();
        static std::string findFontFromSystem(const std::string &font_name);
        static std::vector<FontInfo> getSystemDefaultFont();
    private:
        static bool _is_loaded;
        static FontMap _font_db;
    };

    class BGM {
    public:
        enum PlayStatus {
            Invalid,
            Loading,
            Loaded,
            Playing,
            Paused,
            FadingIn,
            FadingOut
        };
        explicit BGM(MIX_Mixer* mixer, const std::string& path = {});
        ~BGM();

        void setPath(const std::string& path);
        [[nodiscard]] const std::string& path() const;
        [[nodiscard]] bool isLoaded() const;

        bool play(int64_t start_position = 0, bool loop = false, int64_t fade_in_duration = 0);
        void stop(int64_t fade_out_duration = 0);
        void pause();
        bool resume();
        bool forward(int64_t ms = 3000);
        bool backward(int64_t ms = 3000);
        bool playAt(int64_t position);
        [[nodiscard]] int64_t position() const;
        [[nodiscard]] int64_t duration() const;
        [[nodiscard]] PlayStatus playStatus() const;
        [[nodiscard]] std::string playStatusText() const;
        bool setVolume(float volume);
        bool setMuted(bool enabled);
        bool setLRChannel(float left, float right);
        bool set3DPosition(float x, float y, float z);
        bool setSpeedAndPitch(float value = 1.0f);
        [[nodiscard]] bool isMuted() const;
        [[nodiscard]] float volume() const;
        [[nodiscard]] const MIX_StereoGains& getLRChannel() const;
        [[nodiscard]] const MIX_Point3D& get3DPosition() const;
        [[nodiscard]] float speedAndPitch() const;
        void load();
        void unload();
    private:
        PlayStatus _play_status;
        std::string _path;
        float _volume{1.f};
        bool _muted{false};
        MIX_StereoGains _stereo_gains{1.f, 1.f};
        MIX_Point3D _mix_3d{0.f, 1.f, 1.f};
        SDL_PropertiesID _prop_id{0};
        MIX_Mixer* _mixer;
        MIX_Audio* _audio{nullptr};
        MIX_Track* _track{nullptr};
        uint64_t _global_ev_id{0};
    };

    class SFX {
    public:
        explicit SFX(MIX_Mixer* mixer, const std::string& path = {});
        ~SFX();

        void setPath(const std::string& path);
        [[nodiscard]] const std::string& path() const;
        [[nodiscard]] bool isLoaded() const;

        bool play(bool loop = false, int64_t fade_in_duration = 0);
        void stop(int64_t fade_out_duration = 0);
        [[nodiscard]] int64_t position() const;
        [[nodiscard]] int64_t duration() const;
        [[nodiscard]] bool isLoop() const;
        [[nodiscard]] bool isPlaying() const;
        bool setVolume(float volume);
        bool setMuted(bool enabled);
        bool setLRChannel(float left, float right);
        bool set3DPosition(float x, float y, float z);
        bool setSpeedAndPitch(float value = 1.0f);
        [[nodiscard]] bool isMuted() const;
        [[nodiscard]] float volume() const;
        [[nodiscard]] const MIX_StereoGains& getLRChannel() const;
        [[nodiscard]] const MIX_Point3D& get3DPosition() const;
        [[nodiscard]] float speedAndPitch() const;

        void load();
        void unload();
    private:
        bool _is_load{false}, _is_loop{false}, _is_playing{false};
        std::string _path;
        SDL_PropertiesID _prop_id{0};
        float _volume{1.f};
        bool _muted{false};
        MIX_StereoGains _stereo_gains{1.f, 1.f};
        MIX_Point3D _mix_3d{0.f, 1.f, 1.f};
        MIX_Mixer* _mixer;
        MIX_Audio* _audio{nullptr};
        MIX_Track* _track{nullptr};
    };

    class Renderer;
    class TextureProperty {
    public:
        bool clip_mode{false};
        SDL_FRect clip_area{};
        SDL_Color color_alpha{StdColor::White};
        double rotate_angle{0.0};
        SDL_FlipMode flip_mode{SDL_FLIP_NONE};
        TextureProperty() {
            _position = {0, 0};
            _size = {0, 0};
            _scaled_position = {0, 0};
            _scaled_size = {0, 0};
            _scale = {1.f};
            _scaled_clip_geometry = GeometryF{0, 0, 0, 0};
        }
        explicit TextureProperty(TextureProperty* textureProperty) {
            _position = textureProperty->_position;
            _size = textureProperty->_size;
            _scaled_position = textureProperty->_scaled_position;
            _scaled_size = textureProperty->_scaled_size;
            _scale = textureProperty->_scale;
            clip_mode = textureProperty->clip_mode;
            clip_area = textureProperty->clip_area;
            color_alpha = textureProperty->color_alpha;
            rotate_angle = textureProperty->rotate_angle;
            flip_mode = textureProperty->flip_mode;
            _scaled_clip_geometry = textureProperty->_scaled_clip_geometry;
        }
        TextureProperty(const TextureProperty& textureProperty) {
            _position = textureProperty._position;
            _size = textureProperty._size;
            _scaled_position = textureProperty._scaled_position;
            _scaled_size = textureProperty._scaled_size;
            _scale = textureProperty._scale;
            clip_mode = textureProperty.clip_mode;
            clip_area = textureProperty.clip_area;
            color_alpha = textureProperty.color_alpha;
            rotate_angle = textureProperty.rotate_angle;
            flip_mode = textureProperty.flip_mode;
            _scaled_clip_geometry = textureProperty._scaled_clip_geometry;
        }
        void reset(const TextureProperty& property) {
            _position = property._position;
            _size = property._size;
            _scale = property._scale;
            _scaled_position = property._scaled_position;
            _scaled_size = property._scaled_size;
            clip_mode = property.clip_mode;
            clip_area = property.clip_area;
            color_alpha = property.color_alpha;
            rotate_angle = property.rotate_angle;
            flip_mode = property.flip_mode;
            _scaled_clip_geometry = property._scaled_clip_geometry;
        }
        void reset(TextureProperty&& property) {
            _position = property._position;
            _size = property._size;
            _scale = property._scale;
            _scaled_position = property._scaled_position;
            _scaled_size = property._scaled_size;
            clip_mode = property.clip_mode;
            clip_area = property.clip_area;
            color_alpha = property.color_alpha;
            rotate_angle = property.rotate_angle;
            flip_mode = property.flip_mode;
            _scaled_clip_geometry = property._scaled_clip_geometry;
        }

        void move(const Vector2& pos) {
            _position.reset(pos);
            setScale(_scale);
        }
        void move(float x, float y) {
            _position.reset(x, y);
            setScale(_scale);
        }
        [[nodiscard]] const Vector2& position() const {
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
        [[nodiscard]] const Size& size() const {
            return _size;
        }
        void setGeometry(const Vector2& pos, const Size& size) {
            _position.reset(pos);
            _size.reset(size);
            setScale(_scale);
        }
        void setGeometry(float x, float y, float width, float height) {
            _position.reset(x, y);
            _size.reset(width, height);
            setScale(_scale);
        }
        void setGeometry(const GeometryF& geometry) {
            _position.reset(geometry.pos);
            _size.reset(geometry.size);
            setScale(_scale);
        }
        [[nodiscard]] GeometryF geometry() const {
            return GeometryF{_position, _size};
        }
        void setScale(float scale = 1.0f) {
            _scale = scale;

            auto scaled_pos = _position + _anchor;
            _scaled_position.reset((_position.x - scaled_pos.x) * _scale + scaled_pos.x,
                                   (_position.y - scaled_pos.y) * _scale + scaled_pos.y);

            _scaled_size.reset(_size.width * _scale, _size.height * _scale);
            _scaled_anchor.reset(_anchor * _scale);
            if (clip_mode) {
                auto scaled_clip_pos = Vector2{clip_area.x + _anchor.x, clip_area.y + _anchor.y};
                _scaled_clip_geometry.reset((clip_area.x - scaled_clip_pos.x) * _scale + scaled_clip_pos.x,
                                               (clip_area.y - scaled_clip_pos.y) * _scale + scaled_clip_pos.y,
                                               clip_area.w * _scale, clip_area.h * _scale);
            }
        }
        [[nodiscard]] float scale() const {
            return _scale;
        }
        [[nodiscard]] GeometryF scaledGeometry() const {
            return GeometryF{_scaled_position, _scaled_size};
        }
        [[nodiscard]] GeometryF scaledClipAreaGeometry() const {
            return _scaled_clip_geometry;
        }
        void setAnchor(const Vector2& pos) {
            _anchor.reset(pos);
            auto scaled_pos = _position + _anchor;
            _scaled_position.reset((_position.x - scaled_pos.x) * _scale + scaled_pos.x,
                                   (_position.y - scaled_pos.y) * _scale + scaled_pos.y);

            _scaled_size.reset(_size.width * _scale, _size.height * _scale);
            _scaled_anchor.reset(_anchor * _scale);
        }
        void setAnchor(float x, float y) {
            _anchor.reset(x, y);
            auto scaled_pos = _position + _anchor;
            _scaled_position.reset((_position.x - scaled_pos.x) * _scale + scaled_pos.x,
                                   (_position.y - scaled_pos.y) * _scale + scaled_pos.y);

            _scaled_size.reset(_size.width * _scale, _size.height * _scale);
            _scaled_anchor.reset(_anchor * _scale);
        }

        void setAnchorToCenter() {
            setAnchor(_size.width / 2, _size.height / 2);
        }
        [[nodiscard]] const Vector2& anchor() const {
            return _anchor;
        }
        [[nodiscard]] const Vector2& scaledAnchor() const {
            return _scaled_anchor;
        }
    private:
        Vector2 _position;
        Size _size;
        float _scale;
        Vector2 _scaled_position;
        Size _scaled_size;
        GeometryF _scaled_clip_geometry;
        Vector2 _anchor, _scaled_anchor;
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

        [[nodiscard]] Renderer* render() const;

        bool setImagePath(const std::string& path);
        [[nodiscard]] const std::string& imagePath() const;
        bool setImageFromSurface(SDL_Surface* surface, bool deep_copy = false);

        [[nodiscard]] SDL_Texture* self() const;
        [[nodiscard]] SDL_Surface* surface() const;
        [[nodiscard]] bool isValid() const;
        TextureProperty* property();

        virtual void draw();
    private:
        SDL_Surface* _surface;
        SDL_Texture* _texture;
        std::string _path;
        std::unique_ptr<TextureProperty> _property;
        Renderer* _renderer;
    };

    class TextureAtlas : public Texture {
    public:
        struct Tile {
            std::string name;
            std::vector<std::unique_ptr<TextureProperty>> properties;
        };
        using constIter = std::unordered_map<std::string, Tile>::const_iterator;
        using iter = std::unordered_map<std::string, Tile>::iterator;
        TextureAtlas(const TextureAtlas &) = delete;
        TextureAtlas(TextureAtlas &&) = delete;
        TextureAtlas &operator=(const TextureAtlas &) = delete;
        TextureAtlas &operator=(TextureAtlas &&) = delete;

        explicit TextureAtlas(const std::string &path, Renderer *renderer);
        explicit TextureAtlas(SDL_Surface* surface, Renderer *renderer, bool deep_copy = false);
        ~TextureAtlas();

        iter begin() { return _tiles_map.begin(); }
        constIter begin() const { return _tiles_map.cbegin(); }
        iter end() { return _tiles_map.end(); }
        constIter end() const { return _tiles_map.cend(); }
        size_t count() { return _tiles_map.size(); }

        bool addTiles(const std::string& tiles_name, const MyEngine::GeometryF &clip_geometry);
        bool addTilesProperty(const std::string& tiles_name);
        bool eraseTiles(const std::string& tiles_name);
        TextureProperty* tilesProperty(const std::string& tiles_name, size_t index = 0);
        [[nodiscard]] size_t tilesPropertyCount(const std::string& tiles_name) const;
        void setCurrentTiles(const std::string& tiles_name);
        [[nodiscard]] const std::string& currentTiles() const;
        StringList tilesNameList() const;
        [[nodiscard]] bool isTilesNameExist(const std::string& tiles_name) const;

        void draw() override;
        void draw(size_t index = 0);
        void draw(const std::string& tiles_name, size_t index = 0);
    private:
        std::unordered_map<std::string, Tile> _tiles_map;
        std::string _current_tiles;
    };

    class TextureAnimation {
    public:
        struct Frame {
            SDL_Surface *surface;
            SDL_Texture *texture;
            uint32_t duration;
        };
        TextureAnimation(const TextureAnimation &) = delete;
        TextureAnimation(TextureAnimation &&) = delete;
        TextureAnimation &operator=(const TextureAnimation &) = delete;
        TextureAnimation &operator=(TextureAnimation &&) = delete;
        explicit TextureAnimation(const std::string& file_path, Renderer* renderer);
        ~TextureAnimation();

        void setDurationInFrame(size_t index, size_t duration);
        void setDurationPerFrame(size_t duration);
        [[nodiscard]] size_t durationInFrame(size_t index) const;
        [[nodiscard]] size_t currentFrame() const;
        [[nodiscard]] size_t framesCount() const;

        [[nodiscard]] const Frame* indexOfFrame(size_t index) const;
        [[nodiscard]] bool isNull() const;
        [[nodiscard]] TextureProperty* property();

        bool loadAnimation(const std::string& path);
        void draw();
        void play(size_t frame = 0);
        void stop();
    private:
        Renderer* _renderer;
        IMG_Animation* _img_ani;
        std::vector<std::unique_ptr<Frame>> _textures;
        std::unique_ptr<TextureProperty> _property;
        GeometryF _geometry{};
        std::string _file_path{};
        size_t _cur_frame{0};
        uint64_t _start_time{};
        bool _null{true}, _playing{false};
    };
}
#include "Core.h"
#endif // !MYENGINE_COMPONETS_H