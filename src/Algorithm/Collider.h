#ifndef MYENGINE_ALGORITHM_COLLIDER_H
#define MYENGINE_ALGORITHM_COLLIDER_H
#include "../Basic.h"

namespace MyEngine {
    namespace Algorithm {
        namespace Vector2EX {
            inline void normalize(float &x, float &y) {
                const float LENGTH = sqrtf(x * x + y * y);
                if (LENGTH == 0.f) return;
                x /= LENGTH;
                y /= LENGTH;
            }

            inline void normalize(Vector2 &vec) {
                const float LENGTH = sqrtf(vec.x * vec.x + vec.y * vec.y);
                if (LENGTH == 0.f) return;
                vec.x /= LENGTH;
                vec.y /= LENGTH;
            }

            inline float dot(const Vector2& v1, const Vector2& v2) {
                return v1.x * v2.x + v1.y * v2.y;
            }

            inline float dot(float x1, float y1, float x2, float y2) {
                return x1 * x2 + y1 * y2;
            }

            inline Vector2 subtract(const Vector2& v1, const Vector2& v2) {
                return {v1.x - v2.x, v1.y - v2.y};
            }

            struct ExtremalValue {
                float min;
                float max;
            };

            inline ExtremalValue projectPolygon(const std::vector<Vector2>& points, const Vector2& axis) {
                std::vector<float> values;
                for (auto& p : points) {
                    values.emplace_back(dot(p, axis));
                }
                return {std::ranges::min(values), std::ranges::max(values)};
            }
        }

        inline GeometryF translateObjectInParent(const GeometryF& object, const GeometryF& parent) {
            GeometryF new_geometry;
            new_geometry.move(parent.pos + object.pos);
            Size available_size(
                    parent.size.width - object.pos.x,
                    parent.size.height - object.pos.y
            );

            available_size.width = std::max(0.0f, available_size.width);
            available_size.height = std::max(0.0f, available_size.height);

            Size final_size;
            final_size.width = std::min(object.size.width, available_size.width);
            final_size.height = std::min(object.size.height, available_size.height);
            new_geometry.resize(final_size);
            return new_geometry;
        }

        inline int8_t comparePosInGeometry(const Vector2& vec, const GeometryF& geometry) {
            float left =geometry.pos.x;
            float right = left + geometry.size.width;
            float top = geometry.pos.y;
            float bottom = top + geometry.size.height;
            if ((vec.x > left) && (vec.x < right) && (vec.y > top) && (vec.y < bottom)) return 1;

            bool onX = (vec.x == left || vec.x == right);
            bool onY = (vec.y == top  || vec.y == bottom);
            if (onX && onY) return 0;

            return -1;
        }

        inline int8_t comparePosInRect(const Vector2& vec, const Graphics::Rectangle& rect) {
            return comparePosInGeometry(vec, rect.geometry());
        }

        inline int8_t comparePosInRotatedRect(const Vector2& vec, const Graphics::Rectangle& rect) {
            auto rotate_deg = static_cast<float>(rect.rotation());
            if (fabsf(fmodf(rotate_deg, 180.f)) == 0.f) return comparePosInRect(vec, rect);

            float left = rect.geometry().pos.x;
            float right = left + rect.geometry().size.width;
            float top = rect.geometry().pos.y;
            float bottom = top + rect.geometry().size.height;
            float cx = (left + right) * 0.5f, cy = (top + bottom) * 0.5f;
            const float DX = vec.x - cx, DY = vec.y - cy;
            const float RAD = rotate_deg * M_PI / 180.f;
            const float COS = cosf(RAD);
            const float SIN = sinf(RAD);
            // Rotated position
            const float LX = DX * COS + DY * SIN;
            const float LY = -DX * SIN + DY * COS;

            // Is point in rectangle?
            const float HALF_W = rect.geometry().size.width * 0.5f;
            const float HALF_H = rect.geometry().size.height * 0.5f;

            // If the following conditions are met, the point is inside the rotated rectangle.
            bool ret = LX >= -HALF_W && LX <= HALF_W &&
                       LY >= -HALF_H && LY <= HALF_H;
            if (!ret) return -1;
            // Is point is in border?
            if (fabsf(LX - HALF_W) < 0.f || fabsf(LX + HALF_W) < 0.f ||
                fabsf(LY - HALF_H) < 0.f || fabsf(LY + HALF_H) < 0.f) {
                return 0;
            }
            return 1;
        }

