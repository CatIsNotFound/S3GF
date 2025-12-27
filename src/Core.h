#pragma once
#ifndef MYENGINE_CORE_H
#define MYENGINE_CORE_H
#define MYENGINE_FULL_VERSION "v0.1.3-beta"
#include "Exception.h"
#include "Basic.h"
#include "Components.h"
#include "Utils/Cursor.h"

namespace MyEngine {
    class Engine;
    class Window;
    struct TextureProperty;
    class Texture;
    class EventSystem;
    
    namespace RenderCommand {
        class BaseCommand;
        class CommandFactory;
    }

    class Renderer {
    private:
        std::deque<std::unique_ptr<RenderCommand::BaseCommand>> _cmd_list;
        SDL_Renderer* _renderer{nullptr};
        Window* _window{nullptr};
        size_t _render_count{0}, _render_cnt_in_sec{0};
        uint64_t _start_ts{0};
        static SDL_Color _background_color;

        template<typename T, typename ...Args>
        void addCommand(Args... args);
    public:
        enum VSyncMode {
            Disable,
            Enabled,
            HalfRate,
            ThirdRate,
            QuarterRate,
            Adaptive = -1
        };
        explicit Renderer(Window* window = nullptr);
        ~Renderer();
        void setVSyncMode(Renderer::VSyncMode mode);
        [[nodiscard]] VSyncMode currentVSyncMode() const;
        [[nodiscard]] SDL_Renderer* self() const;
        [[nodiscard]] Window* window() const;
        [[nodiscard]] size_t renderCountInSec() const;
        void _update();
        void fillBackground(const SDL_Color& color);
        void fillBackground(SDL_Color&& color);
        void fillBackground(uint64_t rgb_hex = 0);
        void drawPoint(Graphics::Point* point);
        void drawPoints(const std::vector<Graphics::Point*>& point_list);
        void drawLine(Graphics::Line* line);
        void drawLines(const std::vector<Graphics::Line*>& line_list);
        void drawRectangle(Graphics::Rectangle* rectangle);
        void drawRectangles(const std::vector<Graphics::Rectangle*>& rectangle_list);
        void drawTriangle(Graphics::Triangle* triangle);
        void drawTriangles(const std::vector<Graphics::Triangle*>& triangle);
        void drawEllipse(Graphics::Ellipse* ellipse);
        void drawEllipses(const std::vector<Graphics::Ellipse*>& ellipse);
        void drawTexture(SDL_Texture* texture, TextureProperty* property);
        void drawTexture(SDL_Texture* texture, const std::vector<TextureProperty*>& property);
        void drawTextures(const std::vector<SDL_Texture*>& textures, const std::vector<TextureProperty*>& properties);

        void drawText(TTF_Text* text, Vector2& position);
        void drawTexts(TTF_Text* text, const std::vector<Vector2*>& position_list);
        void drawTexts(const std::vector<TTF_Text*>& text_list, const std::vector<Vector2*>& position_list);
        void drawDebugText(const std::string& text, const Vector2& position,
                           const SDL_Color& color = StdColor::Black);
        void drawDebugTexts(const StringList& text_list, const std::vector<Vector2*>& position_list,
                           const SDL_Color& color = StdColor::Black);
        void drawDebugFPS(const Vector2& position = {20, 20}, const SDL_Color& color = StdColor::Black);
        void setViewport(const Geometry& geometry);
        void setClipView(const Geometry& geometry);
        void setBlendMode(const SDL_BlendMode& blend_mode);

        template<typename T, typename ...Args>
        void addCustomCommand(Args... args);
    };

    class Window {
        friend class Renderer;
        friend class EventSystem;
    public:
        struct WindowSize {
            int width, height;
        };
        enum GraphicEngine {
            OpenGL,
            Vulkan
        };
        explicit Window(Engine* object, const std::string& title,
                        int width = 800, int height = 600, GraphicEngine engine = OpenGL);
        ~Window();

        bool move(int x, int y);
        bool resize(int width, int height);
        bool setMinimumSize(int width, int height);
        bool setMaximumSize(int width, int height);
        bool setGeometry(int x, int y, int width, int height);
        
        [[nodiscard]] const Geometry& geometry() const;
        [[nodiscard]] WindowSize minimumSize() const;
        [[nodiscard]] WindowSize maximumSize() const;
        [[nodiscard]] WindowSize windowSize() const;
        [[nodiscard]] uint32_t windowID() const;

        bool show();
        bool hide();
        [[nodiscard]] bool visible() const;
        void close();

