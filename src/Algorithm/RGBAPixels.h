
#ifndef MYENGINE_ALGORITHM_RGBAPIXELS_H
#define MYENGINE_ALGORITHM_RGBAPIXELS_H
#include "../Core.h"
#include "../Utils/RGBAColor.h"

namespace MyEngine {
    namespace Algorithm {
        inline SDL_Color readPixelFromSurface(SDL_Surface* surface, int x, int y, bool* ok = nullptr) {
            if (!surface) {
                Logger::log("readPixelFromSurface: The specified surface is not valid!", Logger::Error);
                if (ok) *ok = false;
                return {};
            }
            if (surface->w <= x || surface->h <= y) {
                Logger::log(Logger::Error, "readPixelFromSurface: Out of range: Failed to read pixel "
                       "from the specified surface at position ({}, {}), the original surface size is {}x{}!",
                        x, y, surface->w, surface->h);
                if (ok) *ok = false;
                return {};
            }
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            const SDL_PixelFormatDetails* format = SDL_GetPixelFormatDetails(surface->format);
            const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            int pixel_offset = y * pitch + x * pixel_bytes;
            auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
            SDL_Color _ret = {};
            SDL_GetRGBA(*pixel_ptr, format, palette, &_ret.r, &_ret.g, &_ret.b, &_ret.a);
            if (ok) *ok = true;
            return _ret;
        }

        inline bool writePixelToSurface(SDL_Surface* surface, int x, int y, SDL_Color new_color) {
            if (!surface) {
                Logger::log("writePixelFromSurface: The specified surface is not valid!", Logger::Error);
                return false;
            }
            if (surface->w <= x || surface->h <= y) {
                Logger::log(Logger::Error, "writePixelFromSurface: Out of range: Failed to read pixel "
                                           "from the specified surface at position ({}, {}), "
                                           "the original surface size is {}x{}!",
                            x, y, surface->w, surface->h);
                return false;
            }
            if (!SDL_LockSurface(surface)) {
                Logger::log(FMT::format("writePixelToSurface: "
                                        "Failed to write pixel from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return false;
            }
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            int pixel_offset = y * pitch + x * pixel_bytes;
            auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
            auto pixel_value = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),
                                           palette, new_color.r, new_color.g, new_color.b, new_color.a);
            memcpy(pixel_ptr, &pixel_value, pixel_bytes);
            SDL_UnlockSurface(surface);
            return true;
        }