        inline int8_t comparePosInPoint(const Vector2& pos, const Graphics::Point& point) {
            auto pt = point.position();
            if (point.size() < 2) {
                return (point.position().isEqual(pos) ? (int8_t)1 : (int8_t)-1);
            }
            float dx = pos.x - pt.x;
            float dy = pos.y - pt.y;
            float dd = dx * dx + dy * dy;
            float rr = (static_cast<float>(point.size()) / 2.f) * (static_cast<float>(point.size()) / 2.f);

            if (dd > rr + 1e-5f) return -1;
            if (std::fabs(dd - rr) < 1e-5f) return 0;
            return 1;
        }

        inline int8_t comparePosInRotatedEllipse(const Vector2& pos, const Graphics::Ellipse& ellipse,
                        const float EPISON = 1e-6) {
            if (ellipse.size().width <= 0 || ellipse.size().height <= 0) return -1;
            const auto THETA = ellipse.rotation() * (M_PI / 180.f);
            const auto SIZE = ellipse.size();
            const Vector2 P = pos - ellipse.centerPosition();
            const float X_ROTATE = P.x * cosf(THETA) + P.y * sinf(THETA);
            const float Y_ROTATE = -P.x * sinf(THETA) + P.y * cosf(THETA);

            const float VALUE = (X_ROTATE * X_ROTATE) / (SIZE.width * SIZE.width) +
                                (Y_ROTATE * Y_ROTATE) / (SIZE.height * SIZE.height);
            if (fabs(VALUE - 1.f) <= EPISON) return 0;
            if (VALUE < 1.f) return 1;
            return -1;
        }

        inline int8_t comparePoints(const Graphics::Point& point1, const Graphics::Point& point2) {
            auto p1 = point1.position(), p2 = point2.position();
            if (point1.size() < 2 && point2.size() < 2) {
                return (point1.position().isEqual(point2.position()) ? (int8_t)1 : (int8_t)-1);
            }
            auto r1 = static_cast<float>(point1.size()) * 0.5f, r2 = static_cast<float>(point2.size()) * 0.5f;
            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;
            float d  = std::hypot(dx, dy);          // 圆心距
            float r_sum = r1 + r2;

            if (d > r_sum + 1e-5f) return -1;                // 完全相离
            if (std::fabs(d - r_sum) < 1e-5f) return 0;   // 外切（只有一个交点）
            return 1;
        }

        inline int8_t compareGeometryF(const GeometryF& geometry1, const GeometryF geometry2) {
            float x1 = geometry1.pos.x, x2 = geometry1.pos.x + geometry1.size.width,
                  y1 = geometry1.pos.y, y2 = geometry1.pos.y + geometry1.size.height,
                  x3 = geometry2.pos.x, x4 = geometry2.pos.x + geometry2.size.width,
                  y3 = geometry2.pos.y, y4 = geometry2.pos.y + geometry2.size.height;

            if (x2 < x3 || x1 > x4 || y2 < y3 || y1 > y4) return -1;
            if (x3 <= x1 && y3 <= y1 && x4 >= x2 && y4 >= y2) return 2;
            if (x1 <= x3 && y1 <= y3 && x2 >= x4 && y2 >= y4) return 3;
            return 1;
        }

        inline int8_t compareRects(const Graphics::Rectangle& rect1, const Graphics::Rectangle& rect2) {
            float x1 = rect1.geometry().pos.x, x2 = rect1.geometry().pos.x + rect1.geometry().size.width,
                  y1 = rect1.geometry().pos.y, y2 = rect1.geometry().pos.y + rect1.geometry().size.height,
                  x3 = rect2.geometry().pos.x, x4 = rect2.geometry().pos.x + rect2.geometry().size.width,
                  y3 = rect2.geometry().pos.y, y4 = rect2.geometry().pos.y + rect2.geometry().size.height;

            if (x2 < x3 || x1 > x4 || y2 < y3 || y1 > y4) return -1;
            if (x3 <= x1 && y3 <= y1 && x4 >= x2 && y4 >= y2) return 2;
            if (x1 <= x3 && y1 <= y3 && x2 >= x4 && y2 >= y4) return 3;
            return 1;
        }

