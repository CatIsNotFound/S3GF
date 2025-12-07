#pragma once
#ifndef MYENGINE_CURSOR_H
#define MYENGINE_CURSOR_H
#include "Logger.h"
#include "../Components.h"

namespace MyEngine {
    /**
     * @class Cursor
     * @brief 鼠标光标
     *
     * 用于全局控制鼠标光标的位置、样式
     * @note 需要使用 `global()` 获取全局
     */
    class Cursor {
    public:
        /**
         * @enum StdCursor
         * @brief 标准鼠标光标样式
         *
         * 获取系统下当前使用的鼠标光标
         */
        enum StdCursor {
            Default             = 0x00,
            Normal              = 0x00,
            Edit                = 0x01,
            Wait                = 0x02,
            Crosshair           = 0x03,
            Busy                = 0x04,
            Resize_NWSE         = 0x05,
            Resize_NESW         = 0x06,
            Resize_Horizontal   = 0x07,
            Resize_EW           = 0x07,
            Resize_Vertical     = 0x08,
            Resize_NS           = 0x08,
            Move                = 0x09,
            Not_Allowed         = 0x0a,
            Forbidden           = 0x0a,
            Hand                = 0x0b,
            Pointer             = 0x0b,
            Resize_NW           = 0x0c,
            Resize_N            = 0x0d,
            Resize_NE           = 0x0e,
            Resize_E            = 0x0f,
            Resize_SE           = 0x10,
            Resize_S            = 0x11,
            Resize_SW           = 0x12,
            Resize_W            = 0x13,
            Count               = 0x14,
            Custom              = 0x15
        };

