
#pragma once
#ifndef MYENGINE_BASIC_H
#define MYENGINE_BASIC_H
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
#include "Libs.h"
#include "Exception.h"
#include "Utils/Logger.h"

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

using StringList    = std::vector<std::string>;

/**
 * \if EN
 * @namespace MyEngine
 * @brief The project namespace.
 * \endif
 */
namespace MyEngine {
    /**
     * \if EN
     * @namespace StdColor
     * @brief Predefined standard RGB colors (40 colors)
     * @ref related/ColorMap.md
     * \endif
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

    /// \if EN
    /// @brief Set how many seconds to delay on this process.
    /// \endif
    inline void delay(uint64_t sec) { std::this_thread::sleep_for(std::chrono::seconds(sec)); }
    /// \if EN
    /// @brief Set how many milliseconds to delay on this process.
    /// \endif
    inline void delayMS(uint64_t ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
    /// \if EN
    /// @brief Set how many microseconds to delay on this process.
    /// \endif
    inline void delayUS(uint64_t us) { std::this_thread::sleep_for(std::chrono::microseconds(us)); }
    /// \if EN
    /// @brief Set how many nanoseconds to delay on this process.
    /// \endif
    inline void delayNS(uint64_t ns) { std::this_thread::sleep_for(std::chrono::nanoseconds(ns)); }

    /**
     * \if EN
     * @class IDGenerator
     * @brief Generate unique ID.
     * 
     * @details Multiple ID generators can be created and generate unique ID values.
     * \endif
     */
    class IDGenerator {
    public:
        IDGenerator() = delete;
        ~IDGenerator() = delete;
        IDGenerator(const IDGenerator&) = delete;
        IDGenerator(IDGenerator&&) = delete;
        IDGenerator& operator=(const IDGenerator&) = delete;
        IDGenerator& operator=(IDGenerator&&) = delete;

        /**
         * \if EN
         * @brief Create a new ID generator.
         *
         * @details You can use the `getID()` function to generate this ID from the specified generator.
         * @return The index of the new ID generator.
         * \endif
         * @see getID
         */
        static size_t newIDGenerator() {
            _id_list.emplace_back(0);
            return _id_list.size() - 1;
        }

        /**
         * \if EN
         * @brief Use ID Generator to get the new EventID.
         * @return The new EventID (Only used for EventSystem class).
         * \endif
         */
        static uint64_t getNewEventID() {
            return ++_id_list[0];
        }
        /**
         * \if EN
         * @brief Use ID Generator to get the new GlobalEventID.
         * @return The new GlobalEventID (Only used for EventSystem class)
         * \endif
         */
        static uint64_t getNewGlobalEventID() {
            return ++_id_list[1];
        }

        /**
         * \if EN
         * @brief Use ID Generator to get the new TextID.
         * @return The new TextID (Only used for TextSystem class)
         * \endif
         */
        static uint64_t getNewTextID() {
            return ++_id_list[2];
        }

        /**
         * \if EN
         * @brief Use the specified ID Generator to get the new ID.
         * 
         * @param index The index of the ID generator.
         * @note If the index is not exist, the function will log an error message and throw `MyEngine::OutOfRangeException` error.
         * @note About the index, this library provides three ID generator indexes, which correspond to the following:
         * @note - `0`: EventID
         * @note - `1`: GlobalEventID
         * @note - `2`: TextID
         * @return The new ID.
         * \endif
         * @throw OutOfRangeException
         */
        static uint64_t getID(size_t index = 0) {
            if (index >= _id_list.size()) {
                auto err = std::format("IDGenerator: The index of {} is not exist!", index);
                Logger::log(err, Logger::Fatal);
                throw OutOfRangeException(err);
            }
            return ++_id_list[index];
        }
    private:
        static std::vector<uint64_t> _id_list;
    };
    inline std::vector<uint64_t> IDGenerator::_id_list(4, 0);

    struct GeometryF;
    /**
     * \if EN
     * @struct Geometry
     * @brief Including position and size. 
     * \endif
     */
    struct Geometry {
        int x, y, width, height;

        Geometry() : x(0), y(0), width(0), height(0) {}

        Geometry(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}

        /**
         * \if EN
         * @brief Update the geometry.
         * @details Set the new position and size of the geometry.
         * @param x The new x position.
         * @param y The new y position.
         * @param width The new width.
         * @param height The new height.
         * @see move
         * @see reshape
         * \endif
         */
        void setGeometry(int x, int y, int width, int height) {
            this->x = x;
            this->y = y;
            this->width = width;
            this->height = height;
        }

        /**
         * \if EN
         * @brief Change the position of the geometry.
         * @details Set the new position of the geometry.
         * @param x The new x position.
         * @param y The new y position.
         * @see setGeometry
         * @see reshape
         * \endif
         */
        void move(int x, int y) {
            this->x = x;
            this->y = y;
        }

        /**
         * \if EN
         * @brief Change the size of the geometry.
         * @details Set the new size of the geometry.
         * @param width The new width.
         * @param height The new height.
         * @see setGeometry
         * @see move
         * \endif
         */
        void resize(int width, int height) {
            this->width = width;
            this->height = height;
        }
    };

    /**
     * \if EN
     * @struct Vector2
     * @brief Including two float values. Used to represent 2D vector.
     * \endif
     */
    struct Vector2 {
        float x, y;

        explicit Vector2() : x(0.0f), y(0.0f) {}

        Vector2(float x, float y) : x(x), y(y) {}

        /**
         * \if EN
         * @brief Reset the vector to the new values.
         * 
         * @param x The new x value.
         * @param y The new y value.
         * \endif
         */
        void reset(float x, float y) {
            this->x = x;
            this->y = y;
        }

