#pragma once
#ifndef MYENGINE_CORE_H
#define MYENGINE_CORE_H
#ifndef MYENGINE_FULL_VERSION
#define MYENGINE_FULL_VERSION "v0.0.0"
#endif
#ifndef MYENGINE_MAJOR_VERSION
#define MYENGINE_MAJOR_VERSION 0
#endif
#ifndef MYENGINE_MINOR_VERSION
#define MYENGINE_MINOR_VERSION 0
#endif
#ifndef MYENGINE_PATCH_VERSION
#define MYENGINE_PATCH_VERSION 0
#endif
#ifndef APP_NAME
#define APP_NAME "Hello world"
#endif
#ifndef APP_VERSION
#define APP_VERSION "v1.0.0"
#endif
#ifndef APP_ID
#define APP_ID "helloWorld.app"
#endif
#include "Exception.h"
#include "Basic.h"
#include "Components.h"
#include "Utils/Cursor.h"

namespace MyEngine {
    class Engine;
    class Window;
    class TextureProperty;
    class Texture;
    class EventSystem;
    class LayerManager;

    enum class MouseStatus : uint8_t {
        None,
        Left,
        Middle,
        LeftMiddle,
        Right,
        LeftRight,
        MiddleRight,
        LeftMiddleRight
    };
    
    namespace RenderCommand {
        class BaseCommand;
        class CommandFactory;
    }

    class Renderer {
        std::deque<std::unique_ptr<RenderCommand::BaseCommand>> _cmd_list;
        SDL_Renderer* _renderer{nullptr};
        Window* _window{nullptr};
        size_t _render_count{0}, _render_cnt_in_sec{0};
        uint64_t _start_ts{0};
        LayerManager* _layer_manager{nullptr};
        static SDL_Color _background_color;
        friend class LayerManager;
        friend class Engine;

        template<typename T, typename ...Args>
        void addCommand(Args... args);
    public:
        enum VSyncMode : int8_t {
            Disable,
            Enabled,
            HalfRate,
            ThirdRate,
            QuarterRate,
            Adaptive = -1
        };
        explicit Renderer(Window* window = nullptr);
        ~Renderer();
    private:
        void setLayerManager(LayerManager* layer_manager);
        [[nodiscard]] LayerManager* layerManager() const;
    public:
        void setVSyncMode(VSyncMode mode);
        [[nodiscard]] VSyncMode currentVSyncMode() const;
        [[nodiscard]] SDL_Renderer* self() const;
        [[nodiscard]] Window* window() const;
        [[nodiscard]] size_t renderCountInSec() const;
        [[nodiscard]] SDL_Surface* capture() const;
        [[nodiscard]] SDL_Surface* capture(Geometry geometry) const;
    private:
        void _update();
    public:
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
        struct FingerEvent {
            uint64_t touch_id{};
            float pressure{};
            Vector2 finger_down_pos{};
            Vector2 distance_pos{};
            bool is_in_window{true};
        };
    public:
        struct WindowSize {
            int width, height;
        };
        enum GraphicEngine : uint8_t {
            OpenGL,
            Vulkan
        };
        enum class WindowType : uint8_t {
            Window,
            Tooltip,
            Popup,
            Tool,
            Borderless
        };
        explicit Window(Engine* engine, const std::string& title,
                        int width = 800, int height = 600, WindowType type = WindowType::Window,
                        GraphicEngine graphic_engine = OpenGL);
        explicit Window(Window* parent, const std::string& title,
                        int width = 800, int height = 600, WindowType type = WindowType::Window,
                        GraphicEngine graphic_engine = OpenGL);
        virtual ~Window();

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

        bool setWindowAlwaysOnTop(bool enabled);
        bool isAlwaysOnTop() const;

        bool minimizeWindow();
        bool maximizeWindow();
        bool restoreWindow();
        [[nodiscard]] bool isMinimizedWindow() const;
        [[nodiscard]] bool isMaximizedWindow() const;
        [[nodiscard]] bool isRestoredWindow() const;

        void setFreezeEnabled(bool enabled);
        bool isFreeze() const;

        void setWindowTitle(const std::string& title);
        [[nodiscard]] std::string_view windowTitle() const;

        void setWindowIcon(const std::string& icon_path);
        [[nodiscard]] SDL_Surface* windowIcon() const;

        void setCursor(Cursor::StdCursor cursor_style);
        [[nodiscard]] Cursor::StdCursor cursor() const;

