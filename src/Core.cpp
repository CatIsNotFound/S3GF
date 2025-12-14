
#include "Core.h"

#include <utility>
#include "Basic.h"
#include "Utils/All.h"

namespace MyEngine {
    std::unique_ptr<EventSystem> EventSystem::_instance{};
    SDL_Color Renderer::_background_color{RGBAColor::White};
    RenderCommand::CommandFactory Renderer::_factory{};
    SDL_WindowID Engine::_main_window_id{0};
    bool Engine::_quit_requested{false};
    int Engine::_return_code{0};
    bool Engine::_show_app_info{true};
    bool FontDatabase::_is_loaded{false};
    FontMap FontDatabase::_font_db{};

    std::unique_ptr<TextSystem> TextSystem::_instance{};
    std::unique_ptr<AudioSystem> AudioSystem::_instance{};

    Renderer::Renderer(Window* window) : _window(window) {
        _renderer = SDL_CreateRenderer(_window->self(), nullptr);
        if (!_renderer) {
            Logger::log("The renderer is not created!", Logger::Fatal);
            Engine::throwFatalError();
        }
    }

    Renderer::~Renderer() {
        if (_renderer) SDL_DestroyRenderer(_renderer);
    }

    SDL_Renderer* Renderer::self() const {
        return _renderer;
    }

    Window* Renderer::window() const {
        return _window;
    }

    void Renderer::setMaxCommandCount(size_t count) {
        _max_cmd_cnt = count;
        /// To avoid memory growth issues caused by excessive rendering frame commands.
        if (_max_frame_cmd_cnt > _max_cmd_cnt) _max_frame_cmd_cnt = _max_cmd_cnt;
        while (_cmd_pool.size() > _max_cmd_cnt) {
            _cmd_pool.pop_back();
        }
    }

    void Renderer::setMaxFrameCommandCount(size_t count) {
        _max_frame_cmd_cnt = count;
        if (_max_frame_cmd_cnt > _max_cmd_cnt) {
            Logger::log("Engine: The maximum number of frame commands currently set has exceeded the limit "
                        "for the maximum number of commands. Once exceeded, "
                        "it will continuously increase the memory usage.", Logger::Warn);
        }
    }

    void Renderer::setDiscardCommandStrategy(MyEngine::Renderer::DiscardStrategy strategy) {
        _dis_st = strategy;
    }

    void Renderer::_update() {
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(_renderer, _background_color.r, _background_color.g,
                                _background_color.b, _background_color.a);
        SDL_RenderClear(_renderer);
        for (auto& cmd : _cmd_list) {
            cmd->exec();
            releaseCmd(std::move(cmd));
        }
        SDL_RenderPresent(_renderer);
        _cmd_list.clear();
        _window->paintEvent();
    }

    void Renderer::fillBackground(const SDL_Color& color, bool covered) {
        auto ptr = getCmdFromPool<FillCMD>(_renderer, color, covered);
        if (ptr) _cmd_list.push_back(std::unique_ptr<FillCMD>(ptr));
    }

    void Renderer::drawPoint(const Graphics::Point &point) {
        auto ptr = getCmdFromPool<PointCMD>(_renderer, point);
        if (ptr) _cmd_list.push_back(std::unique_ptr<PointCMD>(ptr));
    }

    void Renderer::drawPoint(Graphics::Point &&point) {
        auto ptr = getCmdFromPool<PointCMD>(_renderer, point);
        if (ptr) _cmd_list.push_back(std::unique_ptr<PointCMD>(ptr));
    }

    void Renderer::drawLine(const Graphics::Line &line) {
        auto ptr = getCmdFromPool<LineCMD>(_renderer, line);
        if (ptr) _cmd_list.push_back(std::unique_ptr<LineCMD>(ptr));
    }

    void Renderer::drawLine(Graphics::Line &&line) {
        auto ptr = getCmdFromPool<LineCMD>(_renderer, line);
        if (ptr) _cmd_list.push_back(std::unique_ptr<LineCMD>(ptr));
    }

    void Renderer::drawRectangle(const Graphics::Rectangle &rectangle) {
        auto ptr = getCmdFromPool<RectCMD>(_renderer, rectangle);
        if (ptr) _cmd_list.push_back(std::unique_ptr<RectCMD>(ptr));
    }

    void Renderer::drawRectangle(Graphics::Rectangle &&rectangle) {
        auto ptr = getCmdFromPool<RectCMD>(_renderer, rectangle);
        if (ptr) _cmd_list.push_back(std::unique_ptr<RectCMD>(ptr));
    }

    void Renderer::drawTriangle(const Graphics::Triangle &triangle) {
        auto ptr = getCmdFromPool<TriangleCMD>(_renderer, triangle);
        if (ptr) _cmd_list.push_back(std::unique_ptr<TriangleCMD>(ptr));
    }