        /**
         * \if EN
         * @brief Reset the vector to the values of the other vector.
         * 
         * @param vector2 The other vector.
         * \endif
         */
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
            return this->x == v.x && this->y == v.y;
        }

        bool operator!=(const Vector2 &v) const {
            return (this->x != v.x) || (this->y != v.y);
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
         * \if EN
         * @brief judge whether the vector is equal to the other vector.
         * @param v         The other vector.
         * @param EPISON    The allowable error number (default 0.000001f error).
         * @return  Return true if the vector is equal to the other vector, otherwise return `false`.
         * \endif
         */
        [[nodiscard]] bool isEqual(const Vector2 &v, const float EPISON = 1e-6f) const {
            return ((this->x >= v.x - EPISON && this->x <= v.x + EPISON) &&
                    (this->y >= v.y - EPISON && this->y <= v.y + EPISON));
        }
    };

    /**
     * \if EN
     * @struct Size
     * @brief Including two float values. Used to represent 2D size.
     * \endif
     */
    struct Size {
        float width, height;

        explicit Size() : width(0.0f), height(0.0f) {}

        Size(float width, float height) : width(width), height(height) {}

        /**
         * \if EN
         * @brief reset the new size
         * @param width     new width
         * @param height    new height
         * \endif
         */
        void reset(float width, float height) {
            this->width = width;
            this->height = height;
        }

        /**
         * \if EN
         * @brief reset the new size
         * @param size  The new size.
         * \endif
         */
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
     * \if EN
     * @struct GeometryF
     * @brief Includes the position and size using floating-point numbers
     *
     * @details Includes `Vector2` and `Size`, used to adjust the position and size.
     * \endif
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

        void resetPos(float x, float y) {
            this->pos.x = x;
            this->pos.y = y;
        }

        void resetSize(const Size &size) {
            this->size.width = size.width;
            this->size.height = size.height;
        }

        void resetSize(float width, float height) {
            this->size.width = width;
            this->size.height = height;
        }
    };

    /**
     * \if EN
     * @brief Convert a `GeometryF` to a `Geometry` with integer values.
     * @param geo The `GeometryF` to convert.
     * @return The converted `Geometry` with integer values.
     * \endif
     */
    inline Geometry toGeometryInt(const GeometryF& geo) {
        return {(int)geo.pos.x, (int)geo.pos.y, (int)geo.size.width, (int)geo.size.height};
    }
    
    /**
     * \if EN
     * @brief Convert a `Geometry` to a `GeometryF` with floating-point values.
     * @param geo The `Geometry` to convert.
     * @return The converted `GeometryF` with floating-point values.
     * \endif
     */
    inline GeometryF toGeometryFloat(const Geometry& geo) {
        return GeometryF((float)geo.x, (float)geo.y, (float)geo.width, (float)geo.height);
    }

    /**
     * \if EN
     * @class Matrix2D
     * @brief 2D Matrix
     * 
     * @details Supports basic data types and simple structs for storage.
     * \endif
     * @ref related/Matrix2D.md
     */
    template<typename T>
    class Matrix2D {
    private:
        
        std::vector<T> _datas;
        
        uint32_t _row;
        
        uint32_t _col;
        
        std::function<void(T &)> _deleter;
    public:
        using iterator = typename std::vector<T>::iterator;
        using constIterator = typename std::vector<T>::const_iterator;

        /**
         * \if EN
         * @struct Position
         * @brief 2D Matrix Position
         * 
         * @details Represents the position in a 2D matrix with row and column indices.
         * \endif
         */
        struct Position {
            /**
             * \if EN
             * @brief Row index of the position in the matrix.
             * \endif
             */
            uint32_t row;
            /**
             * \if EN
             * @brief Column index of the position in the matrix.
             * \endif
             */
            uint32_t col;

            Position() : row(0), col(0) {}

            Position(uint32_t row, uint32_t col) : row(row), col(col) {}

            bool operator==(const Position &p) const { return (row == p.row && col == p.col); }

            bool operator!=(const Position &p) const { return (row != p.row || col != p.col); }

            bool operator>(const Position &p) const { return (row >= p.row && col > p.col); }

            bool operator<(const Position &p) const { return (row <= p.row && col < p.col); }

            bool operator>=(const Position &p) const { return (row >= p.row && col >= p.col); }

            bool operator<=(const Position &p) const { return (row <= p.row && col <= p.col); }
        };

        /**
         * \if EN
         * @brief Create an empty 2D matrix (with no data)
         * \endif
         * @see reshape
         */
        explicit Matrix2D() : _row(0), _col(0), _datas() {}

        /**
         * \if EN
         * @brief Create a 2D matrix with specified rows and columns
         * @param row Rows
         * @param col Columns
         * @param deleter Deleter (specify if you need to delete pointers, etc.)
         * \endif
         * @see reshape
         * @see setDeleter
         */
        Matrix2D(uint32_t row, uint32_t col, const std::function<void(T &)> &deleter = {});

        /**
         * \if EN
         * @brief Create a 2D matrix with specified rows and columns and a default value
         * @param row  Rows
         * @param col   Columns
         * @param value Default value (used to populate all data)
         * @param deleter Deleter (specify if you need to delete pointers, etc.)
         * \endif
         * @see reshape
         * @see fill
         * @see setDeleter
         */
        Matrix2D(uint32_t row, uint32_t col, const T &value, const std::function<void(T &)> &deleter = {});

        Matrix2D(const Matrix2D<T> &matrix);

        ~Matrix2D();

        /**
         * \if EN
         * @brief Set deleter
         * @param function  Specified function for handling pointer data
         *
         * @details When such a destructor is called, the deleter will be invoked to delete the pointer!
         * \endif
         */
        void setDeleter(const std::function<void(T &)> &function);