        bool setResizable(bool enabled);
        [[nodiscard]] bool resizable() const;

        void setRenderer(Renderer *renderer);
        [[nodiscard]] Renderer* renderer() const;

        void setBorderless(bool enabled);
        [[nodiscard]] bool borderless() const;

        void setWindowOpacity(float opacity);
        [[nodiscard]] float windowOpacity() const;

        void setFullScreen(bool enabled, bool move_to_center = false);
        [[nodiscard]] bool fullScreen() const;

        void setWindowTitle(const std::string& title);
        [[nodiscard]] const std::string& windowTitle() const;

        void setWindowIcon(const std::string& icon_path);
        [[nodiscard]] SDL_Surface* windowIcon() const;

        void setCursor(Cursor::StdCursor cursor_style);
        [[nodiscard]] Cursor::StdCursor cursor() const;

        void setDragDropEnabled(bool enabled);
        [[nodiscard]] bool dragDropEnabled() const;
        [[nodiscard]] bool isDragging() const;
        [[nodiscard]] const Vector2& draggingPosition() const;
        void droppedInfo(char* url, std::string& source) const;

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
        virtual void showEvent();
        virtual void hideEvent();
        virtual void windowMinimizedEvent();
        virtual void windowMaximizedEvent();
        virtual void enteredFullscreenEvent();
        virtual void leaveFullscreenEvent();
        virtual void mouseEnteredEvent();
        virtual void mouseLeftEvent();
        virtual void mouseUpEvent();
        virtual void mouseDownEvent(int);
        virtual void mouseClickedEvent(int);
        virtual void mouseMovedEvent(const Vector2&);
        virtual void keyUpEvent(int);
        virtual void keyDownEvent(int);
        virtual void keyPressedEvent(int);
        virtual void dragInEvent();
        virtual void dragOutEvent();
        virtual void dragMovedEvent(const Vector2 &position, const char *url);
        virtual void dropEvent(const char *url, const char *source);

    private:
        Geometry _window_geometry;

        std::shared_ptr<Renderer> _renderer; 
        SDL_Window* _window{nullptr};
        SDL_Surface* _win_icon{nullptr};
        SDL_WindowID _winID{};
        std::string _title{};
        bool _visible{true};
        bool _resizable{false};
        bool _borderless{false};
        bool _fullscreen{false};
        bool _dragging{false};
        bool _drag_mode{false};
        std::string _drop_source{};
        char _drop_url[255]{};
        Vector2 _mouse_pos{}, _dragging_pos{};
        Cursor::StdCursor _cursor{};
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

        void appendGlobalEvent(uint64_t g_id, const std::function<void()>& event);
        void removeGlobalEvent(uint64_t g_id);

        [[nodiscard]] size_t eventCount() const;
        [[nodiscard]] size_t globalEventCount() const;
        [[nodiscard]] const std::vector<int>& captureKeyboardStatus() const;
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
        int _nums_keys{0};
        std::vector<int> _keys_status;
        uint32_t _mouse_events{0};
        Vector2 _mouse_pos{0, 0}, _mouse_down_dis{0, 0}, _before_mouse_down_pos{0, 0};
        bool _mouse_down_changed{false};
        std::unordered_map<uint64_t, std::function<void(SDL_Event)>> _event_list{};
        std::deque<uint64_t> _del_event_deque, _del_g_event_deque;
        std::unordered_map<uint64_t, std::function<void()>> _global_event_list{};
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
        [[nodiscard]] bool isWindowExist(uint32_t window_id) { return _window_list.contains(window_id); }

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
        using Audio = std::variant<std::monostate, std::unique_ptr<BGM>, std::unique_ptr<SFX>>;

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

        void appendBGM(const std::string& name, const std::string& path, size_t mixer_index = 0);
        void appendSFX(const std::string& name, const std::string& path, size_t mixer_index = 0);
        void remove(const std::string& name);
        BGM* getBGM(const std::string& name);
        SFX* getSFX(const std::string& name);
        [[nodiscard]] size_t size() const;

        void setMixerVolume(float volume, size_t mixer_index = 0);
        [[nodiscard]] float mixerVolume(size_t mixer_index = 0);
        void stopAll();

    private:
        explicit AudioSystem();
        static std::unique_ptr<AudioSystem> _instance;
        bool _is_init{false};
        std::vector<MIX_Mixer*> _mixer_list;
        std::unordered_map<std::string, Audio> _audio_map;
    };
}

#include "RCommand.h"

#endif