
#include "Core.h"

#include <memory>
#include "Basic.h"
#include "Utils/All"
#include "MultiThread/All"

namespace S3GF {
    std::unique_ptr<EventSystem> EventSystem::_instance{};
    SDL_Color Renderer::_background_color{RGBAColor::Black};
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
            Logger::log("The renderer is not created!", Logger::FATAL);
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

    void Renderer::_update() {
        SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_NONE);
        SDL_SetRenderDrawColor(_renderer, _background_color.r, _background_color.g,
                                _background_color.b, _background_color.a);
        SDL_RenderClear(_renderer);
        for (auto& cmd : _cmd_list) {
            cmd->exec();
        }
        SDL_RenderPresent(_renderer);
        _cmd_list.clear();
        _window->paintEvent();
    }

    void Renderer::fillBackground(const SDL_Color& color, bool covered) {
        _cmd_list.push_back(std::make_unique<FillCMD>(_renderer, color, covered));
    }

    void Renderer::drawPoint(const Graphics::Point &point) {
        _cmd_list.push_back(std::make_unique<PointCMD>(_renderer, point));
    }

    void Renderer::drawPoint(Graphics::Point &&point) {
        _cmd_list.push_back(std::make_unique<PointCMD>(_renderer, point));
    }

    void Renderer::drawLine(const Graphics::Line &line) {
        _cmd_list.push_back(std::make_unique<LineCMD>(_renderer, line));
    }

    void Renderer::drawLine(Graphics::Line &&line) {
        _cmd_list.push_back(std::make_unique<LineCMD>(_renderer, line));
    }

    void Renderer::drawRectangle(const Graphics::Rectangle &rectangle) {
        _cmd_list.push_back(std::make_unique<RectCMD>(_renderer, rectangle));
    }

    void Renderer::drawRectangle(Graphics::Rectangle &&rectangle) {
        _cmd_list.push_back(std::make_unique<RectCMD>(_renderer, rectangle));
    }

    void Renderer::drawTriangle(const Graphics::Triangle &triangle) {
        _cmd_list.push_back(std::make_unique<TriangleCMD>(_renderer, triangle));
    }

    void Renderer::drawTriangle(Graphics::Triangle &&triangle) {
        _cmd_list.push_back(std::make_unique<TriangleCMD>(_renderer, triangle));
    }

    void Renderer::drawEllipse(const Graphics::Ellipse &ellipse) {
        _cmd_list.push_back(std::make_unique<EllipseCMD>(_renderer, ellipse));
    }

    void Renderer::drawEllipse(Graphics::Ellipse &&ellipse) {
        _cmd_list.push_back(std::make_unique<EllipseCMD>(_renderer, ellipse));
    }

    void Renderer::drawTexture(SDL_Texture* texture, Property* property) {
        if (!texture || !property) return;
        _cmd_list.push_back(std::make_unique<TextureCMD>(_renderer, texture, property));
    }

    void Renderer::drawText(TTF_Text* text, const Vector2& position) {
        if (!text) return;
        _cmd_list.push_back(std::make_unique<TextCMD>(_renderer, text, position));
    }

    void Renderer::drawPixelText(const std::string &text, const S3GF::Vector2 &position,
                                 const S3GF::Vector2 &scaled, const SDL_Color& color) {
        if (text.empty() || scaled.x == 0 || scaled.y == 0) return;
        _cmd_list.push_back(std::make_unique<PixelTextCMD>(_renderer, text, position, scaled, color));
    }

    void Renderer::setViewport(const Geometry& geometry) {
        if (geometry.width == 0 || geometry.height == 0) {
            _cmd_list.push_back(std::make_unique<ViewportCMD>(_renderer, true));
        } else {
            _cmd_list.push_back(std::make_unique<ViewportCMD>(_renderer, false,
                      SDL_Rect(geometry.x, geometry.y, geometry.width,geometry.height)));
        }
    }

    void Renderer::setClipView(const Geometry& geometry) {
        if (geometry.width == 0 || geometry.height == 0) {
            _cmd_list.push_back(std::make_unique<ClipCMD>(_renderer, true));
        } else {
            _cmd_list.push_back(std::make_unique<ClipCMD>(_renderer, false,
                                                              SDL_Rect(geometry.x, geometry.y, geometry.width,geometry.height)));
        }
    }

    void Renderer::setBlendMode(const SDL_BlendMode &blend_mode) {
        _cmd_list.push_back(std::make_unique<BlendModeCMD>(_renderer, blend_mode));
    }

    void Renderer::FillCMD::exec() {
        bool _ret = false;
        if (_covered) {
            _ret = SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::WARN);
            }
            _ret = SDL_RenderClear(renderer);
            if (!_ret) {
                Logger::log(std::format("Renderer: Render clear failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        } else {
            Renderer::updateBackground(bg_color);
        }
    }

    void Renderer::ClipCMD::exec() {
        bool _ret = SDL_SetRenderClipRect(renderer, (_reset ? nullptr : &_clip_area));
        if (!_ret) {
            Logger::log(std::format("Renderer: Set renderer clip area failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
    }

    void Renderer::ViewportCMD::exec() {
        bool _ret = SDL_SetRenderViewport(renderer, (_reset ? nullptr : &_viewport_area));
        if (!_ret) {
            Logger::log(std::format("Renderer: Set renderer viewport failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
    }

    void Renderer::BlendModeCMD::exec() {
        bool _ret = SDL_SetRenderDrawBlendMode(renderer, _blend_mode);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render draw blend mode failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
    }

    void Renderer::PointCMD::exec() {
        if (!point.size()) return;
        const auto color = point.color();
        const auto pos = point.position();
        auto _ret = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set renderer draw color failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        if (point.size() == 1) {
            _ret = SDL_RenderPoint(renderer, pos.x, pos.y);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render point failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        } else {
            _ret = SDL_RenderGeometry(renderer, nullptr, point.vertices(), point.verticesCount(),
                               point.indices(), point.indicesCount());
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render geometry failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        }
    }

    void Renderer::LineCMD::exec() {
        const auto SIZE = line.size();
        const auto START = line.startPosition();
        const auto END = line.endPosition();
        if (!SIZE) return;
        const auto color = line.color();
        auto _ret = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render draw color failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        if (SIZE == 1) {
            _ret = SDL_RenderLine(renderer, START.x, START.y,
                           END.x, END.y);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render line failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        } else {
            _ret = SDL_RenderGeometry(renderer, nullptr, line.vertices(),
                           line.vertexCount(), line.indices(), line.indicesCount());
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render geometry failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        }
    }

    void Renderer::RectCMD::exec() {
        bool draw_bordered = rectangle.borderSize() > 0;
        bool fill_rect = rectangle.backgroundColor().a > 0;
        GeometryF geometry = rectangle.geometry();
        SDL_FRect r(geometry.pos.x, geometry.pos.y,
                    geometry.size.width, geometry.size.height);
        bool _ret = false;
        if (fill_rect) {
            SDL_Color color = rectangle.backgroundColor();
            _ret = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::WARN);
            }
            _ret = SDL_RenderFillRect(renderer, &r);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render fill rectangle failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        }
        if (draw_bordered) {
            SDL_Color color = rectangle.borderColor();
            _ret = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::WARN);
            }
            if (rectangle.borderSize() == 1) {
                _ret = SDL_RenderRect(renderer, &r);
                if (!_ret) {
                    Logger::log(std::format("Renderer: Set render rectangle failed! Exception: {}",
                                            SDL_GetError()), Logger::ERROR);
                }
            } else {
                _ret = SDL_RenderFillRects(renderer, rectangle.bordersRect(), 4);
                if (!_ret) {
                    Logger::log(std::format("Renderer: Set render fill rectangles failed! Exception: {}",
                                            SDL_GetError()), Logger::ERROR);
                }
            }
        }
    }

    void Renderer::TriangleCMD::exec() {
        bool filled = (triangle.backgroundColor().a >= 0);
        bool bordered = (triangle.borderSize() > 0);
        bool _ret = false;
        if (filled) {
            _ret = SDL_RenderGeometry(renderer, nullptr, triangle.vertices(), 3,
                               triangle.indices(), 3);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render geometry failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        }
        if (bordered) {
            const auto SIZE = triangle.borderSize();
            const auto color = triangle.borderColor();
            _ret = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::WARN);
            }
            int err_cnt = 0;
            if (SIZE == 1) {
                auto p1 = triangle.position(0),
                     p2 = triangle.position(1),
                     p3 = triangle.position(2);
                err_cnt += SDL_RenderLine(renderer, p1.x, p1.y, p2.x, p2.y);
                err_cnt += SDL_RenderLine(renderer, p3.x, p3.y, p2.x, p2.y);
                err_cnt += SDL_RenderLine(renderer, p1.x, p1.y, p3.x, p3.y);
                if (err_cnt < 3) {
                    Logger::log(std::format("Renderer: Set render triangle failed! Exception: {}",
                                            SDL_GetError()), Logger::ERROR);
                }
            } else {
                err_cnt += SDL_RenderGeometry(renderer, nullptr, triangle.borderVertices1(), triangle.borderVerticesCount(),
                                   triangle.borderIndices1(), triangle.borderIndicesCount());
                err_cnt += SDL_RenderGeometry(renderer, nullptr, triangle.borderVertices2(), triangle.borderVerticesCount(),
                                   triangle.borderIndices2(), triangle.borderIndicesCount());
                err_cnt += SDL_RenderGeometry(renderer, nullptr, triangle.borderVertices3(), triangle.borderVerticesCount(),
                                   triangle.borderIndices3(), triangle.borderIndicesCount());
                if (err_cnt < 3) {
                    Logger::log(std::format("Renderer: Set render triangle failed! Exception: {}",
                                            SDL_GetError()), Logger::ERROR);
                }
            }
        }
    }

    void Renderer::EllipseCMD::exec() {
        bool filled = (ellipse.backgroundColor().a >= 0);
        bool bordered = (ellipse.borderSize() > 0);
        bool _ret = false;
        if (filled) {
            _ret = SDL_RenderGeometry(renderer, nullptr, ellipse.vertices(), ellipse.vertexCount(),
                               ellipse.indices(), ellipse.indicesCount());
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render geometry failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        }
        if (bordered) {
            _ret = SDL_RenderGeometry(renderer, nullptr, ellipse.borderVertices(), ellipse.borderVerticesCount(),
                               ellipse.borderIndices(), ellipse.borderIndicesCount());
            if (!_ret) {
                Logger::log(std::format("Renderer: Set render geometry failed! Exception: {}",
                                        SDL_GetError()), Logger::ERROR);
            }
        }
    }

    void Renderer::TextureCMD::exec() {
        if (!_texture || !_property) {
            Logger::log(std::format("Renderer: The texture or texture property is not valid!"),
                        Logger::ERROR);
            return;
        }
        auto color = _property->color_alpha;
        bool _ret = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render draw color failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        _ret = SDL_SetTextureColorMod(_texture, color.r, color.g, color.b);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set texture color failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        _ret = SDL_SetTextureAlphaMod(_texture, color.a);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set texture alpha failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        auto scaled = _property->scaledGeometry();
        auto scaled_pos = scaled.pos;
        auto scaled_size = scaled.size;
        SDL_FRect rect_dest = {scaled_pos.x, scaled_pos.y,
                        scaled_size.width, scaled_size.height};
        _ret = SDL_RenderTextureRotated(renderer, _texture, 
                         _property->clip_mode ? &_property->clip_area : nullptr,
                         &rect_dest, _property->rotate_angle, nullptr, SDL_FLIP_NONE);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render texture failed! Exception: {}",
                                    SDL_GetError()), Logger::ERROR);
            return;
        }
    }

    void Renderer::TextCMD::exec() {
        bool _ret = TTF_DrawRendererText(text, position.x, position.y);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render text failed! Exception: {}",
                                    SDL_GetError()), Logger::ERROR);
        }
    }

    void Renderer::PixelTextCMD::exec() {
        bool _ret = SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render draw color failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        _ret = SDL_SetRenderScale(renderer, scaled.x, scaled.y);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render scale failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        _ret = SDL_RenderDebugText(renderer, pos.x, pos.y, text.c_str());
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render debug text failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
        _ret = SDL_SetRenderScale(renderer, 1.0f, 1.0f);
        if (!_ret) {
            Logger::log(std::format("Renderer: Set render scale failed! Exception: {}",
                                    SDL_GetError()), Logger::WARN);
        }
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
        Logger::log(std::format("Window: created with ID {}", _winID), Logger::DEBUG);
        if (object) {
            object->newWindow(this);
        } else {
            Logger::log("Window: Can't find engine object!", Logger::FATAL);
            Engine::throwFatalError();
        }
    }

    Window::~Window() {
        if (_renderer) {
            _renderer.reset();
        }
        if (_window) {
            SDL_DestroyWindow(_window);
            Logger::log(std::format("Window: ID {} destroyed",  _winID), Logger::DEBUG);
        }
    }

    bool Window::move(int x, int y) {
        bool _ret = SDL_SetWindowPosition(_window, x, y);
        if (!_ret) {
            Logger::log(std::format("Window: Can't move window! Exception: {}", SDL_GetError()), Logger::ERROR);
            return false;
        }
        _window_geometry.x = x;
        _window_geometry.y = y;
        return true;
    }

    bool Window::resize(int width, int height) {
        bool _ret = SDL_SetWindowSize(_window, width, height);
        if (!_ret) {
            Logger::log(std::format("Window: Can't resize window! Exception: {}", SDL_GetError()), Logger::ERROR);
            return false;
        }
        _window_geometry.width = width;
        _window_geometry.height = height;
        return true;
    }

    bool Window::setGeometry(int x, int y, int width, int height) {
        return (!move(x, y) || !resize(width, height)); 
    }

    const Window::Geometry Window::geometry() const {
        return _window_geometry;
    }

    uint32_t Window::windowID() const {
        return _winID;
    }

    bool Window::show() {
        auto _ret = SDL_ShowWindow(_window);
        if (!_ret) {
            Logger::log(std::format("Window: Can't show window! Exception: {}", SDL_GetError()), Logger::ERROR);
            return false;
        }
        _visible = true;
        return true;
    }

    bool Window::hide() {
        bool _ret = SDL_HideWindow(_window);
        if (!_ret) {
            Logger::log(std::format("Window: Can't hide window! Exception: {}", SDL_GetError()), Logger::ERROR);
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
            Logger::log(std::format("Window: Can't set window resizable mode! Exception: {}", SDL_GetError()), Logger::ERROR);
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
            Logger::log("Window: The specified renderer is not valid!", Logger::ERROR);
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
                         Logger::ERROR);
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
                        Logger::ERROR);
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
            Logger::log("The window is not created!", Logger::ERROR);
        }
        return _window;
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
            Logger::log(std::format("Window: Failed to get window size for ID {}!", _winID), Logger::WARN);
        }
    }

    void Window::moveEvent() {
        auto _ret = SDL_GetWindowPosition(_window, &_window_geometry.x, &_window_geometry.y);
        if (!_ret) {
            Logger::log(std::format("Window: Failed to get window position for ID {}!", _winID), Logger::WARN);
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
            Logger::log(std::format("The event with ID {} is already exists!", id), Logger::WARN);
            return;
        }
        _event_list.emplace(id, event);
        Logger::log(std::format("Append a new event with ID {}", id));
    }

    void EventSystem::removeEvent(uint64_t id) {
        if (_event_list.contains(id)) {
            _event_list.erase(id);
            Logger::log(std::format("Removed the event with ID {}", id));
        } else {
            Logger::log(std::format("The event with ID {} is not found!", id), Logger::WARN);
        }
    }

    void EventSystem::clearEvent() {
        _event_list.clear();
        Logger::log("Cleared all events.");
    }

    size_t EventSystem::eventCount() const { return _event_list.size(); }

    bool EventSystem::run() {
        SDL_Event ev;
        if (SDL_PollEvent(&ev)) {
            _is_key_down = (ev.key.type == SDL_EVENT_KEY_DOWN);
            _is_mouse_down = (ev.button.type == SDL_EVENT_MOUSE_BUTTON_DOWN);
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
        return true;
    }

    bool EventSystem::isKeyDown() const { return _is_key_down; }
    bool EventSystem::isMouseButtonDown() const { return _is_mouse_down; }

    Engine::Engine(std::string&& app_name, std::string&& app_version, std::string&& app_id)
        : _app_id(app_id), _app_name(app_name), _app_version(app_version), _running(true) {
        if (_show_app_info) {
            std::cout << std::format("S3GF {} (Based on SDL {}.{}.{})\n",
                                     S3GF_FULL_VERSION, SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION)
                      << "For more information, visit: https://github.com/CatIsNotFound/S3GF \n"
                         "                             https://gitee.com/CatIsNotFound/S3GF\n" << std::endl;
            std::cout << std::format("=== Application Info ===\nID: {} \nName: {} \nVersion: {} \n",
                                     app_id, app_name, app_version) << std::endl;
        }
        if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throwFatalError();
        }
        SDL_SetAppMetadata(app_name.c_str(), app_version.c_str(), app_id.c_str());
        TextSystem::global();
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
            Logger::log("No error found. It will not throw the fatal error!", Logger::INFO);
            return;
        }
        std::string err = std::format("An error has caused the entire program to crash.\nException: {}", get_err_info);
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "FATAL ERROR", err.c_str(), nullptr);
        Logger::log(err, Logger::FATAL);
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
        // AudioSystem::global()->unload();
        SDL_Quit();
    }

    void Engine::running() {
        auto start_time = SDL_GetTicks();
        auto frames = 0ULL;
        auto start_ns = SDL_GetTicksNS();
        while (_running && !_quit_requested) {
            //std::this_thread::sleep_for(std::chrono::milliseconds(1));
            if (_window_list.empty()) break;
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
                _real_fps = frames;
                if (_fps > 14 && _real_fps <= 14) {
                    Logger::log(std::format("Low FPS detected: {} FPS", _real_fps), Logger::WARN);
                }
                frames = 0;
                start_time = SDL_GetTicks();
            }
        }
    }

    Font::Font(const std::string& font_path, float font_size) 
        : _font_size(font_size){
        _font = TTF_OpenFont(font_path.c_str(), font_size);
        if (!_font) {
            Logger::log(std::format("Can't load font from path '{}'.", font_path), Logger::ERROR);
        }
        _font_is_loaded = true;
    }

    Font::~Font() {
        if (_font) {
            TTF_CloseFont(_font);
        }
    }
    
    void Font::setFontSize(float size) {
        auto _ret = TTF_SetFontSize(_font, size);
        if (_ret) {
            _font_size = size;
        }
    }

    float Font::fontSize() const {
        return _font_size;
    }

    void Font::setFontColor(const SDL_Color& color) {
        _font_color = color;
    }

    const SDL_Color Font::fontColor() const {
        return _font_color;
    }

    void Font::setStyle(uint32_t flags) {
        TTF_SetFontStyle(_font, static_cast<TTF_FontStyleFlags>(flags));
        _font_style_flags = flags;
    }

    void Font::setOutline(uint32_t value) {
        _font_outline = value;
    }

    uint32_t Font::outline() const {
        return _font_outline;
    }

    void Font::setOutlineColor(const SDL_Color& color) {
        _outline_color = color;
    }

    const SDL_Color Font::outlineColor() const {
        return _outline_color;
    }

    void Font::setFontDirection(Direction direction) {
        auto _ret = TTF_SetFontDirection(_font, static_cast<TTF_Direction>(direction));
        if (_ret) {
            _font_direction = direction;
        }
    }

    Font::Direction Font::fontDirection() const {
        return _font_direction;
    }

    void Font::setFontHinting(uint32_t flags) {
        TTF_SetFontHinting(_font, static_cast<TTF_HintingFlags>(flags));
        _font_hinting = flags;
    }

    void Font::setFontKerning(bool enabled) {
        TTF_SetFontKerning(_font, enabled);
        _font_kerning = enabled;
    }

    bool Font::fontKerning() const {
        return _font_kerning;
    }

    void Font::setLineSpacing(uint32_t spacing) {
        TTF_SetFontLineSkip(_font, spacing);
        _line_spacing = spacing;
    }

    uint32_t Font::lineSpacing() const {
        return _line_spacing;
    }

    SDL_Surface* Font::toImage(const std::string& text) {
        SDL_Surface* surface;
        if (!_font_is_loaded) {
            Logger::log("Font is not loaded! Did you forgot to load font?", Logger::ERROR);
            return nullptr;
        }
        if (_font_outline) {
            if (_font_color.a > 0) {
                auto filled_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
                TTF_SetFontOutline(_font, _font_outline);
                auto bordered_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _outline_color);
                TTF_SetFontOutline(_font, 0);
                int real_width = bordered_surface->w, real_height = bordered_surface->h;
                surface = SDL_CreateSurface(real_width, real_height, bordered_surface->format);

                SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0));
                SDL_BlitSurface(bordered_surface, nullptr, surface, nullptr);
                SDL_Rect rect(real_width / 2 - filled_surface->w / 2, 
                              real_height / 2 - filled_surface->h / 2, 
                              filled_surface->w, filled_surface->h);
                SDL_BlitSurface(filled_surface, nullptr, surface, &rect);

                SDL_DestroySurface(filled_surface);
                SDL_DestroySurface(bordered_surface);
            } else {
                TTF_SetFontOutline(_font, _font_outline);
                surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _outline_color);
                TTF_SetFontOutline(_font, 0);
            }
        } else {
            surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
        }
        if (!surface) {
            Logger::log(std::format("Can't drawEvent the current text!\nException: {}", SDL_GetError()), Logger::ERROR);
        }
        return surface;
    }

    SDL_Surface* Font::toImage(const std::string& text, const SDL_Color& backgrond_color) {
        SDL_Surface* surface = nullptr;
        if (!_font_is_loaded) {
            Logger::log("Font is not loaded! Did you forget to load font?", Logger::ERROR);
            return nullptr;
        }
        if (_font_outline) {
            if (_font_color.a > 0) {
                auto filled_surface = TTF_RenderText_Blended(_font, text.c_str(), 0, _font_color);
                TTF_SetFontOutline(_font, _font_outline);
                auto bordered_surface = TTF_RenderText_LCD(_font, text.c_str(), 0, _outline_color, backgrond_color);
                TTF_SetFontOutline(_font, 0);
                int real_width = bordered_surface->w, real_height = bordered_surface->h;
                surface = SDL_CreateSurface(real_width, real_height, bordered_surface->format);

                SDL_FillSurfaceRect(surface, nullptr, SDL_MapSurfaceRGBA(surface, 0, 0, 0, 0));
                SDL_BlitSurface(bordered_surface, nullptr, surface, nullptr);
                SDL_Rect rect(real_width / 2 - filled_surface->w / 2, 
                              real_height / 2 - filled_surface->h / 2, 
                              filled_surface->w, filled_surface->h);
                SDL_BlitSurface(filled_surface, nullptr, surface, &rect);

                SDL_DestroySurface(filled_surface);
                SDL_DestroySurface(bordered_surface);
            } else {
                TTF_SetFontOutline(_font, _font_outline);
                surface = TTF_RenderText_LCD(_font, text.c_str(), 0, _outline_color, backgrond_color);
                TTF_SetFontOutline(_font, 0);
            }
        } else {
            surface = TTF_RenderText_LCD(_font, text.c_str(), 0, _font_color, backgrond_color);
        }
        if (!surface) {
            Logger::log(std::format("Can't drawEvent the current text!\nException: {}", SDL_GetError()), Logger::ERROR);
        }
        return surface;
    }


    TTF_Font* Font::self() const {
        return _font;
    }

    TextSystem::TextSystem() {
        if (!TTF_Init()) {
            Logger::log(std::format("TextSystem: Can't load Text System! Exception: {}", SDL_GetError()), Logger::FATAL);
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
            Logger::log(std::format("Font '{}' is already added!", font_name), Logger::ERROR);
            return false;
        }
        if (!renderer) {
            Logger::log("Can't add font! The specified renderer is not valid!", Logger::ERROR);
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
            Logger::log(std::format("Can't load font '{}'! Exception: {}", font_name, SDL_GetError()), Logger::ERROR);
            return false;
        }
        return true;
    }

    bool TextSystem::removeFont(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("Font '{}' is not in the font list!", font_name), Logger::ERROR);
            return false;
        }
        TTF_DestroyRendererTextEngine(_font_map[font_name].engine);
        TTF_DestroySurfaceTextEngine(_font_map[font_name].surface_engine);
        _font_map.erase(font_name);
        return true;
    }

    Font* TextSystem::font(const std::string& font_name) {
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("Font '{}' is not in the font list!", font_name), Logger::ERROR);
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
            Logger::log(std::format("Text ID {} is already added to text list!", text_id), Logger::ERROR);
            return false;
        }
        
        _text_map.emplace(text_id, Text(nullptr, text, font_name));
        _text_map[text_id].self = TTF_CreateText(_font_map[font_name].engine, _font_map[font_name].font->self(), text.c_str(), text.size());
        if (!_text_map[text_id].self) {
            Logger::log(std::format("Can't create text!\nException: {}", SDL_GetError()), Logger::ERROR);
            _text_map.erase(text_id);
            return false;
        }
        return true;
    }

    bool TextSystem::removeText(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
            return false;
        }
        _text_map.erase(text_id);
        return true;
    }

    bool TextSystem::setText(uint64_t text_id, const std::string& text) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(std::format("Can't set text to text ID {}!\nException: {}", text_id, SDL_GetError()), Logger::ERROR);
            return false;
        }
        m_text.text = text;
        return true;
    }

    bool TextSystem::appendText(uint64_t text_id, const std::string& text) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_AppendTextString(m_text.self, text.c_str(), text.size());
        if (!_ret) {
            Logger::log(std::format("Can't set text to text ID {}!\nException: {}", text_id, SDL_GetError()), Logger::ERROR);
            return false;
        }
        m_text.text += text;
        return true;
    }

    bool TextSystem::setTextFont(uint64_t text_id, const std::string& font_name) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
            return false;
        }
        if (!_font_map.contains(font_name)) {
            Logger::log(std::format("Font '{}' is not in the font list!", font_name), Logger::ERROR);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextFont(m_text.self, _font_map[font_name].font->self());
        if (!_ret) {
            Logger::log(std::format("Can't set font '{}' to text ID {}!\nException: {}", 
                    font_name, text_id, SDL_GetError()), Logger::ERROR);
            return false;
        }
        m_text.font_name = font_name;
        return true;
    }

    bool TextSystem::setTextColor(uint64_t text_id, const SDL_Color& color) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
            return false;
        }
        auto& m_text = _text_map[text_id];
        auto _ret = TTF_SetTextColor(m_text.self, color.r, color.g, color.b, color.a);
        if (!_ret) {
            Logger::log(std::format("Can't set font color to text ID {}!\nException: {}", 
                    text_id, SDL_GetError()), Logger::ERROR);
            return false;
        }
        m_text.font_color = color;
        return true;
    }

    TextSystem::Text* TextSystem::indexOfText(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
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
            Logger::log("The specified renderer is not valid!", Logger::ERROR);
            return false;
        }
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
            return false;
        }

        renderer->drawText(_text_map[text_id].self, pos);
        return true;
    }

    SDL_Surface* TextSystem::toImage(uint64_t text_id) {
        if (!_text_map.contains(text_id)) {
            Logger::log(std::format("Text ID {} is not in the text list!", text_id), Logger::ERROR);
            return nullptr;
        }
        if (!_font_map.contains(_text_map[text_id].font_name)) {
            Logger::log(std::format("Text ID {} has not set the font! Try to use `setTextFont()` at first!", text_id), Logger::ERROR);
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
            Logger::log(std::format("Text to image failed! Exception: {}", SDL_GetError()), Logger::ERROR);
            TTF_DestroyText(temp_text);
            return nullptr;
        }
        TTF_DestroyText(temp_text);
        return surface;
    }


    FontMap FontDatabase::getFontDatabaseFromSystem() {
        if (!_is_loaded) {
            StringList find_font_dir;
#ifdef _WIN32
            find_font_dir.emplace_back("C:/Windows/Fonts");
#endif
#ifdef __linux__
            find_font_dir.emplace_back("/usr/share/fonts");
            if (FileSystem::isDir("~/.fonts")) {
                find_font_dir.emplace_back("~/.fonts");
            }
            if (FileSystem::isDir("~/.local/share/fonts")) {
                find_font_dir.emplace_back("~/.local/share/fonts");
            }
#endif
#ifdef __APPLE__
            if (FileSystem::isDir("/System/Library/Fonts")) {
                find_font_dir.emplace_back("/System/Library/Fonts");
            }
            if (FileSystem::isDir("/Library/Fonts")) {
                find_font_dir.emplace_back("/Library/Fonts");
            }
            if (FileSystem::isDir("~/Library/Fonts")) {
                find_font_dir.emplace_back("~/Library/Fonts");
            }
#endif
            for (auto& font_dir : find_font_dir) {
                StringList font_files = FileSystem::listFilesRecursively(font_dir,
                                                            {".ttf", ".otf", ".ttc", ".woff", ".eot"});
                for (auto& file : font_files) {
                    auto short_file = FileSystem::getShortFileName(file, true);
                    if (!_font_db.contains(short_file)) {
                        _font_db.insert({FileSystem::getShortFileName(file, true), file});
                    }
                }
            }
            Logger::log("FontDatabase: Get Font files from system!", Logger::DEBUG);
            _is_loaded = true;
        }
        return _font_db;
    }

    std::string FontDatabase::findFontFromSystem(const std::string &font_name) {
        if (!_is_loaded) getFontDatabaseFromSystem();
        if (!_font_db.contains(font_name)) return {};
        return _font_db[font_name];
    }

    std::vector<FontDatabase::FontInfo> FontDatabase::getSystemDefaultFont() {
        if (!_is_loaded) getFontDatabaseFromSystem();
        std::vector<FontDatabase::FontInfo> default_fonts;
#ifdef _WIN32
        StringList common_fonts = { "arial", "segoeui", "tahoma", "verdana", "calibri" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) default_fonts.push_back({font_name, path});
        }