        /**
         * \if EN
         * @brief Fill all data with the specified value
         * @param value Specified value
         * \endif
         * @see fillN
         */
        void fill(const T &value);

        /**
         * \if EN
         * @brief Fill in the values within the range
         * @param start Specify the starting position (row and column)
         * @param end   Specify the end position (row and column)
         * @param value Specify the value for filling
         * \endif
         * @see fill
         */
        bool fillN(const Matrix2D::Position &start, const Matrix2D::Position &end, const T &value);

        /**
         * \if EN
         * @brief readjust the size of the matrix
         * @param line The new lines
         * @param col  The new columns
         * @note If the matrix is reduced, the excess data will be discarded
         * (if the deleter has been previously specified for the matrix,
         * the deletion operation will be automatically performed).
         * \endif
         * @see setDeleter
         * @see rows
         * @see cols
         */
        void reshape(uint32_t line, uint32_t col);

        /**
         * \if EN
         * @brief Get data from the specified position in the matrix
         * @param row  Specified line (starting from 0)
         * @param col  Specified column (starting from 0)
         * @return Return the data under the corresponding row and column
         * @note You can directly use this function to modify the data.
         * @note If the specified position is out of range, a `MyEngine::OutOfRangeException` will be thrown
         * \endif
         * @see get
         * @throw OutOfRangeException
         */
        T &at(uint32_t row, uint32_t col);

        /**
         * \if EN
         * @brief Get data from the specified position in the matrix
         * @param row  Specified line (starting from 0)
         * @param col  Specified column (starting from 0)
         * @return Return the data under the corresponding row and column
         * @note Different from 'at()' function, this function cannot modify the corresponding data!
         * @note If the specified position is out of range, a `MyEngine::OutOfRangeException` will be thrown
         * \endif
         * @see at
         * @throw OutOfRangeException
         */
        const T &get(uint32_t row, uint32_t col);

        /**
         * \if EN
         * @brief Get the total number of rows in the current matrix
         * @return Return the corresponding row number
         * \endif
         * @see cols
         */
        [[nodiscard]] uint32_t rows() const;

        /**
         * \if EN
         * @brief Get the total number of columns in the current matrix
         * @return Return the corresponding column number
         * \endif
         * @see rows
         */
        [[nodiscard]] uint32_t cols() const;

        /**
         * \if EN
         * @brief Matrix Addition
         * @param other The specified matrix
         *
         * @details Performs addition on all values within the matrices.
         * @note Currently only supports integer and floating-point operations; other data types are not supported!
         * @note The two matrices must be of the same type,
         * meaning they must have exactly the same number of rows and columns.
         * Otherwise, a `MyEngine::InvalidArgumentException` will be thrown.
         * @return Returns a new matrix.
         * \endif
         * @see add
         */
        Matrix2D operator+(const Matrix2D<T> &other) const;

        /**
         * \if EN
         * @brief Matrix Subtraction
         * @param other The specified matrix
         *
         * @details Performs subtraction on all values within the matrices.
         * @note Currently only supports integer and floating-point operations; other data types are not supported!
         * @note The two matrices must be of the same type,
         * meaning they must have exactly the same number of rows and columns.
         * Otherwise, a `MyEngine::InvalidArgumentException` will be thrown.
         * @return Returns a new matrix.
         * \endif
         * @see sub
         */
        Matrix2D operator-(const Matrix2D<T> &other) const;

        /**
         * \if EN
         * @brief Hadamard Product (Element-wise Product)
         * @param other The specified matrix, whose number of rows must equal the current matrix's number of columns
         *
         * @details Performs multiplication on all values within the matrices
         * @note Currently only supports integer and floating-point operations; other data types are not supported!
         * @note Both matrices must be of sizes m * n and n * p, respectively, to be usable!
         * Otherwise, a `MyEngine::BadValueException` exception will be thrown.
         * \endif
         * @throw BadValueException
         * @see times
         */
        Matrix2D operator*(const Matrix2D<T> &other) const;

        bool operator==(const Matrix2D<T> &other) const;

        bool operator!=(const Matrix2D<T> &other) const;

        /**
         * \if EN
         * @brief Get the data at the specified index
         * @param index The specified index
         * @return Returns the data at the corresponding index
         * @note An `MyEngine::OutOfRangeException` will be thrown if the index value is out of range
         * \endif
         * @throw OutOfRangeException
         * @see at
         * @see get
         */
        T &operator[](uint32_t index);

        /**
         * \if EN
         * @brief Get the data at the specified position
         * @param row The specified row number (starting from 0)
         * @param col The specified column number (starting from 0)
         * @return Returns the data at the corresponding position
         * @note If the specified position is out of range, a `MyEngine::OutOfRangeException` will be thrown
         * \endif
         * @throw OutOfRangeException
         * @see get
         * @see at
         */
        T &operator()(uint32_t row, uint32_t col);

        iterator begin() { return _datas.begin(); }

        iterator end() { return _datas.end(); }

        constIterator begin() const { return _datas.begin(); }

        constIterator end() const { return _datas.end(); }

        [[nodiscard]] size_t size() const { return _datas.size(); }

        /**
         * \if EN
         * @brief Matrix addition
         * @param value Specified value
         * @param function Function (this parameter must be specified for complex data types)
         * @details Performs addition on all values in the matrix
         * \endif
         */
        void add(T &value, const std::function<void(T &, T &)> &function = {});

        /**
         * \if EN
         * @brief Matrix addition
         * @param value Specified value
         * @param function Function (this parameter must be specified for complex data types)
         * @details Performs addition on all values in the matrix
         * \endif
         */
        void add(T &&value, const std::function<void(T &, T &)> &function = {});

