#pragma once
#include "Basic.h"
#ifndef S3GF_CORE_H
#define S3GF_CORE_H
#define S3GF_FULL_VERSION "v0.1.1-beta"
#include "Components.h"

namespace S3GF {
    class Engine;
    class Window;
    struct Property;
    class Texture;
    class EventSystem;

    class Renderer {
    public:
        explicit Renderer(Window* window = nullptr);
        ~Renderer();
        [[nodiscard]] SDL_Renderer* self() const;
        [[nodiscard]] Window* window() const;
        void _update();
        void fillBackground(const SDL_Color& color, bool covered = false);
        void drawPoint(const Graphics::Point& point);
        void drawPoint(Graphics::Point&& point);
        void drawLine(const Graphics::Line& line);
        void drawLine(Graphics::Line&& line);
        void drawRectangle(const Graphics::Rectangle& rectangle);
        void drawRectangle(Graphics::Rectangle&& rectangle);
        void drawTriangle(const Graphics::Triangle& triangle);
        void drawTriangle(Graphics::Triangle&& triangle);
        void drawEllipse(const Graphics::Ellipse& ellipse);
        void drawEllipse(Graphics::Ellipse&& ellipse);
        void drawTexture(SDL_Texture* texture, Property* property);
        void drawText(TTF_Text* text, const Vector2& position);
        void drawPixelText(const std::string& text, const Vector2& position,
                           const Vector2& scaled = {1.f, 1.f}, const SDL_Color& color = StdColor::White);
        void setViewport(const Geometry& geometry);
        void setClipView(const Geometry& geometry);
        void setBlendMode(const SDL_BlendMode& blend_mode);
    private:
        static void updateBackground(const SDL_Color& color) {
            _background_color = color;
        }
        struct Command {
            explicit Command(SDL_Renderer* renderer) : renderer(renderer) {}
            virtual void exec() = 0;
            SDL_Renderer* renderer;
        };
        struct FillCMD : public Command {
            explicit FillCMD(SDL_Renderer* renderer, SDL_Color color = StdColor::Black, bool covered = false)
                : Command(renderer), bg_color(color), _covered(covered) {}
            SDL_Color bg_color;
            bool _covered;
            void exec() override;
        };
        struct ClipCMD : public Command {
            explicit ClipCMD(SDL_Renderer* renderer, bool reset = true, SDL_Rect rect = {0, 0, 0, 0})
                : Command(renderer), _reset(reset), _clip_area(rect) {}
            SDL_Rect _clip_area;
            bool _reset;
            void exec() override;
        };
        struct ViewportCMD : public Command {
            explicit ViewportCMD(SDL_Renderer* renderer, bool reset = true, SDL_Rect rect = {0, 0, 0, 0})
                : Command(renderer), _reset(reset), _viewport_area(rect) {}
            SDL_Rect _viewport_area;
            bool _reset;
            void exec() override;
        };
        struct BlendModeCMD : public Command {
            explicit BlendModeCMD(SDL_Renderer* renderer, SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE)
                : Command(renderer), _blend_mode(blend_mode) {}
            SDL_BlendMode _blend_mode;
            void exec() override;
        };
        struct PointCMD : public Command {
            explicit PointCMD(SDL_Renderer* renderer, const Graphics::Point& point)
                : Command(renderer), point(point) {}
            Graphics::Point point;
            void exec() override;
        };
        struct LineCMD : public Command {
            explicit LineCMD(SDL_Renderer* renderer, const Graphics::Line& line)
                : Command(renderer), line(line) {}
            Graphics::Line line;
            void exec() override;
        };
        struct RectCMD : public Command {

