
#pragma once
#ifndef MYENGINE_ALGORITHM_DRAW_H
#define MYENGINE_ALGORITHM_DRAW_H
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
#include "../Libs.h"

namespace MyEngine {
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

        inline void calcPoint(const Vector2& position, float radius, SDL_Color color,
                      std::vector<SDL_Vertex>& vertices, std::vector<int>& indices,
                      const uint16_t count = 32) {
            vertices.clear();
            indices.clear();
            
            const uint16_t actual_count = std::max(count, static_cast<uint16_t>(3));
            
            SDL_FColor fcolor = convert2FColor(color);
            
            vertices.push_back({{position.x, position.y}, fcolor, {0, 0}});
            
            for (uint16_t i = 0; i < actual_count; ++i) {
                float angle = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(actual_count);
                float x = position.x + cosf(angle) * radius;
                float y = position.y + sinf(angle) * radius;
                vertices.push_back({{x, y}, fcolor, {0, 0}});
            }
            
            for (uint16_t i = 0; i < actual_count; ++i) {
                indices.push_back(0);
                indices.push_back(i + 1);
                indices.push_back((i + 1) % actual_count + 1);
            }
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

            float extend = thickness * 0.5f;
            float ex1 = -dx / len * extend;
            float ey1 = -dy / len * extend;
            float ex2 = dx / len * extend;
            float ey2 = dy / len * extend;

            SDL_FColor fcolor = convert2FColor(color);
            vertex[0] = {{x1 + ex1 - px, y1 + ey1 - py}, fcolor, {0, 0}};
            vertex[1] = {{x1 + ex1 + px, y1 + ey1 + py}, fcolor, {0, 0}};
            vertex[2] = {{x2 + ex2 + px, y2 + ey2 + py}, fcolor, {0, 0}};
            vertex[3] = {{x2 + ex2 - px, y2 + ey2 - py}, fcolor, {0, 0}};
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 0;
            indices[4] = 2;
            indices[5] = 3;
        }

        inline void calcRectangleBorder(const GeometryF& geometry, uint16_t border_size,
                                        std::array<SDL_FRect, 4>& borders) {
            const float THICKNESS = border_size;
            const float X = geometry.pos.x;
            const float Y = geometry.pos.y;
            const float W = geometry.size.width;
            const float H = geometry.size.height;
            borders[0] = {X, Y, W, THICKNESS };
            borders[1] = {X, Y + H - THICKNESS, W, THICKNESS };
            borders[2] = {X, Y + THICKNESS, THICKNESS, H - 2 * THICKNESS };
            borders[3] = {X + W - THICKNESS, Y + THICKNESS, THICKNESS, H - 2 * THICKNESS };
        }

        inline void calcFilledRectangleRotated(const GeometryF& geometry, const SDL_Color& color, float degree,
                                               std::array<SDL_Vertex, 4>& vertices, std::array<int, 6>& indices) {
            float cx = geometry.pos.x + geometry.size.width  * 0.5f;
            float cy = geometry.pos.y + geometry.size.height * 0.5f;

            float hw = geometry.size.width  * 0.5f;
            float hh = geometry.size.height * 0.5f;

            struct { float x, y; } local[4] = {
                    { -hw, -hh },   // LT
                    {  hw, -hh },   // RT
                    {  hw,  hh },   // RB
                    { -hw,  hh }    // LB
            };

            SDL_FColor fcolor = convert2FColor(color);

            for (int i = 0; i < 4; ++i) {
                float x = local[i].x;
                float y = local[i].y;

                float rad = degree * M_PI / 180.0f;
                float c = cosf(rad);
                float s = sinf(rad);

                float wx = cx + (x * c - y * s);
                float wy = cy + (x * s + y * c);

                vertices[i] = { {wx, wy}, fcolor, {0, 0} };
            }

            indices = { 0, 1, 2, 0, 2, 3 };
        }