        /**
         * \if EN
         * @brief Matrix subtraction
         * @param value Specified value
         * @param function Function (this parameter must be specified for complex data types)
         * @details Performs subtraction on all values in the matrix
         * \endif
         */
        void sub(T &value, const std::function<void(T &, T &)> &function = {});

        /**
         * \if EN
         * @brief Matrix subtraction
         * @param value Specified value
         * @param function Function (this parameter must be specified for complex data types)
         * @details Performs subtraction on all values in the matrix
         * \endif
         */
        void sub(T &&value, const std::function<void(T &, T &)> &function = {});

        /**
         * \if EN
         * @brief Scalar multiplication
         * @param value Specified value
         * @param function Function
         *
         * @details Perform multiplication on all values in the matrix.
         * @note For complex data types, the `function` parameter must be specified.
         * Otherwise, a `MyEngine::InvalidArgumentException` exception will be thrown
         * \endif
         * @throw InvalidArgumentException
         */
        void times(T &value, const std::function<void(T &, T &)> &function = {});

        /**
         * \if EN
         * @brief Scalar multiplication
         * @param value Specified value
         * @param function Function
         *
         * @details Perform multiplication on all values in the matrix.
         * @note For complex data types, the `function` parameter must be specified.
         * Otherwise, a `MyEngine::InvalidArgumentException` exception will be thrown
         * \endif
         * @throw InvalidArgumentException
         */
        void times(T &&value, const std::function<void(T &, T &)> &function = {});

        /**
         * \if EN
         * @brief Hadamard Product (Element-wise Product)
         * @param other The specified matrix
         *
         * @details Performs multiplication on all values within the matrices.
         * @note For complex data types, the `function` parameter must be specified.
         * Otherwise, a `MyEngine::InvalidArgumentException` exception will be thrown.
         * @note The two matrices must be of the same type,
         * meaning they must have exactly the same number of rows and columns.
         * Otherwise, a `MyEngine::InvalidArgumentException` will be thrown.
         * @return Returns a new matrix.
         * \endif
         * @throw InvalidArgumentException
         */
        void times(const Matrix2D<T> &other, const std::function<void(T &, const T &)> &function = {});

        /**
         * \if EN
         * @brief Matrix Multiplication
         *
         * @details Performs a cross multiplication operation on two matrices.
         * @param other The specified matrix, whose number of rows must equal the original matrix's number of columns.
         * @note Currently only supports integer and floating-point operations; other data types are not supported!
         * @note The original matrix and the specified matrix must have sizes of `m * k` and `k * n`,
         * respectively, to be valid! Otherwise, a `MyEngine::InvalidArgumentException` will be thrown.
         * @return Generates a new matrix of size `m * n`.
         * \endif
         * @throw InvalidArgumentException
         */
        Matrix2D multiply(const Matrix2D<T> &other);

        /**
         * \if EN
         * @brief Transpose Matrix
         * @details Transpose the original `m * n` matrix into an `n * m` matrix.
         * \endif
         * @see rotate
         */
        void transpose();

        /**
         * \if EN
         * @brief Reversed Matrix
         * @details A swap occurs whenever a reverse is selected in any direction.
         * @param reverse_row Reverse between rows (vertical flip)
         * @param reverse_col Reverse between columns (horizontal flip)
         * @note When both parameters are `true`, the matrix is completely reversed
         * \endif
         * @see rotate
         */
        void reverse(bool reverse_row = true, bool reverse_col = false);

        /**
         * \if EN
         * @brief Rotation matrix
         * @param turn_right Whether to rotate 90° to the right; otherwise, rotate 90° to the left
         * \endif
         * @see reverse
         * @see transpose
         */
        void rotate(bool turn_right = true);

        /**
         * \if EN
         * @brief Extracts a specified range of rows from a matrix
         * @param start_row Starting row
         * @param end_row Ending row
         * @return Returns all rows of the matrix within the `[start_row, end_row)` range
         * \endif
         */
        Matrix2D splitRows(uint32_t start_row, uint32_t end_row);

        /**
         * \if EN
         * @brief Extracts a specified range of columns from a matrix
         * @param start_row Starting column
         * @param end_row Ending column
         * @return Returns all columns of the matrix within the `[start_row, end_row)` range
         * \endif
         */
        Matrix2D splitCols(uint32_t start_col, uint32_t end_col);

        /**
         * \if EN
         * @brief Slice Matrix
         * @details Slices the original matrix, starting from the `start_pos` position, takes `rows * cols` elements, and creates a new matrix.
         * @param rows Number of rows in the new matrix
         * @param cols Number of columns in the new matrix
         * @param start_pos Position to start from
         * @return Returns a new 2D matrix of size `(rows * cols)`.
         *
         * @note If the total number of elements taken from `start_pos` is less than the size of the new matrix, the remaining positions are automatically filled with empty data.
         * @note If the specified `start_pos` exceeds the bounds of the original matrix, an empty matrix is returned.
         * \endif
         */
        Matrix2D split(uint32_t rows, uint32_t cols, const Position &start_pos);

        /**
         * \if EN
         * @brief Cut the matrix in a rectangular manner
         * @details Form a rectangle based on the start and end positions, and cut it into an independent matrix.
         * @param start_pos Start position
         * @param end_pos End position
         * @return Returns the cut matrix.
         * \endif
         */
        Matrix2D split(Matrix2D::Position start_pos, Matrix2D::Position end_pos);

