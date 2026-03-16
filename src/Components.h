#pragma once
#ifndef MYENGINE_COMPONETS_H
#define MYENGINE_COMPONETS_H
#include "Basic.h"
#include "MultiThread/Components.h"
#include "Utils/WavWriter.h"

namespace MyEngine {
    class Font {
    public:
        enum Style : uint8_t {
            Regular = 0x0,
            Bold = 0x1,
            Italic = 0x2,
            Underline = 0x4,
            Strikethrough = 0x8
        };
        enum Direction : uint8_t {
            LeftToRight = 4,
            RightToLeft,
            TopToBottom,
            BottomToTop
        };
        enum Hinting : uint8_t {
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
        [[nodiscard]] float fontSize() const;
        void setFontColor(const SDL_Color& color);
        [[nodiscard]] const SDL_Color& fontColor() const;
        void setStyle(uint32_t flags);
        void setOutline(uint32_t value = 0);
        [[nodiscard]] uint32_t outline() const;
        void setOutlineColor(const SDL_Color& color);
        [[nodiscard]] SDL_Color outlineColor() const;
        void setFontDirection(Direction direction);
        [[nodiscard]] Direction fontDirection() const;
        void setFontHinting(uint32_t flags);
        void setFontKerning(bool enabled);
        bool fontKerning() const;
        void setLineSpacing(uint32_t spacing);
        [[nodiscard]] uint32_t lineSpacing() const;
        SDL_Surface* toImage(const std::string& text);
        SDL_Surface* toImage(const std::string& text, const SDL_Color& background_color);

        [[nodiscard]] TTF_Font* self() const;
    private:
        TTF_Font* _font{nullptr};
        std::string _font_path{};
        float _font_size{};
        SColor _font_color{};
        uint32_t _font_style_flags{};
        uint32_t _font_outline{};
        SColor _outline_color{};
        Direction _font_direction{};
        bool _font_kerning{};
        bool _font_is_loaded{false};
        uint32_t _font_hinting{};
        uint32_t _line_spacing{};
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
        static std::vector<FontInfo> _def_fonts;
    };

    class BGM {
    public:
        enum PlayStatus : uint8_t {
            Invalid,
            Loading,
            Loaded,
            Playing,
            Paused,
            FadingIn,
            FadingOut
        };
        explicit BGM(MIX_Mixer* mixer, const std::string& path = {});
        explicit BGM(MIX_Mixer* mixer, MIX_Audio* audio);
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
        [[nodiscard]] const MIX_Audio* audio() const;
        [[nodiscard]] const MIX_Track* track() const;
        [[nodiscard]] MIX_Track *getTrack() const;
    private:
        void init();
        void load();
        void unload();
        std::string _path;
        float _volume{1.f};
        PlayStatus _play_status;
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
        constexpr static int SINE_HZ_C5 = 523;
        constexpr static int SINE_HZ_Db5 = 554;
        constexpr static int SINE_HZ_D5 = 587;
        constexpr static int SINE_HZ_Eb5 = 622;
        constexpr static int SINE_HZ_E5 = 659;
        constexpr static int SINE_HZ_Fb5 = 698;
        constexpr static int SINE_HZ_F5 = 740;
        constexpr static int SINE_HZ_Gb5 = 784;
        constexpr static int SINE_HZ_G5 = 830;
        constexpr static int SINE_HZ_Ab5 = 880;
        constexpr static int SINE_HZ_A5 = 932;
        constexpr static int SINE_HZ_Bb5 = 987;
        constexpr static int SINE_HZ_C6 = 1046;
        explicit SFX(MIX_Mixer* mixer, const std::string& path = {});
        explicit SFX(MIX_Mixer* mixer, int hz, float wave_volume = 1.f, int64_t ms = 5000);
        ~SFX();

        void setPath(const std::string& path);
        [[nodiscard]] const std::string& path() const;
        void loadSineWave(int hz, float wave_volume = 1.f, int64_t ms = 5000);
        [[nodiscard]] bool isLoaded() const;
        void setDefaultSFX(float volume,
                           MIX_StereoGains &&stereo_gains = {1.f, 1.f});
        size_t findFreeIndex();
        [[nodiscard]] size_t lastIndex() const;