    void Renderer::drawTriangle(Graphics::Triangle &&triangle) {
        auto ptr = getCmdFromPool<TriangleCMD>(_renderer, triangle);
        if (ptr) _cmd_list.push_back(std::unique_ptr<TriangleCMD>(ptr));
    }

    void Renderer::drawEllipse(const Graphics::Ellipse &ellipse) {
        auto ptr = getCmdFromPool<EllipseCMD>(_renderer, ellipse);
        if (ptr) _cmd_list.push_back(std::unique_ptr<EllipseCMD>(ptr));
    }

    void Renderer::drawEllipse(Graphics::Ellipse &&ellipse) {
        auto ptr = getCmdFromPool<EllipseCMD>(_renderer, ellipse);
        if (ptr) _cmd_list.push_back(std::unique_ptr<EllipseCMD>(ptr));
    }

    void Renderer::drawTexture(SDL_Texture* texture, TextureProperty* property) {
        if (!texture || !property) return;
        auto ptr = getCmdFromPool<TextureCMD>(_renderer, texture, property);
        if (ptr) _cmd_list.push_back(std::unique_ptr<TextureCMD>(ptr));
    }

    void Renderer::drawTextureTile(TextureAtlas* texture_atlas) {
        auto vertices = texture_atlas->vertices(texture_atlas->currentTiles());
        auto indices = texture_atlas->indices(texture_atlas->currentTiles());
        auto ptr = getCmdFromPool<TextureAtlasCMD>(_renderer, texture_atlas->self(),
                                                   texture_atlas->property(), vertices, indices);
        if (ptr) _cmd_list.push_back(std::unique_ptr<TextureAtlasCMD>(ptr));
    }

    void Renderer::drawTextureTile(TextureAtlas* texture_atlas, const std::string& tiles_name) {
        if (!texture_atlas->isTilesNameExist(tiles_name)) {
            Logger::log(std::format("Renderer: The specified tiles '{}' of the textureAtlas is not exist!",
                                    tiles_name), Logger::Error);
            return;
        }
        auto vertices = texture_atlas->vertices(tiles_name);
        auto indices = texture_atlas->indices(tiles_name);
        auto ptr = getCmdFromPool<TextureAtlasCMD>(_renderer, texture_atlas->self(),
                                                   texture_atlas->property(), vertices, indices);
        if (ptr) _cmd_list.push_back(std::unique_ptr<TextureAtlasCMD>(ptr));
    }

    void Renderer::drawText(TTF_Text* text, const Vector2& position) {
        if (!text) return;
        auto ptr = getCmdFromPool<TextCMD>(_renderer, text, position);
        if (ptr) _cmd_list.push_back(std::unique_ptr<TextCMD>(ptr));
    }

    void Renderer::drawPixelText(const std::string &text, const MyEngine::Vector2 &position,
                                 const SDL_Color& color) {
        if (text.empty()) return;
        auto ptr = getCmdFromPool<PixelTextCMD>(_renderer, text, position, color);
        if (ptr) _cmd_list.push_back(std::unique_ptr<PixelTextCMD>(ptr));
    }

    void Renderer::setViewport(const Geometry& geometry) {
        if (geometry.width == 0 || geometry.height == 0) {
            auto ptr = getCmdFromPool<ViewportCMD>(_renderer, true);
            if (ptr) _cmd_list.push_back(std::unique_ptr<ViewportCMD>(ptr));
        } else {
            auto ptr = getCmdFromPool<ViewportCMD>(_renderer, false,
            SDL_Rect(geometry.x, geometry.y, geometry.width,geometry.height));
            if (ptr) _cmd_list.push_back(std::unique_ptr<ViewportCMD>(ptr));
        }
    }

    void Renderer::setClipView(const Geometry& geometry) {
        if (geometry.width == 0 || geometry.height == 0) {
            auto ptr = getCmdFromPool<ClipCMD>(_renderer, true);
            if (ptr) _cmd_list.push_back(std::unique_ptr<ClipCMD>(ptr));
        } else {
            auto ptr = getCmdFromPool<ClipCMD>(_renderer, false,
                       SDL_Rect(geometry.x, geometry.y, geometry.width,geometry.height));
            if (ptr) _cmd_list.push_back(std::unique_ptr<ClipCMD>(ptr));
        }
    }

    void Renderer::setBlendMode(const SDL_BlendMode &blend_mode) {
        auto ptr = getCmdFromPool<BlendModeCMD>(_renderer, blend_mode);
        if (ptr) _cmd_list.push_back(std::unique_ptr<BlendModeCMD>(ptr));
    }