        void setDragDropEnabled(bool enabled);
        [[nodiscard]] bool dragDropEnabled() const;
        [[nodiscard]] bool isDragging() const;
        [[nodiscard]] const Vector2& draggingPosition() const;
        std::string_view droppedInfo() const;

        bool setWindowShape(SDL_Surface* surface);
        bool clearWindowShape();

        [[nodiscard]] SDL_Window* self() const;
        [[nodiscard]] Window* parent() const;
        [[nodiscard]] Engine* engine() const;
        void installPaintEvent(const std::function<void(Renderer* renderer)>& paint_event, bool push_back = false);

        [[nodiscard]] std::optional<FingerEvent> getFingerEventByID(SDL_FingerID finger_id) const;
        [[nodiscard]] std::vector<SDL_FingerID> getFingersIDList() const;
        [[nodiscard]] size_t getFingersCount() const;
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
        virtual void windowRestoredEvent();
        virtual void enteredFullscreenEvent();
        virtual void leaveFullscreenEvent();
        virtual void mouseEnteredEvent();
        virtual void mouseLeftEvent();
        virtual void mouseUpEvent();
        virtual void mouseDownEvent(MouseStatus button);
        virtual void mouseClickedEvent(MouseStatus button);
        virtual void mouseMovedEvent(const Vector2& position, const Vector2& distance);
        virtual void keyUpEvent(SDL_Scancode keycode);
        virtual void keyDownEvent(SDL_Scancode keycode);
        virtual void keyPressedEvent(SDL_Scancode keycode);
        virtual void fingerDownEvent(SDL_FingerID id, const Vector2& position);
        virtual void fingerUpEvent(SDL_FingerID id, const Vector2& position);
        virtual void fingerMovedEvent(SDL_FingerID id, const Vector2& position, const Vector2& distance);
        virtual void fingerMoveOutEvent(SDL_FingerID id);
        virtual void fingerMoveInEvent(SDL_FingerID id);
        virtual void fingerTappedEvent(SDL_FingerID id, const Vector2& position);
        virtual void dragInEvent();
        virtual void dragOutEvent();
        virtual void dragMovedEvent(const Vector2 &position, const char *data);
        virtual void dropEvent(const char *url);

    private:
        Geometry _window_geometry;
        std::shared_ptr<Renderer> _renderer; 
        SDL_Window* _window{nullptr};
        SDL_Surface* _win_icon{nullptr};
        SDL_WindowID _winID{};
        bool _dragging{false};
        bool _drag_mode{false};
        bool _render{true};
        Cursor::StdCursor _cursor{};
        std::string _drop_url{};
        Vector2 _mouse_pos{}, _dragging_pos{};
        std::deque<std::function<void(Renderer*)>> _paint_event_list{};
        Engine* _engine;
        std::unordered_map<uint64_t, FingerEvent> _finger_event_list{};
    };

    class EventSystem {
        friend class Engine;
    public:
        EventSystem(EventSystem &&) = delete;
        EventSystem(const EventSystem &) = delete;
        EventSystem &operator=(EventSystem &&) = delete;
        EventSystem &operator=(const EventSystem &) = delete;
        ~EventSystem();

        static EventSystem* global(Engine* engine);
        static EventSystem* global();
        void appendEvent(uint64_t id, const std::function<void(SDL_Event)>& event);
        uint64_t appendEvent(const std::function<void(SDL_Event)>& event);
        void removeEvent(uint64_t id);
        bool isEventExist(uint64_t id);

        void appendGlobalEvent(uint64_t g_id, const std::function<void()>& event);
        uint64_t appendGlobalEvent(const std::function<void()>& event);
        void removeGlobalEvent(uint64_t g_id);
        bool isGlobalEventExist(uint64_t g_id);

        [[nodiscard]] size_t eventCount() const;
        [[nodiscard]] size_t globalEventCount() const;
        [[nodiscard]] const std::vector<SDL_Scancode>& captureKeyboardStatus() const;
        [[nodiscard]] bool captureKeyboard(SDL_Scancode code) const;
        [[nodiscard]] MouseStatus captureMouseStatus() const;
        [[nodiscard]] bool captureMouse(MouseStatus mouse_status) const;
        [[nodiscard]] const Vector2& captureMouseAbsDistance() const;
        [[nodiscard]] const Vector2& captureMousePosition() const;
        bool run();
        static std::string_view mouseStatusName(MouseStatus status);
    private:
        explicit EventSystem(Engine* engine) : _engine(engine) {}
        static std::unique_ptr<EventSystem> _instance;
        Engine* _engine{nullptr};
        bool* _kb_events{nullptr};
        int _nums_keys{0};
        bool _mouse_down_changed{false};
        MouseStatus _mouse_events{0};
        std::vector<SDL_Scancode> _keys_status;
        Vector2 _mouse_pos{0, 0}, _mouse_down_dis{0, 0}, _before_mouse_down_pos{0, 0};
        std::vector<uint64_t> _del_event_deque, _del_g_event_deque;
        std::unordered_map<uint64_t, std::function<void(SDL_Event)>> _event_list{};
        std::unordered_map<uint64_t, std::function<void()>> _global_event_list{};
    };