        /**
         * @struct UserCustom
         * @brief 用户自定义鼠标光标数据
         *
         * 包含热点坐标（点击生效点）和光标表面资源
         */
        struct UserCustom {
            /**
             * @struct HotPoint
             * @brief 用于定义鼠标光标的热点坐标
             */
            struct HotPoint { int x, y; };
            HotPoint hot_point;
            std::shared_ptr<SDL_Surface> cursor;
            UserCustom(const HotPoint& hot_point, SSurface* cursor) {
                this->hot_point.x = hot_point.x;
                this->hot_point.y = hot_point.y;
                this->cursor = std::shared_ptr<SSurface>(cursor);
            }
            UserCustom(const UserCustom& copy) {
                hot_point.x = copy.hot_point.x;
                hot_point.y = copy.hot_point.y;
                cursor = copy.cursor;
            }
        };
        explicit Cursor(const Cursor&) = delete;
        explicit Cursor(Cursor&&) = delete;
        Cursor& operator=(const Cursor&) = delete;
        Cursor& operator=(Cursor&&) = delete;
        ~Cursor() {
            unload();
        }
        /**
         * @brief 获取鼠标光标全局
         */
        static Cursor* global() {
            if (!_instance) {
                _instance = std::unique_ptr<Cursor>(new Cursor());
            }
            return _instance.get();
        }
        /**
         * @brief 获取鼠标光标在显示器中所在的位置
         * @return 返回全局坐标
         * @see position
         * @see move
         */
        Vector2 globalPosition() const {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            Vector2 _pos;
            SDL_GetGlobalMouseState(&_pos.x, &_pos.y);
            return _pos;
        }
        /**
         * @brief 获取鼠标光标在获得焦点的窗口下的位置
         * @return 返回鼠标光标相对获得焦点窗口下的位置
         * @see globalPosition
         * @see move
         */
        Vector2 position() const {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            Vector2 _pos;
            SDL_GetMouseState(&_pos.x, &_pos.y);
            return _pos;
        }
        /**
         * @brief 获取鼠标当前聚焦于哪个窗口
         * @return 返回鼠标所在的窗口 ID
         */
        uint64_t focusOn() const {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            return SDL_GetWindowID(SDL_GetMouseFocus());
        }
        /**
         * @brief 移动鼠标光标至指定位置
         * @param pos 指定位置
         * @param window 指定窗口
         * @note 若不指定 window 参数，将默认以全局显示器屏幕为主。
         * @see position
         * @see globalPosition
         */
        void move(const Vector2& pos, const Window* window = nullptr) {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            if (window) {
                SDL_WarpMouseInWindow(window->self(), pos.x, pos.y);
            } else {
                SDL_WarpMouseGlobal(pos.x, pos.y);
            }
        }
        /**
         * @brief 移动光标至指定位置
         * @param x 指定横坐标
         * @param y 指定纵坐标
         * @param window 指定窗口
         * @note 若不指定 window 参数，将默认以全局显示器屏幕为主。
         * @see position
         * @see globalPosition
         */
        void move(float x, float y, const Window* window = nullptr) {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            if (window) {
                SDL_WarpMouseInWindow(window->self(), x, y);
            } else {
                SDL_WarpMouseGlobal(x, y);
            }
        }
        /**
         * @brief 将鼠标光标移动至画面中心点
         * @param window 指定窗口
         * @note 若不设置 window 参数，将默认以全局显示器屏幕为主
         */
        void moveToCenter(const Window* window = nullptr) {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
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
        /**
         * @brief 设置鼠标光标
         * @param cursor 指定光标类型
         * @code
         * Cursor::global()->setCursor(Cursor::Hand);
         * @endcode
         * @note 对于已经开启自定义鼠标光标的情况，将会改变对应类型的自定义光标；
         * 若指定对应的光标类型没有定义鼠标光标，则会使用系统对应的鼠标光标！
         * @see cursor
         * @see userCustomEnabled
         * @see StdCursor
         */
        void setCursor(const StdCursor& cursor) {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
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
            _cursor = SDL_CreateSystemCursor(SStdCursor(_std_cursor));
            SDL_SetCursor(_cursor);
        }
        /**
         * @brief 设置自定义鼠标光标
         * @param path 指定路径下加载鼠标光标
         * @param hot_x 中心点横坐标
         * @param hot_y 中心点纵坐标
         * @note 若指定的 `path` 参数无效，则无法设置！
         *
         * 从指定路径加载图像作为自定义鼠标光标，并设置热点坐标。
         */
        void setCursor(const std::string &path, int hot_x, int hot_y) {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            SDL_Surface *temp = IMG_Load(path.data());
            if (!temp) {
                Logger::log(std::format("[ERROR] Can't load image while setting cursor: {}", path), Logger::ERROR);
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
        /**
         * @brief 是否允许使用自定义鼠标光标
         * @param enabled 启用/禁用
         */
        void setUserCustomEnabled(bool enabled) {
            _custom_cursor = enabled;
        }
        /**
         * @brief 当前是否使用自定义鼠标光标
         *
         */
        bool userCustomEnabled() const {
            return _custom_cursor;
        }
        /**
         * @brief 设置自定义鼠标光标
         * @param stdCursor 指定标准鼠标类型
         * @param path      指定本地路径
         * @param hot_point 指定热点
         * @see setUserCustomEnabled
         * @see userCustomEnabled
         */
        void setCustomCursor(const StdCursor& stdCursor, const std::string& path, const UserCustom::HotPoint& hot_point) {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            auto new_cursor = IMG_Load(path.c_str());
            if (!new_cursor) {
                Logger::log(std::format("[ERROR] Can't set custom cursor, "
                                        "because the current path \"{}\" is not valid!", path), Logger::ERROR);
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
        /**
         * @brief 获取当前鼠标光标样式
         *
         */
        StdCursor cursor() const {
            return _std_cursor;
        }
        /**
         * @brief 显示/隐藏鼠标光标
         * @param visible 决定是否显示鼠标光标
         * @see visible
         */
        void setVisible(bool visible) {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            if (visible) {
                SDL_ShowCursor();
            } else {
                SDL_HideCursor();
            }
            _visible = visible;
        }
        /**
         * @brief 获取当前鼠标光标是否显示
         * @see setVisible
         */
        bool visible() const {
            return _visible;
        }
        void unload() {
            if (_cursor) SDL_DestroyCursor(_cursor);
            if (_surface) SDL_DestroySurface(_surface);
        }

    private:
        explicit Cursor() {
            if (!SDL_HasMouse()) {
                Logger::log("Cursor: No mouse device!", Logger::WARN);
            }
            _cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_DEFAULT);
        }
        StdCursor _std_cursor{Default};
        SCursor* _cursor{nullptr};
        SSurface* _surface{nullptr};
        bool _custom_cursor{false};
        std::map<StdCursor, UserCustom> _user_custom;
        bool _visible{true};
        static std::unique_ptr<Cursor> _instance;
    };

    inline std::unique_ptr<Cursor> Cursor::_instance{nullptr};
}


#endif //MYENGINE_CURSOR_H