        bool play(bool loop = false, int64_t fade_in_duration = 0);
        bool play(size_t index);
        void stop(int64_t fade_out_duration = 0);
        void stop(size_t index, int64_t fade_out_duration);
        void stopAll(int64_t fade_out_duration = 0);
        void resetAll();
        [[nodiscard]] std::optional<int64_t> position(size_t index = 0) const;
        [[nodiscard]] int64_t duration() const;
        [[nodiscard]] std::optional<bool> isLoop(size_t index = 0) const;
        [[nodiscard]] std::optional<bool> isPlaying(size_t index = 0) const;
        bool setVolume(float volume, size_t index = 0);
        bool setLRChannel(float left, float right, size_t index = 0);
        bool set3DPosition(float x, float y, float z, size_t index = 0);
        bool setSpeedAndPitch(float value, size_t index = 0);
        [[nodiscard]] std::optional<float> volume(size_t index = 0) const;
        [[nodiscard]] const std::optional<MIX_StereoGains> getLRChannel(size_t index = 0) const;
        [[nodiscard]] const std::optional<MIX_Point3D> get3DPosition(size_t index = 0) const;
        [[nodiscard]] std::optional<float> speedAndPitch(size_t index = 0) const;
        [[nodiscard]] const MIX_Audio* audio() const;
        [[nodiscard]] const std::optional<MIX_Track*> track(size_t index = 0) const;
        [[nodiscard]] std::optional<MIX_Track*> getTrack(size_t index = 0) const;
        [[nodiscard]] size_t count() const;
        [[nodiscard]] size_t playingCount() const;
    private:
        void load();
        void unload();
        [[nodiscard]] size_t findBusyTrackIndex() const;
        [[nodiscard]] size_t findFreeTrackIndex() const;
        std::string _path;
        bool _is_load{false};
        size_t _last_index{0};
        SDL_PropertiesID _prop_id{0};
        struct Track {
            MIX_Track* track{nullptr};
            MIX_StereoGains stereo_gains{1.f, 1.f};
            MIX_Point3D point_3d{0.f, 1.f, 1.f};
            float volume{1.f};
        };
        MIX_Mixer* _mixer;
        MIX_Audio* _audio{nullptr};
        std::vector<Track> _tracks{};
        Track _default_track{};
    };

    class AudioDecibelMeter;

    class AudioRecorder {
    public:
        enum Status : uint8_t {
            Invalid,
            Idle,
            Recording,
            Playing,
            Processing
        };
        explicit AudioRecorder(SDL_AudioDeviceID output_id = SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK);
        explicit AudioRecorder(SDL_AudioDeviceID input_id, SDL_AudioDeviceID output_id);
        ~AudioRecorder();

        bool setInputDeviceID(SDL_AudioDeviceID input_id);
        bool reload();
        [[nodiscard]] SDL_AudioDeviceID inputDeviceID() const;
        [[nodiscard]] SDL_AudioDeviceID outputDeviceID() const;
        [[nodiscard]] SDL_AudioSpec inputAudioSpec() const;
        [[nodiscard]] SDL_AudioSpec outputAudioSpec() const;
        bool startRecord();
        bool stopRecord();
        [[nodiscard]] bool isRecording() const;
        [[nodiscard]] bool isValid() const;
        [[nodiscard]] SDL_AudioStream* audioStream() const;
        void setOutputFileName(const std::string& file_name);
        void setAudioDecibalMeter(AudioDecibelMeter* decibel_meter);
        AudioDecibelMeter* audioDecibelMeter() const;

    private:
        bool load();
        void unload();
        static void SDLCALL onRecordingF32(void *userdata, SDL_AudioStream *stream,
                                        int additional_amount, int total_amount);
        static void SDLCALL onRecordingS16(void *userdata, SDL_AudioStream *stream,
                                        int additional_amount, int total_amount);

        SDL_AudioStream* _stream{nullptr};
        SDL_AudioDeviceID _input_deviceID{0}, _output_deviceID{0};
        std::vector<float> _pcm_datas{};
        WAVWriter _wav_writer;
        AudioDecibelMeter* _audio_decibel_meter{};
        Status _status{Invalid};
    };

    class AudioDecibelMeter {
        using Audio = std::variant<std::monostate, BGM*, SFX*, MIX_Mixer*, AudioRecorder*>;
        friend class AudioRecorder;
    public:
        constexpr static float MUTED_DB = -96.f;
        explicit AudioDecibelMeter(BGM* bgm);
        explicit AudioDecibelMeter(SFX* sfx);
        explicit AudioDecibelMeter(MIX_Mixer* mixer);
        explicit AudioDecibelMeter(AudioRecorder* recorder);
        explicit AudioDecibelMeter() : _audio(std::monostate{}) {}