#endif
#ifdef __linux__
        StringList common_fonts = { "AdwaitaSans-Regular", "DejaVuSans", "Roboto-Regular", "Ubuntu-R" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) default_fonts.push_back({font_name, path});
        }
#endif
#ifdef __APPLE__
        StringList common_fonts = { "SanFrancisco-Regular", "HelveticaNeue", "ArialMT", "TimesNewRomanPSMT" };
        for (auto& font_name : common_fonts) {
            auto path = findFontFromSystem(font_name);
            if (!path.empty()) default_fonts.push_back({font_name, path});
        }
#endif
        return default_fonts;
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
                SDL_GetError()), Logger::ERROR);
            return false;
        }
        SDL_AudioSpec _audio_spec(SDL_AUDIO_S16, 2, 44100);
        _mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_audio_spec);
        if (!_mixer) {
            Logger::log(std::format("AudioSystem: Can't initilized audio system! Exception: {}", 
                SDL_GetError()), Logger::ERROR);
            return false;
        }
        Logger::log("AudioSystem: Loaded audio system!");
        return true;
    }

    void AudioSystem::unload() {
        if (!_is_init) return;
        if (_mixer) MIX_DestroyMixer(_mixer);
        MIX_Quit();
        Logger::log("AudioSystem: Unloaded audio system!");
        _is_init = false;
    }

}