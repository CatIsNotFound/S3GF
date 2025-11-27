
#ifndef S3GF_RGBACOLOR_H
#define S3GF_RGBACOLOR_H
#include "Libs.h"

namespace S3GF {
    namespace RGBAColor {
        inline uint64_t Color2RGBAValue(const SDL_Color& color, bool alpha_enabled = false) {
            return alpha_enabled ? (color.r * 255 * 255 * 255 + color.g * 255 * 255 + color.b * 255 + color.a) :
                   (color.r * 256 * 256 + color.g * 256 + color.b);
        }

        inline SDL_Color RGBAValue2Color(uint64_t color_value, bool alpha_enabled = false) {
            return (alpha_enabled ? SDL_Color((color_value >> 24) & 255, (color_value >> 16) & 255, (color_value >> 8) & 255, (color_value & 255))
                    : SDL_Color((color_value >> 16) & 255, (color_value >> 8) & 255, (color_value & 255), 255));
        }
    }
}

#endif //S3GF_RGBACOLOR_H
