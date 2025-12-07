
#pragma once
#ifndef MYENGINE_BASIC_H
#define MYENGINE_BASIC_H

#include "Libs.h"
/// Use `MyEngine::delay()` is better.
#define SLEEP(sec) std::this_thread::sleep_for(std::chrono::seconds(sec))
/// Use `MyEngine::delayMS()` is better.
#define SLEEP_MS(millisec) std::this_thread::sleep_for(std::chrono::milliseconds(millisec))
/// Use `MyEngine::delayNS()` is better.
#define SLEEP_NS(nanosec) std::this_thread::sleep_for(std::chrono::nanoseconds(nanosec))

using SRenderer     = SDL_Renderer;
using SSurface      = SDL_Surface;
using STexture      = SDL_Texture;
using SEvent        = SDL_Event;
using SWinEvent     = SDL_WindowEvent;
using SWindow       = SDL_Window;
using SWindowID     = SDL_WindowID;
using SColor        = SDL_Color;
using SCursor       = SDL_Cursor;
using SStdCursor    = SDL_SystemCursor;
using SAudioSpec    = SDL_AudioSpec;

using StringList    = std::vector<std::string>;



#include "Utils/Logger.h"

/**
 * @namespace MyEngine
 * @brief MyEngine Namespace
 * 
 * All libraries will be stored in this namespace.
 */
namespace MyEngine {
    /**
     * @namespace StdColor
     * @brief Predefined Standard Color (40 colors)
     */
    namespace StdColor {
        constexpr SColor Black = {0, 0, 0, 255};
        constexpr SColor White = {255, 255, 255, 255};
        constexpr SColor Red = {255, 0, 0, 255};
        constexpr SColor Green = {0, 255, 0, 255};
        constexpr SColor Blue = {0, 0, 255, 255};
        constexpr SColor Yellow = {255, 255, 0, 255};
        constexpr SColor Cyan = {0, 255, 255, 255};
        constexpr SColor Magenta = {255, 0, 255, 255};

        constexpr SColor DarkRed = {128, 0, 0, 255};
        constexpr SColor FireRed = {200, 0, 0, 255};
        constexpr SColor BrightRed = {255, 50, 50, 255};
        constexpr SColor Coral = {255, 127, 80, 255};
        constexpr SColor Salmon = {250, 128, 114, 255};
        constexpr SColor LightPink = {255, 174, 185, 255};

        constexpr SColor DarkGreen = {0, 128, 0, 255};
        constexpr SColor ForestGreen = {34, 139, 34, 255};
        constexpr SColor LimeGreen = {50, 205, 50, 255};
        constexpr SColor Mint = {189, 252, 201, 255};
        constexpr SColor LightGreen = {144, 238, 144, 255};
        constexpr SColor Olive = {128, 128, 0, 255};

        constexpr SColor DarkBlue = {0, 0, 128, 255};
        constexpr SColor Navy = {0, 32, 64, 255};
        constexpr SColor SkyBlue = {135, 206, 235, 255};
        constexpr SColor LightBlue = {173, 216, 230, 255};
        constexpr SColor RoyalBlue = {65, 105, 225, 255};
        constexpr SColor Indigo = {75, 0, 130, 255};

        constexpr SColor Gold = {255, 215, 0, 255};
        constexpr SColor Orange = {255, 165, 0, 255};
        constexpr SColor DarkOrange = {255, 140, 0, 255};
        constexpr SColor Peach = {255, 218, 185, 255};
        constexpr SColor Amber = {255, 193, 37, 255};
        constexpr SColor Khaki = {240, 230, 140, 255};

        constexpr SColor Purple = {128, 0, 128, 255};
        constexpr SColor DarkPurple = {72, 61, 139, 255};
        constexpr SColor Violet = {148, 0, 211, 255};
        constexpr SColor Fuchsia = {255, 0, 255, 255};
        constexpr SColor Pink = {255, 192, 203, 255};
        constexpr SColor Lavender = {230, 230, 250, 255};

        constexpr SColor Gray = {128, 128, 128, 255};
        constexpr SColor LightGray = {200, 200, 200, 255};
        constexpr SColor DarkGray = {64, 64, 64, 255};
    }

