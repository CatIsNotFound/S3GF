
#include "Core.h"

namespace S3GF {
    std::unique_ptr<EventSystem> EventSystem::_instance{};
    SDL_WindowID Engine::_main_window_id{0};
    bool Engine::_quit_requested{false};
    int Engine::_return_code{0};
    Logger::LogLevel Logger::_base_level = Logger::INFO;
    Logger::LogLevel Logger::_last_log_level = Logger::DEBUG;
    uint64_t Logger::_started_time = SDL_GetTicksNS();
    uint64_t Logger::_running_time = 0;
    std::string Logger::_last_log_info = "";
    std::unique_ptr<TextSystem> TextSystem::_instance{};
    std::unique_ptr<AudioSystem> AudioSystem::_instance{};

    Renderer::Renderer(Window* window) : _window(window) {
        _renderer = SDL_CreateRenderer(_window->self(), nullptr);
        if (!_renderer) {
            Logger::log("The renderer is not created!", Logger::FATAL);
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
        if (_cmd_list.empty()) {
            SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
            SDL_RenderClear(_renderer);
        } else {
            for (auto& cmd : _cmd_list) {
                cmd->exec();
            }
        }
        SDL_RenderPresent(_renderer);
        _cmd_list.clear();
        _window->paintEvent();
    }

    void Renderer::fillBackground(const SDL_Color& color) {
        _cmd_list.push_back(std::make_unique<FillCMD>(_renderer, color));
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

    void Renderer::FillCMD::exec() {
        SDL_SetRenderDrawColor(renderer, bg_color.r, bg_color.g, bg_color.b, bg_color.a);
        SDL_RenderClear(renderer);
    }

    void Renderer::PointCMD::exec() {
        if (!point.size) return;
        SDL_SetRenderDrawColor(renderer, point.color.r, point.color.g, point.color.b, point.color.a);
        if (point.size == 1) {
            SDL_RenderPoint(renderer, point.position.x, point.position.y);
        } else {

        }
    }

    void Renderer::LineCMD::exec() {
        const auto SIZE = line.size();
        const auto START = line.startPosition();
        const auto END = line.endPosition();
        if (!SIZE) return;
        SDL_SetRenderDrawColor(renderer, line.color.r, line.color.g, line.color.b, line.color.a);
        if (SIZE == 1) {
            SDL_RenderLine(renderer, START.x, START.y,
                           END.x, END.y);
            return;
        }
        SDL_RenderGeometry(renderer, nullptr, line.vertexes(),
                           line.vertexCount(), line.indices(), line.indiceCount());
    }

    void Renderer::RectCMD::exec() {
        bool draw_bordered = rectangle.border_size > 0;
        bool fill_rect = rectangle.background_color.a > 0;
        SDL_FRect r(rectangle.geometry.pos.x, rectangle.geometry.pos.y,
                    rectangle.geometry.size.width, rectangle.geometry.size.height);
        if (fill_rect) {
            SDL_SetRenderDrawColor(renderer, rectangle.background_color.r,
                                   rectangle.background_color.g,
                                   rectangle.background_color.b,
                                   rectangle.background_color.a);
            SDL_RenderFillRect(renderer, &r);
        }
        if (draw_bordered) {
            SDL_SetRenderDrawColor(renderer, rectangle.border_color.r,
                                   rectangle.border_color.g,
                                   rectangle.border_color.b,
                                   rectangle.border_color.a);
            if (rectangle.border_size == 1) {
                SDL_RenderRect(renderer, &r);
            } else {
                const float THICKNESS = rectangle.border_size;
                const float X = rectangle.geometry.pos.x;
                const float Y = rectangle.geometry.pos.y;
                const float W = rectangle.geometry.size.width;
                const float H = rectangle.geometry.size.height;
                SDL_FRect rects[4];
                rects[0] = {X, Y, W, THICKNESS };
                rects[1] = {X, Y + H - THICKNESS, W, THICKNESS };
                rects[2] = {X, Y + THICKNESS, THICKNESS, H - 2 * THICKNESS };
                rects[3] = {X + W - THICKNESS, Y + THICKNESS, THICKNESS, H - 2 * THICKNESS };
                SDL_RenderFillRects(renderer, rects, 4);
            }
        }
    }

    void Renderer::TriangleCMD::exec() {
        if (triangle.background_color.a == 0) return;
        SDL_FColor color = Algorithm::convert2FColor(triangle.background_color);
        SDL_Vertex vx[3] = {
                {{triangle.point1.x, triangle.point1.y}, color},
                {{triangle.point1.x, triangle.point1.y}, color},
                {{triangle.point1.x, triangle.point1.y}, color}
        };
        SDL_RenderGeometry(renderer, nullptr, vx, 3, nullptr, 0);
    }

    void Renderer::EllipseCMD::exec() {

    }

    void Renderer::TextureCMD::exec() {
        auto color = _property->color_alpha;
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_SetTextureColorMod(_texture, color.r, color.g, color.b);
        SDL_SetTextureAlphaMod(_texture, color.a);
        auto scaled = _property->scaledGeometry();
        auto scaled_pos = scaled.pos;
        auto scaled_size = scaled.size;
        SDL_FRect rect_dest = {scaled_pos.x, scaled_pos.y,
                        scaled_size.width, scaled_size.height};
        if (_property->clip_mode) {
            SDL_RenderTextureRotated(renderer, _texture, &_property->clip_area, &rect_dest, _property->rotate_angle, nullptr, SDL_FLIP_NONE);
        } else {
            SDL_RenderTextureRotated(renderer, _texture, nullptr, &rect_dest, _property->rotate_angle, nullptr, SDL_FLIP_NONE);
        }
    }

    void Renderer::TextCMD::exec() {
        TTF_DrawRendererText(text, position.x, position.y);
    }

    Window::Window(const std::string& title, int width, int height, GraphicEngine engine) 
        : _title(title), _window_geometry(0, 0, width, height), _visible(true), _resizable(false) {
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
        Logger::log(std::format("Window '{}' created with ID {}", _title, _winID), Logger::DEBUG);
    }

    Window::~Window() {
        if (_renderer) {
            _renderer.reset();
        }
        SDL_DestroyWindow(_window);
        Logger::log(std::format("Window '{}' with ID {} destroyed", _title, _winID), Logger::DEBUG);
    }

    bool Window::move(int x, int y) {
        bool _ret = SDL_SetWindowPosition(_window, x, y);
        if (!_ret) {
            Logger::log(std::format("Can't move window!\nException: {}", SDL_GetError()), Logger::ERROR); 
            return false;
        }
        _window_geometry.x = x;
        _window_geometry.y = y;
        return true;
    }

    bool Window::resize(int width, int height) {
        bool _ret = SDL_SetWindowSize(_window, width, height);
        if (!_ret) {
            Logger::log(std::format("Can't resize window!\nException: {}", SDL_GetError()), Logger::ERROR); 
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
            Logger::log(std::format("Can't show window!\nException: {}", SDL_GetError()), Logger::ERROR); 
            return false;
        }
        _visible = true;
        return true;
    }

    bool Window::hide() {
        bool _ret = SDL_HideWindow(_window);
        if (!_ret) {
            Logger::log(std::format("Can't hide window!\nException: {}", SDL_GetError()), Logger::ERROR); 
            return false;
        }
        _visible = false;
        return true;
    }

    bool Window::visible() const {
        return _visible;
    }

    bool Window::setResizable(bool enabled) {
        auto _ret = SDL_SetWindowResizable(_window, enabled);
        if (!_ret) {
            Logger::log(std::format("Can't set window resizable mode!\nException: {}", SDL_GetError()), Logger::ERROR); 
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
            Logger::log("The specified renderer is not valid!", Logger::ERROR);
            return;
        }
        _renderer = std::shared_ptr<Renderer>(renderer);
    }

    Renderer* Window::renderer() const {
        return _renderer.get();
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
        _paint_event = paint_event;
    }

    void Window::paintEvent() {
        if (_paint_event) {
            _paint_event(_renderer.get());
        }
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

    void Window::unloadEvent() {}

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

    bool EventSystem::run() {
        SDL_Event ev;
        if (SDL_PollEvent(&ev)) {
            if (ev.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                return false;
            }
            for (auto i = _engine->begin(); i != _engine->end(); i++) {
                if (ev.window.windowID != i->first) continue;
                
                if (ev.type == SDL_EVENT_WINDOW_MOVED) {
                    i->second->moveEvent();
                }
                if (ev.type == SDL_EVENT_WINDOW_RESIZED) {
                    i->second->resizeEvent();
                }
                if (ev.type == SDL_EVENT_WINDOW_FOCUS_GAINED) {
                    i->second->getFocusEvent();
                }
                if (ev.type == SDL_EVENT_WINDOW_FOCUS_LOST) {
                    i->second->lostFocusEvent();
                }
            }

            for (auto& event : _event_list) {
                if (event.second) {
                    event.second(ev);
                }
            }
        }
        return true;
    }


    Engine::Engine() : _running(true) {
        if (!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
            throwFatalError();
        }
        TextSystem::global();
        EventSystem::global(this);
        // AudioSystem::global();
        signal(SIGINT, Engine::exit);
    }

    Engine::~Engine() {
        cleanUp();
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
        _window_list.emplace(window->windowID(), window);
    }

    void Engine::removeWindow(SDL_WindowID id) {
        if (_window_list.contains(id)) {
            _window_list.erase(id);
        }
    }

    Window* Engine::window(SDL_WindowID id) const {
        return _window_list.at(id).get();
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

    void Engine::cleanUp() {
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
            // std::this_thread::sleep_for(std::chrono::milliseconds(1));
            SDL_Delay(1);
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
            Logger::log(std::format("Can't draw the current text!\nException: {}", SDL_GetError()), Logger::ERROR);
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
            Logger::log(std::format("Can't draw the current text!\nException: {}", SDL_GetError()), Logger::ERROR);
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
                std::unique_ptr<Font>(new Font(font_path, 12.0f))});
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
        
        _text_map.emplace(text_id, Text(nullptr, text, font_name, StdColor::White));
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

    uint64_t AudioSystem::newAudio() {
        MIX_Audio* new_audio = nullptr;
        
        if (!new_audio) {
            Logger::log(std::format("AudioSystem: Can't create a new audio! Exception: {}", SDL_GetError()), 
                Logger::ERROR);
            return 0;
        }
        uint64_t id = 0;
        do {
            id = RandomGenerator::randBigUInt(1000, 9999999);
        } while (_audios_map.contains(id));
        // _audios_map.emplace(Audio{});
        // _audios_map[id].self = nullptr;
        // _audios_map[id].status = Status::Loaded;
        // _audios_map[id].url = "";
        
        return id;
    }

    // bool AudioSystem::remove(uint64_t audio_id) {

    // }

    // void AudioSystem::clear() {

    // }

    // const AudioSystem::Audio& audio(uint64_t audio_id) const {

    // }

    
}