        /**
         * \if EN
         * @brief Inverse Matrix
         * @return Returns a new matrix to store the calculated result
         * @note Currently only supports integer and floating-point operations, other data types are not supported!
         * @note The current matrix must be `n * n`, otherwise an empty matrix will be returned
         * \endif
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
            Logger::log("Matrix2D::fillN(): One of the specified position is not valid!", Logger::Error);
            return false;
        }
        std::fill(_datas.begin() + st, _datas.begin() + ed, value);
        return true;
    }

    template<typename T>
    void Matrix2D<T>::reshape(uint32_t line, uint32_t col) {
        auto new_size = line * col, origin_size = _row * _col;
        if (_row * _col != new_size) _datas.resize(new_size);
        if (_row * _col > new_size) {
            Logger::log("Matrix2D: You have reduced the original matrix size, some datas will be discarded.",
                        Logger::Warn);
            if (_deleter) {
                for (size_t i = new_size; i < origin_size; ++i) _deleter(_datas[i]);
            }
            _datas.shrink_to_fit();
        }
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
            std::string err = std::format("Matrix2D::operator+(): Matrix dimensions mismatch! "
                                          "Original: ({}, {}), Specified: ({}, {})",
                                          _row, _col, other._row, other._col);
            Logger::log(err, Logger::Error);
            return InvalidArgumentException(err);
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
            std::string err = std::format("Matrix2D::operator-(): Matrix dimensions mismatch! "
                                          "Original: ({}, {}), Specified: ({}, {})",
                                          _row, _col, other._row, other._col);
            Logger::log(err, Logger::Error);
            return InvalidArgumentException(err);
        }
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::operator*(const Matrix2D<T> &other) const {
        if (_col != other._row) {
            std::string err = "Matrix2D::operator*(): Matrix dimensions incompatible for multiplication!";
            Logger::log(err, Logger::Fatal);
            throw BadValueException(err);
        }
        if constexpr (!std::is_integral_v<std::decay_t<T>> &&
                      !std::is_floating_point_v<std::decay_t<T>>) {
            static_assert(std::is_arithmetic_v<std::decay_t<T>>,
                          "Matrix2D::operator*(): Only arithmetic types are supported!");
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
        if (index >= _datas.size()) {
            throw OutOfRangeException(std::format("Matrix2D::operator[](): "
                                                "The index of {} is out of range!", index));
        }
        return _datas[index];
    }

    template<typename T>
    T &Matrix2D<T>::operator()(uint32_t row, uint32_t col) {
        auto idx = row * _col + col;
        if (idx >= _datas.size()) {
            std::string err = std::format("Matrix2D::operator()(): "
                                          "The specified position ({}, {}) is out of range!", row, col);
            Logger::log(err,Logger::Fatal);
            throw OutOfRangeException(err);
        }
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
                std::string err = "Matrix2D::add(): Unsupported data type! Did you forget to specify function?";
                Logger::log(err,Logger::Fatal);
                throw InvalidArgumentException(err);
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
                std::string err = "Matrix2D::add(): Unsupported data type! Did you forget to specify function?";
                Logger::log(err,Logger::Fatal);
                throw InvalidArgumentException(err);
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::sub(T &value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v -= value;
            } else {
                std::string err = "Matrix2D::sub(): Unsupported data type! Did you forget to specify function?";
                Logger::log(err,Logger::Fatal);
                throw InvalidArgumentException(err);
            }
        });
    }

    template<typename T>
    void Matrix2D<T>::sub(T &&value, const std::function<void(T &, T &)> &function) {
        std::for_each(_datas.begin(), _datas.end(), [&function, &value](T &v) {
            if (function) {
                function(v, value);
            } else if constexpr (std::is_integral_v<std::decay_t<T>> ||
                                 std::is_floating_point_v<std::decay_t<T>>) {
                v -= value;
            } else {
                std::string err = "Matrix2D::sub(): Unsupported data type! Did you forget to specify function?";
                Logger::log(err,Logger::Fatal);
                throw InvalidArgumentException(err);
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
                std::string err = "Matrix2D::times(): Unsupported data type! Did you forget to specify function?";
                Logger::log(err,Logger::Fatal);
                throw InvalidArgumentException(err);
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
                std::string err = "Matrix2D::times(): Unsupported data type! Did you forget to specify function?";
                Logger::log(err,Logger::Fatal);
                throw InvalidArgumentException(err);
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
                std::string err = "Matrix2D::times(): Unsupported data type! Did you forget to specify function?";
                Logger::log(err,Logger::Fatal);
                throw InvalidArgumentException(err);
            }
        } else {
            std::string err = std::format("Matrix2D::times(): Matrix dimensions mismatch! "
                                          "Original: ({}, {}), Specified: ({}, {})",
                                          _row, _col, other._row, other._col);
            Logger::log(err,Logger::Fatal);
            throw InvalidArgumentException(err);
        }
    }

    template<typename T>
    Matrix2D<T> Matrix2D<T>::multiply(const Matrix2D<T> &other) {
        if (_col != other._row) {
            std::string err = "Matrix2D::multiply(): Matrix dimensions incompatible for multiplication!";
            Logger::log(err,Logger::Fatal);
            throw InvalidArgumentException(err);
        }
        Matrix2D<T> result(_row, other._col);
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
            Logger::log("Matrix2D::splitRows(): The specified start row and end row cannot be the same!",
                        Logger::Error);
            return Matrix2D<T>();
        }
        end_row = std::min(end_row, _row);
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
            Logger::log("Matrix2D::splitCols(): The specified start row and end row cannot be the same!",
                        Logger::Error);
            return Matrix2D<T>();
        }
        end_col = std::min(end_col, _col);
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
            Logger::log("Matrix2D::split(): The specified start position is not valid!", Logger::Error);
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
            Logger::log("Matrix2D::inverse(): The specified matrix size is not matched!", Logger::Error);
            return Matrix2D<T>();
        }
        if constexpr (!std::is_integral_v<std::decay_t<T>> &&
                      !std::is_floating_point_v<std::decay_t<T>>) {
            static_assert(!std::is_integral_v<std::decay_t<T>> &&
                          !std::is_floating_point_v<std::decay_t<T>>,
                          "Matrix2d::inverse(): Can't support the current data type!");
        }
        const uint32_t n = _row;
        Matrix2D<T> _ret(n, n);

        Matrix2D<T> augmented(n, 2 * n);

        for (uint32_t i = 0; i < n; ++i) {
            for (uint32_t j = 0; j < n; ++j) {
                augmented(i, j) = _datas[i * _col + j];
                augmented(i, j + n) = (i == j) ? static_cast<T>(1) : static_cast<T>(0);
            }
        }

        for (uint32_t i = 0; i < n; ++i) {
            uint32_t max_row = i;
            for (uint32_t k = i + 1; k < n; ++k) {
                if (std::abs(augmented(k, i)) > std::abs(augmented(max_row, i))) {
                    max_row = k;
                }
            }

            if (std::abs(augmented(max_row, i)) < static_cast<T>(1e-10)) {
                Logger::log("Matrix2D::inverse(): Matrix is singular and cannot be inverted!", Logger::Error);
                return Matrix2D<T>();
            }

            if (max_row != i) {
                for (uint32_t j = 0; j < 2 * n; ++j) {
                    std::swap(augmented(i, j), augmented(max_row, j));
                }
            }

            T pivot = augmented(i, i);
            for (uint32_t j = 0; j < 2 * n; ++j) {
                augmented(i, j) /= pivot;
            }

            for (uint32_t k = 0; k < n; ++k) {
                if (k != i && std::abs(augmented(k, i)) > static_cast<T>(1e-10)) {
                    T factor = augmented(k, i);
                    for (uint32_t j = 0; j < 2 * n; ++j) {
                        augmented(k, j) -= factor * augmented(i, j);
                    }
                }
            }
        }

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
     * @brief Basic Graphic
     *
     * Includes all basic shapes, such as Point, Line, Rectangle, Ellipse, and other basic shapes.
     */
    namespace Graphics {
        class Point {
        private:
            void update() {
                _count = std::min(64, std::max(4, static_cast<int>(M_PI * _size / 2)));
                Algorithm::calcPoint(_position, static_cast<float>(_size) / 2.f,
                                           _color, _vertices, _indices, _count);
            }
            Vector2 _position;
            uint16_t _size;
            SDL_Color _color;
            std::vector<SDL_Vertex> _vertices;
            std::vector<int> _indices;
            uint16_t _count{32};
        public:
            explicit Point() : _position(0, 0), _size(1), _color(StdColor::LightGray), _count(32) { update(); }
            Point(float x, float y, uint16_t size = 1, const SDL_Color& color = StdColor::LightGray,
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
            [[nodiscard]] const Vector2& position() const { return _position; }
            [[nodiscard]] uint16_t size() const { return _size; }
            [[nodiscard]] const SDL_Color& color() const { return _color; }
            [[nodiscard]] const SDL_Vertex* vertices() const { return _vertices.data(); }
            [[nodiscard]] const int* indices() const { return _indices.data(); }
            [[nodiscard]] size_t verticesCount() const { return _vertices.size(); }
            [[nodiscard]] size_t indicesCount() const { return _indices.size(); }
        };