    /// Set how many seconds to delay on this process.
    inline void delay(uint64_t sec) { std::this_thread::sleep_for(std::chrono::seconds(sec)); }
    /// Set how many milliseconds to delay on this process.
    inline void delayMS(uint64_t ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
    /// Set how many nanoseconds to delay on this process.
    inline void delayNS(uint64_t ns) { std::this_thread::sleep_for(std::chrono::nanoseconds(ns)); }

    /// For EventSystem while using IDGenerator.
    constexpr size_t NewEventID = 1;
    /// For EventSystem while using IDGenerator.
    constexpr size_t NewGlobalEventID = 2;

    /// ID Generator
    class IDGenerator {
    public:
        IDGenerator() = delete;
        ~IDGenerator() = delete;
        IDGenerator(const IDGenerator&) = delete;
        IDGenerator(IDGenerator&&) = delete;
        IDGenerator& operator=(const IDGenerator&) = delete;
        IDGenerator& operator=(IDGenerator&&) = delete;
        static size_t newIDGenerator() {
            _id_list.emplace_back(0);
            return _id_list.size() - 1;
        }
        static uint64_t getNewEventID() {
            return ++_id_list[NewEventID];
        }
        static uint64_t getNewGlobalEventID() {
            return ++_id_list[NewGlobalEventID];
        }
        static uint64_t getID(size_t index = 0) {
            if (index >= _id_list.size()) {
                Logger::log(std::format("IDGenerator: The index of {} is not exist!", index));
                return 0;
            }
            return ++_id_list[index];
        }
    private:
        static std::vector<uint64_t> _id_list;
    };
    inline std::vector<uint64_t> IDGenerator::_id_list(3, 0);

    struct GeometryF;
    /**
     * @struct Geometry
     * @brief Including position and size.
     */
    struct Geometry {
        int x, y, width, height;

        Geometry() : x(0), y(0), width(0), height(0) {}

        Geometry(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}

        void setGeometry(int x, int y, int width, int height) {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }

        void setPosition(int x, int y) {
            this->x = x;
            this->y = y;
        }

        void resize(int width, int height) {
            this->width = width;
            this->height = height;
        }
    };

    /**
     * @struct Vector2
     * @brief 向量坐标
     *
     * 用于指定某对象的位置
     */
    struct Vector2 {
        float x, y;

        explicit Vector2() : x(0.0f), y(0.0f) {}

        Vector2(float x, float y) : x(x), y(y) {}

        void reset(float x, float y) {
            this->x = x;
            this->y = y;
        }

        void reset(const Vector2 &vector2) {
            this->x = vector2.x;
            this->y = vector2.y;
        }

        Vector2 operator+(const Vector2 &v) const { return {this->x + v.x, this->y + v.y}; }

        Vector2 operator-(const Vector2 &v) const { return {this->x - v.x, this->y - v.y}; }

        Vector2 operator*(const Vector2 &v) const { return {this->x * v.x, this->y * v.y}; }

        Vector2 operator*(float v) const { return {this->x * v, this->y * v}; };

        Vector2 operator/(const Vector2 &v) const { return {this->x / v.x, this->y / v.y}; }

        Vector2 operator/(float v) const { return (v != 0) ? Vector2(this->x / v, this->y / v) : *this; }

        void operator+=(const Vector2 &v) {
            this->x += v.x;
            this->y += v.y;
        }

        void operator-=(const Vector2 &v) {
            this->x -= v.x;
            this->y -= v.y;
        }

        void operator*=(const Vector2 &v) {
            this->x *= v.x;
            this->y *= v.y;
        }

        void operator*=(float v) {
            this->x *= v;
            this->y *= v;
        }

        void operator/=(const Vector2 &v) {
            this->x /= v.x;
            this->y /= v.y;
        }

        void operator/=(float v) {
            if (v == 0) return;
            this->x /= v;
            this->y /= v;
        }

        bool operator==(const Vector2 &v) const {
            return ((this->x != v.x) ? false : ((this->y != v.y) ? false : true));
        }

        bool operator!=(const Vector2 &v) const {
            return ((this->x != v.x) ? true : ((this->y != v.y) ? true : false));
        }

        bool operator>(const Vector2 &v) const {
            return (this->x > v.x && this->y > v.y);
        }

        bool operator>=(const Vector2 &v) const {
            return (this->x >= v.x && this->y >= v.y);
        }

        bool operator<(const Vector2 &v) const {
            return (this->x < v.x && this->y < v.y);
        }

        bool operator<=(const Vector2 &v) const {
            return (this->x <= v.x && this->y <= v.y);
        }

        /**
         * @brief 判断两个坐标位置是否大致相等
         * @param v         指定另一个坐标
         * @param EPISON    允许的误差数 （默认 0.000001f 误差）
         * @retval true  表示相等
         * @retval false 表示不相等
         */
        [[nodiscard]] bool isEqual(const Vector2 &v, const float EPISON = 1e-6f) const {
            return ((this->x >= v.x - EPISON && this->x <= v.x + EPISON) &&
                    (this->y >= v.y - EPISON && this->y <= v.y + EPISON));
        }
    };

    /**
     * @struct Size
     * @brief 尺寸、大小
     *
     * 用于调整对象的尺寸、大小
     */
    struct Size {
        float width, height;

        explicit Size() : width(0.0f), height(0.0f) {}

        Size(float width, float height) : width(width), height(height) {}

        /**
         * @brief 重新设置新的尺寸
         * @param width
         * @param height
         */
        void reset(float width, float height) {
            this->width = width;
            this->height = height;
        }

        void reset(const Size &size) {
            this->width = size.width;
            this->height = size.height;
        }

        Size operator+(const Size &s) const { return {this->width + s.width, this->height + s.height}; }

        Size operator-(const Size &s) const { return {this->width - s.width, this->height - s.height}; }

        Size operator*(const Size &s) const { return {this->width * s.width, this->height * s.height}; }

        Size operator*(float v) const { return {this->width * v, this->height * v}; }

        Size operator/(const Size &s) const { return {this->width / s.width, this->height / s.height}; }

        Size operator/(float v) const { return (v == 0) ? *this : Size(this->width / v, this->height / v); }

        void operator+=(const Size &s) {
            this->width += s.width;
            this->height += s.height;
        }

        void operator-=(const Size &s) {
            this->width -= s.width;
            this->height -= s.height;
        }

        void operator*=(const Size &s) {
            this->width *= s.width;
            this->height *= s.height;
        }

        void operator*=(float v) {
            this->width *= v;
            this->height *= v;
        }

        void operator/=(const Size &s) {
            this->width /= s.width;
            this->height /= s.height;
        }

        void operator/=(float v) {
            if (v == 0) return;
            this->width /= v;
            this->height /= v;
        }

        bool operator==(const Size &s) const {
            return (this->width != s.width) ? false : ((this->height != this->height) ? false : true);
        }

        bool operator!=(const Size &s) const {
            return (this->width != s.width) ? true : ((this->height != this->height) ? true : false);
        }

        bool operator>(const Size &s) const {
            return (this->width > s.width && this->height > s.height);
        }

        bool operator>=(const Size &s) const {
            return (this->width >= s.width && this->height >= s.height);
        }

        bool operator<(const Size &s) const {
            return (this->width < s.width && this->height < s.height);
        }

        bool operator<=(const Size &s) const {
            return (this->width <= s.width && this->height <= s.height);
        }
    };

    /**
     * @struct GeometryF
     * @brief 位置、大小
     *
     * 针对于 Vector2 和 Size 组成，用于调整位置、大小
     */
    struct GeometryF {
        Vector2 pos;
        Size size;

        explicit GeometryF(float x, float y, float w, float h)
                : pos(x, y), size(w, h) {}

        explicit GeometryF(const Vector2 &pos, const Size &size)
                : pos(pos), size(size) {}

        explicit GeometryF(Vector2 &&pos, Size &&size)
                : pos(pos), size(size) {}

        explicit GeometryF() : pos(0, 0), size(0, 0) {}

        void reset(const Vector2 &pos, const Size &size) {
            this->pos.x = pos.x;
            this->pos.y = pos.y;
            this->size.width = size.width;
            this->size.height = size.height;
        }

        void reset(float x, float y, float width, float height) {
            this->pos.x = x;
            this->pos.y = y;
            this->size.width = width;
            this->size.height = height;
        }

        void reset(const GeometryF& geometry) {
            this->pos.x = geometry.pos.x;
            this->pos.y = geometry.pos.y;
            this->size.width = geometry.size.width;
            this->size.height = geometry.size.height;
        }

        void resetPos(const Vector2 &pos) {
            this->pos.x = pos.x;
            this->pos.y = pos.y;
        }

        void resetSize(const Size &size) {
            this->size.width = size.width;
            this->size.height = size.height;
        }
    };

    inline Geometry toGeometryInt(const GeometryF& geo) {
        return {(int)geo.pos.x, (int)geo.pos.y, (int)geo.size.width, (int)geo.size.height};
    }

    inline GeometryF toGeometryFloat(const Geometry& geo) {
        return GeometryF((float)geo.x, (float)geo.y, (float)geo.width, (float)geo.height);
    }

    /**
     * @struct Matrix2D
     * @brief 二维矩阵
     * @since v1.1.0-alpha
     *
     * 支持使用基本数据类型及简单的结构体进行存储。
     */
    template<typename T>
    struct Matrix2D {
    private:
        
        std::vector<T> _datas;
        
        uint32_t _row;
        
        uint32_t _col;
        
        std::function<void(T &)> _deleter;
    public:
        using iterator = typename std::vector<T>::iterator;
        using constIterator = typename std::vector<T>::const_iterator;

        struct Position {
            uint32_t row;
            uint32_t col;

            Position() : row(0), col(0) {}

            Position(uint32_t row, uint32_t col) : row(row), col(col) {}

            bool isValid() const { return row && col; }

            bool operator==(const Position &p) const { return (row == p.row && col == p.col); }

            bool operator!=(const Position &p) const { return (row != p.row || col != p.col); }

            bool operator>(const Position &p) const { return (row >= p.row && col > p.col); }

            bool operator<(const Position &p) const { return (row <= p.row && col < p.col); }

            bool operator>=(const Position &p) const { return (row >= p.row && col >= p.col); }

            bool operator<=(const Position &p) const { return (row <= p.row && col <= p.col); }
        };

        /**
         * @brief 创建一个空白的二维矩阵（没有任何数据）
         */
        explicit Matrix2D() : _row(0), _col(0), _datas() {}

        /**
         * @brief 创建指定行列的二维矩阵
         * @param row 行
         * @param col  列
         * @param deleter 删除器（若需要删除指针等情况时指定）
         */
        Matrix2D(uint32_t row, uint32_t col, const std::function<void(T &)> &deleter = {});

        /**
         * @brief 创建指定行列与默认值的二维矩阵
         * @param row  行
         * @param col   列
         * @param value 默认值（用于填充所有数据）
         * @param deleter 删除器（若需要删除指针等情况时指定）
         */
        Matrix2D(uint32_t row, uint32_t col, const T &value, const std::function<void(T &)> &deleter = {});

        /**
         * @brief 复制原有的二维矩阵
         * @param matrix    指定二维矩阵
         */
        Matrix2D(const Matrix2D<T> &matrix);

        ~Matrix2D();

        /**
         * @brief 设置删除器
         * @param function  指定函数
         *
         * 当此类析构时，将调用删除器以删除指针！
         */
        void setDeleter(const std::function<void(T &)> &function);

        /**
         * @brief 填充所有值
         * @param value 指定值
         */
        void fill(const T &value);

        /**
         * @brief 填充范围内的值
         * @param start 指定开始位置（行列）
         * @param end   指定结束位置（行列）
         * @param value 指定填充的值
         */
        bool fillN(const Matrix2D::Position &start, const Matrix2D::Position &end, const T &value);

        /**
         * @brief 重新调整新的大小
         * @param line 行
         * @param col  列
         */
        void resize(uint32_t line, uint32_t col);

        /**
         * @brief 获取矩阵中指定行列的数据
         * @param row 行
         * @param col  列
         * @return 返回对应行列下的数据
         * @see get
         */
        T &at(uint32_t row, uint32_t col);

        /**
         * @brief 获取矩阵中指定行列的数据
         * @param row 行
         * @param col  列
         * @return 返回对应行列下的数据
         * @note 区别于 `at`，此函数为常量版本，无法修改里面的数据！
         * @see at
         */
        const T &get(uint32_t row, uint32_t col);

        /**
         * @brief 获取当前矩阵的总行数
         * @return 返回对应的行数
         */
        [[nodiscard]] uint32_t rows() const;

        /**
         * @brief 获取当前矩阵的总列数
         * @return 返回对应的列数
         */
        [[nodiscard]] uint32_t cols() const;

        Matrix2D operator+(const Matrix2D<T> &other) const;

        Matrix2D operator-(const Matrix2D<T> &other) const;

        /**
         * @brief 矩阵乘法
         * @param other     指定矩阵，其指定的行数必需与现有的列数相等
         *
         * 将矩阵里的所有值进行乘法操作
         * @note 当前仅支持整数、浮点数运算，不支持其它数据类型的运算！
         * @note 两个矩阵必需分别为 m * n, n * p 的大小才可用！
         */
        Matrix2D operator*(const Matrix2D<T> &other) const;

        bool operator==(const Matrix2D<T> &other) const;

        bool operator!=(const Matrix2D<T> &other) const;

        T &operator[](uint32_t index);

        T &operator()(uint32_t row, uint32_t col);

        iterator begin() { return _datas.begin(); }

        iterator end() { return _datas.end(); }

        constIterator begin() const { return _datas.begin(); }

        constIterator end() const { return _datas.end(); }

        /**
         * @brief 全局相加
         * @param value     指定值
         * @param function  函数（对于复杂的数据类型，此参数必需指定）
         *
         * 将矩阵里的所有值进行相加操作
         */
        void add(T &value, const std::function<void(T &, T &)> &function = {});

        /**
         * @brief 全局相加
         * @param value     指定值
         * @param function  函数（对于复杂的数据类型，此参数必需指定）
         *
         * 将矩阵里的所有值进行相加操作
         */
        void add(T &&value, const std::function<void(T &, T &)> &function = {});

        /**
         * @brief 全局相减
         * @param value 指定值
         * @param function  函数（对于复杂的数据类型，此参数必需指定）
         *
         * 将矩阵里的所有值进行相减操作
         */
        void minus(T &value, const std::function<void(T &, T &)> &function = {});

        /**
         * @brief 全局相减
         * @param value 指定值
         * @param function  函数（对于复杂的数据类型，此参数必需指定）
         *
         * 将矩阵里的所有值进行相减操作
         */
        void minus(T &&value, const std::function<void(T &, T &)> &function = {});

        /**
         * @brief 全局乘法
         * @param value     指定值
         * @param function  函数（对于复杂的数据类型，此参数必需指定）
         *
         * 将矩阵里的所有值进行相乘操作
         */
        void times(T &value, const std::function<void(T &, T &)> &function = {});

        /**
         * @brief 全局点乘
         * @param value     指定值
         * @param function  函数（对于复杂的数据类型，此参数必需指定）
         *
         * 将矩阵里的所有值进行点乘操作
         */
        void times(T &&value, const std::function<void(T &, T &)> &function = {});

        /**
         * @brief 矩阵点乘
         * @param other     指定矩阵
         * @param function  函数（对于复杂的数据类型，此参数必需指定）
         * @note 两个矩阵的大小必需完全一样（即行列必需相等）！
         */
        void times(const Matrix2D<T> &other, const std::function<void(T &, const T &)> &function = {});

        /**
         * @brief 矩阵乘法
         *
         * 将矩阵里的所有值进行乘法操作
         * @param other     指定矩阵，其指定的行数必需与现有的列数相等
         * @note 当前仅支持整数、浮点数运算，不支持其它数据类型的运算！
         * @note 两个矩阵必需分别为 m * n, n * p 的大小才可用！
         */
        void multiply(const Matrix2D<T> &other);

        /**
         * @brief 转置矩阵
         *
         * 将原有的 m * n 大小的矩阵转置为 n * m 大小的矩阵。
         * @see rotate
         */
        void transpose();

        /**
         * @brief 翻转矩阵
         *
         * 当选择任何一个方向的逆序时，都会发生交换。
         * @param reverse_row 行与行之间逆序（垂直翻转）
         * @param reverse_col  列与列之间逆序（水平翻转）
         * @note 当两个参数都为 `true` 时，矩阵将完全逆序
         * @see rotate
         */
        void reverse(bool reverse_row = true, bool reverse_col = false);

        /**
         * @brief 旋转矩阵
         * @param turn_right 是否向右旋转 90°，反之向左旋转 90°
         * @see reverse
         * @see transpose
         */
        void rotate(bool turn_right = true);

        /**
         * @brief 切割指定行矩阵
         * @param start_row     起始行
         * @param end_row       结束行
         * @return 返回矩阵行位于 `[start_row, end_row)` 区间内的所有行矩阵
         */
        Matrix2D splitRows(uint32_t start_row, uint32_t end_row);

        /**
         * @brief 切割指定列矩阵
         * @param start_col     起始列
         * @param end_col       结束列
         * @return 返回矩阵行位于 `[start_col, end_col)` 区间内的所有列矩阵
         */
        Matrix2D splitCols(uint32_t start_col, uint32_t end_col);

        /**
         * @brief 切割矩阵
         *
         * 切割原有的矩阵，根据 `start_pos` 位置开始，取 `rows * cols` 个数据并创建成新的矩阵。
         * @param rows      新的行数
         * @param cols      新的列数
         * @param start_pos 从哪个位置开始
         * @return 返回新的大小为 `(rows * cols)` 的二维矩阵。
         *
         * @note 从 `start_pos` 位置起，若取得数据的总个数小于新的矩阵大小，则剩余部分自动填充为空数据。
         * @note 指定的 `start_pos` 位置若超出原有矩阵的范围，将返回空矩阵。
         */
        Matrix2D split(uint32_t rows, uint32_t cols, const Position &start_pos);

        /**
         * @brief 按照矩形的方式切割矩阵
         *
         * 根据起始位置和结束位置包围成一个矩形，并将其切割成独立的矩阵。
         * @param start_pos     起始位置
         * @param end_pos       结束位置
         * @return 返回切割后的矩阵。
         *
         */
        Matrix2D split(Matrix2D::Position start_pos, Matrix2D::Position end_pos);

        /**
         * @brief 逆矩阵
         * @return 返回新的矩阵，用于存储求得的结果
         * @note 当前仅支持整数、浮点数运算，不支持其它数据类型的运算！
         * @note 两个矩阵的大小必需完全一致！否则将返回空矩阵！
         */
        Matrix2D<T> inverse();
    };

    template<typename T>
    void Matrix2D<T>::setDeleter(const std::function<void(T &)> &function) {
        _deleter = function;
    }

    template<typename T>
    Matrix2D<T>::Matrix2D(uint32_t row, uint32_t col, const std::function<void(T &)> &deleter)
            : _row(row), _col(col), _deleter(deleter) {
        _datas.resize(_row * _col);
    }

    template<typename T>
    Matrix2D<T>::Matrix2D(uint32_t row, uint32_t col, const T &value, const std::function<void(T &)> &deleter)
            : _row(row), _col(col), _deleter(deleter) {
        _datas.resize(_row * _col);
        fill(value);
    }

    template<typename T>
    Matrix2D<T>::Matrix2D(const Matrix2D<T> &matrix)
            : _row(matrix._row), _col(matrix._col), _deleter(matrix._deleter) {
        _datas = matrix._datas;
    }

    template<typename T>
    Matrix2D<T>::~Matrix2D() {
        if (_deleter) {
            for (auto &_d: _datas) {
                _deleter(_d);
            }
        }
    }

    template<typename T>
    void Matrix2D<T>::fill(const T &value) {
        std::fill(_datas.begin(), _datas.end(), value);
    }

    template<typename T>
    bool Matrix2D<T>::fillN(const Matrix2D::Position &start, const Matrix2D::Position &end, const T &value) {
        auto st = start.row * _col + start.col, ed = end.row * _col + end.col;
        if (ed >= _datas.size()) {
            SDL_Log("[ERROR] One of the specified position is not valid!");
            return false;
        }
        std::fill(_datas.begin() + st, _datas.begin() + ed, value);
        return true;
    }

    template<typename T>
    void Matrix2D<T>::resize(uint32_t line, uint32_t col) {
        if (_row * _col != line * col) _datas.resize(line * col);
        _row = line;
        _col = col;
    }

    template<typename T>
    T &Matrix2D<T>::at(uint32_t row, uint32_t col) {
        auto idx = row * _col + col;
        if (idx >= _datas.size()) throw std::out_of_range("[FATAL] The specified position is out of range!");
        return _datas.at(idx);
    }

    template<typename T>
    const T &Matrix2D<T>::get(uint32_t row, uint32_t col) {
        auto idx = row * _col + col;
        if (idx >= _datas.size()) throw std::out_of_range("[FATAL] The specified position is out of range!");
        return _datas.at(idx);
    }

    template<typename T>
    uint32_t Matrix2D<T>::rows() const {
        return _row;
    }

    template<typename T>
    uint32_t Matrix2D<T>::cols() const {
        return _col;
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::operator+(const Matrix2D<T> &other) const {
        if ((_row == other._row) && (_col == other._col)) {
            Matrix2D<T> _ret(_row, _col);
            for (size_t _idx = 0; _idx < _datas.size(); ++_idx) {
                _ret[_idx] = _datas[_idx] + other._datas[_idx];
            }
            return _ret;
        } else {
            SDL_Log("[ERROR] Matrix dimensions mismatch!\nOriginal: (%d, %d), Specified: (%d, %d)",
                    _row, _col, other._row, other._col);
            return Matrix2D<T>();
        }
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::operator-(const Matrix2D<T> &other) const {
        if ((_row == other._row) && (_col == other._col)) {
            Matrix2D<T> _ret(_row, _col);
            for (size_t _idx = 0; _idx < _datas.size(); ++_idx) {
                _ret[_idx] = _datas[_idx] - other._datas[_idx];
            }
            return _ret;
        } else {
            SDL_Log("[ERROR] Matrix dimensions mismatch!\nOriginal: (%d, %d), Specified: (%d, %d)",
                    _row, _col, other._row, other._col);
            return Matrix2D<T>();
        }
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::operator*(const Matrix2D<T> &other) const {
        if (_col != other._row) {
            SDL_Log("[ERROR] Matrix dimensions incompatible for multiplication!");
            return Matrix2D<T>();
        }
        if constexpr (!std::is_integral_v<std::decay_t<T>> &&
                      !std::is_floating_point_v<std::decay_t<T>>) {
            static_assert(!std::is_integral_v<std::decay_t<T>> &&
                          !std::is_floating_point_v<std::decay_t<T>>,
                          "[FATAL] Can't support the current data type!");
        }
        Matrix2D<T> result(_row, other._col, other._deleter);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t k = 0; k < _col; ++k) {
                const T &a_ik = _datas[i * _col + k];
                for (size_t j = 0; j < other._col; ++j) {
                    result[i * other._col + j] += a_ik * other._datas[k * other._col + j];
                }
            }
        }
        return result;
    }

    template<typename T>
    bool Matrix2D<T>::operator==(const Matrix2D<T> &other) const {
        if (_row != other._row || _col != other._col) return false;
        for (size_t i = 0; i < _datas.size(); ++i) {
            if (_datas[i] != other._datas[i]) return false;
        }
        return true;
    }

    template<typename T>
    bool Matrix2D<T>::operator!=(const Matrix2D<T> &other) const {
        if (_row != other._row || _col != other._col) return true;
        for (size_t i = 0; i < _datas.size(); ++i) {
            if (_datas[i] != other._datas[i]) return true;
        }
        return false;
    }

    template<typename T>
    T &Matrix2D<T>::operator[](uint32_t index) {
        return _datas[index];
    }

    template<typename T>
    T &Matrix2D<T>::operator()(uint32_t row, uint32_t col) {
        auto idx = row * _col + col;
        if (idx >= _datas.size()) throw std::out_of_range("[FATAL] The specified position is out of range!");
        return _datas[idx];
    }

    template<typename T>
    void Matrix2D<T>::add(T &value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v += value;
            } else {
                SDL_Log("[ERROR] Unsupported data type!\np.s: Did you forget to specify how to add values?");
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::add(T &&value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v += value;
            } else {
                SDL_Log("[ERROR] Unsupported data type!\n"
                        "p.s: Did you forget to specify how to add values?");
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::minus(T &value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v -= value;
            } else {
                SDL_Log("[ERROR] Unsupported data type!\n"
                        "p.s: Did you forget to specify how to minus values?");
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::minus(T &&value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v -= value;
            } else {
                SDL_Log("[ERROR] Unsupported data type!\n"
                        "p.s: Did you forget to specify how to minus values?");
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::times(T &value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v *= value;
            } else {
                SDL_Log("[ERROR] Unsupported data type!\n"
                        "p.s: Did you forget to specify how to times values?");
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::times(T &&value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v *= value;
            } else {
                SDL_Log("[ERROR] Unsupported data type!\n"
                        "p.s: Did you forget to specify how to times values?");
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::times(const Matrix2D<T> &other, const std::function<void(T &, const T &)> &function) {
        if ((_row == other._row) && (_col == other._col)) {
            if (function) {
                for (size_t i = 0; i < _datas.size(); ++i) {
                    function(_datas[i], other._datas[i]);
                }
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                for (size_t i = 0; i < _datas.size(); ++i) {
                    _datas[i] *= other._datas[i];
                }
            } else {
                SDL_Log("[ERROR] Unsupported data type!\n"
                        "p.s: Did you forget to specify how to times values?");
            }
        } else {
            SDL_Log("[ERROR] Matrix dimensions mismatch!\nOriginal: (%u, %u), Specified: (%u, %u)",
                    _row, _col, other._row, other._col);
        }
    }

    template<typename T>
    void Matrix2D<T>::multiply(const Matrix2D<T> &other) {
        if (_col != other._row) {
            SDL_Log("[ERROR] Matrix dimensions incompatible for multiplication!");
            return;
        }
        std::vector<T> result(_row * other._col);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t k = 0; k < _col; ++k) {
                const T &a_ik = _datas[i * _col + k];
                for (size_t j = 0; j < other._col; ++j) {
                    result[i * other._col + j] += a_ik * other._datas[k * other._col + j];
                }
            }
        }
        _datas = std::move(result);
        _col = other._col;
    }

    template<typename T>
    void Matrix2D<T>::transpose() {
        if (!_row || !_col) return;
        if (_row == _col) {
            for (size_t i = 0; i < _row; ++i) {
                for (size_t j = i + 1; j < _col; ++j) {
                    std::swap(_datas[i * _col + j], _datas[j * _col + i]);
                }
            }
            return;
        }
        std::vector<T> temp_vec;
        temp_vec.reserve(_row * _col);
        for (size_t j = 0; j < _col; ++j) {
            for (size_t i = 0; i < _row; ++i) {
                temp_vec.push_back(_datas[i * _col + j]);
            }
        }
        std::swap(_datas, temp_vec);
        std::swap(_row, _col);
    }

    template<typename T>
    void Matrix2D<T>::reverse(bool reverse_row, bool reverse_col) {
        if (reverse_row && reverse_col) {
            const size_t M_SIZE = _datas.size() / 2;
            size_t _idx = 0, _last_idx = _datas.size() - 1;
            for (size_t i = 0; i < M_SIZE; ++i) {
                std::swap(_datas[_idx++], _datas[_last_idx--]);
            }
        } else if (reverse_col) {
            const size_t M_SIZE_IN_COL = _col / 2;
            for (size_t i = 0; i < _row; ++i) {
                for (size_t j = 0; j < M_SIZE_IN_COL; ++j) {
                    std::swap(_datas[i * _col + j], _datas[i * _col + _col - j - 1]);
                }
            }
        } else if (reverse_row) {
            const size_t M_SIZE_IN_LINE = _row / 2;
            for (size_t i = 0; i < _col; ++i) {
                for (size_t j = 0; j < M_SIZE_IN_LINE; ++j) {
                    std::swap(_datas[j * _col + i], _datas[(_row - j - 1) * _col + i]);
                }
            }
        }
    }

    template<typename T>
    void Matrix2D<T>::rotate(bool turn_right) {
        if (!_row || !_col) return;
        std::vector<T> temp;
        temp.reserve(_row * _col);
        if (turn_right) {
            for (size_t c = 0; c < _col; ++c) {
                for (size_t r = 0; r < _row; ++r) {
                    temp.emplace_back(_datas[(_row - r - 1) * _col + c]);
                }
            }
        } else {
            for (size_t c = 0; c < _col; ++c) {
                for (size_t r = 0; r < _row; ++r) {
                    temp.emplace_back(_datas[r * _col + (_col - c - 1)]);
                }
            }
        }
        std::swap(temp, _datas);
        if (_row != _col) std::swap(_row, _col);
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::splitRows(uint32_t start_row, uint32_t end_row) {
        if (start_row == end_row) {
            SDL_Log("[ERROR] The specified start row and end row cannot be the same!");
            return Matrix2D<T>();
        }
        if (end_row > _row) {
            SDL_Log("[ERROR] The specified end row exceeds the total rows of the original matrix!");
            return Matrix2D<T>();
        }
        if (start_row > end_row) std::swap(start_row, end_row);
        auto new_rows = end_row - start_row;
        Matrix2D<T> _ret(new_rows, _col);
        for (size_t i = 0; i < new_rows; ++i) {
            for (size_t j = 0; j < _col; ++j) {
                _ret[i * _col + j] = _datas[(start_row + i) * _col + j];
            }
        }
        return _ret;
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::splitCols(uint32_t start_col, uint32_t end_col) {
        if (start_col == end_col) {
            SDL_Log("[ERROR] The specified start col and end col cannot be the same!");
            return Matrix2D<T>();
        }
        if (end_col > _col) {
            SDL_Log("[ERROR] The specified end col exceeds the total cols of the original matrix!");
            return Matrix2D<T>();
        }
        if (start_col > end_col) std::swap(start_col, end_col);
        auto new_cols = end_col - start_col;
        Matrix2D<T> _ret(_row, new_cols);
        for (size_t i = 0; i < _row; ++i) {
            for (size_t j = 0; j < new_cols; ++j) {
                _ret[i * new_cols + j] = _datas[i * _col + j + start_col];
            }
        }
        return _ret;
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::split(uint32_t rows, uint32_t cols, const Matrix2D::Position &start_pos) {
        if (start_pos.row >= _row || start_pos.col >= _col) {
            SDL_Log("[ERROR] The specified start position is not valid!");
            return Matrix2D();
        }
        Matrix2D<T> _ret(rows, cols);
        auto size = rows * cols;
        auto start_index = start_pos.row * _col + start_pos.col;
        auto elapsed = _datas.size() - start_index;
        bool is_less = elapsed < size;
        if (!is_less) {
            for (size_t i = 0; i < _ret._datas.size(); ++i) {
                _ret[i] = _datas[start_index++];
            }
        } else {
            for (size_t i = 0; i < elapsed; ++i) {
                _ret[i] = _datas[start_index++];
            }
            for (size_t i = elapsed; i < _ret._datas.size(); ++i) {
                _ret[i] = {};
            }
        }
        return _ret;
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::split(Matrix2D::Position start_pos,
                                   Matrix2D::Position end_pos) {
        if (start_pos > end_pos) {
            auto temp = start_pos;
            start_pos = end_pos;
            end_pos = temp;
        }
        if (end_pos.row >= _row) {
            end_pos.row = _row - 1;
        }
        if (end_pos.col >= _col) {
            end_pos.col = _col - 1;
        }
        auto rows = end_pos.row - start_pos.row + 1;
        auto cols = end_pos.col - start_pos.col + 1;
        Matrix2D<T> _ret(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                _ret[i * _ret._col + j] = _datas[(start_pos.row + i) * _col + j + start_pos.col];
            }
        }
        return _ret;
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::inverse() {
        if (_row != _col) {
            SDL_Log("[ERROR] The specified matrix size is not matched!");
            return Matrix2D<T>();
        }
        if constexpr (!std::is_integral_v<std::decay_t<T>> &&
                      !std::is_floating_point_v<std::decay_t<T>>) {
            static_assert(!std::is_integral_v<std::decay_t<T>> &&
                          !std::is_floating_point_v<std::decay_t<T>>,
                          "[FATAL] Can't support the current data type!");
        }
        const uint32_t n = _row;
        Matrix2D<T> _ret(n, n);
        // 创建增广矩阵 [A|I]
        Matrix2D<T> augmented(n, 2 * n);
        // 填充增广矩阵：左侧为原矩阵，右侧为单位矩阵
        for (uint32_t i = 0; i < n; ++i) {
            for (uint32_t j = 0; j < n; ++j) {
                augmented(i, j) = _datas[i * _col + j];  // 原矩阵
                augmented(i, j + n) = (i == j) ? static_cast<T>(1) : static_cast<T>(0);  // 单位矩阵
            }
        }
        // 使用高斯-约旦消元法
        for (uint32_t i = 0; i < n; ++i) {
            // 寻找主元（部分主元选择）
            uint32_t max_row = i;
            for (uint32_t k = i + 1; k < n; ++k) {
                if (std::abs(augmented(k, i)) > std::abs(augmented(max_row, i))) {
                    max_row = k;
                }
            }
            // 如果主元为0，矩阵不可逆
            if (std::abs(augmented(max_row, i)) < static_cast<T>(1e-10)) {
                SDL_Log("[ERROR] Matrix is singular and cannot be inverted!");
                return Matrix2D<T>();
            }
            // 交换行
            if (max_row != i) {
                for (uint32_t j = 0; j < 2 * n; ++j) {
                    std::swap(augmented(i, j), augmented(max_row, j));
                }
            }
            // 将主元行标准化
            T pivot = augmented(i, i);
            for (uint32_t j = 0; j < 2 * n; ++j) {
                augmented(i, j) /= pivot;
            }
            // 消去其他行的当前列
            for (uint32_t k = 0; k < n; ++k) {
                if (k != i && std::abs(augmented(k, i)) > static_cast<T>(1e-10)) {
                    T factor = augmented(k, i);
                    for (uint32_t j = 0; j < 2 * n; ++j) {
                        augmented(k, j) -= factor * augmented(i, j);
                    }
                }
            }
        }
        // 提取逆矩阵（增广矩阵的右侧部分）
        for (uint32_t i = 0; i < n; ++i) {
            for (uint32_t j = 0; j < n; ++j) {
                _ret(i, j) = augmented(i, j + n);
            }
        }
        return _ret;
    }
}

#include "Algorithm/Draw.h"

namespace MyEngine {
    /**
     * @namespace Graphics
     * @brief 基本图形
     *
     * 包含所有基本图形，如：点、线段、矩形、椭圆等基本图形。
     */
    namespace Graphics {
        class Point {
        private:
            void update() {
                MyEngine::Algorithm::calcPoint(_position, _size / 2.f,
                                           _color, _vertices, _indices, _count);
            }
            Vector2 _position;
            uint16_t _size;
            SDL_Color _color;
            std::vector<SDL_Vertex> _vertices;
            std::vector<int> _indices;
            uint16_t _count{32};
        public:
            explicit Point() : _position(0, 0), _size(1), _color(StdColor::Black), _count(32) { update(); }
            Point(float x, float y, uint16_t size = 1, const SDL_Color& color = StdColor::Black,
                  uint16_t count = 32)
                : _position(x, y), _size(size), _color(color), _count(count) { update(); }
            void move(float x, float y) { _position.reset(x, y); update(); }
            void move(const Vector2& new_pos) { _position.reset(new_pos); update(); }
            void resize(uint16_t new_size) { _size = new_size; update(); }
            void setColor(const SDL_Color& color) { _color = color; update(); }
            void setSegment(uint16_t segment = 32) { _count = segment; }
            void reset(const Vector2& pos, uint16_t size, const SDL_Color& color, uint16_t segment = 32) {
                _position.reset(pos);
                _size = size;
                _color = color;
                _count = segment;
                update();
            }
            void reset(float x, float y, uint16_t size, const SDL_Color& color, uint16_t segment = 32) {
                _position.reset(x, y);
                _size = size;
                _color = color;
                _count = segment;
                update();
            }
            const Vector2& position() const { return _position; }
            uint16_t size() const { return _size; }
            const SDL_Color& color() const { return _color; }
            const SDL_Vertex* vertices() const { return _vertices.data(); }
            const int* indices() const { return _indices.data(); }
            size_t verticesCount() const { return _vertices.size(); }
            size_t indicesCount() const { return _indices.size(); }
        };

        class Line {
        public:
            explicit Line() : _start_position(), _end_position(), _size(1), _color(StdColor::Black) {}
            explicit Line(float x1, float y1, float x2, float y2, uint16_t size, const SDL_Color& color)
                : _start_position(x1, y1), _end_position(x2, y2), _size(size), _color(color) {update();}
            explicit Line(const Vector2& start, const Vector2& end, uint16_t size, const SDL_Color &color)
                : _start_position(start), _end_position(end), _size(size), _color(color) {update();}

            const int *indices() { return _indices.data(); }
            const SDL_Vertex *vertices() { return _vertices.data(); }
            size_t indicesCount() const { return _indices.size(); }
            size_t vertexCount() const { return _vertices.size(); }
            const Vector2& startPosition() const { return _start_position; }
            const Vector2& endPosition() const { return _end_position; }
            void setStartPosition(const Vector2& pos) { _start_position.reset(pos.x, pos.y); update(); }
            void setStartPosition(float x, float y) { _start_position.reset(x, y); update(); }
            void setEndPosition(const Vector2& pos) { _end_position.reset(pos.x, pos.y); update(); }
            void setEndPosition(float x, float y) { _end_position.reset(x, y); update(); }
            uint8_t size() const { return _size; }
            void setSize(uint8_t new_size) { _size = new_size; update(); }
            const SDL_Color& color() const { return _color; }
            void setColor(const SDL_Color& color) { _color = color; update(); }
            void reset(float sx, float sy, float ex, float ey, uint8_t size, const SDL_Color &color) {
                _start_position.reset(sx, sy);
                _end_position.reset(ex, ey);
                _size = size;
                _color = color;
                update();
            }
            void reset(const Vector2& start, const Vector2& end, uint8_t size, const SDL_Color& color) {
                _start_position.reset(start);
                _end_position.reset(end);
                _size = size;
                _color = color;
                update();
            }
        private:
            void update() {
                MyEngine::Algorithm::calcLine(_start_position.x, _start_position.y,
                                          _end_position.x, _end_position.y,
                                          _size, _color, _vertices, _indices);
            }
            Vector2 _start_position;
            Vector2 _end_position;
            uint8_t _size;
            SDL_Color _color;
            std::array<int, 6> _indices;
            std::array<SDL_Vertex, 4> _vertices;
        };

        class Rectangle {
        private:
            GeometryF _geometry;
            uint16_t _border_size;
            SDL_Color _border_color;
            SDL_Color _background_color;
            std::array<SDL_FRect, 4> _borders;

        public:
            explicit Rectangle() : _geometry(0, 0, 0, 0), _border_size(0),
                                   _border_color(StdColor::Black), _background_color(StdColor::Black) {}
            explicit Rectangle(float x, float y, float w, float h,
                               uint16_t border = 1, const SDL_Color& border_color = StdColor::Black,
                               const SDL_Color& background_color = StdColor::Black)
                               : _geometry(x, y, w, h), _border_size(border),
                                 _border_color(border_color), _background_color(background_color) {
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void reset(const GeometryF& geometry, uint16_t border,
                       const SDL_Color& border_color, const SDL_Color& background_color) {
                _geometry.reset(geometry);
                _border_size = border;
                _border_color = border_color;
                _background_color = background_color;
                Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void reset(const Vector2& pos, const Size& size, uint16_t border,
                       const SDL_Color& border_color, const SDL_Color& background_color) {
                _geometry.reset(pos, size);
                _border_size = border;
                _border_color = border_color;
                _background_color = background_color;
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void reset(float x, float y, float w, float h, uint16_t border,
                       const SDL_Color& border_color, const SDL_Color& background_color) {
                _geometry.reset(x, y, w, h);
                _border_size = border;
                _border_color = border_color;
                _background_color = background_color;
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void setGeometry(float x, float y, float w, float h) {
                _geometry.reset(x, y, w, h);
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void setGeometry(const Vector2& pos, const Size& size) {
                _geometry.reset(pos, size);
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void setGeometry(const GeometryF& geometry) {
                _geometry = geometry;
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void move(const Vector2& pos) {
                _geometry.pos.reset(pos);
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void move(float x, float y) {
                _geometry.pos.reset(x, y);
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void resize(const Size& size) {
                _geometry.size.reset(size);
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void resize(float w, float h) {
                _geometry.size.reset(w, h);
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }

            void setBorderSize(uint16_t border_size) {
                _border_size = border_size;
                MyEngine::Algorithm::calcRectangleBorder(_geometry, _border_size, _borders);
            }
            void setBorderColor(const SDL_Color& color) {
                _border_color = color;
            }
            void setBackgroundColor(const SDL_Color& color) {
                _background_color = color;
            }
            const GeometryF& geometry() const { return _geometry; }
            uint16_t borderSize() const { return _border_size; }
            const SDL_Color& borderColor() const { return _border_color; }
            const SDL_Color& backgroundColor() const { return _background_color; }
            const SDL_FRect* bordersRect() const { return _borders.data(); }
        };

        class Triangle {
        public:
            explicit Triangle() : _p1(0, 0), _p2(0, 0), _p3(0, 0),
                                  _border_size(0), _border_color(StdColor::Black), _background_color(StdColor::Black) {}
            explicit Triangle(float x1, float y1, float x2, float y2, float x3, float y3,
                              uint16_t border_size = 0, const SDL_Color& border_color = StdColor::Black,
                              const SDL_Color& back_color = StdColor::Black)
                : _p1(x1, y1), _p2(x2, y2), _p3(x3, y3),
                  _border_size(border_size), _border_color(border_color), _background_color(back_color)
                { updateTri(); updateBorder(); }

            void reset(float x1, float y1, float x2, float y2, float x3, float y3,
                       uint16_t border_size, const SDL_Color& border_color, const SDL_Color& back_color) {
                _p1.reset(x1, y1);
                _p2.reset(x2, y2);
                _p3.reset(x3, y3);
                _border_size = border_size;
                _border_color = border_color;
                _background_color = back_color;
                updateTri();
                updateBorder();
            }
            void reset(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3,
                       uint16_t border_size, const SDL_Color& border_color, const SDL_Color& back_color) {
                _p1.reset(pos1);
                _p2.reset(pos2);
                _p3.reset(pos3);
                _border_size = border_size;
                _border_color = border_color;
                _background_color = back_color;
                updateTri();
                updateBorder();
            }
            void setPosition(uint8_t index, const Vector2& pos) {
                switch (index % 3) {
                    case 0:
                        _p1.reset(pos);
                        break;
                    case 1:
                        _p2.reset(pos);
                        break;
                    case 2:
                        _p3.reset(pos);
                        break;
                }
                updateTri();
                updateBorder();
            }
            void setBorder(uint16_t border_size, const SDL_Color& color) {
                _border_size = border_size;
                _border_color = color;
                updateBorder();
            }
            void setBackgroundColor(const SDL_Color& color) {
                _background_color = color;
                updateTri();
            }
            const Vector2& position(uint8_t index = 0) const {
                switch (index % 3) {
                    case 0:
                        return _p1;
                    case 1:
                        return _p2;
                    case 2:
                        return _p3;
                }
                return _p1;
            }
            uint16_t borderSize() const { return _border_size; }
            const SDL_Color& borderColor() const { return _border_color; }
            const SDL_Color& backgroundColor() const { return _background_color; }
            const int *indices() { return _indices.data(); }
            const SDL_Vertex *vertices() { return _vertices.data(); }
            const int *borderIndices1() { return _bdi1.data(); }
            const int *borderIndices2() { return _bdi2.data(); }
            const int *borderIndices3() { return _bdi3.data(); }
            const SDL_Vertex *borderVertices1() { return _bd1.data(); }
            const SDL_Vertex *borderVertices2() { return _bd2.data(); }
            const SDL_Vertex *borderVertices3() { return _bd3.data(); }
            size_t borderIndicesCount() const { return _bdi1.size(); }
            size_t borderVerticesCount() const { return _bd1.size(); }
            size_t indicesCount() const { return _indices.size(); }
            size_t vertexCount() const { return _vertices.size(); }
        private:
            void updateTri() {
                MyEngine::Algorithm::calcTriangle(_p1, _p2, _p3, _background_color, _vertices, _indices);
            }
            void updateBorder() {
                if (_border_size == 0) return;
                MyEngine::Algorithm::calcLine(_p1.x, _p1.y, _p2.x, _p2.y, _border_size, _border_color,_bd1, _bdi1);
                MyEngine::Algorithm::calcLine(_p2.x, _p2.y, _p3.x, _p3.y, _border_size, _border_color,_bd2, _bdi2);
                MyEngine::Algorithm::calcLine(_p1.x, _p1.y, _p3.x, _p3.y, _border_size, _border_color,_bd3, _bdi3);
            }
            Vector2 _p1;
            Vector2 _p2;
            Vector2 _p3;
            uint16_t _border_size;
            SDL_Color _border_color;
            SDL_Color _background_color;
            std::array<SDL_Vertex, 3> _vertices;
            std::array<int, 3> _indices;
            std::array<SDL_Vertex, 4> _bd1;
            std::array<SDL_Vertex, 4> _bd2;
            std::array<SDL_Vertex, 4> _bd3;
            std::array<int, 6> _bdi1;
            std::array<int, 6> _bdi2;
            std::array<int, 6> _bdi3;
        };

        class Ellipse {
        public:
            explicit Ellipse() : _center_point(0, 0), _radius(0, 0),
                                 _border_size(0), _border_color(StdColor::Black), _background_color(StdColor::Black),
                                 _degree(0.f), _count(0) {}

            Ellipse(float cx, float cy, float rw, float rh, uint16_t border_size = 1,
                    const SDL_Color& border_color = StdColor::Black,
                    const SDL_Color& back_color = StdColor::Black,
                    float degree = 0.f, uint16_t segment = 32)
                    : _center_point(cx, cy), _radius(rw, rh), _border_size(border_size),
                      _border_color(border_color), _background_color(back_color),
                      _degree(degree), _count(segment) {
                MyEngine::Algorithm::calcEllipse(_center_point, _radius,
                                             _background_color, _degree, _count,
                                             _vertices, _indices);
                MyEngine::Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                 _border_color, _degree, _count,
                                                 _border_vertices, _border_indices);
            }

            void reset(float cx, float cy, float rw, float rh, uint16_t border_size,
                       const SDL_Color& border_color, const SDL_Color& back_color, float degree,
                       uint16_t segment = 32) {
                _center_point.reset(cx, cy);
                _radius.reset(rw, rh);
                _border_size = border_size;
                _border_color = border_color;
                _background_color = back_color;
                _degree = degree;
                _count = segment;
                MyEngine::Algorithm::calcEllipse(_center_point, _radius,
                                             _background_color, _degree, _count,
                                             _vertices, _indices);
                MyEngine::Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                 _border_color, _degree, _count,
                                                 _border_vertices, _border_indices);
            }

            void reset(const Vector2& center_pos, const Size& radius, uint16_t border_size,
                       const SDL_Color& border_color, const SDL_Color& back_color, float degree,
                       uint16_t segment = 32) {
                _center_point.reset(center_pos);
                _radius.reset(radius);
                _border_size = border_size;
                _border_color = border_color;
                _background_color = back_color;
                _degree = degree;
                _count = segment;
                MyEngine::Algorithm::calcEllipse(_center_point, _radius,
                                             _background_color, _degree, _count,
                                             _vertices, _indices);
                MyEngine::Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                 _border_color, _degree, _count,
                                                 _border_vertices, _border_indices);
            }

            void setGeometry(float x, float y, float rw, float rh) {
                _center_point.reset(x, y);
                _radius.reset(rw, rh);
                MyEngine::Algorithm::calcEllipse(_center_point, _radius,
                                             _background_color, _degree, _count,
                                             _vertices, _indices);
                MyEngine::Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                 _border_color, _degree, _count,
                                                 _border_vertices, _border_indices);
            }

            void setGeometry(const Vector2& position, const Size& size) {
                _center_point.reset(position);
                _radius.reset(size);
                MyEngine::Algorithm::calcEllipse(_center_point, _radius,
                                             _background_color, _degree, _count,
                                             _vertices, _indices);
                MyEngine::Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                 _border_color, _degree, _count,
                                                 _border_vertices, _border_indices);
            }

            void setBorder(uint16_t size, const SDL_Color& color) {
                _border_size = size;
                _border_color = color;
                MyEngine::Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                 _border_color, _degree, _count,
                                                 _border_vertices, _border_indices);
            }

            void setBackground(const SDL_Color& color) {
                _background_color = color;
                MyEngine::Algorithm::calcEllipse(_center_point, _radius,
                                             _background_color, _degree, _count,
                                             _vertices, _indices);
            }

            void setRotate(float rotate) {
                _degree = rotate;
                MyEngine::Algorithm::calcEllipse(_center_point, _radius,
                                             _background_color, _degree, _count,
                                             _vertices, _indices);
                MyEngine::Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                 _border_color, _degree, _count,
                                                 _border_vertices, _border_indices);
            }

            const Vector2& centerPosition() const { return _center_point; }
            const Size& radius() const { return _radius; }
            uint16_t borderSize() const { return _border_size; }
            const SDL_Color& borderColor() const { return _border_color; }
            const SDL_Color& backgroundColor() const { return _background_color; }
            float rotateDegree() const { return _degree; }
            const int *indices() const { return _indices.data(); }
            const SDL_Vertex *vertices() const { return _vertices.data(); }
            size_t indicesCount() const { return _indices.size(); }
            size_t vertexCount() const { return _vertices.size(); }
            const int *borderIndices() const { return _border_indices.data(); }
            const SDL_Vertex *borderVertices() const { return _border_vertices.data(); }
            size_t borderIndicesCount() const { return _border_indices.size(); }
            size_t borderVerticesCount() const { return _border_vertices.size(); }

        private:
            Vector2 _center_point;
            Size _radius;
            uint16_t _border_size;
            SDL_Color _border_color;
            SDL_Color _background_color;
            float _degree;
            uint16_t _count{32};
            std::vector<SDL_Vertex> _vertices, _border_vertices;
            std::vector<int> _indices, _border_indices;
        };
    }
}



#endif //MYENGINE_BASIC_H