    Window::Window(Engine* object, const std::string& title, int width, int height,  GraphicEngine engine)
        : _title(title), _window_geometry(0, 0, width, height), _visible(true), _resizable(false), _engine(object) {
        if (engine == VULKAN)
            _window = SDL_CreateWindow(_title.c_str(), width, height, SDL_WINDOW_VULKAN);
        else
            _window = SDL_CreateWindow(_title.c_str(), width, height, SDL_WINDOW_OPENGL);
        if (!_window) {
            Engine::throwFatalError();
        }
        _renderer = std::make_shared<Renderer>(this);
        _winID = SDL_GetWindowID(_window);
        SDL_GetWindowPosition(_window, &_window_geometry.x, &_window_geometry.y);
        Logger::log(std::format("Window: created with ID {}", _winID), Logger::Debug);
        if (object) {
            object->newWindow(this);
        } else {
            Logger::log("Window: Can't find engine object!", Logger::Fatal);
            Engine::throwFatalError();
        }
    }

    Window::~Window() {
        if (_renderer) {
            _renderer.reset();
        }
        if (_window) {
            SDL_DestroyWindow(_window);
            Logger::log(std::format("Window: ID {} destroyed",  _winID), Logger::Debug);
        }
    }

    bool Window::move(int x, int y) {
        bool _ret = SDL_SetWindowPosition(_window, x, y);
        if (!_ret) {
            Logger::log(std::format("Window: Can't move window! Exception: {}", SDL_GetError()), Logger::Error);
            return false;
        }
        _window_geometry.x = x;
        _window_geometry.y = y;
        return true;
    }

    bool Window::resize(int width, int height) {
        bool _ret = SDL_SetWindowSize(_window, width, height);
        if (!_ret) {
            Logger::log(std::format("Window: Can't resize window! Exception: {}", SDL_GetError()), Logger::Error);
            return false;
        }
        _window_geometry.width = width;
        _window_geometry.height = height;
        return true;
    }

    bool Window::setGeometry(int x, int y, int width, int height) {
        return (!move(x, y) || !resize(width, height)); 
    }

    const Window::Geometry& Window::geometry() const {
        return _window_geometry;
    }

    uint32_t Window::windowID() const {
        return _winID;
    }

    bool Window::show() {
        auto _ret = SDL_ShowWindow(_window);
        if (!_ret) {
            Logger::log(std::format("Window: Can't show window! Exception: {}", SDL_GetError()), Logger::Error);
            return false;
        }
        _visible = true;
        return true;
    }

    bool Window::hide() {
        bool _ret = SDL_HideWindow(_window);
        if (!_ret) {
            Logger::log(std::format("Window: Can't hide window! Exception: {}", SDL_GetError()), Logger::Error);
            return false;
        }
        _visible = false;
        return true;
    }

    bool Window::visible() const {
        return _visible;
    }

    void Window::close() {
        unloadEvent();
    }

    bool Window::setResizable(bool enabled) {
        auto _ret = SDL_SetWindowResizable(_window, enabled);
        if (!_ret) {
            Logger::log(std::format("Window: Can't set window resizable mode! Exception: {}", SDL_GetError()), Logger::Error);
            return false;
        }
        _resizable = enabled;
        return true;
    }

    bool Window::resizable() const {
        return _resizable;
    }

    void Window::setRenderer(Renderer* renderer) {
        if (!renderer) {
            Logger::log("Window: The specified renderer is not valid!", Logger::Error);
            return;
        }
        _renderer = std::shared_ptr<Renderer>(renderer);
    }

    Renderer* Window::renderer() const {
        return _renderer.get();
    }

    void Window::setBorderless(bool enabled) {
        bool _ok = SDL_SetWindowBordered(_window, !enabled);
        if (_ok) _borderless = enabled;
        else Logger::log(std::format("Window (ID {}): Can't set borderless for this window!", _winID),
                         Logger::Error);
    }

    bool Window::borderless() const {
        return _borderless;
    }

    void Window::setFullScreen(bool enabled, bool move_to_center) {
        if (!move_to_center) {
            _mouse_pos = Cursor::global()->globalPosition();
        }
        bool _ok = SDL_SetWindowFullscreen(_window, enabled);
        if (!_ok) {
            Logger::log(std::format("Window (ID {}): Can't set fullscreen for this window!", _winID),
                        Logger::Error);
        } else _fullscreen = enabled;
        if (move_to_center) {
            Cursor::global()->moveToCenter(_fullscreen ? this : nullptr);
        } else {
            Cursor::global()->move(_mouse_pos, nullptr);
        }
    }

    bool Window::fullScreen() const {
        return _fullscreen;
    }

    void Window::setWindowTitle(const std::string& title) {
        _title = title;
        SDL_SetWindowTitle(_window, title.c_str());
    }

    const std::string& Window::windowTitle() const {
        return _title;
    } 

    SDL_Window* Window::self() const {
        if (!_window) {
            Logger::log("The window is not created!", Logger::Error);
        }
        return _window;
    }