        void viewBGM(BGM* bgm);
        void viewSFX(SFX* sfx);
        void viewMixer(MIX_Mixer* mixer);
        void unload();
        [[nodiscard]] float mixDecibel(size_t index = 0) const;
        [[nodiscard]] float leftDecibel(size_t index = 0) const;
        [[nodiscard]] float rightDecibel(size_t index = 0) const;
        [[nodiscard]] float leftPeakDecibel(size_t index = 0) const;
        [[nodiscard]] float rightPeakDecibel(size_t index = 0) const;
        [[nodiscard]] float mixPeakDecibel(size_t index = 0) const;
    private:
        void init();
        void uninitialized();
        static float linearToDecibel(float linear);
        static void SDLCALL cookedBGM(void *userdata, MIX_Track *,
                const SDL_AudioSpec *spec, float *pcm, int samples);
        static void SDLCALL cookedSFX(void *userdata, MIX_Track *,
                const SDL_AudioSpec *spec, float *pcm, int samples);
        static void SDLCALL cookedMixer(void *userdata, MIX_Mixer *,
                const SDL_AudioSpec *spec, float *pcm, int samples);

        Audio _audio;
        struct LevelMeter {
            float _mix_dB{MUTED_DB};
            float _mix_peak_dB{MUTED_DB};
            float _left_dB{MUTED_DB};
            float _right_dB{MUTED_DB};
            float _left_peak_dB{MUTED_DB};
            float _right_peak_dB{MUTED_DB};
        };
        std::vector<LevelMeter> _level_meters;
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
                _scaled_clip_geometry.setGeometry((clip_area.x - scaled_clip_pos.x) * _scale + scaled_clip_pos.x,
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
        Texture(Texture &&) = delete;
        Texture &operator=(const Texture &) = delete;
        Texture &operator=(Texture &&) = delete;
        explicit Texture(const std::string &path, Renderer *renderer);
        explicit Texture(SDL_Surface*&& surface, Renderer *renderer, bool deep_copy = false);
        explicit Texture(Renderer* renderer, SDL_PixelFormat format, int width, int height, SDL_TextureAccess access);
        explicit Texture(const Texture& texture);
        virtual ~Texture();

        [[nodiscard]] Renderer* renderer() const;

        bool setImagePath(const std::string& path);
        [[nodiscard]] const std::string& imagePath() const;
        bool setImageFromSurface(SDL_Surface*&& surface, bool deep_copy = false);

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

    enum class MouseStatus : uint8_t;
    class Window;

    class TriggerArea {
        static constexpr uint8_t Status_OnArea{1};
        static constexpr uint8_t Status_TriggeredArea{1 << 1};
        static constexpr uint8_t Status_DisabledArea{1 << 2};
        static constexpr uint8_t Status_MouseButtonDown{1 << 3};
        static constexpr uint8_t Status_FingerDown{1 << 4};
        static constexpr uint8_t Status_KeyboardPressedDown{1 << 5};

    public:
        explicit TriggerArea(GeometryF geometry, Window* window);
        virtual ~TriggerArea();

        void setGeometry(float x, float y, float w, float h);
        void setGeometry(const Vector2& pos, const Size& size);
        void setGeometry(const GeometryF& geometry);
        void move(float x, float y);
        void move(const Vector2& pos);
        void resize(float w, float h);
        void resize(const Size& size);
        [[nodiscard]] const GeometryF& geometry() const;
        [[nodiscard]] const Vector2& position() const;
        [[nodiscard]] const Size& size() const;

        void setEnabled(bool enabled);
        [[nodiscard]] bool isEnabled() const;
        [[nodiscard]] bool isOnArea() const;
        [[nodiscard]] bool isTriggeredOnArea() const;
        [[nodiscard]] uint8_t events() const;

        void setTriggerKey(SDL_Scancode keycode);
        [[nodiscard]] const SDL_Scancode& triggerKey() const;
        void setTriggerEvent(const std::function<void()>& callback_function);

        const Window* window() const;
    protected:
        virtual void mouseDownEvent(MouseStatus button);
        virtual void mouseUpEvent(MouseStatus button);
        virtual void mouseMovedEvent(const Vector2& pos, const Vector2& dis);
        virtual void mouseMovedInEvent();
        virtual void mouseMovedOutEvent();
        virtual void mouseClickedEvent(MouseStatus button);
        virtual void keyPressedEvent(SDL_Scancode keycode);
        virtual void keyDownEvent(SDL_Scancode keycode);
        virtual void keyUpEvent(SDL_Scancode keycode);
        virtual void fingerDownEvent(SDL_FingerID id);
        virtual void fingerUpEvent(SDL_FingerID id);
        virtual void fingerTouchedEvent(SDL_FingerID id);
        virtual void fingerMovedEvent(SDL_FingerID id, const Vector2& pos, const Vector2& dis);
        virtual void fingerMovedInEvent();
        virtual void fingerMovedOutEvent();

    private:
        GeometryF _geometry;
        Window* _window;
        uint64_t _event_id;
        SDL_Scancode _key{};
        uint8_t _events{};
        MouseStatus _last_mouse_status{};
        std::function<void()> _callback{};
    };
}
#include "Core.h"
#endif // !MYENGINE_COMPONETS_H