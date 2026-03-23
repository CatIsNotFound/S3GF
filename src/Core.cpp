
#include "include/MyEngine/Core.h"
#include "include/MyEngine/Basic.h"
#include "include/MyEngine/Utils/All.h"
#include "include/MyEngine/Renderer/BaseCommand.h"
#include "include/MyEngine/Renderer/CommandFactory.h"
#include "include/MyEngine/Algorithm/Collider.h"

namespace MyEngine {
    std::unique_ptr<EventSystem> EventSystem::_instance{};
    SDL_Color Renderer::_background_color{RGBAColor::White};
    SDL_WindowID Engine::_main_window_id{0};
    bool Engine::_quit_requested{false};
    Engine::FileDialogResult Engine::_file_dialog_result{};
    int Engine::_return_code{0};
    bool Engine::_show_app_info{true};
    bool FontDatabase::_is_loaded{false};
    FontMap FontDatabase::_font_db{};
    std::vector<FontDatabase::FontInfo> FontDatabase::_def_fonts{};

    Renderer::Renderer(Window* window) : _window(window) {
        _renderer = SDL_CreateRenderer(_window->self(), nullptr);
        if (!_renderer) {
            Logger::log("The renderer is not created!", Logger::Fatal);
            Engine::throwFatalError();
        }
    }

    Renderer::~Renderer() {
        if (!_cmd_list.empty()) {
            for (auto& cmd : _cmd_list) {
                RenderCommand::CommandFactory::release(std::move(cmd));
            }
        }
        if (_renderer) {
            SDL_DestroyRenderer(_renderer);
            _renderer = nullptr;
        }
    }

    void Renderer::setLayerManager(LayerManager *layer_manager) {
        _layer_manager = std::unique_ptr<LayerManager>(layer_manager);
    }

    LayerManager* Renderer::layerManager() const {
        return _layer_manager.get();
    }

    void Renderer::setVSyncMode(Renderer::VSyncMode mode) {
        SDL_SetRenderVSync(_renderer, static_cast<int>(mode));
    }

    Renderer::VSyncMode Renderer::currentVSyncMode() const {
        int ret;
        SDL_GetRenderVSync(_renderer, &ret);
        return static_cast<Renderer::VSyncMode>(ret);
    }

    SDL_Renderer* Renderer::self() const {
        return _renderer;
    }

    Window* Renderer::window() const {
        return _window;
    }

    size_t Renderer::renderCountInSec() const {
        return _render_cnt_in_sec;
    }

    SDL_Surface* Renderer::capture() const {
        return SDL_RenderReadPixels(_renderer, nullptr);
    }

    SDL_Surface* Renderer::capture(Geometry geometry) const {
        SDL_Rect rect(geometry.x, geometry.y, geometry.width, geometry.height);
        return SDL_RenderReadPixels(_renderer, &rect);
    }

    void Renderer::_update() {
        SDL_SetRenderDrawColor(_renderer, _background_color.r, _background_color.g,
                                _background_color.b, _background_color.a);
        SDL_RenderClear(_renderer);
        for (auto& cmd : _cmd_list) {
            cmd->exec();
            RenderCommand::CommandFactory::release(std::move(cmd));
            _render_count++;
        }
        SDL_RenderPresent(_renderer);
        _cmd_list.clear();
        auto now = SDL_GetTicks();
        if (now - _start_ts >= 1000) {
            _start_ts = SDL_GetTicks();
            _render_cnt_in_sec = _render_count;
            _render_count = 0;
        }
        _window->paintEvent();
    }

    void Renderer::fillBackground(const SDL_Color &color) {
        addCommand<RenderCommand::FillCMD>(_renderer, color);
    }

    void Renderer::fillBackground(SDL_Color &&color) {
        addCommand<RenderCommand::FillCMD>(_renderer, std::move(color));
    }

    void Renderer::fillBackground(uint64_t rgb_hex) {
        addCommand<RenderCommand::FillCMD>(_renderer, RGBAColor::hexCode2RGBA(rgb_hex));
    }

    void Renderer::drawPoint(Graphics::Point *point) {
        addCommand<RenderCommand::PointCMD>(_renderer, point);
    }

    void Renderer::drawPoints(const std::vector<Graphics::Point*>& point_list) {
        addCommand<RenderCommand::PointCMD>(_renderer, nullptr,
                        RenderCommand::BaseCommand::Mode::Multiple, point_list.size(), point_list);
    }

    void Renderer::drawLine(Graphics::Line *line) {
        addCommand<RenderCommand::LineCMD>(_renderer, line);
    }

    void Renderer::drawLines(const std::vector<Graphics::Line*>& line_list) {
        addCommand<RenderCommand::LineCMD>(_renderer, nullptr,
                       RenderCommand::BaseCommand::Mode::Multiple, line_list.size(), line_list);
    }

    void Renderer::drawRectangle(Graphics::Rectangle* rectangle) {
        addCommand<RenderCommand::RectangleCMD>(_renderer, rectangle);
    }

    void Renderer::drawRectangles(const std::vector<Graphics::Rectangle*> &rectangle_list) {
        addCommand<RenderCommand::RectangleCMD>(_renderer, nullptr,
                        RenderCommand::BaseCommand::Mode::Multiple, rectangle_list.size(), rectangle_list);
    }

    void Renderer::drawTriangle(Graphics::Triangle* triangle) {
        addCommand<RenderCommand::TriangleCMD>(_renderer, triangle);
    }

    void Renderer::drawTriangles(const std::vector<Graphics::Triangle*> &triangle_list) {
        addCommand<RenderCommand::TriangleCMD>(_renderer, nullptr,
                       RenderCommand::BaseCommand::Mode::Multiple, triangle_list.size(), triangle_list);
    }

    void Renderer::drawEllipse(Graphics::Ellipse *ellipse) {
        addCommand<RenderCommand::EllipseCMD>(_renderer, ellipse);
    }

    void Renderer::drawEllipses(const std::vector<Graphics::Ellipse*> &ellipse_list) {
        addCommand<RenderCommand::EllipseCMD>(_renderer, nullptr,
                      RenderCommand::BaseCommand::Mode::Multiple, ellipse_list.size(), ellipse_list);
    }

    void Renderer::drawTexture(SDL_Texture* texture, TextureProperty* property) {
        if (!texture || !property) return;
        addCommand<RenderCommand::TextureCMD>(_renderer, texture, property);
    }

    void Renderer::drawTexture(SDL_Texture* texture, const std::vector<TextureProperty*>& properties) {
        if (!texture || properties.empty()) return;
        addCommand<RenderCommand::TextureCMD>(_renderer, texture, nullptr, RenderCommand::BaseCommand::Mode::Multiple,
                                  properties.size(), properties);
    }

    void Renderer::drawTextures(const std::vector<SDL_Texture*>& textures,
                                const std::vector<TextureProperty*>& properties) {
        addCommand<RenderCommand::TextureCMD>(_renderer, nullptr, nullptr, RenderCommand::BaseCommand::Mode::Custom,
                                  properties.size(), properties, textures);
    }

    void Renderer::drawText(TTF_Text* text, Vector2& position) {
        if (!text) return;
        addCommand<RenderCommand::TextCMD>(_renderer, text, position);
    }

    void Renderer::drawTexts(TTF_Text* text, const std::vector<Vector2*>& position_list) {
        if (!text) return;
        addCommand<RenderCommand::TextCMD>(_renderer, text, Vector2(), RenderCommand::BaseCommand::Mode::Multiple,
                                           position_list.size(), position_list);
    }

    void Renderer::drawTexts(const std::vector<TTF_Text*>& text_list, const std::vector<Vector2*>& position_list) {
        addCommand<RenderCommand::TextCMD>(_renderer, nullptr, Vector2(), RenderCommand::BaseCommand::Mode::Multiple,
                                           position_list.size(), position_list, text_list);
    }

    void Renderer::drawDebugText(const std::string &text, const MyEngine::Vector2 &position,
                                 const SDL_Color& color) {
        if (text.empty()) return;
        addCommand<RenderCommand::DebugTextCMD>(_renderer, text, position, color);
    }

    void Renderer::drawDebugTexts(const StringList& text_list, const std::vector<Vector2*>& position_list,
                                  const SDL_Color& color) {
        if (text_list.empty()) return;
        addCommand<RenderCommand::DebugTextCMD>(_renderer, std::string(), Vector2(), color,
                            RenderCommand::BaseCommand::Mode::Multiple, position_list.size(),
                            text_list, position_list);
    }

    void Renderer::drawDebugFPS(const MyEngine::Vector2 &position, const SDL_Color &color) {
        addCommand<RenderCommand::DebugTextCMD>(_renderer,
                    FMT::format("FPS: {}", window()->_engine->fps()), position, color);
    }

    void Renderer::setViewport(const Geometry& geometry) {
        if (geometry.width == 0 || geometry.height == 0) {
            addCommand<RenderCommand::ViewPortCMD>(_renderer, true, geometry);
        } else {
            addCommand<RenderCommand::ViewPortCMD>(_renderer, false, geometry);
        }
    }