    Engine* Window::engine() const {
        return _engine;
    }

    void Window::installPaintEvent(const std::function<void(Renderer* renderer)>& paint_event) {
        _paint_event_list.push_back(paint_event);
    }

    void Window::paintEvent() {
        std::for_each(_paint_event_list.begin(), _paint_event_list.end(), [this] (auto& ev) {
            if (ev) ev(_renderer.get());
        });
    }

    void Window::resizeEvent() {
        auto _ret = SDL_GetWindowSize(_window, &_window_geometry.width, &_window_geometry.height);
        if (!_ret) {
            Logger::log(std::format("Window: Failed to get window size for ID {}!", _winID), Logger::Warn);
        }
    }

    void Window::moveEvent() {
        auto _ret = SDL_GetWindowPosition(_window, &_window_geometry.x, &_window_geometry.y);
        if (!_ret) {
            Logger::log(std::format("Window: Failed to get window position for ID {}!", _winID), Logger::Warn);
        }
    }

    void Window::getFocusEvent() {}

    void Window::lostFocusEvent() {}

    void Window::unloadEvent() {
        if (_engine) {
            Logger::log(std::format("Window: Unload window id {}", _winID));
            _engine->removeWindow(_winID);
        } else {
            Logger::log(std::format("Window: Unload window id {} failed!", _winID));
        }
    }

    EventSystem::~EventSystem() = default;

    EventSystem* EventSystem::global(Engine* engine) {
        if (!_instance) {
            _instance = std::unique_ptr<EventSystem>(new EventSystem(engine));
        } 
        return _instance.get();
    }

    EventSystem* EventSystem::global() {
        return (_instance ? _instance.get() : nullptr);
    }

    void EventSystem::appendEvent(uint64_t id, const std::function<void(SDL_Event)>& event) {
        if (_event_list.contains(id)) {
            Logger::log(std::format("EventSystem: The event with ID {} is already exists! "
                                    "It will overwrite it!", id), Logger::Warn);
            _event_list[id] = event;
            return;
        }
        _event_list.emplace(id, event);
        Logger::log(std::format("EventSystem: Append a new event with ID {}", id));
    }

    void EventSystem::removeEvent(uint64_t id) {
        if (_event_list.contains(id)) {
            _event_list.erase(id);
            Logger::log(std::format("EventSystem: Removed the event with ID {}", id));
        } else {
            Logger::log(std::format("EventSystem: The event with ID {} is not found!", id), Logger::Warn);
        }
    }

    void EventSystem::clearEvent() {
        _event_list.clear();
        Logger::log("EventSystem: Cleared all events.");
    }

    void EventSystem::appendGlobalEvent(uint64_t g_id, const std::function<void()>& event) {
        if (_global_event_list.contains(g_id)) {
            Logger::log(std::format("EventSystem: The global event with ID {} is already exists! "
                                    "It will overwrite it!", g_id), Logger::Warn);
            _global_event_list[g_id] = event;
        } else {
            _global_event_list.emplace(g_id, event);
            Logger::log(std::format("EventSystem: Append a global event by ID {}", g_id));
        }
    }

    void EventSystem::removeGlobalEvent(uint64_t g_id) {
        if (_global_event_list.contains(g_id)) {
            _global_event_list.erase(g_id);
            Logger::log(std::format("EventSystem: Removed a global event with ID {}", g_id));
        } else {
            Logger::log(std::format("EventSystem: The global event with ID {} is not found!", g_id),
                        Logger::Warn);
        }
    }

    void EventSystem::clearGlobalEvent() {
        _global_event_list.clear();
        Logger::log("EventSystem: Cleared all global events!");
    }


    size_t EventSystem::eventCount() const { return _event_list.size(); }

    bool EventSystem::run() {
        SDL_Event ev;
        if (SDL_PollEvent(&ev)) {
            _kb_events = const_cast<bool*>(SDL_GetKeyboardState(nullptr));
            _mouse_events = SDL_GetMouseState(&_mouse_pos.x, &_mouse_pos.y);
            if (!_mouse_down_changed) {
                /// When any of mouse buttons is pressed down, triggered...
                if (_mouse_events > 0) {
                    _mouse_down_changed = true;
                    _before_mouse_down_pos.reset(_mouse_pos);
                }
            } else {
                if (_mouse_events > 0) {
                    _mouse_down_dis.reset(_mouse_pos - _before_mouse_down_pos);
                } else {
                    _mouse_down_changed = false;
                    _mouse_down_dis.reset(0, 0);
                }
            }
            if (!_engine->windowCount()) return false;
            auto win_id_list = _engine->windowIDList();
            std::for_each(win_id_list.begin(), win_id_list.end(), [this, &ev](uint32_t id) {
                auto win = _engine->window(id);
                if (!win || ev.window.windowID != id) return;
                if (ev.type == SDL_EVENT_WINDOW_MOVED) {
                    win->moveEvent();
                }
                if (ev.type == SDL_EVENT_WINDOW_RESIZED) {
                    win->resizeEvent();
                }
                if (ev.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
                    win->getFocusEvent();
                }
                if (ev.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
                    win->lostFocusEvent();
                }
                if (ev.window.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                    win->unloadEvent();
                }
            });

            for (auto& event : _event_list) {
                if (event.second) event.second(ev);
            }
        }
        for (auto& e : _global_event_list) {
            if (e.second) e.second();
        }
        return true;
    }