        inline bool writePixelsToSurface(SDL_Surface* surface, int x1, int y1, int x2, int y2, SDL_Color new_color) {
            if (!surface) {
                Logger::log("writePixelsToSurface: The specified surface is not valid!", Logger::Error);
                return false;
            }
            if (y1 >= y2 && x1 > x2) {
                std::swap(x1, x2);
                std::swap(y1, y2);
            }
            if (surface->w <= x1 || surface->h <= y1) {
                Logger::log("writePixelsToSurface: The start position is out of range!", Logger::Error);
                return false;
            }
            if (surface->w <= x2) x2 = surface->w - 1;
            if (surface->h <= y2) y2 = surface->h - 1;
            if (!SDL_LockSurface(surface)) {
                Logger::log(FMT::format("writePixelToSurface: "
                                        "Failed to write pixel from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return false;
            }
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            for (int y = y1; y <= y2; ++y) {
                for (int x = x1; x <= x2; ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    auto pixel_value = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),
                                                   palette, new_color.r, new_color.g, new_color.b, new_color.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            return true;
        }

        inline bool writePixelsToSurface(SDL_Surface* surface, const Matrix2D<SDL_Color>& color_map,
                                         Matrix2D<SDL_Color>::Position start_pos = {0, 0}) {
            if (!surface) {
                Logger::log("writePixelsToSurface: The specified surface is not valid!", Logger::Error);
                return false;
            }
            if (surface->w <= (int)start_pos.col || surface->h <= (int)start_pos.row) {
                Logger::log("writePixelsToSurface: The start position is out of range!", Logger::Error);
                return false;
            }

            if (!SDL_LockSurface(surface)) {
                Logger::log(FMT::format("writePixelToSurface: "
                                        "Failed to write pixel from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return false;
            }
            auto max_row = start_pos.row + color_map.rows();
            auto max_col = start_pos.col + color_map.cols();
            max_row = (surface->h < (int)max_row ? surface->h : max_row);
            max_col = (surface->w < (int)max_col ? surface->w : max_col);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            const SDL_Palette* palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            for (int y = (int)start_pos.row; y <= (int)(max_row); ++y) {
                for (int x = (int)start_pos.col; x <= (int)(max_col); ++x) {
                    auto& new_color = color_map.get(y, x);
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    auto pixel_value = SDL_MapRGBA(SDL_GetPixelFormatDetails(surface->format),
                                                   palette, new_color.r, new_color.g, new_color.b, new_color.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            Logger::log(Logger::Debug, "Success: Rendered {}x{} pixels, statred from ({}, {})",
                        max_col, max_row, start_pos.row, start_pos.col);
            return true;
        }

        inline Matrix2D<SDL_Color> readPixelsFromSurface(SDL_Surface* surface, bool* ok = nullptr) {
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                Logger::log(FMT::format("readPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return Matrix2D<SDL_Color>(0, 0);
            }
            Matrix2D<SDL_Color> _map(surface->h, surface->w);
            SDL_PixelFormat fmt = surface->format;
            auto fmt_details = SDL_GetPixelFormatDetails(fmt);
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(fmt);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            for (int y = 0; y < surface->h; ++y) {
                for (int x = 0; x < surface->w; ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    uint32_t pixelValue = *pixel_ptr;
                    SDL_Color& k = _map.at(y, x);
                    SDL_GetRGBA(pixelValue, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                }
            }

            SDL_UnlockSurface(surface);
            if (ok) *ok = true;
            return _map;
        }

        inline Matrix2D<SDL_Color> readPixelsOnlyFromSurface(SDL_Surface* surface,
                         Matrix2D<SDL_Color>::Position&& start_pos, Matrix2D<SDL_Color>::Position&& end_pos,
                         bool* ok = nullptr) {
            Matrix2D<SDL_Color> _map(0, 0);
            if ((int)start_pos.row >= surface->w || (int)start_pos.col >= surface->h) {
                if (ok) *ok = false;
                return _map;
            }
            if (start_pos > end_pos) std::swap(start_pos, end_pos);
            uint32_t new_width = end_pos.col - start_pos.col + 1,
                     new_height = end_pos.row - start_pos.row + 1;
            if ((int)end_pos.col >= surface->w) new_width -= end_pos.col - surface->w + 1;
            if ((int)end_pos.row >= surface->h) new_height -= end_pos.row - surface->h + 1;
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                Logger::log(FMT::format("readPixelsOnlyFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return _map;
            }
            _map.reshape(new_height, new_width);
            SDL_PixelFormat fmt = surface->format;
            auto fmt_details = SDL_GetPixelFormatDetails(fmt);
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(fmt);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            for (int y = (int)start_pos.row; y < (int)(start_pos.row + new_height); ++y) {
                for (int x = (int)start_pos.col; x < (int)(start_pos.col + new_width); ++x) {
                    if (x == (int)(start_pos.row) && y < (int)(start_pos.col)) continue;
                    if (x == (int)(end_pos.row) && y > (int)(end_pos.col)) break;
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    uint32_t pixelValue = *pixel_ptr;
                    SDL_Color& k = _map.at(y - start_pos.row, x - start_pos.col);
                    SDL_GetRGBA(pixelValue, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;
            return _map;
        }

        inline SDL_Surface* drawSurfaceByColorMap(const Matrix2D<SDL_Color>& color_map,
                                                  SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                                  bool* ok = nullptr) {
            if (color_map.cols() == 0 || color_map.rows() == 0) {
                Logger::log("drawSurfaceByColorMap: The color map is not valid!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            if (color_map.cols() > INT_MAX || color_map.rows() > INT_MAX) {
                Logger::log("drawSurfaceByColorMap: Surface size is too large!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            SDL_Surface* surface = SDL_CreateSurface((int)color_map.cols(),
                                                     (int)color_map.rows(), pixel_format);
            if (!surface) {
                Logger::log(FMT::format("drawSurfaceByColorMap: Failed to create surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            auto fmt_details = SDL_GetPixelFormatDetails(pixel_format);
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                SDL_DestroySurface(surface);
                Logger::log(FMT::format("readPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return nullptr;
            }
            for (int y = 0; y < (int)color_map.rows(); ++y) {
                for (int x = 0; x < (int)color_map.cols(); ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    SDL_Color k = color_map.get(y, x);
                    auto pixel_value = SDL_MapRGBA(fmt_details, palette, k.r, k.g, k.b, k.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;

            return surface;
        }

        inline SDL_Surface* drawSurface(const SDL_Color& color,
                                        int width, int height,
                                        SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                        bool* ok = nullptr) {
            if (width <= 0 || height <= 0) {
                Logger::log("drawSurface: "
                            "The width or height is not valid!", Logger::Error);
                if (ok) *ok = false;
                return nullptr;
            }
            SDL_Surface* surface = SDL_CreateSurface(width, height, pixel_format);
            if (!surface) {
                Logger::log(Logger::Error, "drawSurface: Failed to create surface! "
                                        "Exception: {}", SDL_GetError());
                if (ok) *ok = false;
                return nullptr;
            }
            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            auto fmt_details = SDL_GetPixelFormatDetails(pixel_format);
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                SDL_DestroySurface(surface);
                Logger::log(FMT::format("readPixelsFromSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return nullptr;
            }
            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    auto pixel_value = SDL_MapRGBA(fmt_details, palette, color.r, color.g, color.b, color.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(surface);
            if (ok) *ok = true;

            return surface;
        }

        inline SDL_Surface* drawSurface(uint64_t hex_code, int width, int height,
                                        SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                        bool* ok = nullptr) {
            return drawSurface(RGBAColor::hexCode2RGBA(hex_code, false),
                               width, height, pixel_format, ok);
        }

        inline SDL_Surface* drawFilledCircleSurface(uint32_t radius, const SDL_Color& color = StdColor::White,
                                                    SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                                    bool* ok = nullptr) {
            auto new_size = std::clamp(static_cast<int>(radius * 2), 8, INT_MAX);
            auto surface = SDL_CreateSurface(new_size, new_size, pixel_format);
            if (!surface) {
                Logger::log(Logger::Error, "drawFilledCircleSurface: Failed to create surface! ");
                if (ok) *ok = false;
                return nullptr;
            }

            auto palette = SDL_GetSurfacePalette(surface);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);
            auto fmt_details = SDL_GetPixelFormatDetails(pixel_format);
            SDL_FillSurfaceRect(surface, nullptr, 0);
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                SDL_DestroySurface(surface);
                Logger::log(FMT::format("drawFilledCircleSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return nullptr;
            }

            const int CX = new_size / 2, CY = CX;
            SDL_Color new_color(color.r, color.g, color.b, 255);
            for (int y = 0; y < new_size; ++y) {
                for (int x = 0; x < new_size; ++x) {
                    // Calculate distance of the center of circle
                    float dx = static_cast<float>(x - CX) + 0.5f;
                    float dy = static_cast<float>(y - CY) + 0.5f;
                    float dist = sqrtf(dx * dx + dy * dy);

                    // Distance field anti-aliasing: ��0.5 pixel gradient on edges
                    float alpha = std::clamp(1.f - (dist - (static_cast<float>(radius) - 0.5f)), 0.f, 1.f);
                    new_color.a = static_cast<uint8_t>(alpha * 255.f);
                    if (new_color.a <= 0) continue;

                    // Draw pixel
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    auto pixel_value = SDL_MapRGBA(fmt_details, palette,
                                        new_color.r, new_color.g, new_color.b, new_color.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }

            SDL_UnlockSurface(surface);
            if (ok) *ok = true;

            return surface;
        }

        inline SDL_Surface* drawFilledRoundedRectangleSurface(int width, int height, int radius,
                                                const SDL_Color& color = StdColor::White,
                                                SDL_PixelFormat pixel_format = SDL_PIXELFORMAT_RGBA8888,
                                                bool* ok = nullptr) {
            if (width < 0 || height < 0) {
                Logger::log(Logger::Error, "drawFilledCircleSurface: Invalid width or height! ");
                if (ok) *ok = false;
                return nullptr;
            }
            const int CIR_DIAMETER = radius * 2;
            width = std::clamp(width, CIR_DIAMETER, INT_MAX);
            height = std::clamp(height, CIR_DIAMETER, INT_MAX);

            auto surface = SDL_CreateSurface(width, height, pixel_format);
            if (!surface) {
                Logger::log(Logger::Error, "drawFilledCircleSurface: Failed to create surface! ");
                if (ok) *ok = false;
                return nullptr;
            }
            if (!SDL_LockSurface(surface)) {
                if (ok) *ok = false;
                SDL_DestroySurface(surface);
                Logger::log(FMT::format("drawFilledCircleSurface: "
                                        "Failed to get pixels from the specified surface! "
                                        "Exception: {}", SDL_GetError()), Logger::Error);
                return nullptr;
            }

            // Draw 2 rects
            const int TW = width - radius;
            const int TH = height - radius;
            if (TW > radius)
                writePixelsToSurface(surface, radius, 0, TW, height, color);
            if (TH > radius)
                writePixelsToSurface(surface, 0, radius, width, TH, color);
            if (radius <= 0) {
                SDL_UnlockSurface(surface);
                if (ok) *ok = true;
                return surface;
            }
            auto palette = SDL_GetSurfacePalette(surface);
            auto fmt_details = SDL_GetPixelFormatDetails(pixel_format);
            int pitch = surface->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(surface->format);
            auto pixel_buffer = static_cast<uint8_t*>(surface->pixels);

            // Draw 4 quarter circles at corners
            const int CX[4] = {radius, width - radius, radius, width - radius};
            const int CY[4] = {radius, radius, height - radius, height - radius};
            
            SDL_Color new_color(color.r, color.g, color.b, 255);
            
            // Draw each quarter circle
            for (int corner = 0; corner < 4; ++corner) {
                int cx = CX[corner];
                int cy = CY[corner];
                
                // Determine the region for this quarter circle
                int x_start = (corner % 2 == 0) ? 0 : width - radius;
                int x_end = (corner % 2 == 0) ? radius : width;
                int y_start = (corner < 2) ? 0 : height - radius;
                int y_end = (corner < 2) ? radius : height;
                
                for (int y = y_start; y < y_end; ++y) {
                    for (int x = x_start; x < x_end; ++x) {
                        // Calculate distance from the center of this quarter circle
                        float dx = static_cast<float>(x - cx) + 0.5f;
                        float dy = static_cast<float>(y - cy) + 0.5f;
                        float dist = sqrtf(dx * dx + dy * dy);

                        // Distance field anti-aliasing: 0.5 pixel gradient on edges
                        float alpha = std::clamp(1.f - (dist - (static_cast<float>(radius) - 0.5f)), 0.f, 1.f);
                        new_color.a = static_cast<uint8_t>(alpha * 255.f);
                        if (new_color.a <= 0) continue;

                        // Draw pixel
                        int pixel_offset = y * pitch + x * pixel_bytes;
                        auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                        auto pixel_value = SDL_MapRGBA(fmt_details, palette,
                                            new_color.r, new_color.g, new_color.b, new_color.a);
                        memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                    }
                }
            }

            SDL_UnlockSurface(surface);
            if (ok) *ok = true;

            return surface;
        }

        inline SDL_Surface* processSurface(SDL_Surface* surface,
                                           const std::function<void(SDL_Color&)>& process_function,
                                           bool* ok = nullptr) {
            if (!surface) {
                Logger::log(Logger::Error, "Process surface failed: "
                                           "The specified surface is not valid!");
                if (ok) *ok = false;
                return nullptr;
            }
            SDL_Surface* _ret = SDL_DuplicateSurface(surface);
            if (!_ret) {
                Logger::log(Logger::Error, "Process surface failed: "
                                           "Can't copy from the specified surface.");
                if (ok) *ok = false;
                return nullptr;
            }
            if (!SDL_LockSurface(_ret)) {
                if (ok) *ok = false;
                SDL_DestroySurface(_ret);
                Logger::log(Logger::Error, "Process surface failed: Failed to lock surface! "
                                        "Exception: {}", SDL_GetError());
                return nullptr;
            }
            auto palette = SDL_GetSurfacePalette(_ret);
            int pitch = _ret->pitch;
            int pixel_bytes = SDL_BYTESPERPIXEL(_ret->format);
            auto pixel_buffer = static_cast<uint8_t*>(_ret->pixels);
            auto fmt_details = SDL_GetPixelFormatDetails(_ret->format);
            for (int y = 0; y < _ret->h; ++y) {
                for (int x = 0; x < _ret->w; ++x) {
                    int pixel_offset = y * pitch + x * pixel_bytes;
                    auto pixel_ptr = reinterpret_cast<uint32_t*>(pixel_buffer + pixel_offset);
                    uint32_t pixel_value = *pixel_ptr;
                    SDL_Color k = StdColor::White;
                    SDL_GetRGBA(pixel_value, fmt_details, palette, &k.r, &k.g, &k.b, &k.a);
                    process_function(k);
                    pixel_value = SDL_MapRGBA(fmt_details, palette, k.r, k.g, k.b, k.a);
                    memcpy(pixel_ptr, &pixel_value, pixel_bytes);
                }
            }
            SDL_UnlockSurface(_ret);
            if (ok) *ok = true;
            return _ret;
        }

        inline SDL_Surface* applyFilledWhiteSurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                color.r = 255; color.g = 255; color.b = 255;
            }, ok);
        }

        inline SDL_Surface* applyInvertedColorSurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                color.r = 255 - color.r; color.g = 255 - color.g; color.b = 255 - color.b;
            }, ok);
        }

        inline SDL_Surface* applyGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = static_cast<uint8_t>((77 * color.r + 150 * color.g + 29 * color.b) >> 8);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* applyDarkGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = std::min(std::min(color.r, color.g), color.b);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* applyLightGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = std::max(std::max(color.r, color.g), color.b);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* applyAvgGraySurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                auto gray = static_cast<uint8_t>((color.r + color.g + color.b) / 3);
                color = { gray, gray, gray, color.a };
            }, ok);
        }

        inline SDL_Surface* applyBrightnessSurface(SDL_Surface* surface, int16_t value, bool* ok = nullptr) {
            value = std::clamp(value, (int16_t)-255, (int16_t)255);
            return processSurface(surface, [&value](SDL_Color& color) {
                color.r = std::clamp(color.r + value, 0, 255);
                color.g = std::clamp(color.g + value, 0, 255);
                color.b = std::clamp(color.b + value, 0, 255);
            }, ok);
        }

        inline SDL_Surface* applyContrastSurface(SDL_Surface* surface, float alpha, bool* ok = nullptr) {
            alpha = std::clamp(alpha, 0.f, 3.f);
            
            return processSurface(surface, [&alpha](SDL_Color& color) {
                float r_val = (static_cast<float>(color.r) - 128.0f) * alpha + 128.0f;
                float g_val = (static_cast<float>(color.g) - 128.0f) * alpha + 128.0f;
                float b_val = (static_cast<float>(color.b) - 128.0f) * alpha + 128.0f;

                color.r = static_cast<uint8_t>(std::clamp(r_val, 0.0f, 255.0f));
                color.g = static_cast<uint8_t>(std::clamp(g_val, 0.0f, 255.0f));
                color.b = static_cast<uint8_t>(std::clamp(b_val, 0.0f, 255.0f));
            }, ok);
        }

        inline SDL_Surface* applyAdvancedContrastSurface(SDL_Surface* surface, float alpha, float brightness = 0.0f, bool* ok = nullptr) {
            alpha = std::clamp(alpha, 0.f, 3.f);
            brightness = std::clamp(brightness, -1.0f, 1.0f);
            
            float factor = (259.0f * (alpha * 255.0f + 255.0f)) / (255.0f * (259.0f - alpha * 255.0f));
            int brightness_offset = static_cast<int>(brightness * 50.0f);
            
            return processSurface(surface, [&](SDL_Color& color) {
                color.r = static_cast<uint8_t>(std::clamp(
                    static_cast<int>(factor * (static_cast<float>(color.r) - 128.0f) + 128.0f) + brightness_offset,
                    0, 255
                ));
                color.g = static_cast<uint8_t>(std::clamp(
                    static_cast<int>(factor * (static_cast<float>(color.g) - 128.0f) + 128.0f) + brightness_offset,
                    0, 255
                ));
                color.b = static_cast<uint8_t>(std::clamp(
                    static_cast<int>(factor * (static_cast<float>(color.b) - 128.0f) + 128.0f) + brightness_offset,
                    0, 255
                ));
            }, ok);
        }

        inline SDL_Surface* applySaturationSurface(SDL_Surface* surface, float saturation, bool* ok = nullptr) {
            saturation = std::clamp(saturation, 0.f, 3.f);
            return processSurface(surface, [&saturation](SDL_Color& color) {
                float gray = (0.299f * color.r + 0.587f * color.g + 0.114f * color.b) / 255.f;
                color.r = static_cast<uint8_t>(std::clamp(gray * 255.f + saturation * (color.r - gray * 255.f), 0.f,
                                                          255.f));
                color.g = static_cast<uint8_t>(std::clamp(gray * 255.f + saturation * (color.g - gray * 255.f), 0.f,
                                                          255.f));
                color.b = static_cast<uint8_t>(std::clamp(gray * 255.f + saturation * (color.b - gray * 255.f), 0.f,
                                                          255.f));
            }, ok);
        }

        inline SDL_Surface* applySepiaToneSurface(SDL_Surface* surface, bool* ok = nullptr) {
            return processSurface(surface, [](SDL_Color& color) {
                uint8_t r = static_cast<uint8_t>(std::min(
                    255, static_cast<int>(0.393f * color.r + 0.769f * color.g + 0.189f * color.b)));
                uint8_t g = static_cast<uint8_t>(std::min(
                    255, static_cast<int>(0.349f * color.r + 0.686f * color.g + 0.168f * color.b)));
                uint8_t b = static_cast<uint8_t>(std::min(
                    255, static_cast<int>(0.272f * color.r + 0.534f * color.g + 0.131f * color.b)));
                color.r = r; color.g = g; color.b = b;
            }, ok);
        }

        inline SDL_Surface* applyGammaCorrectionSurface(SDL_Surface* surface, float gamma, bool* ok = nullptr) {
            gamma = std::max(gamma, 0.1f);
            float inv_gamma = 1.0f / gamma;
            return processSurface(surface, [&inv_gamma](SDL_Color& color) {
                color.r = static_cast<uint8_t>(255 * pow(color.r / 255.0f, inv_gamma));
                color.g = static_cast<uint8_t>(255 * pow(color.g / 255.0f, inv_gamma));
                color.b = static_cast<uint8_t>(255 * pow(color.b / 255.0f, inv_gamma));
            }, ok);
        }

        inline SDL_Surface* applyPosterizeSurface(SDL_Surface* surface, int levels, bool* ok = nullptr) {
            levels = std::clamp(levels, 2, 256);
            float factor = 256.0f / levels;
            return processSurface(surface, [&factor](SDL_Color& color) {
                color.r = static_cast<uint8_t>(static_cast<int>(color.r / factor) * factor);
                color.g = static_cast<uint8_t>(static_cast<int>(color.g / factor) * factor);
                color.b = static_cast<uint8_t>(static_cast<int>(color.b / factor) * factor);
            }, ok);
        }

        inline SDL_Surface* applySolarizeSurface(SDL_Surface* surface, int threshold = 128, bool* ok = nullptr) {
            threshold = std::clamp(threshold, 0, 255);
            return processSurface(surface, [&threshold](SDL_Color& color) {
                if (color.r > threshold) {
                    float excess = static_cast<float>(color.r - threshold) / (255.0f - threshold);
                    color.r = static_cast<uint8_t>(
                        static_cast<float>(255 - color.r) * excess + static_cast<float>(color.r) * (1.0f - excess)
                    );
                }
                if (color.g > threshold) {
                    float excess = static_cast<float>(color.g - threshold) / (255.0f - threshold);
                    color.g = static_cast<uint8_t>(
                        static_cast<float>(255 - color.g) * excess + static_cast<float>(color.g) * (1.0f - excess)
                    );
                }
                if (color.b > threshold) {
                    float excess = static_cast<float>(color.b - threshold) / (255.0f - threshold);
                    color.b = static_cast<uint8_t>(
                        static_cast<float>(255 - color.b) * excess + static_cast<float>(color.b) * (1.0f - excess)
                    );
                }
            }, ok);
        }

        inline SDL_Surface* applySolarizeSurface(SDL_Surface* surface, int threshold, float intensity, bool* ok = nullptr) {
            threshold = std::clamp(threshold, 0, 255);
            intensity = std::clamp(intensity, 0.0f, 1.0f);
            
            return processSurface(surface, [&](SDL_Color& color) {
                if (color.r > threshold) {
                    float excess = static_cast<float>(color.r - threshold) / (255.0f - threshold);
                    float inverted = static_cast<float>(255 - color.r);
                    color.r = static_cast<uint8_t>(
                        inverted * excess * intensity + static_cast<float>(color.r) * (1.0f - excess * intensity)
                    );
                }
                if (color.g > threshold) {
                    float excess = static_cast<float>(color.g - threshold) / (255.0f - threshold);
                    float inverted = static_cast<float>(255 - color.g);
                    color.g = static_cast<uint8_t>(
                        inverted * excess * intensity + static_cast<float>(color.g) * (1.0f - excess * intensity)
                    );
                }
                if (color.b > threshold) {
                    float excess = static_cast<float>(color.b - threshold) / (255.0f - threshold);
                    float inverted = static_cast<float>(255 - color.b);
                    color.b = static_cast<uint8_t>(
                        inverted * excess * intensity + static_cast<float>(color.b) * (1.0f - excess * intensity)
                    );
                }
            }, ok);
        }

        inline SDL_Surface *applyColorBalanceSurface(SDL_Surface *surface, int red_shift, int green_shift,
                                                     int blue_shift, bool *ok = nullptr) {
            red_shift = std::clamp(red_shift, -255, 255);
            green_shift = std::clamp(green_shift, -255, 255);
            blue_shift = std::clamp(blue_shift, -255, 255);
            return processSurface(surface, [&](SDL_Color& color) {
                color.r = std::clamp(color.r + red_shift, 0, 255);
                color.g = std::clamp(color.g + green_shift, 0, 255);
                color.b = std::clamp(color.b + blue_shift, 0, 255);
            }, ok);
        }
    }
}

#endif //MYENGINE_ALGORITHM_RGBAPIXELS_H