    void Renderer::setClipView(const Geometry& geometry) {
        if (geometry.width == 0 || geometry.height == 0) {
            addCommand<RenderCommand::ClipViewCMD>(_renderer, true, geometry);
        } else {
            addCommand<RenderCommand::ClipViewCMD>(_renderer, false, geometry);
        }
    }

    void Renderer::setBlendMode(const SDL_BlendMode &blend_mode) {
        addCommand<RenderCommand::BlendModeCMD>(_renderer, blend_mode);
    }

    Window::Window(Engine* engine, const std::string& title, int width, int height, WindowType type, GraphicEngine graphic_engine)
            : _window_geometry(0, 0, width, height), _engine(engine) {
        SDL_WindowFlags flags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_HIDDEN;
        if (type == WindowType::Popup)      flags |= SDL_WINDOW_POPUP_MENU;
        if (type == WindowType::Tooltip)    flags |= SDL_WINDOW_TOOLTIP;
        if (type == WindowType::Tool)       flags |= SDL_WINDOW_UTILITY;
        if (type == WindowType::Borderless) flags |= SDL_WINDOW_BORDERLESS;
        if (graphic_engine == OpenGL)       flags |= SDL_WINDOW_OPENGL;
        if (graphic_engine == Vulkan)       flags |= SDL_WINDOW_VULKAN;

        _window = SDL_CreateWindow(title.c_str(), width, height, flags);
        if (!_window) {
            Logger::log(Logger::Fatal, "Failed to create window! \nException: {}", SDL_GetError());
            Engine::throwFatalError();
        }
        _renderer = std::make_shared<Renderer>(this);
        _winID = SDL_GetWindowID(_window);
        SDL_GetWindowPosition(_window, &_window_geometry.x, &_window_geometry.y);
        Logger::log(Logger::Debug, "Window: created with ID {}", _winID);
        if (engine) {
            engine->newWindow(this);
        } else {
            Logger::log("Window: Can't find engine object!", Logger::Fatal);
            Engine::throwFatalError();
        }
    }

    Window::Window(Window *parent, const std::string &title, int width, int height, WindowType type, GraphicEngine graphic_engine)
            : _window_geometry(0, 0, width, height), _engine(parent->engine()) {
        SDL_WindowFlags flags = SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_TRANSPARENT | SDL_WINDOW_HIDDEN;
        if (type == WindowType::Popup)      flags |= SDL_WINDOW_POPUP_MENU;
        if (type == WindowType::Tooltip)    flags |= SDL_WINDOW_TOOLTIP;
        if (type == WindowType::Tool)       flags |= SDL_WINDOW_UTILITY | SDL_WINDOW_ALWAYS_ON_TOP;
        if (type == WindowType::Borderless) flags |= SDL_WINDOW_BORDERLESS;
        if (graphic_engine == OpenGL)       flags |= SDL_WINDOW_OPENGL;
        if (graphic_engine == Vulkan)       flags |= SDL_WINDOW_VULKAN;

        if (type == WindowType::Popup || type == WindowType::Tooltip) {
            _window = SDL_CreatePopupWindow(parent->self(),
                0, 0, width, height, flags);
        } else {
            _window = SDL_CreateWindow(title.c_str(), width, height, flags);
        }
        if (!_window) {
            Logger::log(Logger::Fatal, "Failed to create window! \nException: {}", SDL_GetError());
            Engine::throwFatalError();
        }
        _renderer = std::make_shared<Renderer>(this);
        _winID = SDL_GetWindowID(_window);
        SDL_GetWindowPosition(_window, &_window_geometry.x, &_window_geometry.y);
        Logger::log(Logger::Debug, "Window: created with ID {}", _winID);
        if (_engine) {
            _engine->newWindow(this, parent->windowID(), _winID);
        } else {
            Logger::log("Window: Can't find engine object!", Logger::Fatal);
            Engine::throwFatalError();
        }
    }

    Window::~Window() {
        if (_renderer) {
            _renderer.reset();
        }
        if (_win_icon) SDL_DestroySurface(_win_icon);
        if (_window) {
            SDL_DestroyWindow(_window);
            _window = nullptr;
            Logger::log(Logger::Debug, "Window: ID {} destroyed", _winID);
        }
    }

    bool Window::move(int x, int y) {
        bool _ret = SDL_SetWindowPosition(_window, x, y);
        if (!_ret) {
            Logger::log(Logger::Error, "Window: Can't move window! Exception: {}", SDL_GetError());
            return false;
        }
        _window_geometry.x = x;
        _window_geometry.y = y;
        return true;
    }

    bool Window::resize(int width, int height) {
        bool _ret = SDL_SetWindowSize(_window, width, height);
        if (!_ret) {
            Logger::log(Logger::Error, "Window: Can't reshape window! Exception: {}", SDL_GetError());
            return false;
        }
        _window_geometry.width = width;
        _window_geometry.height = height;
        return true;
    }

    bool Window::setMinimumSize(int width, int height) {
        bool _ret = SDL_SetWindowMinimumSize(_window, width, height);
        if (!_ret) {
            Logger::log(Logger::Error, "Window: Can't reshape window! Exception: {}", SDL_GetError());
            return false;
        }
        return true;
    }

    bool Window::setMaximumSize(int width, int height) {
        bool _ret = SDL_SetWindowMaximumSize(_window, width, height);
        if (!_ret) {
            Logger::log(Logger::Error, "Window: Can't reshape window! Exception: {}", SDL_GetError());
            return false;
        }
        return true;
    }

    bool Window::setGeometry(int x, int y, int width, int height) {
        return (!move(x, y) || !resize(width, height)); 
    }

    const Geometry& Window::geometry() const {
        return _window_geometry;
    }

    Window::WindowSize Window::minimumSize() const {
        WindowSize ret(0, 0);
        SDL_GetWindowMinimumSize(_window, &ret.width, &ret.height);
        return ret;
    }

    Window::WindowSize Window::maximumSize() const {
        WindowSize ret(0, 0);
        SDL_GetWindowMaximumSize(_window, &ret.width, &ret.height);
        return ret;
    }

    Window::WindowSize Window::windowSize() const {
        WindowSize ret(0, 0);
        SDL_GetWindowSize(_window, &ret.width, &ret.height);
        return ret;
    }

    uint32_t Window::windowID() const {
        return _winID;
    }

    bool Window::show() {
        auto _ret = SDL_ShowWindow(_window);
        if (!_ret) {
            Logger::log(Logger::Error, "Window: Can't show window! Exception: {}", SDL_GetError());
            return false;
        }
        return true;
    }

    bool Window::hide() {
        bool _ret = SDL_HideWindow(_window);
        if (!_ret) {
            Logger::log(Logger::Error, "Window: Can't hide window! Exception: {}", SDL_GetError());
            return false;
        }
        return true;
    }

    bool Window::visible() const {
        return !(SDL_GetWindowFlags(_window) & SDL_WINDOW_HIDDEN);
    }

    void Window::close() {
        unloadEvent();
    }

    bool Window::setResizable(bool enabled) {
        auto _ret = SDL_SetWindowResizable(_window, enabled);
        if (!_ret) {
            Logger::log(Logger::Error, "Window: Can't set window resizable mode! Exception: {}", SDL_GetError());
            return false;
        }
        return true;
    }

    bool Window::resizable() const {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_RESIZABLE;
    }

    void Window::setRenderer(Renderer* renderer) {
        if (!renderer) {
            Logger::log("Window: The specified renderer is not valid!", Logger::Error);
            return;
        }
        if (!_renderer) _renderer = std::shared_ptr<Renderer>(renderer);
    }

    Renderer* Window::renderer() const {
        return _renderer.get();
    }

    void Window::setBorderless(bool enabled) {
        bool _ok = SDL_SetWindowBordered(_window, !enabled);
        if (!_ok)
            Logger::log(Logger::Error, "Window (ID {}): Can't set borderless for this window! Exception: {}", _winID, SDL_GetError());
    }

    bool Window::borderless() const {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_BORDERLESS;
    }

    void Window::setWindowOpacity(float opacity) {
        bool _ok = SDL_SetWindowOpacity(_window, std::clamp(opacity, 0.0f, 1.0f));
        if (!_ok) {
            Logger::log(Logger::Error, "Window (ID {}): Can't set window opacity for this window! Exception: {}", _winID, SDL_GetError());
        }
    }

    float Window::windowOpacity() const {
        float ret = SDL_GetWindowOpacity(_window);
        if (ret < 0) {
            Logger::log(Logger::Error, "Window (ID {}): Can't get window opacity for this window! Exception: {}", _winID, SDL_GetError());
        }
        return ret;
    }

    void Window::setFullScreen(bool enabled, bool move_to_center) {
        if (!move_to_center) {
            _mouse_pos = Cursor::global()->globalPosition();
        }
        bool _ok = SDL_SetWindowFullscreen(_window, enabled);
        if (!_ok) {
            Logger::log(Logger::Error, "Window (ID {}): Can't set fullscreen for this window!", _winID);
            return;
        }
        if (move_to_center) {
            Cursor::global()->moveToCenter(fullScreen() ? this : nullptr);
        } else {
            Cursor::global()->move(_mouse_pos, nullptr);
        }
    }

