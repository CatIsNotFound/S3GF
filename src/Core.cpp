
#include "Core.h"
#include "Basic.h"
#include "Utils/All.h"
#include "Renderer/BaseCommand.h"
#include "Renderer/CommandFactory.h"

namespace MyEngine {
    std::unique_ptr<EventSystem> EventSystem::_instance{};
    SDL_Color Renderer::_background_color{RGBAColor::White};
    SDL_WindowID Engine::_main_window_id{0};
    bool Engine::_quit_requested{false};
    int Engine::_return_code{0};
    bool Engine::_show_app_info{true};
    bool FontDatabase::_is_loaded{false};
    FontMap FontDatabase::_font_db{};
    std::vector<FontDatabase::FontInfo> FontDatabase::_def_fonts{};

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
        if (!_cmd_list.empty()) {
            for (auto& cmd : _cmd_list) {
                RenderCommand::CommandFactory::release(std::move(cmd));
            }
        }
        if (_renderer) SDL_DestroyRenderer(_renderer);
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
                    std::format("FPS: {}", window()->_engine->fps()), position, color);
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

    Window::Window(Engine* object, const std::string& title, int width, int height,  GraphicEngine engine)
        : _title(title), _window_geometry(0, 0, width, height),
          _visible(true), _resizable(false), _engine(object) {
        if (engine == Vulkan)
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
        if (_win_icon) SDL_DestroySurface(_win_icon);
        if (_window) {
            SDL_DestroyWindow(_window);
            Logger::log(std::format("Window: ID {} destroyed",  _winID), Logger::Debug);
        }
    }

    bool Window::move(int x, int y) {
        bool _ret = SDL_SetWindowPosition(_window, x, y);
        if (!_ret) {
            Logger::log(std::format("Window: Can't move window! Exception: {}", SDL_GetError()),
                        Logger::Error);
            return false;
        }
        _window_geometry.x = x;
        _window_geometry.y = y;
        return true;
    }

    bool Window::resize(int width, int height) {
        bool _ret = SDL_SetWindowSize(_window, width, height);
        if (!_ret) {
            Logger::log(std::format("Window: Can't reshape window! Exception: {}", SDL_GetError()),
                        Logger::Error);
            return false;
        }
        _window_geometry.width = width;
        _window_geometry.height = height;
        return true;
    }

    bool Window::setMinimumSize(int width, int height) {
        bool _ret = SDL_SetWindowMinimumSize(_window, width, height);
        if (!_ret) {
            Logger::log(std::format("Window: Can't reshape window! "
                                    "Exception: {}", SDL_GetError()), Logger::Error);
            return false;
        }
        return true;
    }

    bool Window::setMaximumSize(int width, int height) {
        bool _ret = SDL_SetWindowMaximumSize(_window, width, height);
        if (!_ret) {
            Logger::log(std::format("Window: Can't reshape window! "
                                    "Exception: {}", SDL_GetError()), Logger::Error);
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
        if (!_renderer) _renderer = std::shared_ptr<Renderer>(renderer);
    }

    Renderer* Window::renderer() const {
        return _renderer.get();
    }

    void Window::setBorderless(bool enabled) {
        bool _ok = SDL_SetWindowBordered(_window, !enabled);
        if (_ok) _borderless = enabled;
        else Logger::log(std::format("Window (ID {}): Can't set borderless for this window! "
                                     "Exception: {}", _winID, SDL_GetError()),
                         Logger::Error);
    }

    bool Window::borderless() const {
        return _borderless;
    }

    void Window::setWindowOpacity(float opacity) {
        if (opacity < 0.f || opacity > 1.f) return;
        bool _ok = SDL_SetWindowOpacity(_window, opacity);
        if (!_ok) {
            Logger::log(std::format("Window (ID {}): Can't set window opacity for this window! "
                                    "Exception: {}", _winID, SDL_GetError()),
                        Logger::Error);
        }
    }

    float Window::windowOpacity() const {
        float ret = SDL_GetWindowOpacity(_window);
        if (ret == -1.0f) {
            Logger::log(std::format("Window (ID {}): Can't get window opacity for this window! "
                                    "Exception: {}", _winID, SDL_GetError()),
                        Logger::Error);
        }
        return ret;
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

    void Window::setWindowIcon(const std::string& icon_path) {
        _win_icon = IMG_Load(icon_path.c_str());
        if (!_win_icon) {
            Logger::log(std::format("Window (ID {}): Can't set icon for this window!"
                                    "Exception: {}", _winID, SDL_GetError()),
                        Logger::Error);
        }
        bool ok = SDL_SetWindowIcon(_window, _win_icon);
        if (!ok) {
            Logger::log(std::format("Window (ID {}): Can't set icon for this window!"
                                    "Exception: {}", _winID, SDL_GetError()),
                        Logger::Error);
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

    void Window::droppedInfo(char *url, std::string& source) const {
        if (url)    strncpy(url, _drop_url, strlen(_drop_url));
        source.assign(_drop_source.begin(), _drop_source.end());
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
            Logger::log(std::format("Window: Failed to get window size for ID {}!", _winID),
                        Logger::Warn);
        }
    }

    void Window::moveEvent() {
        auto _ret = SDL_GetWindowPosition(_window, &_window_geometry.x, &_window_geometry.y);
        if (!_ret) {
            Logger::log(std::format("Window: Failed to get window position for ID {}!", _winID),
                        Logger::Warn);
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

    void Window::showEvent() {}

    void Window::hideEvent() {}

    void Window::windowMinimizedEvent() {}

    void Window::windowMaximizedEvent() {}

    void Window::enteredFullscreenEvent() {}

    void Window::leaveFullscreenEvent() {}

    void Window::mouseEnteredEvent() { Cursor::global()->setCursor(_cursor); }
    void Window::mouseLeftEvent() {}

    void Window::mouseUpEvent() {}
    void Window::mouseDownEvent(int) {}
    void Window::mouseClickedEvent(int) {}
    void Window::mouseMovedEvent(const MyEngine::Vector2 &) {}

    void Window::keyUpEvent(int) {}
    void Window::keyDownEvent(int) {}
    void Window::keyPressedEvent(int) {}
    void Window::dragInEvent() {}
    void Window::dragOutEvent() {}
    void Window::dragMovedEvent(const Vector2 &position, const char *url) {}
    void Window::dropEvent(const char *url, const char *source) {}

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
            // _event_list.erase(id);
            _del_event_deque.push_back(id);
            Logger::log(std::format("EventSystem: Removed the event with ID {}", id));
        } else {
            Logger::log(std::format("EventSystem: The event with ID {} is not found!", id), Logger::Warn);
        }
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
            // _global_event_list.erase(g_id);
            _del_g_event_deque.push_back(g_id);
            Logger::log(std::format("EventSystem: Removed a global event with ID {}", g_id));
        } else {
            Logger::log(std::format("EventSystem: The global event with ID {} is not found!", g_id),
                        Logger::Warn);
        }
    }

    size_t EventSystem::eventCount() const { return _event_list.size(); }

    bool EventSystem::run() {
        SDL_Event ev;
        if (SDL_PollEvent(&ev)) {
            _kb_events = const_cast<bool*>(SDL_GetKeyboardState(&_nums_keys));
            _keys_status.clear();
            for (int i = 0; i < _nums_keys; ++i) {
                if (_kb_events[i]) _keys_status.emplace_back(i);
            }

            _mouse_events = SDL_GetMouseState(&_mouse_pos.x, &_mouse_pos.y);
            if (!_mouse_down_changed) {
                // When any of mouse buttons is pressed down, triggered...
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
            if (!_engine->windowCount()) return false; // Exit engine when no windows loaded.
            auto win_id_list = _engine->windowIDList();
            static bool mouse_down = false, key_down = false;
            std::for_each(win_id_list.begin(), win_id_list.end(), [this, &ev](uint32_t id) {
                auto win = _engine->window(id);
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
                    return;
                } else if (ev.window.type == SDL_EVENT_WINDOW_HIDDEN) {
                    win->hideEvent();
                } else if (ev.window.type == SDL_EVENT_WINDOW_SHOWN) {
                    win->showEvent();
                } else if (ev.window.type == SDL_EVENT_WINDOW_MINIMIZED) {
                    win->windowMinimizedEvent();
                } else if (ev.window.type == SDL_EVENT_WINDOW_MAXIMIZED) {
                    win->windowMaximizedEvent();
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
                if (!mouse_down) {
                    if (_mouse_events > 0) {
                        win->mouseDownEvent(static_cast<int>(_mouse_events));
                        mouse_down = true;
                    }
                } else {
                    if (_mouse_events > 0) {
                        win->mouseMovedEvent(_mouse_down_dis);
                    } else {
                        win->mouseUpEvent();
                        win->mouseClickedEvent(static_cast<int>(_mouse_events));
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
                        memset(win->_drop_url, 0, 255);
                        win->_drop_source.clear();
                    }
                } else {
                    if (ev.drop.type == SDL_EVENT_DROP_COMPLETE) {
                        win->dragOutEvent();
                        win->_dragging_pos.reset(0, 0);
                        win->_dragging = false;
                        win->_drop_source.clear();
                        memset(win->_drop_url, 0, 255);
                    } else if (ev.drop.type == SDL_EVENT_DROP_FILE) {
                        win->dropEvent(ev.drop.data, ev.drop.source);
                        strncpy(win->_drop_url, ev.drop.data, 128);
                        if (ev.drop.source) {
                            win->_drop_source.resize(strlen(ev.drop.source));
                            strncpy(win->_drop_source.data(), ev.drop.source, strlen(ev.drop.source));
                        }
                        win->_dragging_pos.reset(0, 0);
                        win->_dragging = false;
                    } else if (ev.drop.type == SDL_EVENT_DROP_TEXT) {
                        win->dropEvent(ev.drop.data, nullptr);
                        strncpy(win->_drop_url, ev.drop.data, 128);
                        if (ev.drop.source) {
                            win->_drop_source.resize(strlen(ev.drop.source));
                            strncpy(win->_drop_source.data(), ev.drop.source, strlen(ev.drop.source));
                        }
                        win->_dragging = false;
                    } else {
                        auto real_pos = Cursor::global()->globalPosition() - toGeometryFloat(win->geometry()).pos;
                        win->_dragging_pos.reset(real_pos);
                        win->dragMovedEvent(real_pos, ev.drop.source);
                    }
                }
            });

            for (auto& event : _event_list) {
                if (event.second) event.second(ev);
            }
            for (auto& id : _del_event_deque) {
                _event_list.erase(id);
            }
        }
        for (auto& e : _global_event_list) {
            if (e.second) e.second();
        }
        for (auto& id : _del_g_event_deque) {
            _global_event_list.erase(id);
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
        if (mouse_status == Left) {
            return _mouse_events % 2;
        } else if (mouse_status == Right) {
            return _mouse_events >= Right;
        } else if (mouse_status == Middle) {
            return !(_mouse_events % 3) || (_mouse_events == LeftMiddleRight);
        }
        return _mouse_events == mouse_status;
    }

    const Vector2& EventSystem::captureMouseAbsDistance() const {
        return _mouse_down_dis;
    }

    const Vector2& EventSystem::captureMousePosition() const {
        return _mouse_pos;
    }

    const std::vector<int>& EventSystem::captureKeyboardStatus() const {
        return _keys_status;
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
            std::cout << std::format("========== Application Info ==========\n"
                                     "ID: {} \nName: {} \nVersion: {} \n",
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
        if (_window_list.contains(id)) {
            return _window_list.at(id).get();
        } else {
            auto err = std::format("Engine: Window id {} is not created!", id);
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
        }
    }

    std::vector<uint32_t> Engine::windowIDList() const {
        std::vector<uint32_t> id_list;
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
        std::string err = std::format("An error has caused the entire program to crash.\nException: {}",
                                      get_err_info);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FATAL ERROR", err.c_str(), nullptr);
        Logger::log(err, Logger::Fatal);
        throw EngineException(err);
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
                    Logger::log(std::format("Engine: Low FPS detected: {} FPS, "
                            "try to use `Engine::setFPS()` to limit the maximum frames in a second", _real_fps),
                                Logger::Warn);
                }
                frames = 0;
                start_time = SDL_GetTicks();
            }
        }
    }

    TextSystem::TextSystem() {
        if (!TTF_Init()) {
            Logger::log(std::format("TextSystem: Can't load Text System! "
                                    "Exception: {}", SDL_GetError()), Logger::Fatal);
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

    bool TextSystem::addFont(const std::string& font_name, const std::string& font_path, Renderer* renderer,
                             float font_size) {
        if (_font_map.contains(font_name)) {
            Logger::log(std::format("TextSystem: Font '{}' is already added!",
                                    font_name), Logger::Error);
            return false;
        }
        if (!renderer) {
            Logger::log("TextSystem: Can't add font! "
                        "The specified renderer is not valid!", Logger::Error);
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
            Logger::log(std::format("TextSystem: Can't load font '{}'! "
                                    "Exception: {}", font_name, SDL_GetError()), Logger::Error);
            return false;
        }
        return true;
    }

    bool TextSystem::removeFont(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("TextSystem: Font '{}' is not in the font list!",
                                    font_name), Logger::Error);
            return false;
        }
        TTF_DestroyRendererTextEngine(_font_map[font_name].engine);
        TTF_DestroySurfaceTextEngine(_font_map[font_name].surface_engine);
        _font_map.erase(font_name);
        return true;
    }

    Font* TextSystem::font(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            auto err = std::format("TextSystem: Font '{}' is not in the font list!", font_name);
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
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
            auto err = std::format("TextSystem: Font '{}' is not in the font list!", font_name);
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
            Logger::log(std::format("TextSystem: Text ID {} is already added to text list!",
                                    text_id), Logger::Error);
            return false;
        }
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("TextSystem: Text ID {} can not add the font '{}'!",
                                    text_id, font_name), Logger::Error);
            return false;
        }
        _text_map.emplace(text_id, Text(nullptr, text, font_name));
        _text_map[text_id].self = TTF_CreateText(_font_map[font_name].engine,
                                                 _font_map[font_name].font->self(),
                                                 text.c_str(),
                                                 text.size());
        if (!_text_map[text_id].self) {
            Logger::log(std::format("TextSystem: Can't create text! "
                                    "Exception: {}", SDL_GetError()), Logger::Error);
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
            Logger::log(std::format("TextSystem: Text ID {} is not in the text list!",
                                    text_id), Logger::Error);
            return false;
        }
        _text_map.erase(text_id);
        return true;
    }

    bool TextSystem::setText(uint64_t text_id, const std::string& text) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("TextSystem: Text ID {} is not in the text list!",
                                    text_id), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(std::format("TextSystem: Can't set text to text ID {}! "
                                    "Exception: {}", text_id, SDL_GetError()), Logger::Error);
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
            Logger::log(std::format("TextSystem: Text ID {} is not in the text list!",
                                    text_id), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_AppendTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(std::format("TextSystem: Can't set text to text ID {}! "
                                    "Exception: {}", text_id, SDL_GetError()), Logger::Error);
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
            Logger::log(std::format("TextSystem: Text ID {} is not in the text list!",
                                    text_id), Logger::Error);
            return false;
        }
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("TextSystem: Font '{}' is not in the font list!",
                                    font_name), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextFont(m_text.self, _font_map[font_name].font->self());
        if (!_ret) {
            Logger::log(std::format("TextSystem: Can't set font '{}' to text ID {}! Exception: {}",
                    font_name, text_id, SDL_GetError()), Logger::Error);
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
            Logger::log(std::format("TextSystem: Text ID {} is not in the text list!",
                                    text_id), Logger::Error);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextColor(m_text.self, color.r, color.g, color.b, color.a);
        if (!_ret) {
            Logger::log(std::format("TextSystem: Can't set font color to text ID {}! Exception: {}",
                    text_id, SDL_GetError()), Logger::Error);
            return false;
        }
        m_text.font_color = color;
        return true;
    }

    TextSystem::Text* TextSystem::indexOfText(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            auto err = std::format("TextSystem: Text ID {} is not in the text list!", text_id);
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
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
            Logger::log(std::format("TextSystem: Text ID {} is not in the text list!",
                                    text_id), Logger::Error);
            return false;
        }
        auto temp_pos = pos;
        renderer->drawText(_text_map[text_id].self, temp_pos);
        return true;
    }

    bool TextSystem::updateFont(const std::string &font_name) {
        if (!_font_map.contains(font_name)) {
            auto err = std::format("TextSystem: Font '{}' is not in the font list!", font_name);
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
            Logger::log(std::format("TextSystem: Text ID {} is not in the text list!",
                                    text_id), Logger::Error);
            return nullptr;
        }
        if (!_font_map.contains(_text_map[text_id].font_name)) {
            Logger::log(std::format("TextSystem: Text ID {} has not set the font! "
                                    "Try to use `setTextFont()` at first!", text_id), Logger::Error);
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
            Logger::log(std::format("TextSystem: Text to image failed! "
                                    "Exception: {}", SDL_GetError()), Logger::Error);
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
            Logger::log(std::format("AudioSystem: Can't initialized audio system! Exception: {}",
                SDL_GetError()), Logger::Error);
            return false;
        }

        SDL_AudioSpec _audio_spec(SDL_AUDIO_S16, 2, 44100);
        auto new_mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
        if (!new_mixer) {
            Logger::log(std::format("AudioSystem: Can't initialized audio system! Exception: {}",
                SDL_GetError()), Logger::Error);
            return false;
        }
        _mixer_list.push_back(new_mixer);
        Logger::log("AudioSystem: Loaded audio system!");
        return true;
    }

    void AudioSystem::unload() {
        if (!_is_init) return;
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
            auto err = std::format("AudioSystem: Mixer #{} is not valid! "
                                   "Did you forget to call `AudioSystem::addNewMixer()` function?", index);
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
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
                Logger::log(std::format("AudioSystem: Loaded BGM from path '{}' to Mixer #{}.", path,
                                        mixer_index));
            } else {
                Logger::log(std::format("AudioSystem: Load BGM from path '{}' to Mixer #{} failed!", path,
                                        mixer_index), Logger::Error);
            }
        } else {
            Logger::log(std::format("AudioSystem: Append BGM failed! "
                                    "The name of '{}' is already exist!", name), Logger::Warn);
        }
    }

    void AudioSystem::appendSFX(const std::string &name, const std::string &path, size_t mixer_index) {
        if (!_audio_map.contains(name)) {
            _audio_map.emplace(name, std::make_unique<SFX>(_mixer_list[mixer_index], path));
            if (std::get<std::unique_ptr<SFX>>(_audio_map.at(name))->isLoaded()) {
                Logger::log(std::format("AudioSystem: Loaded SFX from path '{}' to Mixer #{}.", path,
                                        mixer_index));
            } else {
                Logger::log(std::format("AudioSystem: Load SFX from path '{}' to Mixer #{} failed!", path,
                                        mixer_index), Logger::Error);
            }
        } else {
            Logger::log(std::format("AudioSystem: Append SFX failed! "
                                    "The name of '{}' is already exist!", name), Logger::Warn);
        }
    }

    void AudioSystem::remove(const std::string &name) {
        if (_audio_map.contains(name)) {
            _audio_map.erase(name);
            Logger::log(std::format("AudioSystem: Removed audio '{}'!", name));
        }
    }

    BGM *AudioSystem::getBGM(const std::string &name) {
        if (_audio_map.contains(name)) {
            if (std::holds_alternative<std::unique_ptr<BGM>>(_audio_map.at(name))) {
                return std::get<std::unique_ptr<BGM>>(_audio_map.at(name)).get();
            } else {
                auto err = std::format("AudioSystem: Audio '{}' is not the BGM type! ", name);
                Logger::log(err, Logger::Fatal);
                throw NullPointerException(err);
            }
        } else {
            auto err = std::format("AudioSystem: Audio '{}' is not exist! ", name);
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
        }
    }

    SFX *AudioSystem::getSFX(const std::string &name) {
        if (_audio_map.contains(name)) {
            if (std::holds_alternative<std::unique_ptr<SFX>>(_audio_map.at(name))) {
                return std::get<std::unique_ptr<SFX>>(_audio_map.at(name)).get();
            } else {
                auto err = std::format("AudioSystem: Audio '{}' is not the SFX type! ", name);
                Logger::log(err, Logger::Fatal);
                throw NullPointerException(err);
            }
        } else {
            auto err = std::format("AudioSystem: Audio '{}' is not exist! ", name);
            Logger::log(err, Logger::Fatal);
            throw NullPointerException(err);
        }
    }

    size_t AudioSystem::size() const {
        return _audio_map.size();
    }

    void AudioSystem::setMixerVolume(float volume, size_t mixer_index) {
        if (mixer_index >= _mixer_list.size()) {
            Logger::log(std::format("AudioSystem: Mixer #{} is not valid! "
                        "Did you forget to called `AudioSystem::addNewMixer()`", mixer_index), Logger::Error);
            return;
        }
        MIX_SetMasterGain(_mixer_list[mixer_index], volume);
    }

    float AudioSystem::mixerVolume(size_t mixer_index) {
        if (mixer_index >= _mixer_list.size()) {
            Logger::log(std::format("AudioSystem: Mixer #{} is not valid! "
                        "Did you forget to called `AudioSystem::addNewMixer()`", mixer_index), Logger::Error);
            return 0.f;
        }
        return MIX_GetMasterGain(_mixer_list[mixer_index]);
    }

    void AudioSystem::stopAll() {
        for (auto& [name, audio] : _audio_map) {
            if (audio.index() == 1) {
                std::get<std::unique_ptr<BGM>>(audio)->stop();
            } else if (audio.index() == 2) {
                std::get<std::unique_ptr<SFX>>(audio)->stop();
            }
        }
    }
}