    size_t EventSystem::globalEventCount() const {
        return _global_event_list.size();
    }

    uint32_t EventSystem::captureMouseStatus() const {
        return _mouse_events;
    }

    bool EventSystem::captureMouse(EventSystem::MouseStatus mouse_status) const {
        return _mouse_events == mouse_status;
    }

    const Vector2& EventSystem::captureMouseAbsDistance() const {
        return _mouse_down_dis;
    }

    const Vector2& EventSystem::captureMousePosition() const {
        return _mouse_pos;
    }

    const bool *EventSystem::captureKeyboardStatus() const {
        return _kb_events;
    }

    bool EventSystem::captureKeyboard(SDL_Scancode code) const {
        return _kb_events[code];
    }

    Engine::Engine(std::string&& app_name, std::string&& app_version, std::string&& app_id)
        : _app_id(app_id), _app_name(app_name), _app_version(app_version), _running(true) {
        if (_show_app_info) {
            std::cout << std::format("MyEngine {} (Based on SDL {}.{}.{})\n",
                                     MYENGINE_FULL_VERSION, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION)
                      << "For more information, visit: https://github.com/CatIsNotFound/MyEngine \n"
                         "                             https://gitee.com/CatIsNotFound/MyEngine\n" << std::endl;
            std::cout << std::format("=== Application Info ===\nID: {} \nName: {} \nVersion: {} \n",
                                     app_id, app_name, app_version) << std::endl;
        }
        if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throwFatalError();
        }
        SDL_SetAppMetadata(app_name.c_str(), app_version.c_str(), app_id.c_str());
        Logger::log("Engine: Started up application!");
        TextSystem::global();
        AudioSystem::global();
        EventSystem::global(this);
        // AudioSystem::global();
        signal(SIGINT, Engine::exit);
    }

    Engine::~Engine() {
        cleanUp();
    }

    void Engine::disabledShowAppInfo() {
        _show_app_info = false;
    }

    void Engine::setApplicationID(std::string &&app_id) {
        _app_id = std::move(app_id);
        SDL_SetAppMetadata(_app_name.c_str(), _app_version.c_str(), _app_id.c_str());
    }
    void Engine::setApplicationID(const std::string &app_id) {
        _app_id = app_id;
        SDL_SetAppMetadata(_app_name.c_str(), _app_version.c_str(), _app_id.c_str());
    }
    void Engine::setApplicationName(std::string &&app_name) {
        _app_name = std::move(app_name);
        SDL_SetAppMetadata(_app_name.c_str(), _app_version.c_str(), _app_id.c_str());
    }
    void Engine::setApplicationName(const std::string &app_name) {
        _app_name = app_name;
        SDL_SetAppMetadata(_app_name.c_str(), _app_version.c_str(), _app_id.c_str());
    }
    void Engine::setApplicationVersion(std::string &&app_version) {
        _app_version = std::move(app_version);
        SDL_SetAppMetadata(_app_name.c_str(), _app_version.c_str(), _app_id.c_str());
    }
    void Engine::setApplicationVersion(const std::string &app_version) {
        _app_version = app_version;
        SDL_SetAppMetadata(_app_name.c_str(), _app_version.c_str(), _app_id.c_str());
    }

    const std::string& Engine::applicationID() const { return _app_id; }
    const std::string& Engine::applicationName() const { return _app_name; }
    const std::string& Engine::applicationVersion() const { return _app_version; }

    void Engine::setLimitMaxMemorySize(size_t mem_in_kb) {
        _max_mem_kb = mem_in_kb;
        _warn_mem_kb = static_cast<size_t>(static_cast<float>(_max_mem_kb) * 0.85f);
    }
    size_t Engine::limitMaxMemorySize() const { return _max_mem_kb; }

    bool Engine::isRunning() const {
        return _running;
    }

    void Engine::exit(int code) {
        _return_code = code;
        _quit_requested = true;
    }

    int Engine::exec() {
        running();
        return _return_code;
    }

    void Engine::newWindow(Window* window) {
        if (!window) return;
        if (_main_window_id == 0) _main_window_id = window->windowID();
        if (!_window_list.contains(window->windowID()))
            _window_list.emplace(window->windowID(), window);
    }

    void Engine::removeWindow(SDL_WindowID id) {
        if (_window_list.contains(id)) {
            _window_list.erase(id);
        }
    }

    Window* Engine::window(SDL_WindowID id) const {
        if (_window_list.contains(id))
            return _window_list.at(id).get();
        else
            return nullptr;
    }

    std::vector<uint32_t> Engine::windowIDList() const {
        std::vector<uint32_t> id_list(_window_list.size());
        std::for_each(_window_list.begin(), _window_list.end(), [&id_list](const auto& window) {
            id_list.push_back(window.second->windowID());
        });
        return id_list;
    }

    void Engine::setFPS(uint32_t fps) {
        _fps = fps;
        if (!_fps) {
            _frame_in_ns = 0; 
            return;
        }
        _frame_in_ns = 1.0e9 / _fps;
    }

    uint32_t Engine::fps() const {
        return _real_fps;
    }

    void Engine::throwFatalError() {
        std::string get_err_info = Logger::lastError();
        if (get_err_info.empty()) {
            Logger::log("No error found. It will not throw the fatal error!", Logger::Info);
            return;
        }
        std::string err = std::format("An error has caused the entire program to crash.\nException: {}", get_err_info);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FATAL ERROR", err.c_str(), nullptr);
        Logger::log(err, Logger::Fatal);
        throw std::runtime_error(err);
    }

    void Engine::installCleanUpEvent(const std::function<void()> &event) {
        _clean_up_event = event;
    }

    void Engine::cleanUp() {
        if (_clean_up_event) {
            _clean_up_event();
        }
        for (auto& win : _window_list) {
            win.second.reset();
        }
        TextSystem::global()->unload();
        AudioSystem::global()->unload();
        SDL_Quit();
        Logger::log("Engine: Clean up finished!");
    }

    void Engine::running() {
        auto start_time = SDL_GetTicks();
        auto frames = 0ULL;
        auto start_ns = SDL_GetTicksNS();
        while (_running && !_quit_requested) {
            /// Event processing and rendering processing
            _running = EventSystem::global(this)->run();
            if (!_running) break;
            auto current_time = SDL_GetTicks();
            auto current_ns = SDL_GetTicksNS();
            if ((double)(current_ns - start_ns) >= _frame_in_ns) {
                for (auto& win : _window_list) {
                    win.second->renderer()->_update();
                }
                start_ns = SDL_GetTicksNS();
                frames += 1;
            }
            if (current_time - start_time >= 1000) {
                /// Real time monitoring of memory usage, if set max memory size.
                if (_max_mem_kb) {
                    bool ok;
                    _used_mem_kb = SysMemory::getCurProcUsedMemSize(&ok);
                    if (ok) {
                        if (_used_mem_kb >= _max_mem_kb) {
                            Logger::log("Engine: The memory size currently used has exceeded "
                                        "the maximum memory size set by this application. "
                                        "The application will be closed!", Logger::Fatal);
                            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Memory overflow",
                                                     "The memory size currently used has exceeded "
                                                     "the maximum memory size set by this application. \n"
                                                     "The application will be closed!", nullptr);
                            exit(1);
                        } else if (_used_mem_kb >= _warn_mem_kb) {
                            Logger::log("Engine: The current memory usage is less than 15%! "
                                        "After exceeding the set value, the application will be closed!",
                                        Logger::Warn);
                        }
                    } else {
                        Logger::log("Engine: Can't get current process memory size!", Logger::Warn);
                    }
                } else {
                    bool ok;
                    auto status = SysMemory::getSystemMemoryStatus(&ok);
                    auto av_per = static_cast<float>(status.available_mem) / static_cast<float>(status.total_mem);
                    if (av_per <= 0.05f) {
                        Logger::log("Engine: The current system memory is less than 5%. "
                                    "The engine has crashed.", Logger::Fatal);
                        throw std::runtime_error("The current available system memory is less than 5%. The engine has crashed.");
                    } else if (av_per <= 0.15f) {
                        Logger::log("Engine: The available memory space of the system is less than 15%. "
                                    "If it falls below 5%, the engine will be crashed!", Logger::Warn);
                    }
                }
                /// Update the render frame
                _real_fps = frames;
                if (_fps > 14 && _real_fps <= 14) {
                    Logger::log(std::format("Low FPS detected: {} FPS", _real_fps), Logger::Warn);
                }
                frames = 0;
                start_time = SDL_GetTicks();
            }
        }
    }

    TextSystem::TextSystem() {
        if (!TTF_Init()) {
            Logger::log(std::format("TextSystem: Can't load Text System! Exception: {}", SDL_GetError()), Logger::Fatal);
            return;
        }
        Logger::log("TextSystem: Loaded text system");
        _is_loaded = true;
    }

    TextSystem::~TextSystem() = default;
        
    TextSystem* TextSystem::global() {
        if (!_instance) {
            _instance = std::unique_ptr<TextSystem>(new TextSystem());
        }
        return _instance.get();
    }

    bool TextSystem::isLoaded() const {
        return _is_loaded;
    }

    void TextSystem::unload() {
        std::for_each(_text_map.begin(), _text_map.end(), [](auto& text) {
            TTF_DestroyText(text.second.self);
        });
        std::for_each(_font_map.begin(), _font_map.end(), [](auto& font) {
            TTF_DestroyRendererTextEngine(font.second.engine);
            TTF_DestroySurfaceTextEngine(font.second.surface_engine);
            font.second.font.reset();
        });
        TTF_Quit();
        Logger::log("TextSystem: Unloaded text system");
    }

    bool TextSystem::addFont(const std::string& font_name, const std::string& font_path, Renderer* renderer) {
        if (_font_map.contains(font_name)) {
            Logger::log(std::format("Font '{}' is already added!", font_name), Logger::Error);
            return false;
        }
        if (!renderer) {
            Logger::log("Can't add font! The specified renderer is not valid!", Logger::Error);
            return false;
        }
        _font_map.emplace(font_name, 
                FontEngine{TTF_CreateRendererTextEngine(renderer->self()), TTF_CreateSurfaceTextEngine(),
                std::make_unique<Font>(font_path, 12.0f)});
        auto& new_font = _font_map.at(font_name);
        if (!new_font.font->self()) {
            TTF_DestroyRendererTextEngine(new_font.engine);
            TTF_DestroySurfaceTextEngine(new_font.surface_engine);
            _font_map.erase(font_name);
            Logger::log(std::format("Can't load font '{}'! Exception: {}", font_name, SDL_GetError()), Logger::Error);
            return false;
        }
        return true;
    }

    bool TextSystem::removeFont(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("Font '{}' is not in the font list!", font_name), Logger::Error);
            return false;
        }
        TTF_DestroyRendererTextEngine(_font_map[font_name].engine);
        TTF_DestroySurfaceTextEngine(_font_map[font_name].surface_engine);
        _font_map.erase(font_name);
        return true;
    }

    Font* TextSystem::font(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("Font '{}' is not in the font list!", font_name), Logger::Error);
            return nullptr;
        }
        return _font_map.at(font_name).font.get();
    }

    StringList TextSystem::fontNameList() const {
        StringList font_list(_font_map.size());
        std::for_each(_font_map.begin(), _font_map.end(), [&font_list](const auto& font) {
            font_list.push_back(font.first);
        });
        return font_list;
    }

    bool TextSystem::addText(uint64_t text_id, const std::string& font_name, const std::string& text) {
        if (_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is already added to text list!", text_id), Logger::Error);
            return false;
        }
        
        _text_map.emplace(text_id, Text(nullptr, text, font_name));
        _text_map[text_id].self = TTF_CreateText(_font_map[font_name].engine, _font_map[font_name].font->self(), text.c_str(), text.size());
        if (!_text_map[text_id].self) {
            Logger::log(std::format("Can't create text!\nException: {}", SDL_GetError()), Logger::Error);
            _text_map.erase(text_id);
            return false;
        }
        return true;
    }

    bool TextSystem::removeText(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return false;
        }
        _text_map.erase(text_id);
        return true;
    }

    bool TextSystem::setText(uint64_t text_id, const std::string& text) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(std::format("Can't set text to text ID {}!\nException: {}", text_id, SDL_GetError()), Logger::Error);
            return false;
        }
        m_text.text = text;
        return true;
    }

    bool TextSystem::appendText(uint64_t text_id, const std::string& text) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_AppendTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(std::format("Can't set text to text ID {}!\nException: {}", text_id, SDL_GetError()), Logger::Error);
            return false;
        }
        m_text.text += text;
        return true;
    }

    bool TextSystem::setTextFont(uint64_t text_id, const std::string& font_name) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return false;
        }
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("Font '{}' is not in the font list!", font_name), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextFont(m_text.self, _font_map[font_name].font->self());
        if (!_ret) {
            Logger::log(std::format("Can't set font '{}' to text ID {}!\nException: {}", 
                    font_name, text_id, SDL_GetError()), Logger::Error);
            return false;
        }
        m_text.font_name = font_name;
        return true;
    }

    bool TextSystem::setTextColor(uint64_t text_id, const SDL_Color& color) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextColor(m_text.self, color.r, color.g, color.b, color.a);
        if (!_ret) {
            Logger::log(std::format("Can't set font color to text ID {}!\nException: {}", 
                    text_id, SDL_GetError()), Logger::Error);
            return false;
        }
        m_text.font_color = color;
        return true;
    }

    TextSystem::Text* TextSystem::indexOfText(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return nullptr;
        }
        return &_text_map.at(text_id);
    }

    std::vector<uint64_t> TextSystem::textIDList() const {
        std::vector<uint64_t> id_list(_text_map.size());
        std::for_each(_text_map.begin(), _text_map.end(), [&id_list](const auto& text) {
            id_list.push_back(text.first);
        });
        return id_list;
    }

    bool TextSystem::drawText(uint64_t text_id, const Vector2& pos, Renderer* renderer) {
        if (!renderer) {
            Logger::log("The specified renderer is not valid!", Logger::Error);
            return false;
        }
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return false;
        }

        renderer->drawText(_text_map[text_id].self, pos);
        return true;
    }

    SDL_Surface* TextSystem::toImage(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::Error);
            return nullptr;
        }
        if (!_font_map.contains(_text_map[text_id].font_name)) {
            Logger::log(std::format("Text ID {} has not set the font! Try to use `setTextFont()` at first!", text_id), Logger::Error);
            return nullptr;
        }
        auto& font_engine = _font_map[_text_map[text_id].font_name];
        auto& text =  _text_map[text_id].text;
        TTF_Text* temp_text = TTF_CreateText(font_engine.surface_engine, font_engine.font->self(), text.c_str(), text.size());
        auto t_surface = font_engine.font->toImage(text);
        SDL_Surface* surface = SDL_CreateSurface(t_surface->w, t_surface->h, SDL_PIXELFORMAT_ABGR64);
        SDL_DestroySurface(t_surface);
        auto _ret = TTF_DrawSurfaceText(temp_text, 0, 0, surface);
        if (!_ret) {
            Logger::log(std::format("Text to image failed! Exception: {}", SDL_GetError()), Logger::Error);
            TTF_DestroyText(temp_text);
            return nullptr;
        }
        TTF_DestroyText(temp_text);
        return surface;
    }

    AudioSystem* AudioSystem::global() {
        if (!_instance) {
            _instance = std::unique_ptr<AudioSystem>(new AudioSystem());
        }
        return _instance.get();
    }
    
    AudioSystem::AudioSystem() {
        _is_init = load();
    }

    AudioSystem::~AudioSystem() {}

    bool AudioSystem::isValid() const { return _is_init; }
 
    bool AudioSystem::load() {
        if (!MIX_Init()) {
            Logger::log(std::format("AudioSystem: Can't initilized audio system! Exception: {}", 
                SDL_GetError()), Logger::Error);
            return false;
        }

        SDL_AudioSpec _audio_spec(SDL_AUDIO_S16, 2, 44100);
        auto new_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
        if (!new_mixer) {
            Logger::log(std::format("AudioSystem: Can't initilized audio system! Exception: {}", 
                SDL_GetError()), Logger::Error);
            return false;
        }
        _mixer_list.push_back(new_mixer);
        Logger::log("AudioSystem: Loaded audio system!");
        return true;
    }

    void AudioSystem::unload() {
        if (!_is_init) return;
        /// TODO: Not sure the audio map should be unload?
        /// ...

        std::for_each(_mixer_list.begin(), _mixer_list.end(), [this](MIX_Mixer* m) {
            if (m) MIX_DestroyMixer(m);
        });
        MIX_Quit();
        Logger::log("AudioSystem: Unloaded audio system!");
        _is_init = false;
    }

    void AudioSystem::addNewMixer(size_t count) {
        if (!_is_init) return;
        while (count--) {
            SDL_AudioSpec _audio_spec(SDL_AUDIO_S16, 2, 44100);
            auto new_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
            if (!new_mixer) {
                Logger::log(std::format("AudioSystem: Can't create the new mixer device! Exception: {}",
                                        SDL_GetError()), Logger::Error);
            } else {
                _mixer_list.push_back(new_mixer);
            }
        }
    }

    MIX_Mixer *AudioSystem::mixer(size_t index) const {
        if (_mixer_list.size() <= index) {
            Logger::log(std::format("AudioSystem: Can't get index '{}' of mixer!"
                                    " Did you forget to call `AudioSystem::addNewMixer()` function?", index),
                        Logger::Error);
            return nullptr;
        }
        return _mixer_list.at(index);
    }

    size_t AudioSystem::mixerCount() const {
        return _mixer_list.size();
    }

    void AudioSystem::appendBGM(const std::string &name, const std::string &path) {

    }

    void AudioSystem::appendSFX(const std::string &name, const std::string &path) {

    }

    void AudioSystem::remove(const std::string &name) {

    }

    BGM *AudioSystem::getBGM(const std::string &name) {
        return nullptr;
    }

    SFX *AudioSystem::getSFX(const std::string &name) {
        return nullptr;
    }

    size_t AudioSystem::size() const {
        return _audio_map.size();
    }

}