    bool Window::fullScreen() const {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_FULLSCREEN;
    }

    bool Window::setWindowAlwaysOnTop(bool enabled) {
        auto _ret = SDL_SetWindowAlwaysOnTop(_window, enabled);
        if (_ret)
            SDL_SetWindowSize(_window, _window_geometry.width, _window_geometry.height);
        return _ret;
    }

    bool Window::isAlwaysOnTop() const {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_ALWAYS_ON_TOP;
    }

    bool Window::minimizeWindow() {
        return SDL_MinimizeWindow(_window);
    }

    bool Window::maximizeWindow() {
        if (!resizable()) {
            Logger::log("Engine: Failed to maximize window! Did you forget to called `setResizable(true)`?",
                        Logger::Error);
            return false;
        }
        return SDL_MaximizeWindow(_window);
    }

    bool Window::restoreWindow() {
        return SDL_RestoreWindow(_window);
    }

    bool Window::isMinimizedWindow() const {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED;
    }

    bool Window::isMaximizedWindow() const {
        return SDL_GetWindowFlags(_window) & SDL_WINDOW_MAXIMIZED;
    }

    bool Window::isRestoredWindow() const {
        return !(SDL_GetWindowFlags(_window) &
                    (SDL_WINDOW_MINIMIZED | SDL_WINDOW_MAXIMIZED | SDL_WINDOW_FULLSCREEN));
    }

    void Window::setFreezeEnabled(bool enabled) {
        _render = !enabled;
    }

    bool Window::isFreeze() const {
        return !_render;
    }

    void Window::setWindowTitle(const std::string& title) {
        SDL_SetWindowTitle(_window, title.c_str());
    }

    std::string_view Window::windowTitle() const {
        return SDL_GetWindowTitle(_window);
    }

    void Window::setWindowIcon(const std::string& icon_path) {
        _win_icon = IMG_Load(icon_path.c_str());
        if (!_win_icon) {
            Logger::log(Logger::Error, "Window (ID {}): Can't set icon for this window! Exception: {}", _winID, SDL_GetError());
        }
        bool ok = SDL_SetWindowIcon(_window, _win_icon);
        if (!ok) {
            Logger::log(Logger::Error, "Window (ID {}): Can't set icon for this window! Exception: {}", _winID, SDL_GetError());
        }
    }

    SDL_Surface* Window::windowIcon() const {
        return _win_icon;
    }

    void Window::setCursor(Cursor::StdCursor cursor_style) {
        _cursor = cursor_style;
    }

    Cursor::StdCursor Window::cursor() const {
        return _cursor;
    }

    void Window::setDragDropEnabled(bool enabled) {
        _drag_mode = enabled;
        if (!enabled) _dragging_pos.reset(0, 0);
    }

    bool Window::dragDropEnabled() const {
        return _drag_mode;
    }

    bool Window::isDragging() const {
        return _drag_mode && _dragging;
    }

    const Vector2& Window::draggingPosition() const {
        return _dragging_pos;
    }

    std::string_view Window::droppedInfo() const {
        return _drop_url;
    }

    bool Window::setWindowShape(SDL_Surface *surface) {
        auto ret = SDL_SetWindowShape(_window, surface);
        if (surface) SDL_DestroySurface(surface);
        return ret;
    }

    bool Window::clearWindowShape() {
        return SDL_SetWindowShape(_window, nullptr);
    }

    SDL_Window* Window::self() const {
        if (!_window) {
            Logger::log("The window is not created!", Logger::Error);
        }
        return _window;
    }

    Window *Window::parent() const {
        auto parent = SDL_GetWindowParent(_window);
        if (!parent) {
            auto p_id = _engine->windowParentID(_winID);
            if (p_id.has_value()) return _engine->window(p_id.value()).value();
            return nullptr;
        }
        auto id = SDL_GetWindowID(_window);
        if (_engine->isWindowExist(id)) return _engine->window(id).value();
        return nullptr;
    }

    Engine* Window::engine() const {
        return _engine;
    }

    void Window::installPaintEvent(const std::function<void(Renderer* renderer)>& paint_event, bool push_back) {
        if (push_back) _paint_event_list.push_back(paint_event);
        else _paint_event_list.push_front(paint_event);
    }

    void Window::paintEvent() {
        for (auto& ev : _paint_event_list) {
            if (ev) ev(_renderer.get());
        }
    }

    void Window::resizeEvent() {
        auto _ret = SDL_GetWindowSize(_window, &_window_geometry.width, &_window_geometry.height);
        if (!_ret) {
            Logger::log(Logger::Warn, "Window: Failed to get window size for ID {}", _winID);
        }
    }

    void Window::moveEvent() {
        auto _ret = SDL_GetWindowPosition(_window, &_window_geometry.x, &_window_geometry.y);
        if (!_ret) {
            Logger::log(Logger::Warn, "Window: Failed to get window position for ID {}", _winID);
        }
    }

    void Window::getFocusEvent() {}

    void Window::lostFocusEvent() {}

    void Window::unloadEvent() {
        if (_engine) {
            Logger::log(Logger::Debug, "Window: Unloaded window id {}", _winID);
            _engine->removeWindow(_winID);
        } else {
            Logger::log(Logger::Error, "Window: Unload window id {} failed!", _winID);
        }
    }

    void Window::showEvent() {}
    void Window::hideEvent() {}
    void Window::windowMinimizedEvent() {}
    void Window::windowMaximizedEvent() {}
    void Window::windowRestoredEvent() {}
    void Window::enteredFullscreenEvent() {}
    void Window::leaveFullscreenEvent() {}
    void Window::mouseEnteredEvent() { Cursor::global()->setCursor(_cursor); }
    void Window::mouseLeftEvent() {}
    void Window::mouseUpEvent() {}
    void Window::mouseDownEvent(MouseStatus button) {}
    void Window::mouseClickedEvent(MouseStatus button) {}
    void Window::mouseMovedEvent(const Vector2 &position, const Vector2 &distance) {}
    void Window::keyUpEvent(SDL_Scancode keycode) {}
    void Window::keyDownEvent(SDL_Scancode keycode) {}
    void Window::keyPressedEvent(SDL_Scancode keycode) {}
    void Window::fingerDownEvent(SDL_FingerID id, const Vector2 &position) {}
    void Window::fingerUpEvent(SDL_FingerID id, const Vector2 &position) {}
    void Window::fingerMovedEvent(SDL_FingerID id, const Vector2 &position,
                                  const MyEngine::Vector2 &distance) {}
    void Window::fingerMoveOutEvent(SDL_FingerID id) {}
    void Window::fingerMoveInEvent(SDL_FingerID id) {}
    void Window::fingerTappedEvent(SDL_FingerID id, const Vector2 &position) {}
    void Window::dragInEvent() {}
    void Window::dragOutEvent() {}
    void Window::dragMovedEvent(const Vector2 &position, const char *data) {}
    void Window::dropEvent(const char *url) {}

    std::optional<Window::FingerEvent> Window::getFingerEventByID(SDL_FingerID finger_id) const {
        return _finger_event_list.contains(finger_id) ?
            _finger_event_list.at(finger_id) : std::optional<Window::FingerEvent>();
    }

    std::vector<SDL_FingerID> Window::getFingersIDList() const {
        std::vector<SDL_FingerID> _list;
        for (auto& id : _finger_event_list) {
            _list.push_back(id.first);
        }
        return _list;
    }