        inline int8_t compareRotatedRects(const Graphics::Rectangle& rect1, const Graphics::Rectangle& rect2) {
            auto rotate_deg_1 = rect1.rotation();
            auto rotate_deg_2 = rect2.rotation();
            const bool NO_ROTATE_1 = (fabsf(fmodf(rotate_deg_1, 180.f)) == 0.f);
            const bool NO_ROTATE_2 = (fabsf(fmodf(rotate_deg_2, 180.f)) == 0.f);
            if (NO_ROTATE_1 && NO_ROTATE_2)
                return compareRects(rect1, rect2);

            // Rect 1
            float left_1 = rect1.geometry().pos.x;
            float right_1 = rect1.geometry().pos.x + rect1.geometry().size.width;
            float top_1 = rect1.geometry().pos.y;
            float bottom_1 = rect1.geometry().pos.y + rect1.geometry().size.height;
            const float CX_1 = (left_1 + right_1) * 0.5f, CY_1 = (top_1 + bottom_1) * 0.5f;
            const float HW_1 = rect1.geometry().size.width * 0.5f;
            const float HH_1 = rect1.geometry().size.height * 0.5f;
            const float RAD_1 = rotate_deg_1 * M_PI / 180.f;
            const float COS_1 = cosf(RAD_1);
            const float SIN_1 = sinf(RAD_1);
            // Rect 2
            float left_2 = rect2.geometry().pos.x;
            float right_2 = rect2.geometry().pos.x + rect2.geometry().size.width;
            float top_2 = rect2.geometry().pos.y;
            float bottom_2 = rect2.geometry().pos.y + rect2.geometry().size.height;
            const float CX_2 = (left_2 + right_2) * 0.5f, CY_2 = (top_2 + bottom_2) * 0.5f;
            const float HW_2 = rect2.geometry().size.width * 0.5f;
            const float HH_2 = rect2.geometry().size.height * 0.5f;
            const float RAD_2 = rotate_deg_2 * M_PI / 180.f;
            const float COS_2 = cosf(RAD_2);
            const float SIN_2 = sinf(RAD_2);

            // Axis (x, y) Unit normal vector
            float axes[4][2] = {
                    {COS_1, SIN_1},
                    {-SIN_1, COS_1},
                    {COS_2, SIN_2},
                    {-SIN_2, COS_2}
            };
            // Normalize all axes (to avoid length affecting projection)
            for (auto & axis : axes) {
                Vector2EX::normalize(axis[0], axis[1]);
            }

            // Project and check the separating axis
            bool on_border = false;
            for (int i = 0; i < 4; ++i) {
                const float ax = axes[i][0];
                const float ay = axes[i][1];

                const float p1x = CX_1 + (-HW_1 * COS_1 - HH_1 * (-SIN_1));
                const float p1y = CY_1 + (-HW_1 * SIN_1 - HH_1 * COS_1);
                const float p2x = CX_1 + (HW_1 * COS_1 - HH_1 * (-SIN_1));
                const float p2y = CY_1 + (HW_1 * SIN_1 - HH_1 * COS_1);
                const float p3x = CX_1 + (HW_1 * COS_1 + HH_1 * (-SIN_1));
                const float p3y = CY_1 + (HW_1 * SIN_1 + HH_1 * COS_1);
                const float p4x = CX_1 + (-HW_1 * COS_1 + HH_1 * (-SIN_1));
                const float p4y = CY_1 + (-HW_1 * SIN_1 + HH_1 * COS_1);

                const float proj1_1 = Vector2EX::dot(p1x, p1y, ax, ay);
                const float proj1_2 = Vector2EX::dot(p2x, p2y, ax, ay);
                const float proj1_3 = Vector2EX::dot(p3x, p3y, ax, ay);
                const float proj1_4 = Vector2EX::dot(p4x, p4y, ax, ay);
                const float min1 = std::min({proj1_1, proj1_2, proj1_3, proj1_4});
                const float max1 = std::max({proj1_1, proj1_2, proj1_3, proj1_4});

                const float q1x = CX_2 + (-HW_2 * COS_2 - HH_2 * (-SIN_2));
                const float q1y = CY_2 + (-HW_2 * SIN_2 - HH_2 * COS_2);
                const float q2x = CX_2 + (HW_2 * COS_2 - HH_2 * (-SIN_2));
                const float q2y = CY_2 + (HW_2 * SIN_2 - HH_2 * COS_2);
                const float q3x = CX_2 + (HW_2 * COS_2 + HH_2 * (-SIN_2));
                const float q3y = CY_2 + (HW_2 * SIN_2 + HH_2 * COS_2);
                const float q4x = CX_2 + (-HW_2 * COS_2 + HH_2 * (-SIN_2));
                const float q4y = CY_2 + (-HW_2 * SIN_2 + HH_2 * COS_2);

                const float proj2_1 = Vector2EX::dot(q1x, q1y, ax, ay);
                const float proj2_2 = Vector2EX::dot(q2x, q2y, ax, ay);
                const float proj2_3 = Vector2EX::dot(q3x, q3y, ax, ay);
                const float proj2_4 = Vector2EX::dot(q4x, q4y, ax, ay);
                const float min2 = std::min({proj2_1, proj2_2, proj2_3, proj2_4});
                const float max2 = std::max({proj2_1, proj2_2, proj2_3, proj2_4});

                // Check if the projection is separated
                // if it is separated, return -1.
                if (max1 < min2 || max2 < min1) {
                    return -1;
                }
                // Check for boundary contact
                if (fabsf(max1 - min2) < 0.f || fabsf(max2 - min1) < 0.f) {
                    on_border = true;
                }
            }
            return on_border ? 0 : 1;
        }