    class Engine {
        using constIter = std::unordered_map<SDL_WindowID, std::unique_ptr<Window>>::const_iterator;
        using iter = std::unordered_map<SDL_WindowID, std::unique_ptr<Window>>::iterator;
    public:
        enum MessageBoxType {
            Information,
            Warning,
            Fatal
        };
        Engine(const Engine&) = delete;
        Engine(Engine&&) = delete;
        Engine& operator=(const Engine&) = delete;
        Engine& operator=(Engine&&) = delete;
        explicit Engine(const char *app_name = APP_NAME, const char *app_version = APP_VERSION,
                        const char *app_id = APP_ID);
        ~Engine();
        static void disabledShowAppInfo();

        static void setApplicationID(const char *app_id);
        static void setApplicationName(const char *app_name);
        static void setApplicationVersion(const char *app_version);
        static void setApplicationCopyright(const char *app_copyright);
        static void setApplicationAuthor(const char *app_author);
        static void setApplicationTypeName(const char *app_type);
        static void setApplicationURL(const char *app_url);

        static std::string_view applicationID();
        static std::string_view applicationName();
        static std::string_view applicationVersion();
        static std::string_view applicationCopyright();
        static std::string_view applicationAuthor();
        static std::string_view applicationTypeName();
        static std::string_view applicationURL();
        static void openApplicationURL(bool* ok = nullptr);

        void setLimitMaxMemorySize(size_t mem_in_kb);
        [[nodiscard]] size_t limitMaxMemorySize() const;
        void setRenderSetup(uint32_t max_commands, bool auto_incresement = false);

        [[nodiscard]] bool isRunning() const;
        static void exit(int code = 0);
        int exec();

        void newWindow(Window* window, SDL_WindowID parent_window_id = 0, SDL_WindowID child_window_id = 0);
        void removeWindow(SDL_WindowID id);
        [[nodiscard]] std::optional<Window *> window(SDL_WindowID id = _main_window_id) const;
        [[nodiscard]] std::vector<uint32_t> windowIDList() const;
        [[nodiscard]] constIter begin() const { return _window_list.cbegin(); }
        iter begin() { return _window_list.begin(); }
        [[nodiscard]] constIter end() const { return _window_list.cend(); }
        iter end() { return _window_list.end(); }
        size_t windowCount() { return _window_list.size(); }
        [[nodiscard]] bool isWindowExist(uint32_t window_id) const { return _window_list.contains(window_id); }
        std::vector<SDL_WindowID> windowIDList(SDL_WindowID parent_window_id);
        std::vector<SDL_WindowID> parentWindowIDList();
        std::optional<SDL_WindowID> windowParentID(SDL_WindowID id);


        void setFPS(uint32_t fps);
        [[nodiscard]] uint32_t fps() const;
        static void throwFatalError();
        template <typename T>
        static void throwCustomFatalError() {
            bool ok;
            auto err = copeWithFatalError(&ok);
            throw T(err);
        }

        void installCleanUpEvent(const std::function<void()>& event);

        bool messageBox(MessageBoxType type, const std::string& title, const std::string& message,
                uint32_t parent_window_id = _main_window_id);

        void openFileDialog(const StringList &filters,
                            const std::string &default_path = FileSystem::homePath(), Window *parent_window = nullptr);

        void openDirectoryDialog(
            const std::string &default_path = FileSystem::homePath(),
            Window *parent_window = nullptr);

        void saveFileDialog(const StringList &filters,
                            const std::string &default_path = FileSystem::homePath(), Window *parent_window = nullptr);

        std::string getFileDialogURL(bool* is_user_cancelled = nullptr, bool* is_finished = nullptr);

    private:
        void cleanUp();
        void running();
        static std::string copeWithFatalError(bool* ok = nullptr);
        static void SDLCALL getFileDirectoryURL(void *userdata, const char * const *filelist, int filter);
        static void SDLCALL getFilePathURL(void *userdata, const char * const *filelist, int filter);
        static void getFileFilter(const std::string& str, std::string &name, std::string &pattern);
        struct FileDialogResult {
            std::string url{};
            bool user_cancelled{};
            bool is_finished{};
        };
        static FileDialogResult _file_dialog_result;
        static bool _quit_requested;
        static int _return_code;
        static SDL_WindowID _main_window_id;
        static bool _show_app_info;