        inline void calcRectangleRotated(const GeometryF& geometry, const SDL_Color& color, uint16_t size, float degree,
                                         std::array<SDL_Vertex, 8>& vertices, std::array<int, 24>& indices) {
            float cx = geometry.pos.x + geometry.size.width  * 0.5f;
            float cy = geometry.pos.y + geometry.size.height * 0.5f;
            float hw = geometry.size.width  * 0.5f;
            float hh = geometry.size.height * 0.5f;
            float halfW = size * 0.5f;

            struct { float x, y; } outer[4] = {
                    { -hw        , -hh         },
                    {  hw        , -hh         },
                    {  hw        ,  hh         },
                    { -hw        ,  hh         }
            };

            struct { float x, y; } inner[4] = {
                    { -hw + halfW, -hh + halfW },
                    {  hw - halfW, -hh + halfW },
                    {  hw - halfW,  hh - halfW },
                    { -hw + halfW,  hh - halfW }
            };

            SDL_FColor fcolor = convert2FColor(color);

            float rad = degree * M_PI / 180.0f;
            float c = cosf(rad), s = sinf(rad);

            for (int i = 0; i < 4; ++i) {
                float ox = cx + (outer[i].x * c - outer[i].y * s);
                float oy = cy + (outer[i].x * s + outer[i].y * c);
                float ix = cx + (inner[i].x * c - inner[i].y * s);
                float iy = cy + (inner[i].x * s + inner[i].y * c);

                vertices[i]     = { {ox, oy}, fcolor, {0, 0} };
                vertices[i + 4] = { {ix, iy}, fcolor, {0, 0} };
            }

            int* idx = indices.data();
            for (int i = 0; i < 4; ++i) {
                int n = i * 6;
                int o0 = i;
                int o1 = (i + 1) & 3;
                int i0 = i + 4;
                int i1 = ((i + 1) & 3) + 4;

                // Triangle 1
                idx[n + 0] = o0;
                idx[n + 1] = o1;
                idx[n + 2] = i1;
                // Triangle 2
                idx[n + 3] = o0;
                idx[n + 4] = i1;
                idx[n + 5] = i0;
            }
        }

        inline void calcTriangle(const Vector2& pos1, const Vector2& pos2, const Vector2& pos3, const SDL_Color& color,
                                 std::array<SDL_Vertex, 3>& vertices, std::array<int, 3>& indices) {
            SDL_FColor fcolor = convert2FColor(color);
            vertices[0] = {{pos1.x, pos1.y}, fcolor};
            vertices[1] = {{pos2.x, pos2.y}, fcolor};
            vertices[2] = {{pos3.x, pos3.y}, fcolor};
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
        }

        inline void calcEllipse(const Vector2& center_pt, const Size& radius, const SDL_Color& color,
                                float degree, uint16_t segment, std::vector<SDL_Vertex>& vertices,
                                std::vector<int>& indices) {
            vertices.clear();
            indices.clear();
            auto colorF = convert2FColor(color);
            float rot = degree * M_PI / 180.f;
            float cosRot = cosf(rot), sinRot = sinf(rot);

            vertices.resize(segment + 2);
            vertices[0] = { {center_pt.x, center_pt.y}, colorF, {0, 0} };
            for (int i = 0; i <= segment; ++i) {
                float theta = 2.f * M_PI * i / segment;
                float x = radius.width * cosf(theta);
                float y = radius.height * sinf(theta);
                vertices[i + 1] = SDL_Vertex{{ center_pt.x + x * cosRot - y * sinRot,
                               center_pt.y + x * sinRot + y * cosRot },
                                             colorF, {0, 0} };
            }
            indices.resize(segment * 3);
            for (int i = 0; i < segment; ++i) {
                indices[3 * i] = 0;
                indices[3 * i + 1] = i + 1;
                indices[3 * i + 2] = i + 2;
            }
        }

        inline void calcEllipseRing(const Vector2& center_pt, const Size& radius, uint16_t border_size,
                                    const SDL_Color& border_color, float degree, uint16_t segment,
                                    std::vector<SDL_Vertex>& vertices, std::vector<int>& indices) {
            float rx2 = radius.width - border_size, ry2 = radius.height - border_size;
            if (rx2 <= 0 || ry2 <= 0) return;
            auto colorF = convert2FColor(border_color);
            vertices.clear();
            indices.clear();

            float rot = degree * M_PI / 180.f;
            float cosR = cosf(rot), sinR = sinf(rot);

            vertices.reserve((segment + 1) * 2);
            for (int i = 0; i <= segment; ++i) {
                float theta = 2.f * M_PI * i / segment;
                float ex = radius.width * cosf(theta);
                float ey = radius.height * sinf(theta);
                float ix = rx2 * cosf(theta);
                float iy = ry2 * sinf(theta);
                vertices.push_back({ {center_pt.x + ex * cosR - ey * sinR,
                                      center_pt.y + ex * sinR + ey * cosR}, colorF,
                                     {0,0} });
                vertices.push_back({ {center_pt.x + ix * cosR - iy * sinR,
                               center_pt.y + ix * sinR + iy * cosR}, colorF, {0,0} });
            }

            for (int i = 0; i < segment * 2; i += 2) {
                indices.insert(indices.end(), { i, i + 1, i + 2,   i + 1, i + 3, i + 2 });
            }

            indices[indices.size() - 2] = 0;
            indices[indices.size() - 1] = 1;
        }
    }
}

#endif //MYENGINE_ALGORITHM_DRAW_H
