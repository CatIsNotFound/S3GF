#pragma once
#ifndef S3GF_COMPONETS_H
#define S3GF_COMPONETS_H
#include "Basic.h"
#include "MultiThread/Components.h"

namespace S3GF {
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

        void setFontSize(float size);
        float fontSize() const;
        void setFontColor(const SDL_Color& color);
        const SDL_Color fontColor() const;
        void setStyle(uint32_t flags);
        void setOutline(uint32_t value = 0);
        uint32_t outline() const;
        void setOutlineColor(const SDL_Color& color);
        const SDL_Color outlineColor() const;
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
        [[nodiscard]] bool isMuted() const;
        [[nodiscard]] float volume() const;
        [[nodiscard]] const MIX_StereoGains& getLRChannel() const;
        [[nodiscard]] const MIX_Point3D& get3DPosition() const;
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
        [[nodiscard]] bool isMuted() const;
        [[nodiscard]] float volume() const;
        [[nodiscard]] const MIX_StereoGains& getLRChannel() const;
        [[nodiscard]] const MIX_Point3D& get3DPosition() const;

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
    struct TextureProperty {
        bool clip_mode;
        SDL_FRect clip_area;
        SDL_Color color_alpha;
        double rotate_angle;
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
        TextureProperty* property();

        virtual void draw() const;
    private:
        SDL_Surface* _surface;
        SDL_Texture* _texture;
        std::string _path;
        std::unique_ptr<TextureProperty> _property;
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

        void setTiles(const std::string& tiles_name, TextureProperty&& tiles_property);
        TextureProperty* tilesProperty(const std::string& tiles_name);
        void setCurrentTiles(const std::string& tiles_name);
        [[nodiscard]] const std::string& currentTiles() const;

        void draw() const override;
        void draw(const std::string& tiles_name) const;
    private:
        std::unordered_map<std::string, std::shared_ptr<TextureProperty>> _tiles_map;
        std::string _current_tiles;
    };
}
#include "Core.h"
#endif // !S3GF_COMPONETS_H