        inline uint8_t compareRotatedEllipses(const Graphics::Ellipse& ellipse1, const Graphics::Ellipse& ellipse2,
                uint8_t sample_points = 64) {
            /// get sample points
            std::function getSamplePoints(
                [] (const Graphics::Ellipse& ellipse, uint8_t pts) {
                std::vector<Vector2> points;
                const auto COS_R = cosf(ellipse.rotation());
                const auto SIN_R = sinf(ellipse.rotation());
                for (uint8_t i = 0; i < pts; ++i) {
                    auto temp = 2 * M_PI * i / pts;
                    const auto TEMP_X = ellipse.size().width * cosf(temp);
                    const auto TEMP_Y = ellipse.size().height * sinf(temp);
                    const auto NEW_X = COS_R * TEMP_X - SIN_R * TEMP_Y + ellipse.centerPosition().x;
                    const auto NEW_Y = SIN_R * TEMP_X + COS_R * TEMP_Y + ellipse.centerPosition().y;
                    points.emplace_back(NEW_X, NEW_Y);
                }
                return points;
            });
            auto pts1 = getSamplePoints(ellipse1, sample_points);
            auto pts2 = getSamplePoints(ellipse2, sample_points);


            return 0;
        }

        inline int8_t compareCircleRect(const Graphics::Point& point,
                                        const Graphics::Rectangle& rect) {
            if (point.size() <= 1) return comparePosInRect(point.position(), rect);
            const Vector2& p = point.position();
            const GeometryF& g = rect.geometry();

            float r = static_cast<float>(point.size()) * 0.5f;

            float nx = std::clamp(p.x, g.pos.x, g.pos.x + g.size.width);
            float ny = std::clamp(p.y, g.pos.y, g.pos.y + g.size.height);

            float dx = p.x - nx;
            float dy = p.y - ny;
            float dd = dx * dx + dy * dy;
            float rr = r * r;

            if (dd > rr + 1e-5f) return -1;
            if (std::abs(dd - rr) < 1e-5f) return 0;
            return 1;
        }
    }
}

#endif //MYENGINE_ALGORITHM_COLLIDER_H