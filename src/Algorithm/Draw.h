
#pragma once
#ifndef S3GF_DRAW_H
#define S3GF_DRAW_H

#include "Libs.h"

namespace S3GF {
    namespace Algorithm {
        inline SDL_FColor convert2FColor(const SDL_Color &color) {
            return {color.r / 255.f, color.g / 255.f, color.b / 255.f, color.a / 255.f};
        }

        inline SDL_Color convert2Color(const SDL_FColor &color) {
            return {static_cast<uint8_t>(255 * color.r),
                    static_cast<uint8_t>(255 * color.g),
                    static_cast<uint8_t>(255 * color.b),
                    static_cast<uint8_t>(255 * color.a)};
        }

        inline void calcLine(float x1, float y1, float x2, float y2, uint16_t thickness, const SDL_Color &color,
                             std::array<SDL_Vertex, 4> &vertex, std::array<int, 6> &indices) {
            float dx = x2 - x1;
            float dy = y2 - y1;
            float len = hypotf(dx, dy);
            if (len == 0.0f) return;

            float nx = -dy / len;
            float ny = dx / len;

            float px = nx * (float) thickness * 0.5f;
            float py = ny * (float) thickness * 0.5f;

            SDL_FColor fcolor = convert2FColor(color);
            vertex[0] = {{x1 - px, y1 - py}, fcolor, {0, 0}};
            vertex[1] = {{x1 + px, y1 + py}, fcolor, {0, 0}};
            vertex[2] = {{x2 + px, y2 + py}, fcolor, {0, 0}};
            vertex[3] = {{x2 - px, y2 - py}, fcolor, {0, 0}};
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 0;
            indices[4] = 2;
            indices[5] = 3;
        }
    }
}

#endif //S3GF_DRAW_H