            explicit RectCMD(SDL_Renderer* renderer, const Graphics::Rectangle& rect)
                : Command(renderer), rectangle(rect) {}
            Graphics::Rectangle rectangle;
            void exec() override;
        };
        struct TriangleCMD : public Command {
            explicit TriangleCMD(SDL_Renderer* renderer, const Graphics::Triangle& tri)
                    : Command(renderer), triangle(tri) {}
            Graphics::Triangle triangle;
            void exec() override;
        };
        struct EllipseCMD : public Command {
            explicit EllipseCMD(SDL_Renderer* renderer, const Graphics::Ellipse& elli)
                    : Command(renderer), ellipse(elli) {}
            Graphics::Ellipse ellipse;
            void exec() override;
        };
        struct TextureCMD : public Command {
            explicit TextureCMD(SDL_Renderer* renderer, SDL_Texture* texture, Property* property)
                : Command(renderer), _texture(texture), _property(property) {}
            SDL_Texture* _texture;
            Property* _property;
            void exec() override;
        };
        struct TextCMD : public Command {
            explicit TextCMD(SDL_Renderer* renderer, TTF_Text* text, const Vector2& position)
                : Command(renderer), text(text), position(position) {}
            TTF_Text* text;
            Vector2 position;
            void exec() override;
        };
        struct PixelTextCMD : public Command {
            explicit PixelTextCMD(SDL_Renderer* renderer, const std::string& text,
                                  const Vector2& pos, const Vector2& scaled, const SDL_Color& color)
                : Command(renderer), text(text), pos(pos), scaled(scaled), color(color) {}
            std::string text;
            Vector2 pos, scaled;
            SDL_Color color;
            void exec() override;
        };
        
        SDL_Renderer* _renderer{nullptr};
        Window* _window{nullptr};
        static SDL_Color _background_color;
        std::vector<std::unique_ptr<Command>> _cmd_list;
    };

    class Window {
        friend class Renderer;
        friend class EventSystem;
    public:
        struct Geometry {
            int x, y, width, height;
        };
        enum GraphicEngine {
            OPENGL,
            VULKAN
        };
        explicit Window(Engine* object, const std::string& title,
                        int width = 800, int height = 600, GraphicEngine engine = OPENGL);
        ~Window();

        bool move(int x, int y);
        bool resize(int width, int height);
        bool setGeometry(int x, int y, int width, int height);
        
        const Geometry geometry() const;
        uint32_t windowID() const;

        bool show();
        bool hide();
        bool visible() const;
        void close();

        bool setResizable(bool enabled);
        bool resizable() const;

        void setRenderer(Renderer* renderer);
        Renderer* renderer() const;

        void setBorderless(bool enabled);
        bool borderless() const;

        void setFullScreen(bool enabled, bool move_to_center = false);
        bool fullScreen() const;

        void setWindowTitle(const std::string& title);
        const std::string& windowTitle() const; 

        SDL_Window* self() const;
        Engine* engine() const;
        void installPaintEvent(const std::function<void(Renderer* renderer)>& paint_event);
    protected:
        virtual void paintEvent();
        virtual void resizeEvent();
        virtual void moveEvent();
        virtual void getFocusEvent();
        virtual void lostFocusEvent();
        virtual void unloadEvent();
    private:
        Geometry _window_geometry;
        std::shared_ptr<Renderer> _renderer; 
        SDL_Window* _window{nullptr};
        SDL_WindowID _winID;
        std::string _title;
        bool _visible{true};
        bool _resizable{false};
        bool _borderless{false};
        bool _fullscreen{false};
        Vector2 _mouse_pos{0, 0};
        std::vector<std::function<void(Renderer*)>> _paint_event_list;
        Engine* _engine;
    };

    class EventSystem {
    public:
        EventSystem(EventSystem &&) = delete;
        EventSystem(const EventSystem &) = delete;
        EventSystem &operator=(EventSystem &&) = delete;
        EventSystem &operator=(const EventSystem &) = delete;
        ~EventSystem();

        static EventSystem* global(Engine* engine);
        static EventSystem* global();
        void appendEvent(uint64_t id, const std::function<void(SDL_Event)>& event);
        void removeEvent(uint64_t id);
        void clearEvent();
        size_t eventCount() const;
        bool run();
        [[nodiscard]] bool isKeyDown() const;
        [[nodiscard]] bool isMouseButtonDown() const;
    private:
        explicit EventSystem(Engine* engine) : _engine(engine) {}
        static std::unique_ptr<EventSystem> _instance;
        Engine* _engine{nullptr};
        bool _is_mouse_down{false}, _is_key_down{false};
        std::map<uint64_t, std::function<void(SDL_Event)>> _event_list;
    };

    class Engine {
    public:
        using constIter = std::unordered_map<SDL_WindowID, std::unique_ptr<Window>>::const_iterator;
        using iter = std::unordered_map<SDL_WindowID, std::unique_ptr<Window>>::iterator;
        explicit Engine(std::string&& app_name = "Hello world", std::string&& app_version = "v1.0.0",
                        std::string&& app_id = "HelloWorld.app");
        ~Engine();
        static void disabledShowAppInfo();