        class Line {
        public:
            explicit Line() : _start_position(), _end_position(), _size(1), _color(StdColor::LightGray) {}
            explicit Line(float x1, float y1, float x2, float y2, uint8_t size, const SDL_Color& color)
                : _start_position(x1, y1), _end_position(x2, y2), _size(size), _color(color) {update();}
            explicit Line(const Vector2& start, const Vector2& end, uint8_t size, const SDL_Color &color)
                : _start_position(start), _end_position(end), _size(size), _color(color) {update();}

            const int *indices() { return _indices.data(); }
            const SDL_Vertex *vertices() { return _vertices.data(); }
            [[nodiscard]] size_t indicesCount() const { return _indices.size(); }
            [[nodiscard]] size_t vertexCount() const { return _vertices.size(); }
            [[nodiscard]] const Vector2& startPosition() const { return _start_position; }
            [[nodiscard]] const Vector2& endPosition() const { return _end_position; }
            void setStartPosition(const Vector2& pos) { _start_position.reset(pos.x, pos.y); update(); }
            void setStartPosition(float x, float y) { _start_position.reset(x, y); update(); }
            void setEndPosition(const Vector2& pos) { _end_position.reset(pos.x, pos.y); update(); }
            void setEndPosition(float x, float y) { _end_position.reset(x, y); update(); }
            [[nodiscard]] uint8_t size() const { return _size; }
            void setSize(uint8_t new_size) { _size = new_size; update(); }
            [[nodiscard]] const SDL_Color& color() const { return _color; }
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
                Algorithm::calcLine(_start_position.x, _start_position.y,
                                          _end_position.x, _end_position.y,
                                          _size, _color, _vertices, _indices);
            }
            Vector2 _start_position;
            Vector2 _end_position;
            uint8_t _size;
            SDL_Color _color;
            std::array<int, 6> _indices{};
            std::array<SDL_Vertex, 4> _vertices{};
        };

        class Rectangle {
        public:
            explicit Rectangle() : _geometry(), _border_size(0),
                                   _border_color(StdColor::DarkGray),
                                   _background_color(StdColor::LightGray), _rotate(0) {}
            explicit Rectangle(const GeometryF& geometry, uint16_t border = 1,
                               const SDL_Color& border_color = StdColor::LightGray,
                               const SDL_Color& background_color = StdColor::LightGray, float degree = 0)
                : _geometry(geometry), _border_size(border), _border_color(border_color),
                  _background_color(background_color), _rotate(degree) {
                updateGeometry();
                updateBorderGeometry();
            }
            explicit Rectangle(float x, float y, float w, float h, uint16_t border = 1,
                               const SDL_Color& border_color = StdColor::DarkGray,
                               const SDL_Color& background_color = StdColor::LightGray, float degree = 0)
                    : _geometry(x, y, w, h), _border_size(border), _border_color(border_color),
                      _background_color(background_color), _rotate(degree) {
                updateGeometry();
                updateBorderGeometry();
            }

