#pragma once
#include <utility>

#include "Basic.h"
#ifndef MYENGINE_CORE_H
#define MYENGINE_CORE_H
#define MYENGINE_FULL_VERSION "v0.1.2-beta"
#include "Components.h"
#include "Renderer/CommandFactory.h"

namespace MyEngine {
    class Engine;
    class Window;
    struct TextureProperty;
    class Texture;
    class EventSystem;

    class Renderer {
    public:
        enum class DiscardStrategy {
            Oldest,
            Newest
        };
        explicit Renderer(Window* window = nullptr);
        ~Renderer();
        [[nodiscard]] SDL_Renderer* self() const;
        [[nodiscard]] Window* window() const;
        void setMaxCommandCount(size_t count);
        void setMaxFrameCommandCount(size_t count);
        void setDiscardCommandStrategy(DiscardStrategy strategy);
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
        void drawTexture(SDL_Texture* texture, TextureProperty* property);
        void drawTextureTile(TextureAtlas* texture_atlas);
        void drawTextureTile(TextureAtlas* texture_atlas, const std::string& tiles_name);
        void drawText(TTF_Text* text, const Vector2& position);
        void drawPixelText(const std::string& text, const Vector2& position,
                           const SDL_Color& color = StdColor::White);

        void setViewport(const Geometry& geometry);
        void setClipView(const Geometry& geometry);
        void setBlendMode(const SDL_BlendMode& blend_mode);
    private:
        static void updateBackground(const SDL_Color& color) {
            _background_color = color;
        }
        /// Command Mode for Renderer
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
            void reset(SDL_Renderer* renderer, SDL_Color color = StdColor::Black, bool covered = false);
        };
        struct ClipCMD : public Command {
            explicit ClipCMD(SDL_Renderer* renderer, bool reset = true, SDL_Rect rect = {})
                : Command(renderer), _reset(reset), _clip_area(rect) {}
            SDL_Rect _clip_area;
            bool _reset;
            void exec() override;
            void reset(SDL_Renderer* renderer, bool reset = true, SDL_Rect rect = {});
        };
        struct ViewportCMD : public Command {
            explicit ViewportCMD(SDL_Renderer* renderer, bool reset = true, SDL_Rect rect = {})
                : Command(renderer), _reset(reset), _viewport_area(rect) {}
            SDL_Rect _viewport_area;
            bool _reset;
            void exec() override;
            void reset(SDL_Renderer* renderer, bool reset = true, SDL_Rect rect = {});
        };
        struct BlendModeCMD : public Command {
            explicit BlendModeCMD(SDL_Renderer* renderer, SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE)
                : Command(renderer), _blend_mode(blend_mode) {}
            SDL_BlendMode _blend_mode;
            void exec() override;
            void reset(SDL_Renderer* renderer, SDL_BlendMode blend_mode = SDL_BLENDMODE_NONE);
        };
        struct PointCMD : public Command {
            explicit PointCMD(SDL_Renderer* renderer, Graphics::Point  point)
                : Command(renderer), point(std::move(point)) {}
            Graphics::Point point;
            void exec() override;
            void reset(SDL_Renderer* renderer, const Graphics::Point& point);
        };
        struct LineCMD : public Command {
            explicit LineCMD(SDL_Renderer* renderer, const Graphics::Line& line)
                : Command(renderer), line(line) {}
            Graphics::Line line;
            void exec() override;
            void reset(SDL_Renderer* renderer, const Graphics::Line& line);
        };
        struct RectCMD : public Command {
            explicit RectCMD(SDL_Renderer* renderer, const Graphics::Rectangle& rect)
                : Command(renderer), rectangle(rect) {}
            Graphics::Rectangle rectangle;
            void exec() override;
            void reset(SDL_Renderer* renderer, const Graphics::Rectangle& rect);
        };
        struct TriangleCMD : public Command {
            explicit TriangleCMD(SDL_Renderer* renderer, const Graphics::Triangle& tri)
                    : Command(renderer), triangle(tri) {}
            Graphics::Triangle triangle;
            void exec() override;
            void reset(SDL_Renderer* renderer, const Graphics::Triangle& tri);
        };
        struct EllipseCMD : public Command {
            explicit EllipseCMD(SDL_Renderer* renderer, Graphics::Ellipse  elli)
                    : Command(renderer), ellipse(std::move(elli)) {}
            Graphics::Ellipse ellipse;
            void exec() override;
            void reset(SDL_Renderer* renderer, const Graphics::Ellipse& elli);
        };
        struct TextureCMD : public Command {
            explicit TextureCMD(SDL_Renderer* renderer, SDL_Texture* texture, TextureProperty* property)
                : Command(renderer), _texture(texture), _property(property) {}
            SDL_Texture* _texture;
            TextureProperty* _property;
            void exec() override;
            void reset(SDL_Renderer* renderer, SDL_Texture* texture, TextureProperty* property);
        };
        struct TextureAtlasCMD : public Command {
            explicit TextureAtlasCMD(SDL_Renderer* renderer, SDL_Texture* texture, TextureProperty* property,
                                     std::vector<SDL_Vertex> vertices, std::vector<int> indices)
                 : Command(renderer), _texture(texture), _property(property), _vertices(std::move(vertices)),
                    _indices(std::move(indices)) {}
            SDL_Texture* _texture;
            TextureProperty* _property;
            std::vector<SDL_Vertex> _vertices;
            std::vector<int> _indices;
            void exec() override;
            void reset(SDL_Renderer* renderer, SDL_Texture* texture, TextureProperty* property,
                       std::vector<SDL_Vertex> vertices, std::vector<int> indices);
        };
        struct TextCMD : public Command {
            explicit TextCMD(SDL_Renderer* renderer, TTF_Text* text, const Vector2& position)
                : Command(renderer), text(text), position(position) {}
            TTF_Text* text;
            Vector2 position;
            void exec() override;
            void reset(SDL_Renderer* renderer, TTF_Text* text, const Vector2& position);
        };
        struct PixelTextCMD : public Command {
            explicit PixelTextCMD(SDL_Renderer* renderer, std::string  text,
                                  const Vector2& pos, const SDL_Color& color)
                : Command(renderer), text(std::move(text)), pos(pos), color(color) {}
            std::string text;
            Vector2 pos;
            SDL_Color color;
            void exec() override;
            void reset(SDL_Renderer* renderer, const std::string& text,
                       const Vector2& pos, const SDL_Color& color);
        };