    size_t Window::getFingersCount() const { return _finger_event_list.size(); }

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
            Logger::log(Logger::Warn, "EventSystem: The event with ID {} is already exists! It will overwrite it!", id);
            _event_list[id] = event;
            return;
        }
        _event_list.emplace(id, event);
        Logger::log(Logger::Debug, "EventSystem: Append a new event with ID {}", id);
    }

    uint64_t EventSystem::appendEvent(const std::function<void(SDL_Event)> &event) {
        uint64_t id = IDGenerator::getNewEventID();
        appendEvent(id, event);
        return id;
    }

    void EventSystem::removeEvent(uint64_t id) {
        if (_event_list.contains(id)) {
            // _event_list.erase(id);
            _del_event_deque.push_back(id);
            Logger::log(Logger::Debug, "EventSystem: Requested to remove event with ID {}", id);
        } else {
            Logger::log(Logger::Warn, "EventSystem: The event with ID {} is not found!", id);
        }
    }

    bool EventSystem::isEventExist(uint64_t id) {
        return _event_list.contains(id);
    }

    void EventSystem::appendGlobalEvent(uint64_t g_id, const std::function<void()>& event) {
        if (_global_event_list.contains(g_id)) {
            Logger::log(Logger::Warn, "EventSystem: The global event with ID {} is already exists! It will overwrite it!", g_id);
            _global_event_list[g_id] = event;
        } else {
            _global_event_list.emplace(g_id, event);
            Logger::log(Logger::Debug, "EventSystem: Append a global event by ID {}", g_id);
        }
    }

    uint64_t EventSystem::appendGlobalEvent(const std::function<void()> &event) {
        uint64_t id = IDGenerator::getNewGlobalEventID();
        appendGlobalEvent(id, event);
        return id;
    }

    void EventSystem::removeGlobalEvent(uint64_t g_id) {
        if (_global_event_list.contains(g_id)) {
            // _global_event_list.erase(g_id);
            _del_g_event_deque.push_back(g_id);
            Logger::log(Logger::Debug, "EventSystem: Requested to remove global event with ID {}", g_id);
        } else {
            Logger::log(Logger::Warn, "EventSystem: The global event with ID {} is not found!", g_id);
        }
    }

    bool EventSystem::isGlobalEventExist(uint64_t g_id) {
        return _global_event_list.contains(g_id);
    }

    size_t EventSystem::eventCount() const { return _event_list.size(); }

    bool EventSystem::run() {
        SDL_Event ev;
        bool running = true;
        if (SDL_PollEvent(&ev)) {
            auto win_id_list = _engine->windowIDList();

            // Keyboard Event
            _kb_events = const_cast<bool*>(SDL_GetKeyboardState(&_nums_keys));
            _keys_status.clear();
            for (int i = 0; i < _nums_keys; ++i) {
                if (_kb_events[i]) _keys_status.emplace_back(static_cast<SDL_Scancode>(i));
            }

            // Mouse Event
            _mouse_events = static_cast<MouseStatus>(SDL_GetMouseState(&_mouse_pos.x, &_mouse_pos.y));
            if (!_mouse_down_changed) {
                // When any of mouse buttons is pressed down, triggered...
                if (_mouse_events > MouseStatus::None) {
                    _mouse_down_changed = true;
                    _before_mouse_down_pos.reset(_mouse_pos);
                }
            } else {
                if (_mouse_events > MouseStatus::None) {
                    _mouse_down_dis.reset(_mouse_pos - _before_mouse_down_pos);
                } else {
                    _mouse_down_changed = false;
                    _mouse_down_dis.reset(0, 0);
                }
            }

            // Finger Event
            // - Used to handle global touch events on a touchscreen
            bool is_finger_event = false;
            static std::vector<uint32_t> finger_events = {SDL_EVENT_FINGER_UP, SDL_EVENT_FINGER_DOWN,
                                                          SDL_EVENT_FINGER_MOTION, SDL_EVENT_FINGER_CANCELED};
            if (std::any_of(finger_events.begin(), finger_events.end(),
                            [&ev](uint32_t type) { return ev.type == type; })) {
                is_finger_event = true;
                Window* win = nullptr;
                Vector2 cur_pos;
                if (!win_id_list.empty() && _engine->isWindowExist(ev.tfinger.windowID)) {
                    win = _engine->window(ev.tfinger.windowID).value();
                    cur_pos.x = static_cast<float>(win->_window_geometry.width) * ev.tfinger.x;
                    cur_pos.y = static_cast<float>(win->_window_geometry.height) * ev.tfinger.y;
                }
                if (win) {
                    Window::FingerEvent* f_ev{};
                    bool is_on = false;
                    switch (ev.tfinger.type) {
                    case SDL_EVENT_FINGER_DOWN:
                        win->_finger_event_list.try_emplace(ev.tfinger.fingerID,
                                Window::FingerEvent(ev.tfinger.touchID, ev.tfinger.pressure,
                                        cur_pos));
                        win->fingerDownEvent(ev.tfinger.fingerID, cur_pos);
                        break;
                    case SDL_EVENT_FINGER_UP:
                        is_on = win->_finger_event_list.at(ev.tfinger.fingerID).is_in_window;
                        win->_finger_event_list.erase(ev.tfinger.fingerID);
                        win->fingerUpEvent(ev.tfinger.fingerID, cur_pos);
                        if (is_on) win->fingerTappedEvent(ev.tfinger.fingerID, cur_pos);
                        break;
                    case SDL_EVENT_FINGER_MOTION:
                        if (!win->_finger_event_list.contains(ev.tfinger.fingerID)) {
                            win->_finger_event_list.try_emplace(ev.tfinger.fingerID,
                                Window::FingerEvent(ev.tfinger.touchID, ev.tfinger.pressure, cur_pos));
                        }
                        f_ev = &(win->_finger_event_list.at(ev.tfinger.fingerID));
                        f_ev->distance_pos = cur_pos - f_ev->finger_down_pos;
                        win->fingerMovedEvent(ev.tfinger.fingerID, cur_pos, f_ev->distance_pos);
                        if (Algorithm::comparePosInGeometry(Cursor::global()->globalPosition(),
                                        toGeometryFloat(win->geometry())) < 0) {
                            if (f_ev->is_in_window) {
                                win->fingerMoveOutEvent(ev.tfinger.fingerID);
                                f_ev->is_in_window = false;
                            }
                        } else {
                            if (!f_ev->is_in_window) {
                                win->fingerMoveInEvent(ev.tfinger.fingerID);
                                f_ev->is_in_window = true;
                            }
                        }
                        break;
                    case SDL_EVENT_FINGER_CANCELED:
                        win->_finger_event_list.erase(ev.tfinger.fingerID);
                        break;
                    default:
                        break;
                    }
                }
            }

            // Windows Event
            if (!is_finger_event && !win_id_list.empty()) {
                static bool mouse_down = false, key_down = false;
                std::ranges::for_each(win_id_list, [&] (uint32_t id) {
                    if (!_engine->isWindowExist(id)) return;
                    auto win = _engine->window(id).value();
                    // Clear the fingers list, if it has any fingers.
                    if (!win->_finger_event_list.empty()) win->_finger_event_list.clear();
                    // Cope with the window event.
                    if (ev.window.windowID != id) return;
                    if (ev.window.type == SDL_EVENT_WINDOW_MOVED) {
                        win->moveEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_RESIZED) {
                        win->resizeEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
                        win->getFocusEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
                        win->lostFocusEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                        win->unloadEvent();
                        win_id_list = _engine->windowIDList();
                        if (win_id_list.empty()) running = false;
                        return;
                    } else if (ev.window.type == SDL_EVENT_WINDOW_HIDDEN) {
                        win->hideEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_SHOWN) {
                        win->showEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_MINIMIZED) {
                        win->windowMinimizedEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_MAXIMIZED) {
                        win->windowMaximizedEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_RESTORED) {
                        win->windowRestoredEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_ENTER_FULLSCREEN) {
                        win->enteredFullscreenEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_LEAVE_FULLSCREEN) {
                        win->leaveFullscreenEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_MOUSE_ENTER) {
                        win->mouseEnteredEvent();
                    } else if (ev.window.type == SDL_EVENT_WINDOW_MOUSE_LEAVE) {
                        win->mouseLeftEvent();
                    }

                    // Keyboard event
                    if (!key_down) {
                        if (!_keys_status.empty()) {
                            key_down = true;
                            win->keyDownEvent(ev.key.scancode);
                        }
                    } else {
                        if (_keys_status.empty()) {
                            key_down = false;
                            win->keyUpEvent(ev.key.scancode);
                            win->keyPressedEvent(ev.key.scancode);
                        } else if (!ev.key.repeat) {
                            auto scancode = ev.key.scancode;
                            if (scancode) {
                                if (std::find(_keys_status.begin(), _keys_status.end(),
                                              ev.key.scancode) != _keys_status.end()) {
                                    win->keyDownEvent(ev.key.scancode);
                                } else {
                                    win->keyUpEvent(ev.key.scancode);
                                    win->keyPressedEvent(ev.key.scancode);
                                }
                            }
                        }
                    }

                    // Mouse event
                    static MouseStatus old_mouse_event{MouseStatus::None};
                    if (!mouse_down) {
                        if (_mouse_events > MouseStatus::None) {
                            win->mouseDownEvent(static_cast<MouseStatus>(_mouse_events));
                            mouse_down = true;
                            old_mouse_event = _mouse_events;
                        }
                    } else {
                        if (_mouse_events > MouseStatus::None) {
                            win->mouseMovedEvent(_mouse_pos, _mouse_down_dis);
                            old_mouse_event = _mouse_events;
                        } else {
                            mouse_down = false;
                            win->mouseUpEvent();
                            win->mouseClickedEvent(static_cast<MouseStatus>(old_mouse_event));
                            old_mouse_event = MouseStatus::None;
                        }
                    }

                    // Drag and drop event
                    // - Cope with dragging and dropped
                    // - Must set `Window::setDragDropEnabled()` function to enabled.
                    if (!win->_drag_mode) return;
                    if (!win->_dragging) {
                        if (ev.drop.type == SDL_EVENT_DROP_BEGIN) {
                            win->_dragging = true;
                            win->_dragging_pos.reset(
                                    Cursor::global()->globalPosition() - toGeometryFloat(win->geometry()).pos);
                            win->dragInEvent();
                        }
                    } else {
                        if (ev.drop.type == SDL_EVENT_DROP_COMPLETE) {
                            win->dragOutEvent();
                            win->_dragging_pos.reset(0, 0);
                            win->_dragging = false;
                        } else if (ev.drop.type == SDL_EVENT_DROP_FILE) {
                            win->dropEvent(ev.drop.data);
                            win->_drop_url.assign(ev.drop.data);
                                                  win->_dragging_pos.reset(0, 0);
                                                  win->_dragging = false;
                                              } else if (ev.drop.type == SDL_EVENT_DROP_TEXT) {
                                                  win->dropEvent(ev.drop.data);
                                                  win->_drop_url.assign(ev.drop.data);
                                                  win->_dragging = false;
                                              } else {
                                                  auto real_pos = Cursor::global()->globalPosition() - toGeometryFloat(win->geometry()).pos;
                                                  win->_dragging_pos.reset(real_pos);
                                                  win->dragMovedEvent(real_pos, ev.drop.data);
                                              }
                                          }
                                      });
            }

            for (auto& event : _event_list) {
                if (event.second) event.second(ev);
            }
            for (auto& id : _del_event_deque) {
                _event_list.erase(id);
                Logger::log(Logger::Debug, "EventSystem: Removed the event with ID {}", id);
            }
        }
        for (auto& e : _global_event_list) {
            if (e.second) e.second();
        }
        for (auto& id : _del_g_event_deque) {
            _global_event_list.erase(id);
            Logger::log(Logger::Debug, "EventSystem: Removed a global event with ID {}", id);
        }
        _del_event_deque.clear();
        _del_g_event_deque.clear();
        return running;
    }

    size_t EventSystem::globalEventCount() const {
        return _global_event_list.size();
    }

    MouseStatus EventSystem::captureMouseStatus() const {
        return _mouse_events;
    }

    bool EventSystem::captureMouse(MouseStatus mouse_status) const {
        if (mouse_status == MouseStatus::Left) {
            return static_cast<uint8_t>(_mouse_events) % 2 > 0;
        } else if (mouse_status == MouseStatus::Right) {
            return _mouse_events >= MouseStatus::Right;
        } else if (mouse_status == MouseStatus::Middle) {
            return !(static_cast<uint8_t>(_mouse_events) % 3) || (_mouse_events == MouseStatus::LeftMiddleRight);
        }
        return _mouse_events == mouse_status;
    }

    const Vector2& EventSystem::captureMouseAbsDistance() const {
        return _mouse_down_dis;
    }

    const Vector2& EventSystem::captureMousePosition() const {
        return _mouse_pos;
    }

    const std::vector<SDL_Scancode>& EventSystem::captureKeyboardStatus() const {
        return _keys_status;
    }

    bool EventSystem::captureKeyboard(SDL_Scancode code) const {
        return _kb_events[code];
    }

    std::string_view EventSystem::mouseStatusName(MouseStatus status) {
        switch (status) {
            case MouseStatus::None:
                return "None";
            case MouseStatus::Left:
                return "Left";
            case MouseStatus::Middle:
                return "Middle";
            case MouseStatus::LeftMiddle:
                return "Left Middle";
            case MouseStatus::Right:
                return "Right";
            case MouseStatus::LeftRight:
                return "Left Right";
            case MouseStatus::MiddleRight:
                return "Middle Right";
            case MouseStatus::LeftMiddleRight:
                return "Left Middle Right";
        }
        return {};
    }

    Engine::Engine(const char *app_name, const char *app_version, const char *app_id)
        : _running(true) {
        SDL_SetAppMetadata(app_name, app_version, app_id);
        if (_show_app_info) {
            std::cout << FMT::format("MyEngine {} (Based on SDL {}.{}.{})\n",
                                     MYENGINE_FULL_VERSION, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION)
                      << "For more information, visit: https://github.com/CatIsNotFound/MyEngine \n"
                         "                             https://gitee.com/CatIsNotFound/MyEngine\n" << std::endl;
            std::cout << FMT::format("========== Application Info ==========\n"
                                     "ID: {} \nName: {} \nVersion: {} \n",
                                     app_id, app_name, app_version) << std::endl;
        }
        if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throwFatalError();
        }
        SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "0");
        SDL_SetHint(SDL_HINT_PEN_MOUSE_EVENTS, "0");
        Logger::log("Engine: Started up application!");
        TextSystem::global();
        if (!TextSystem::global()->_is_loaded) {
            TextSystem::global()->load();
        }
        if (!AudioSystem::global()->_is_init) {
            AudioSystem::global()->load();
        }
        EventSystem::global(this);
        signal(SIGINT, Engine::exit);
    }

    Engine::~Engine() {
        if (_running) {
            cleanUp();
            Logger::log("Engine: Shutdown application! Did you forget to call `exec()` function?", Logger::Info);
        }
    }

    void Engine::disabledShowAppInfo() {
        _show_app_info = false;
    }

    void Engine::setApplicationID(const char *app_id) {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, app_id);
    }

    void Engine::setApplicationName(const char *app_name) {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, app_name);
    }

    void Engine::setApplicationVersion(const char *app_version) {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, app_version);
    }

    void Engine::setApplicationCopyright(const char *app_copyright) {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, app_copyright);
    }

    void Engine::setApplicationAuthor(const char *app_author) {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, app_author);
    }

    void Engine::setApplicationTypeName(const char *app_type) {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, app_type);
    }

    void Engine::setApplicationURL(const char *app_url) {
        SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, app_url);
    }

    std::string_view Engine::applicationID() {
        return SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING);
    }

    std::string_view Engine::applicationName() {
        return SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING);
    }

    std::string_view Engine::applicationVersion() {
        return SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING);
    }

    std::string_view Engine::applicationCopyright() {
        return SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING);
    }

    std::string_view Engine::applicationAuthor() {
        return SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING);
    }

    std::string_view Engine::applicationTypeName() {
        return SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING);
    }

    std::string_view Engine::applicationURL() {
        return SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING);
    }

    void Engine::openApplicationURL(bool* ok) {
        if (!SDL_OpenURL(SDL_GetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING))) {
            Logger::log(Logger::Warn, "Engine: Can't open application URL by browser! Exception: {}",
                        SDL_GetError());
            if (ok) *ok = false;
            return;
        }
        if (ok) *ok = true;
    }

    void Engine::setLimitMaxMemorySize(size_t mem_in_kb) {
        _max_mem_kb = mem_in_kb;
        _warn_mem_kb = static_cast<size_t>(static_cast<float>(_max_mem_kb) * 0.85f);
    }

    size_t Engine::limitMaxMemorySize() const { return _max_mem_kb; }

    void Engine::setRenderSetup(uint32_t max_commands, bool auto_incresement) {
        RenderCommand::CommandFactory::initFactory(max_commands, auto_incresement);
    }

    bool Engine::isRunning() const {
        return _running;
    }

    void Engine::exit(int code) {
        _return_code = code;
        _quit_requested = true;
    }

    int Engine::exec() {
        running();
        cleanUp();
        return _return_code;
    }

    void Engine::newWindow(Window* window, SDL_WindowID parent_window_id, SDL_WindowID child_window_id) {
        if (!window) return;
        if (_main_window_id == 0) _main_window_id = window->windowID();
        if (!_window_list.contains(window->windowID()))
            _window_list.emplace(window->windowID(), window);
        if (parent_window_id > 0 && _window_list.contains(parent_window_id)) {
            if (_parent_window_list.contains(parent_window_id)) {
                _parent_window_list.at(parent_window_id).emplace_back(child_window_id);
            } else {
                _parent_window_list.insert({parent_window_id, {child_window_id}});
            }
        }
    }

    void Engine::removeWindow(SDL_WindowID id) {
        if (_window_list.contains(id)) {
            auto win = _window_list.at(id).get();
            auto parent = win->parent();
            if (parent) {
                auto parent_win_id = parent->windowID();
                if (_parent_window_list.contains(parent_win_id)) {
                    std::erase_if(_parent_window_list.at(parent_win_id),
                        [&id] (const auto& this_id) {
                            return this_id == id;
                    });
                }
            }
            if (_parent_window_list.contains(id)) {
                for (auto& sub_id : _parent_window_list.at(id)) {
                    removeWindow(sub_id);
                }
                _parent_window_list.erase(id);
            }
            auto delete_win = std::move(_window_list.at(id));
            _window_list.erase(id);
            delete_win.reset();
        }
    }

    std::optional<Window *> Engine::window(SDL_WindowID id) const {
        if (_window_list.contains(id)) {
            return _window_list.at(id).get();
        }
        auto err = FMT::format("Engine: Window id {} is not created or is already removed!", id);
        Logger::log(err, Logger::Fatal);
        Engine::throwCustomFatalError<NullPointerException>();
        return {};
    }

    std::vector<uint32_t> Engine::windowIDList() const {
        std::vector<uint32_t> id_list;
        std::for_each(_window_list.begin(), _window_list.end(), [&id_list](const auto& window) {
            id_list.push_back(window.second->windowID());
        });
        return id_list;
    }

    std::vector<SDL_WindowID> Engine::windowIDList(SDL_WindowID parent_window_id) {
        if (_parent_window_list.contains(parent_window_id)) {
            return _parent_window_list.at(parent_window_id);
        }
        return {};
    }

    std::vector<SDL_WindowID> Engine::parentWindowIDList() {
        std::vector<SDL_WindowID> id_list;
        std::ranges::for_each(_parent_window_list,
                              [&id_list](const auto& id) { id_list.push_back(id.first); });
        return id_list;
    }

    std::optional<SDL_WindowID> Engine::windowParentID(SDL_WindowID id) {
        for (auto& [sub_id, id_list] : _parent_window_list) {
            for (auto& child_id : id_list) {
                if (child_id == id) return sub_id;
            }
        }
        return std::nullopt;
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
        bool ok;
        auto err = copeWithFatalError(&ok);
        if (!ok) return;
        throw EngineException(err);
    }

    void Engine::installCleanUpEvent(const std::function<void()> &event) {
        _clean_up_event = event;
    }

    bool Engine::messageBox(MessageBoxType type, const std::string &title, const std::string &message,
                uint32_t parent_window_id) {
        SDL_MessageBoxFlags flags = SDL_MESSAGEBOX_INFORMATION;
        if (type == MessageBoxType::Warning)     flags = SDL_MESSAGEBOX_WARNING;
        if (type == MessageBoxType::Fatal)       flags = SDL_MESSAGEBOX_ERROR;
        auto ret = SDL_ShowSimpleMessageBox(flags, title.c_str(), message.c_str(),
                _window_list.contains(parent_window_id) ? _window_list.at(parent_window_id).get()->self() : nullptr);
        return ret;
    }

    void Engine::openFileDialog(const StringList &filters,
                                const std::string &default_path, Window *parent_window) {
        std::vector<SDL_DialogFileFilter> m_filters;
        struct TempFilter {
            std::string name;
            std::string pattern;
        };
        std::vector<TempFilter> _temp_filters;
        for (size_t i = 0; i < filters.size(); ++i) {
            std::string name, pattern;
            getFileFilter(filters[i], name, pattern);
            _temp_filters.emplace_back(name, pattern);
        }
        for (auto& filter : _temp_filters) {
            m_filters.emplace_back(filter.name.data(), filter.pattern.data());
        }
        SDL_ShowOpenFileDialog(&Engine::getFilePathURL, &_file_dialog_result,
                parent_window ? parent_window->self() : nullptr, m_filters.data(), m_filters.size(),
                default_path.data(), false);
    }

    void Engine::openDirectoryDialog(const std::string &default_path, Window *parent_window) {
        SDL_ShowOpenFolderDialog(&Engine::getFileDirectoryURL, &_file_dialog_result,
            parent_window ? parent_window->self() : nullptr, default_path.data(), false);
    }

    void Engine::saveFileDialog(const StringList &filters, const std::string &default_path,
                                Window *parent_window) {
        std::vector<SDL_DialogFileFilter> m_filters;
        struct TempFilter {
            std::string name;
            std::string pattern;
        };
        std::vector<TempFilter> _temp_filters;
        for (size_t i = 0; i < filters.size(); ++i) {
            std::string name, pattern;
            getFileFilter(filters[i], name, pattern);
            _temp_filters.emplace_back(name, pattern);
        }
        for (auto& filter : _temp_filters) {
            m_filters.emplace_back(filter.name.data(), filter.pattern.data());
        }
        SDL_ShowSaveFileDialog(&Engine::getFilePathURL, &_file_dialog_result,
                parent_window ? parent_window->self() : nullptr, m_filters.data(), m_filters.size(),
                default_path.data());
    }

    std::string Engine::getFileDialogURL(bool* is_user_cancelled, bool* is_finished) {
        if (!_file_dialog_result.is_finished) {
            if (is_user_cancelled) *is_user_cancelled = true;
            if (is_finished) *is_finished = _file_dialog_result.is_finished;
            return {};
        }
        auto url = _file_dialog_result.url;
        if (is_user_cancelled) *is_user_cancelled = _file_dialog_result.user_cancelled;
        if (is_finished) *is_finished = _file_dialog_result.is_finished;
        _file_dialog_result.is_finished = false;
        _file_dialog_result.url.clear();
        return url;
    }

    void Engine::cleanUp() {
        if (_clean_up_event) {
            _clean_up_event();
        }
        for (auto& win : _window_list) {
            win.second.reset();
        }
        _window_list.clear();
        TextSystem::global()->unload();
        AudioSystem::global()->unload();
        // Clear all events. [p.s: Only exec while Engine doing clean up]
        EventSystem::global()->_event_list.clear();
        EventSystem::global()->_global_event_list.clear();
        SDL_Quit();
        if (_running) _running = false;
        Logger::log("Engine: Clean up finished!");
    }

    void Engine::running() {
        auto start_time = SDL_GetTicks();
        auto frames = 0U;
        auto start_ns = SDL_GetTicksNS();
        while (_running && !_quit_requested) {
            /// Event processing and rendering processing
            _running = EventSystem::global(this)->run();
            if (!_running) break;
            auto current_time = SDL_GetTicks();
            auto current_ns = SDL_GetTicksNS();
            if ((double)(current_ns - start_ns) >= _frame_in_ns) {
                for (auto& win : _window_list) {
                    if (!win.second->isFreeze()) win.second->renderer()->_update();
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
                        throw EngineException("The current available system memory is less than 5%. "
                                              "The engine has crashed.");
                    } else if (av_per <= 0.15f) {
                        Logger::log("Engine: The available memory space of the system is less than 15%. "
                                    "If it falls below 5%, the engine will be crashed!", Logger::Warn);
                    }
                }
                /// Update the render frame
                _real_fps = frames;
                if (_fps > 14 && _real_fps <= 14) {
                    Logger::log(Logger::Warn, "Engine: Low FPS detected: {} FPS, try to use `Engine::setFPS()` to limit the maximum frames in a second", _real_fps);
                }
                frames = 0;
                start_time = SDL_GetTicks();
            }
        }
    }

    std::string Engine::copeWithFatalError(bool* ok) {
        std::string get_err_info = Logger::lastError();
        if (get_err_info.empty()) {
            Logger::log("No error found. It will not throw the fatal error!", Logger::Debug);
            if (ok) *ok = false;
            return "No error found, but it still throw the fatal error!";
        }
        std::string err = FMT::format("An error has caused the entire program to crash.\nException: {}",
                                      get_err_info);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FATAL ERROR", err.c_str(), nullptr);
        Logger::log(err, Logger::Fatal);
        if (ok) *ok = true;
        return err;
    }

    void Engine::getFileDirectoryURL(void *userdata, const char * const *filelist, int filter) {
        auto res = static_cast<FileDialogResult*>(userdata);
        if (!filelist || !*filelist) {
            res->user_cancelled = true;
        } else {
            res->url.assign(filelist[0]);
            res->user_cancelled = false;
        }
        res->is_finished = true;
    }

    void Engine::getFilePathURL(void *userdata, const char * const *filelist, int filter) {
        auto res = static_cast<FileDialogResult*>(userdata);
        if (!filelist || !*filelist) {
            res->user_cancelled = true;
        } else {
            res->url.assign(filelist[0]);
            res->user_cancelled = false;
        }
        res->is_finished = true;
    }

    void Engine::getFileFilter(const std::string &str, std::string &name, std::string &pattern) {
        // For example: All files(*.*)  -> "All files" : (*)
        //              Image file(*.png;*.jpg;*.gif) -> "Image file" : (png;jpg;gif)

        auto left_braket_pos = str.find_first_of('(');
        auto right_braket_pos = str.find_last_of(')');
        if (left_braket_pos == std::string::npos || right_braket_pos == std::string::npos) {
            name = str;
            pattern = "";
            return;
        }

        name = str.substr(0, left_braket_pos);
        auto f_pattern = str.substr(left_braket_pos + 1, right_braket_pos - left_braket_pos - 1);
        size_t first_pos = 0;
        while (true) {
            first_pos = f_pattern.find("*.", first_pos);
            if (first_pos == std::string::npos) break;
            f_pattern = f_pattern.replace(first_pos, 2, "");
        }
        pattern = f_pattern;
    }

    TextSystem::TextSystem() {
        load();
    }

    bool TextSystem::isLoaded() const {
        return _is_loaded;
    }

    void TextSystem::load() {
        if (!TTF_Init()) {
            Logger::log(Logger::Fatal, "TextSystem: Failed to load Text System! "
                                       "Exception: {}", SDL_GetError());
            return;
        }
        Logger::log("TextSystem: Loaded text system");
        _is_loaded = true;
    }

    void TextSystem::unload() {
        if (!_is_loaded) return;
        std::for_each(_text_map.begin(), _text_map.end(), [](auto& text) {
            TTF_DestroyText(text.second.self);
        });
        std::for_each(_font_map.begin(), _font_map.end(), [](auto& font) {
            TTF_DestroyRendererTextEngine(font.second.engine);
            TTF_DestroySurfaceTextEngine(font.second.surface_engine);
            font.second.font.reset();
        });
        TTF_Quit();
        _is_loaded = false;
        Logger::log("TextSystem: Unloaded text system");
    }

    bool TextSystem::addFont(const std::string& font_name, const std::string& font_path, Renderer* renderer,
                             float font_size) {
        if (_font_map.contains(font_name)) {
            Logger::log(Logger::Error, "TextSystem: Font '{}' is already added!", font_name);
            return false;
        }
        if (!renderer) {
            Logger::log(Logger::Error, "TextSystem: Can't add font! The specified renderer is not valid!");
            return false;
        }
        _font_map.emplace(font_name, 
                FontEngine{TTF_CreateRendererTextEngine(renderer->self()),
                           TTF_CreateSurfaceTextEngine(),
                           std::make_unique<Font>(font_path, font_size)});
        auto& new_font = _font_map.at(font_name);
        if (!new_font.font->self()) {
            TTF_DestroyRendererTextEngine(new_font.engine);
            TTF_DestroySurfaceTextEngine(new_font.surface_engine);
            _font_map.erase(font_name);
            Logger::log(Logger::Error, "TextSystem: Can't load font '{}'! Exception: {}", font_name, SDL_GetError());
            return false;
        }
        return true;
    }

    bool TextSystem::removeFont(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            Logger::log(Logger::Error, "TextSystem: Font '{}' is not in the font list!", font_name);
            return false;
        }
        TTF_DestroyRendererTextEngine(_font_map[font_name].engine);
        TTF_DestroySurfaceTextEngine(_font_map[font_name].surface_engine);
        _font_map.erase(font_name);
        return true;
    }

    Font* TextSystem::font(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            auto err = FMT::format("TextSystem: Font '{}' is not in the font list!", font_name);
            Logger::log(err, Logger::Fatal);
            Engine::throwCustomFatalError<NullPointerException>();
        }
        return _font_map.at(font_name).font.get();
    }

    bool TextSystem::isFontContain(const std::string& font_name) const {
        return _font_map.contains(font_name);
    }

    StringList TextSystem::fontNameList() const {
        StringList font_list(_font_map.size());
        std::for_each(_font_map.begin(), _font_map.end(), [&font_list](const auto& font) {
            font_list.push_back(font.first);
        });
        return font_list;
    }

    bool TextSystem::setFontSize(const std::string &font_name, float font_size) {
        if (!_font_map.contains(font_name)) {
            auto err = FMT::format("TextSystem: Font '{}' is not in the font list!", font_name);
            Logger::log(err, Logger::Error);
            return false;
        }
        _font_map[font_name].font->setFontSize(font_size);
        for (auto& [id, text] : _text_map) {
            if (text.font_name == font_name) {
                int w = 0, h = 0;
                TTF_GetTextSize(text.self, &w, &h);
                text.text_size.reset((float)w, (float)h);
            }
        }
        return true;
    }

    bool TextSystem::addText(uint64_t text_id, const std::string& font_name, const std::string& text) {
        if (_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is already added to text list!", text_id);
            return false;
        }
        if (!_font_map.contains(font_name)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} can not add the font '{}'!", text_id, font_name);
            return false;
        }
        _text_map.emplace(text_id, Text(nullptr, text, font_name));
        _text_map[text_id].self = TTF_CreateText(_font_map[font_name].engine,
                                                 _font_map[font_name].font->self(),
                                                 text.c_str(),
                                                 text.size());
        if (!_text_map[text_id].self) {
            Logger::log(Logger::Error, "TextSystem: Can't create text! Exception: {}", SDL_GetError());
            _text_map.erase(text_id);
            return false;
        }
        int w = 0, h = 0;
        TTF_GetTextSize(_text_map[text_id].self, &w, &h);
        _text_map[text_id].text_size.reset(static_cast<float>(w), static_cast<float>(h));
        return true;
    }

    bool TextSystem::removeText(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is not in the text list!", text_id);
            return false;
        }
        _text_map.erase(text_id);
        return true;
    }

    bool TextSystem::setText(uint64_t text_id, const std::string& text) {
        if (!_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is not in the text list!", text_id);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(Logger::Error, "TextSystem: Can't set text to text ID {}! Exception: {}", text_id, SDL_GetError());
            return false;
        }
        m_text.text = text;
        int w = 0, h = 0;
        TTF_GetTextSize(_text_map[text_id].self, &w, &h);
        _text_map[text_id].text_size.reset(static_cast<float>(w), static_cast<float>(h));
        return true;
    }

    bool TextSystem::appendText(uint64_t text_id, const std::string& text) {
        if (!_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is not in the text list!", text_id);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_AppendTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(Logger::Error, "TextSystem: Can't set text to text ID {}! Exception: {}", text_id, SDL_GetError());
            return false;
        }
        m_text.text += text;
        int w = 0, h = 0;
        TTF_GetTextSize(_text_map[text_id].self, &w, &h);
        _text_map[text_id].text_size.reset(static_cast<float>(w), static_cast<float>(h));
        return true;
    }

    bool TextSystem::setTextFont(uint64_t text_id, const std::string& font_name) {
        if (!_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is not in the text list!", text_id);
            return false;
        }
        if (!_font_map.contains(font_name)) {
            Logger::log(Logger::Error, "TextSystem: Font '{}' is not in the font list!", font_name);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextFont(m_text.self, _font_map[font_name].font->self());
        if (!_ret) {
            Logger::log(Logger::Error, "TextSystem: Can't set font '{}' to text ID {}! Exception: {}", font_name, text_id, SDL_GetError());
            return false;
        }
        m_text.font_name = font_name;
        int w = 0, h = 0;
        TTF_GetTextSize(_text_map[text_id].self, &w, &h);
        _text_map[text_id].text_size.reset(static_cast<float>(w), static_cast<float>(h));
        return true;
    }

    bool TextSystem::setTextColor(uint64_t text_id, const SDL_Color& color) {
        if (!_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is not in the text list!", text_id);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextColor(m_text.self, color.r, color.g, color.b, color.a);
        if (!_ret) {
            Logger::log(Logger::Error, "TextSystem: Can't set font color to text ID {}! Exception: {}", text_id, SDL_GetError());
            return false;
        }
        m_text.font_color = color;
        return true;
    }

    TextSystem::Text* TextSystem::indexOfText(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            auto err = FMT::format("TextSystem: Text ID {} is not in the text list!", text_id);
            Logger::log(err, Logger::Fatal);
            Engine::throwCustomFatalError<NullPointerException>();
        }
        return &_text_map.at(text_id);
    }

    bool TextSystem::isTextContain(uint64_t text_id) const {
        return _text_map.contains(text_id);
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
            Logger::log("TextSystem: The specified renderer is not valid!", Logger::Error);
            return false;
        }
        if (!_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is not in the text list!", text_id);
            return false;
        }
        auto temp_pos = pos;
        renderer->drawText(_text_map[text_id].self, temp_pos);
        return true;
    }

    bool TextSystem::updateFont(const std::string &font_name) {
        if (!_font_map.contains(font_name)) {
            auto err = FMT::format("TextSystem: Font '{}' is not in the font list!", font_name);
            Logger::log(err, Logger::Error);
            return false;
        }
        for (auto& [id, text] : _text_map) {
            if (text.font_name == font_name) {
                int w = 0, h = 0;
                TTF_GetTextSize(text.self, &w, &h);
                text.text_size.reset((float)w, (float)h);
            }
        }
        return true;
    }

    SDL_Surface* TextSystem::toImage(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} is not in the text list!", text_id);
            return nullptr;
        }
        if (!_font_map.contains(_text_map[text_id].font_name)) {
            Logger::log(Logger::Error, "TextSystem: Text ID {} has not set the font! Please use `setTextFont()` to set the font!", text_id);
            return nullptr;
        }
        auto& font_engine = _font_map[_text_map[text_id].font_name];
        auto& text =  _text_map[text_id].text;
        TTF_Text* temp_text = TTF_CreateText(font_engine.surface_engine, font_engine.font->self(), text.c_str(), text.size());
        auto t_surface = font_engine.font->toImage(text);
        SDL_Surface* surface = SDL_CreateSurface(t_surface->w, t_surface->h, SDL_PIXELFORMAT_RGBA8888);
        SDL_DestroySurface(t_surface);
        auto _ret = TTF_DrawSurfaceText(temp_text, 0, 0, surface);
        if (!_ret) {
            Logger::log(Logger::Error, "TextSystem: Text to image failed! Exception: {}", SDL_GetError());
            TTF_DestroyText(temp_text);
            return nullptr;
        }
        TTF_DestroyText(temp_text);
        return surface;
    }
    
    AudioSystem::AudioSystem() {
        _is_init = load();
    }

    bool AudioSystem::isValid() const { return _is_init; }
 
    bool AudioSystem::load() {
        if (!MIX_Init()) {
            Logger::log(Logger::Error, "AudioSystem: Can't initialized audio system! Exception: {}", SDL_GetError());
            return false;
        }
        _is_init = true;
        SDL_AudioSpec _audio_spec;
        SDL_GetAudioDeviceFormat(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec, nullptr);
        auto new_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
        if (!new_mixer) {
            Logger::log(Logger::Error, "AudioSystem: Can't initialized audio system! Exception: {}", SDL_GetError());
            return false;
        }
        _mixer_list.push_back(new_mixer);
        Logger::log("AudioSystem: Loaded audio system!");
        return true;
    }

    void AudioSystem::unload() {
        _audio_map.clear();
        for (auto& mixer : _mixer_list) {
            if (mixer) MIX_DestroyMixer(mixer);
        }
        _mixer_list.clear();
        if (!_is_init) return;
        MIX_Quit();
        Logger::log("AudioSystem: Unloaded audio system!");
        _is_init = false;
    }

    bool AudioSystem::addNewMixer(size_t count, SDL_AudioDeviceID device_id) {
        if (!_is_init) {
            Logger::log("AudioSystem: The audio system is not initialized!", Logger::Fatal);
            Engine::throwFatalError();
            return false;
        }
        auto audio_dev_id = (device_id == 0 ? SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK : device_id);
        if (!SDL_IsAudioDevicePlayback(audio_dev_id)) {
            Logger::log("AudioSystem: The specified audio device ID is not the playback device!", Logger::Fatal);
            Engine::throwCustomFatalError<NullPointerException>();
        }
        while (count--) {
            SDL_AudioSpec _audio_spec;
            SDL_GetAudioDeviceFormat(audio_dev_id, &_audio_spec, nullptr);
            auto new_mixer = MIX_CreateMixerDevice(audio_dev_id, &_audio_spec);
            if (!new_mixer) {
                Logger::log(Logger::Error, "AudioSystem: Can't create the new mixer device! "
                                           "Exception: {}", SDL_GetError());
                return false;
            } else {
                _mixer_list.push_back(new_mixer);
            }
        }
        return true;
    }

    MIX_Mixer *AudioSystem::mixer(size_t index) const {
        if (_mixer_list.size() <= index) {
            auto err = FMT::format("AudioSystem: Mixer #{} is not valid! "
                                   "Did you forget to call `AudioSystem::addNewMixer()` function?", index);
            Logger::log(err, Logger::Fatal);
            Engine::throwCustomFatalError<NullPointerException>();
        }
        return _mixer_list.at(index);
    }

    size_t AudioSystem::mixerCount() const {
        return _mixer_list.size();
    }

    void AudioSystem::appendBGM(const std::string &name, const std::string &path, size_t mixer_index) {
        if (!_audio_map.contains(name)) {
            _audio_map.emplace(name, std::make_unique<BGM>(_mixer_list[mixer_index], path));
            if (std::get<std::unique_ptr<BGM>>(_audio_map.at(name))->isLoaded()) {
                Logger::log(Logger::Debug, "AudioSystem: Loaded BGM from path '{}' to Mixer #{}.", path, mixer_index);
            } else {
                Logger::log(Logger::Error, "AudioSystem: Load BGM from path '{}' to Mixer #{} failed!", path, mixer_index);
            }
        } else {
            Logger::log(Logger::Warn, "AudioSystem: Append BGM failed! The name of '{}' is already exist!", name);
        }
    }

    void AudioSystem::appendSFX(const std::string &name, const std::string &path, size_t mixer_index) {
        if (!_audio_map.contains(name)) {
            _audio_map.emplace(name, std::make_unique<SFX>(_mixer_list[mixer_index], path));
            if (std::get<std::unique_ptr<SFX>>(_audio_map.at(name))->isLoaded()) {
                Logger::log(Logger::Debug, "AudioSystem: Loaded SFX from path '{}' to Mixer #{}.", path, mixer_index);
            } else {
                Logger::log(Logger::Error, "AudioSystem: Load SFX from path '{}' to Mixer #{} failed!", path, mixer_index);
            }
        } else {
            Logger::log(Logger::Warn, "AudioSystem: Append SFX failed! The name of '{}' is already exist!", name);
        }
    }

    void AudioSystem::remove(const std::string &name) {
        if (_audio_map.contains(name)) {
            _audio_map.erase(name);
            Logger::log(Logger::Debug, "AudioSystem: Removed audio '{}'!", name);
        }
    }

    BGM* AudioSystem::getBGM(const std::string &name) {
        if (_audio_map.contains(name)) {
            if (std::holds_alternative<std::unique_ptr<BGM>>(_audio_map.at(name))) {
                return std::get<std::unique_ptr<BGM>>(_audio_map.at(name)).get();
            } else {
                auto err = FMT::format("AudioSystem: Audio '{}' is not the BGM type! ", name);
                Logger::log(err, Logger::Fatal);
                Engine::throwFatalError();
            }
        } else {
            auto err = FMT::format("AudioSystem: Audio '{}' is not exist! ", name);
            Logger::log(err, Logger::Fatal);
            Engine::throwFatalError();
        }
        return nullptr;
    }

    SFX *AudioSystem::getSFX(const std::string &name) {
        if (_audio_map.contains(name)) {
            if (std::holds_alternative<std::unique_ptr<SFX>>(_audio_map.at(name))) {
                return std::get<std::unique_ptr<SFX>>(_audio_map.at(name)).get();
            } else {
                auto err = FMT::format("AudioSystem: Audio '{}' is not the SFX type! ", name);
                Logger::log(err, Logger::Fatal);
                Engine::throwFatalError();
            }
        } else {
            auto err = FMT::format("AudioSystem: Audio '{}' is not exist! ", name);
            Logger::log(err, Logger::Fatal);
            Engine::throwFatalError();
        }
        return nullptr;
    }

    bool AudioSystem::isBGM(const std::string &name) const {
        return (_audio_map.contains(name) && std::holds_alternative<std::unique_ptr<BGM>>(_audio_map.at(name)));
    }

    bool AudioSystem::isSFX(const std::string &name) const {
        return (_audio_map.contains(name) && std::holds_alternative<std::unique_ptr<SFX>>(_audio_map.at(name)));
    }

    bool AudioSystem::isAudio(const std::string &name) const {
        return _audio_map.contains(name);
    }

    StringList AudioSystem::audioNamesList() const {
        StringList _ret;
        for (auto& name : _audio_map) {
            _ret.push_back(name.first);
        }
        return _ret;
    }

    size_t AudioSystem::size() const {
        return _audio_map.size();
    }

    void AudioSystem::setMixerVolume(float volume, size_t mixer_index) {
        if (mixer_index >= _mixer_list.size()) {
            Logger::log(Logger::Error, "AudioSystem: Mixer #{} is not valid! "
                                       "Did you forget to called `AudioSystem::addNewMixer()`", mixer_index);
            return;
        }

#if SDL_MIXER_MAJOR_VERSION < 3
        MIX_SetMasterGain(_mixer_list[mixer_index], volume);
#else
        MIX_SetMixerGain(_mixer_list[mixer_index], volume);
#endif
    }
    float AudioSystem::mixerVolume(size_t mixer_index) {
        if (mixer_index >= _mixer_list.size()) {
            Logger::log(Logger::Error, "AudioSystem: Mixer #{} is not valid! Did you forget to called `AudioSystem::addNewMixer()`", mixer_index);
            return 0.f;
        }

#if SDL_MIXER_MAJOR_VERSION < 3
        return MIX_GetMasterGain(_mixer_list[mixer_index]);
#else
        return MIX_GetMixerGain(_mixer_list[mixer_index]);
#endif
    }

    void AudioSystem::stopAll() {
        for (auto& [name, audio] : _audio_map) {
            if (std::holds_alternative<std::unique_ptr<BGM>>(audio)) {
                std::get<std::unique_ptr<BGM>>(audio)->stop();
            } else if (std::holds_alternative<std::unique_ptr<SFX>>(audio)) {
                std::get<std::unique_ptr<SFX>>(audio)->resetAll();
            }
        }
    }

    void AudioSystem::forcedStopAll() {
        for (auto& mixer : _mixer_list) {
            if (!MIX_StopAllTracks(mixer, 0)) {
                Logger::log(Logger::Warn, "AudioSystem: Failed to stop mixer at (0x{:x}). Exception: {}",
                    (size_t)mixer, SDL_GetError());
            }
        }
    }

    size_t AudioSystem::addAudioRecorder(AudioRecorder *recorder) {
        if (!recorder) return SIZE_MAX;
        _recoder_list.emplace_back(std::unique_ptr<AudioRecorder>(recorder));
        return _recoder_list.size() - 1;
    }

    void AudioSystem::removeAudioRecorder(size_t index) {
        if (index >= _recoder_list.size()) {
            Logger::log(Logger::Error, "AudioSystem: The index of {} is not exist in recorder list.", index);
            return;
        }
        _recoder_list.erase(_recoder_list.begin() + index);
    }

    AudioRecorder *AudioSystem::audioRecoder(size_t index) {
        if (index >= _recoder_list.size()) {
            Logger::log(Logger::Fatal, "AudioSystem: The index of {} is not exist "
                                       "in audio recorder list!", index);
            Engine::throwCustomFatalError<NullPointerException>();
        }
        return _recoder_list.at(index).get();
    }

    size_t AudioSystem::audioRecorderCount() const {
        return _recoder_list.size();
    }
}