            void reset(float x, float y, float w, float h, uint16_t border = 1,
                       const SDL_Color& border_color = StdColor::LightGray,
                       const SDL_Color& background_color = StdColor::LightGray, float degree = 0) {
                _geometry.reset(x, y, w, h);
                _border_size = border;
                _border_color = border_color;
                _background_color = background_color;
                _rotate = degree;
                updateGeometry();
                updateBorderGeometry();
            }

            void reset(const GeometryF& geometry, uint16_t border = 1,
                       const SDL_Color& border_color = StdColor::LightGray,
                       const SDL_Color& background_color = StdColor::LightGray, float degree = 0) {
                _geometry.reset(geometry);
                _border_size = border;
                _border_color = border_color;
                _background_color = background_color;
                _rotate = degree;
                updateGeometry();
                updateBorderGeometry();
            }

            void move(float x, float y) {
                _geometry.pos.reset(x, y);
                updateGeometry();
                updateBorderGeometry();
            }

            void move(const Vector2& position) {
                _geometry.pos.reset(position);
                updateGeometry();
                updateBorderGeometry();
            }

            void resize(float w, float h) {
                _geometry.size.reset(w, h);
                updateGeometry();
                updateBorderGeometry();
            }

            void resize(const Size& size) {
                _geometry.size.reset(size);
                updateGeometry();
                updateBorderGeometry();
            }

            void setBorder(uint16_t border_size, const SDL_Color& color) {
                _border_size = border_size;
                _border_color = color;
                updateBorderGeometry();
            }

            [[nodiscard]] uint16_t borderSize() const { return _border_size; }

            void setBorderColor(const SDL_Color& color) {
                _border_color = color;
                for (auto& vertex : _border_vertices) {
                    vertex.color = Algorithm::convert2FColor(_border_color);
                }
            }

            [[nodiscard]] const SDL_Color& borderColor() const {
                return _border_color;
            }

            void setBackgroundColor(const SDL_Color& color) {
                _background_color = color;
                for (auto& vertex : _vertices) {
                    vertex.color = Algorithm::convert2FColor(_background_color);
                }
            }

            [[nodiscard]] const SDL_Color& backgroundColor() const {
                return _background_color;
            }

            void setRotate(float degree) {
                _rotate = degree;
                updateGeometry();
                updateBorderGeometry();
            }

            [[nodiscard]] float rotate() const { return _rotate; }

            void setGeometry(float x, float y, float w, float h) {
                _geometry.reset(x, y, w, h);
                updateGeometry();
                updateBorderGeometry();
            }

            void setGeometry(const GeometryF& geometry) {
                _geometry.reset(geometry);
                updateGeometry();
                updateBorderGeometry();
            }

            [[nodiscard]] const GeometryF& geometry() const { return _geometry; }

            [[nodiscard]] const SDL_Vertex* vertices() const { return _vertices.data(); }
            [[nodiscard]] size_t verticesCount() const { return _vertices.size(); }
            [[nodiscard]] const int* indices() const { return _indices.data(); }
            [[nodiscard]] size_t indicesCount() const { return _indices.size(); }
            [[nodiscard]] const SDL_Vertex* borderVertices() const { return _border_vertices.data(); }
            [[nodiscard]] size_t borderVerticesCount() const { return _border_vertices.size(); }
            [[nodiscard]] const int* borderIndices() const { return _border_indices.data(); }
            [[nodiscard]] size_t borderIndicesCount() const { return _border_indices.size(); }
        private:
            void updateGeometry() {
                // if (_background_color.a > 0 ) {
                    Algorithm::calcFilledRectangleRotated(_geometry, _background_color, _rotate,
                                                          _vertices, _indices);
                // }
            }

            void updateBorderGeometry() {
                // if (_border_size > 0 && _border_color.a > 0) {
                    Algorithm::calcRectangleRotated(_geometry, _border_color,
                                                    _border_size, _rotate,
                                                    _border_vertices, _border_indices);
                // }
            }

            GeometryF _geometry;
            uint16_t _border_size;
            SDL_Color _border_color;
            SDL_Color _background_color;
            float _rotate;
            std::array<SDL_Vertex, 4> _vertices{};
            std::array<SDL_Vertex, 8> _border_vertices{};
            std::array<int, 6> _indices{};
            std::array<int, 24> _border_indices{};
        };