        std::deque<std::unique_ptr<Command>> _cmd_pool;

        template<typename CMD, typename... Args>
        CMD* getCmdFromPool(Args... args);

        void releaseCmd(std::unique_ptr<Command> command);

        RenderCommand::CommandFactory& factory() {
            return _factory;
        }

        std::deque<std::unique_ptr<Command>> _cmd_list;
        DiscardStrategy _dis_st{DiscardStrategy::Oldest};
        static RenderCommand::CommandFactory _factory;
        SDL_Renderer* _renderer{nullptr};
        Window* _window{nullptr};
        static SDL_Color _background_color;
        size_t _max_cmd_cnt{2048}, _max_frame_cmd_cnt{2048};
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
        
        [[nodiscard]] const Geometry& geometry() const;
        [[nodiscard]] uint32_t windowID() const;

        bool show();
        bool hide();
        [[nodiscard]] bool visible() const;
        void close();

        bool setResizable(bool enabled);
        [[nodiscard]] bool resizable() const;

        void setRenderer(Renderer* renderer);
        [[nodiscard]] Renderer* renderer() const;

        void setBorderless(bool enabled);
        [[nodiscard]] bool borderless() const;

        void setFullScreen(bool enabled, bool move_to_center = false);
        [[nodiscard]] bool fullScreen() const;

        void setWindowTitle(const std::string& title);
        [[nodiscard]] const std::string& windowTitle() const;

        [[nodiscard]] SDL_Window* self() const;
        [[nodiscard]] Engine* engine() const;
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
        enum MouseStatus {
            None,
            Left,
            Middle,
            LeftMiddle,
            Right,
            LeftRight,
            MiddleRight,
            LeftMiddleRight
        };
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
        void appendGlobalEvent(uint64_t g_id, const std::function<void()>& event);
        void removeGlobalEvent(uint64_t g_id);
        void clearGlobalEvent();
        [[nodiscard]] size_t eventCount() const;
        [[nodiscard]] size_t globalEventCount() const;
        [[nodiscard]] const bool* captureKeyboardStatus() const;
        [[nodiscard]] bool captureKeyboard(SDL_Scancode code) const;
        [[nodiscard]] uint32_t captureMouseStatus() const;
        [[nodiscard]] bool captureMouse(MouseStatus mouse_status) const;
        [[nodiscard]] const Vector2& captureMouseAbsDistance() const;
        [[nodiscard]] const Vector2& captureMousePosition() const;
        bool run();
    private:
        explicit EventSystem(Engine* engine) : _engine(engine) {}
        static std::unique_ptr<EventSystem> _instance;
        Engine* _engine{nullptr};
        bool* _kb_events{nullptr};
        uint32_t _mouse_events{0};
        Vector2 _mouse_pos{0, 0}, _mouse_down_dis{0, 0}, _before_mouse_down_pos{0, 0};
        bool _mouse_down_changed{false};
        std::map<uint64_t, std::function<void(SDL_Event)>> _event_list;
        std::map<uint64_t, std::function<void()>> _global_event_list;
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

        void setLimitMaxMemorySize(size_t mem_in_kb);
        [[nodiscard]] size_t limitMaxMemorySize() const;

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
        size_t _used_mem_kb{0}, _max_mem_kb{0}, _warn_mem_kb{0};
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

    class AudioSystem {
    public:
        using Audio = std::variant<std::monostate, BGM, SFX>;

        AudioSystem(AudioSystem &&) = delete;
        AudioSystem(const AudioSystem &) = delete;
        AudioSystem &operator=(AudioSystem &&) = delete;
        AudioSystem &operator=(const AudioSystem &) = delete;
        static AudioSystem* global();
        ~AudioSystem();
        bool isValid() const;
        bool load();
        void unload();
        void addNewMixer(size_t count = 1);
        [[nodiscard]] MIX_Mixer* mixer(size_t index = 0) const;
        [[nodiscard]] size_t mixerCount() const;

        void appendBGM(const std::string& name, const std::string& path);
        void appendSFX(const std::string& name, const std::string& path);
        void remove(const std::string& name);
        BGM* getBGM(const std::string& name);
        SFX* getSFX(const std::string& name);
        [[nodiscard]] size_t size() const;

    private:
        explicit AudioSystem();
        static std::unique_ptr<AudioSystem> _instance;
        bool _is_init{false};
        std::vector<MIX_Mixer*> _mixer_list;
        std::unordered_map<std::string, std::unique_ptr<Audio>> _audio_map;
    };
}

#endif