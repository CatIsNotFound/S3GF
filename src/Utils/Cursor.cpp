#include "Cursor.h"

namespace MyEngine {
    Vector2 Cursor::globalPosition() const {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
        }
        Vector2 _pos;
        SDL_GetGlobalMouseState(&_pos.x, &_pos.y);
        return _pos;
    }

    Vector2 Cursor::position() const {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
        }
        Vector2 _pos;
        SDL_GetMouseState(&_pos.x, &_pos.y);
        return _pos;
    }

    uint64_t Cursor::focusOn() const {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return 0;
        }
        return SDL_GetWindowID(SDL_GetMouseFocus());
    }

    void Cursor::move(const Vector2& pos, const Window* window) {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        if (window) {
            SDL_WarpMouseInWindow(window->self(), pos.x, pos.y);
        } else {
            SDL_WarpMouseGlobal(pos.x, pos.y);
        }
    }

    void Cursor::move(float x, float y, const Window* window) {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        if (window) {
            SDL_WarpMouseInWindow(window->self(), x, y);
        } else {
            SDL_WarpMouseGlobal(x, y);
        }
    }

    void Cursor::moveToCenter(const Window* window) {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        Vector2 pos;
        if (window) {
            int w, h;
            SDL_GetWindowSize(window->self(), &w, &h);
            pos.reset((float)w / 2, (float)h / 2);
            SDL_WarpMouseInWindow(window->self(), pos.x, pos.y);
        } else {
            SDL_Rect rect;
            SDL_GetDisplayBounds(SDL_GetPrimaryDisplay(), &rect);
            pos.reset((float)rect.w / 2, (float)rect.h / 2);
            SDL_WarpMouseGlobal(pos.x, pos.y);
        }
    }

    void Cursor::setCursor(const StdCursor& cursor) {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        if (this->cursor() == cursor) return;
        if (_custom_cursor && _user_custom.contains(cursor)) {
            auto user_cursor = _user_custom.at(cursor);
            if (user_cursor.cursor) {
                SDL_DestroyCursor(_cursor);
            }
            _cursor = SDL_CreateColorCursor(user_cursor.cursor.get(),
                                            user_cursor.hot_point.x, user_cursor.hot_point.y);
            SDL_SetCursor(_cursor);
            _std_cursor = cursor;
            return;
        }
        _std_cursor = cursor;
        SDL_DestroyCursor(_cursor);
        if (_surface) {
            SDL_DestroySurface(_surface);
            _surface = nullptr;
        }
        _cursor = SDL_CreateSystemCursor(SysCursor(_std_cursor));
        SDL_SetCursor(_cursor);
    }

    void Cursor::setCursor(const std::string &path, int hot_x, int hot_y) {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        SDL_Surface *temp = IMG_Load(path.data());
        if (!temp) {
            Logger::log(FMT::format("[ERROR] Can't load image while setting cursor: {}", path), Logger::Error);
            return;
        }
        SDL_DestroyCursor(_cursor);
        if (_surface) {
            SDL_DestroySurface(_surface);
        }
        _surface = temp;
        _std_cursor = Custom;
        _cursor = SDL_CreateColorCursor(_surface, hot_x, hot_y);
        SDL_SetCursor(_cursor);
    }

    void Cursor::setUserCustomEnabled(bool enabled) {
        _custom_cursor = enabled;
    }

    bool Cursor::userCustomEnabled() const {
        return _custom_cursor;
    }

    void Cursor::setCustomCursor(const StdCursor& stdCursor, const std::string& path, const UserCustom::HotPoint& hot_point) {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        auto new_cursor = IMG_Load(path.c_str());
        if (!new_cursor) {
            Logger::log(FMT::format("[ERROR] Can't set addCustomCommand cursor, "
                                    "because the current path \"{}\" is not valid!", path), Logger::Error);
            return;
        }
        if (_user_custom.contains(stdCursor)) {
            if (_user_custom.at(stdCursor).cursor)
                SDL_DestroySurface(_user_custom.at(stdCursor).cursor.get());
            _user_custom.at(stdCursor).cursor = std::unique_ptr<SDL_Surface>(new_cursor);
            _user_custom.at(stdCursor).hot_point = hot_point;
        } else {
            _user_custom.emplace(stdCursor, UserCustom(hot_point, new_cursor));
        }
    }

    Cursor::StdCursor Cursor::cursor() const {
        return _std_cursor;
    }

    void Cursor::setVisible(bool visible) {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        if (visible) {
            SDL_ShowCursor();
        } else {
            SDL_HideCursor();
        }
        _visible = visible;
    }

    bool Cursor::visible() const {
        return _visible;
    }

    void Cursor::unload() {
        if (_cursor) SDL_DestroyCursor(_cursor);
        if (_surface) SDL_DestroySurface(_surface);
    }

    Cursor::Cursor() {
        if (!SDL_HasMouse()) {
            Logger::log("Cursor: No mouse device!", Logger::Warn);
            return;
        }
        _cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
    }
}