        class Triangle {
        public:
            explicit Triangle() : _p1(0, 0), _p2(0, 0), _p3(0, 0),
                                  _border_size(0), _border_color(StdColor::DarkGray),
                                  _background_color(StdColor::LightGray) {}
            explicit Triangle(float x1, float y1, float x2, float y2, float x3, float y3,
                              uint16_t border_size = 0, const SDL_Color& border_color = StdColor::DarkGray,
                              const SDL_Color& back_color = StdColor::LightGray)
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
            void setBorderColor(const SDL_Color& color) {
                _border_color = color;
                updateBorder();
            }
            void setBackgroundColor(const SDL_Color& color) {
                _background_color = color;
                for (auto& vertex : _vertices) {
                    vertex.color = Algorithm::convert2FColor(color);
                }
            }
            [[nodiscard]] const Vector2& position(uint8_t index = 0) const {
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
            [[nodiscard]] uint16_t borderSize() const { return _border_size; }
            [[nodiscard]] const SDL_Color& borderColor() const { return _border_color; }
            [[nodiscard]] const SDL_Color& backgroundColor() const { return _background_color; }
            const int *indices() { return _indices.data(); }
            const SDL_Vertex *vertices() { return _vertices.data(); }
            const int *borderIndices1() { return _bdi1.data(); }
            const int *borderIndices2() { return _bdi2.data(); }
            const int *borderIndices3() { return _bdi3.data(); }
            const SDL_Vertex *borderVertices1() { return _bd1.data(); }
            const SDL_Vertex *borderVertices2() { return _bd2.data(); }
            const SDL_Vertex *borderVertices3() { return _bd3.data(); }
            [[nodiscard]] size_t borderIndicesCount() const { return _bdi1.size(); }
            [[nodiscard]] size_t borderVerticesCount() const { return _bd1.size(); }
            [[nodiscard]] size_t indicesCount() const { return _indices.size(); }
            [[nodiscard]] size_t vertexCount() const { return _vertices.size(); }
        private:
            void updateTri() {
                // if (_background_color.a > 0) {
                    Algorithm::calcTriangle(_p1, _p2, _p3, _background_color, _vertices, _indices);
                // }
            }
            void updateBorder() {
                // if (_border_size > 0 && _border_color.a > 0) {
                    Algorithm::calcLine(_p1.x, _p1.y, _p2.x, _p2.y, _border_size,
                                        _border_color, _bd1, _bdi1);
                    Algorithm::calcLine(_p2.x, _p2.y, _p3.x, _p3.y, _border_size,
                                        _border_color, _bd2, _bdi2);
                    Algorithm::calcLine(_p1.x, _p1.y, _p3.x, _p3.y, _border_size,
                                        _border_color, _bd3, _bdi3);
                // }
            }
            Vector2 _p1;
            Vector2 _p2;
            Vector2 _p3;
            uint16_t _border_size;
            SDL_Color _border_color;
            SDL_Color _background_color;
            std::array<SDL_Vertex, 3> _vertices{};
            std::array<int, 3> _indices{};
            std::array<SDL_Vertex, 4> _bd1{};
            std::array<SDL_Vertex, 4> _bd2{};
            std::array<SDL_Vertex, 4> _bd3{};
            std::array<int, 6> _bdi1{};
            std::array<int, 6> _bdi2{};
            std::array<int, 6> _bdi3{};
        };

        class Ellipse {
        public:
            explicit Ellipse() : _center_point(0, 0), _radius(0, 0),
                                 _border_size(0), _border_color(StdColor::DarkGray),
                                 _background_color(StdColor::LightGray), _degree(0.f), _count(0) {}

            Ellipse(float cx, float cy, float rw, float rh, uint16_t border_size = 1,
                    const SDL_Color& border_color = StdColor::DarkGray,
                    const SDL_Color& back_color = StdColor::LightGray,
                    float degree = 0.f, uint16_t segment = 32)
                    : _center_point(cx, cy), _radius(rw, rh), _border_size(border_size),
                      _border_color(border_color), _background_color(back_color),
                      _degree(degree), _count(segment) {
                updateFilledEllipse();
                updateEllipse();
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
                updateFilledEllipse();
                updateEllipse();
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
                updateFilledEllipse();
                updateEllipse();
            }

            void setGeometry(float x, float y, float rw, float rh) {
                _center_point.reset(x, y);
                _radius.reset(rw, rh);
                updateFilledEllipse();
                updateEllipse();
            }

            void setGeometry(const Vector2& position, const Size& size) {
                _center_point.reset(position);
                _radius.reset(size);
                updateFilledEllipse();
                updateEllipse();
            }

            void setBorder(uint16_t size, const SDL_Color& color) {
                _border_size = size;
                _border_color = color;
                updateEllipse();
            }

            void setBorderColor(const SDL_Color& color) {
                _border_color = color;
                for (auto& vertex : _border_vertices) {
                    vertex.color = Algorithm::convert2FColor(color);
                }
            }

            void setBackground(const SDL_Color& color) {
                _background_color = color;
                for (auto& vertex : _vertices) {
                    vertex.color = Algorithm::convert2FColor(color);
                }
            }

            void setRotate(float rotate) {
                _degree = rotate;
                updateFilledEllipse();
                updateEllipse();
            }

            [[nodiscard]] const Vector2& centerPosition() const { return _center_point; }
            [[nodiscard]] const Size& radius() const { return _radius; }
            [[nodiscard]] uint16_t borderSize() const { return _border_size; }
            [[nodiscard]] const SDL_Color& borderColor() const { return _border_color; }
            [[nodiscard]] const SDL_Color& backgroundColor() const { return _background_color; }
            [[nodiscard]] float rotateDegree() const { return _degree; }
            [[nodiscard]] const int *indices() const { return _indices.data(); }
            [[nodiscard]] const SDL_Vertex *vertices() const { return _vertices.data(); }
            [[nodiscard]] size_t indicesCount() const { return _indices.size(); }
            [[nodiscard]] size_t vertexCount() const { return _vertices.size(); }
            [[nodiscard]] const int *borderIndices() const { return _border_indices.data(); }
            [[nodiscard]] const SDL_Vertex *borderVertices() const { return _border_vertices.data(); }
            [[nodiscard]] size_t borderIndicesCount() const { return _border_indices.size(); }
            [[nodiscard]] size_t borderVerticesCount() const { return _border_vertices.size(); }

        private:
            void updateFilledEllipse() {
                // if (_background_color.a > 0) {
                    Algorithm::calcEllipse(_center_point, _radius,
                                                     _background_color, _degree, _count,
                                                     _vertices, _indices);
                // }
            }
            void updateEllipse() {
                // if (_border_size > 0 && _border_color.a > 0) {
                    Algorithm::calcEllipseRing(_center_point, _radius, _border_size,
                                                         _border_color, _degree, _count,
                                                         _border_vertices, _border_indices);
                // }
            }
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