        void setApplicationID(const std::string& app_id);
        void setApplicationID(std::string&& app_id);
        void setApplicationName(const std::string& app_id);
        void setApplicationName(std::string&& app_id);
        void setApplicationVersion(const std::string& app_version);
        void setApplicationVersion(std::string&& app_version);


        [[nodiscard]] const std::string& applicationID() const;
        [[nodiscard]] const std::string& applicationName() const;
        [[nodiscard]] const std::string& applicationVersion() const;

        bool isRunning() const;
        static void exit(int code = 0);
        int exec();

        void newWindow(Window* window);
        void removeWindow(SDL_WindowID id);
        Window* window(SDL_WindowID id = _main_window_id) const;
        std::vector<uint32_t> windowIDList() const;
        constIter begin() const { return _window_list.cbegin(); }
        iter begin() { return _window_list.begin(); }
        constIter end() const { return _window_list.cend(); }
        iter end() { return _window_list.end(); }
        size_t windowCount() { return _window_list.size(); }

        void setFPS(uint32_t fps);
        uint32_t fps() const; 
        static void throwFatalError();

        void installCleanUpEvent(const std::function<void()>& event);

    private:
        void cleanUp();
        void running();
        bool _running;
        static int _return_code;
        static bool _quit_requested;
        uint32_t _fps{0};
        double _frame_in_ns{0};
        uint32_t _real_fps{0};
        static SDL_WindowID _main_window_id;
        static bool _show_app_info;
        std::unordered_map<SDL_WindowID, std::unique_ptr<Window>> _window_list;
        std::function<void()> _clean_up_event;
        std::string _app_name, _app_id, _app_version;
    };

    class Font {
    public:
        /**
         * @enum Style
         * @brief 字体样式
         */
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

    class TextSystem {
    public:
        struct Text {
            TTF_Text* self;
            std::string text;
            std::string font_name;
            SDL_Color font_color{StdColor::Black};
        };
        struct FontEngine {
            TTF_TextEngine* engine;
            TTF_TextEngine* surface_engine;
            std::shared_ptr<Font> font;
        };
        TextSystem(TextSystem &&) = delete;
        TextSystem(const TextSystem &) = delete;
        TextSystem &operator=(TextSystem &&) = delete;
        TextSystem &operator=(const TextSystem &) = delete;
        ~TextSystem();
        
        static TextSystem* global();
        bool isLoaded() const;
        void unload();
        bool addFont(const std::string& font_name, const std::string& font_path, Renderer* renderer);
        bool removeFont(const std::string& font_name);
        Font* font(const std::string& font_name);
        StringList fontNameList() const;
        
        bool addText(uint64_t text_id, const std::string& font_name, const std::string& text);
        bool removeText(uint64_t text_id);
        bool setText(uint64_t text_id, const std::string& text);
        bool appendText(uint64_t text_id, const std::string& text);
        bool setTextFont(uint64_t text_id, const std::string& font_name);
        bool setTextColor(uint64_t text_id, const SDL_Color& color);
        Text* indexOfText(uint64_t text_id);
        std::vector<uint64_t> textIDList() const;
        bool drawText(uint64_t text_id, const Vector2& pos, Renderer* renderer);
        SDL_Surface* toImage(uint64_t text_id);
    private:
        explicit TextSystem();
        static std::unique_ptr<TextSystem> _instance;
        bool _is_loaded{false};
        std::map<uint64_t, Text> _text_map;
        std::unordered_map<std::string, FontEngine> _font_map;
        TTF_TextEngine* _text_engine{nullptr};
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

    class AudioSystem {
    public:
        enum class Status {
            Unknown,
            NoLoaded,
            Loading,
            Invalid,
            Loaded
        };
        
        struct Audio {
            MIX_Audio* self;
            Status status;
            std::string url;
            bool is_stream;
            union {
                MIX_Track* self{nullptr};
                uint64_t duration;
                uint64_t position;
            } track;
        };

        AudioSystem(AudioSystem &&) = delete;
        AudioSystem(const AudioSystem &) = delete;
        AudioSystem &operator=(AudioSystem &&) = delete;
        AudioSystem &operator=(const AudioSystem &) = delete;
        static AudioSystem* global();
        ~AudioSystem();
        bool isValid() const;
        bool load();
        void unload();
        
    private:
        explicit AudioSystem();
        static std::unique_ptr<AudioSystem> _instance;
        bool _is_init{false};
        MIX_Mixer* _mixer;
        std::unordered_map<uint64_t, Audio> _audios_map;
    };
}

#endif