        double _frame_in_ns{0};
        uint32_t _fps{0};
        uint32_t _real_fps{0};
        bool _running{};
        std::unordered_map<SDL_WindowID, std::unique_ptr<Window>> _window_list;
        std::unordered_map<SDL_WindowID, std::vector<SDL_WindowID>> _parent_window_list;
        std::function<void()> _clean_up_event;
        size_t _used_mem_kb{0}, _max_mem_kb{0}, _warn_mem_kb{0};
    };

    class TextSystem : public Template::Singleton<TextSystem> {
        friend class Template::Singleton<TextSystem>;
        friend class Engine;
    public:
        struct Text {
            TTF_Text* self;
            std::string text;
            std::string font_name;
            Size text_size{};
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
        ~TextSystem() = default;

        bool isLoaded() const;
        bool addFont(const std::string& font_name, const std::string& font_path, Renderer* renderer,
                     float font_size = 9.f);
        bool removeFont(const std::string& font_name);
        Font* font(const std::string& font_name);
        [[nodiscard]] bool isFontContain(const std::string& font_name) const;
        StringList fontNameList() const;

        bool setFontSize(const std::string& font_name, float font_size);
        bool addText(uint64_t text_id, const std::string& font_name, const std::string& text);
        bool removeText(uint64_t text_id);
        bool setText(uint64_t text_id, const std::string& text);
        bool appendText(uint64_t text_id, const std::string& text);
        bool setTextFont(uint64_t text_id, const std::string& font_name);
        bool setTextColor(uint64_t text_id, const SDL_Color& color);
        Text* indexOfText(uint64_t text_id);
        [[nodiscard]] bool isTextContain(uint64_t text_id) const;
        [[nodiscard]] std::vector<uint64_t> textIDList() const;
        bool drawText(uint64_t text_id, const Vector2& pos, Renderer* renderer);
        bool updateFont(const std::string& font_name);
        SDL_Surface* toImage(uint64_t text_id);
    private:
        explicit TextSystem();
        void load();
        void unload();
        bool _is_loaded{false};
        std::map<uint64_t, Text> _text_map;
        std::unordered_map<std::string, FontEngine> _font_map;
    };

    class AudioSystem : public Template::Singleton<AudioSystem> {
        friend class Template::Singleton<AudioSystem>;
        friend class Engine;
    public:
        using Audio = std::variant<std::monostate, std::unique_ptr<BGM>, std::unique_ptr<SFX>>;

        AudioSystem(AudioSystem &&) = delete;
        AudioSystem(const AudioSystem &) = delete;
        AudioSystem &operator=(AudioSystem &&) = delete;
        AudioSystem &operator=(const AudioSystem &) = delete;
        ~AudioSystem() = default;
        bool isValid() const;

        bool addNewMixer(size_t count = 1, SDL_AudioDeviceID device_id = 0);
        [[nodiscard]] MIX_Mixer* mixer(size_t index = 0) const;
        [[nodiscard]] size_t mixerCount() const;

        void appendBGM(const std::string& name, const std::string& path, size_t mixer_index = 0);
        void appendSFX(const std::string& name, const std::string& path, size_t mixer_index = 0);
        void remove(const std::string& name);
        BGM* getBGM(const std::string& name);
        SFX* getSFX(const std::string& name);
        bool isBGM(const std::string& name) const;
        bool isSFX(const std::string& name) const;
        bool isAudio(const std::string& name) const;
        StringList audioNamesList() const;
        [[nodiscard]] size_t size() const;

        void setMixerVolume(float volume, size_t mixer_index = 0);
        [[nodiscard]] float mixerVolume(size_t mixer_index = 0);
        void stopAll();
        void forcedStopAll();

        size_t addAudioRecorder(AudioRecorder* recorder);
        void removeAudioRecorder(size_t index);
        AudioRecorder* audioRecoder(size_t index);
        size_t audioRecorderCount() const;
    private:
        explicit AudioSystem();
        bool load();
        void unload();
        bool _is_init{false};
        std::vector<MIX_Mixer*> _mixer_list{};
        std::unordered_map<std::string, Audio> _audio_map{};
        std::vector<std::unique_ptr<AudioRecorder>> _recoder_list{};
    };
}

#include "Renderer/RCommand.h"
#include "Layers/LayerManager.h"

#endif
