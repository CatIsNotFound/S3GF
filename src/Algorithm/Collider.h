
#ifndef MYENGINE_COLLIDER_H
#define MYENGINE_COLLIDER_H
#include "../Basic.h"

namespace MyEngine {
    namespace Algorithm {
        inline int8_t comparePosInRect(const Vector2& vec, const Graphics::Rectangle& rect) {
            float left = rect.geometry().pos.x;
            float right = left + rect.geometry().size.width;
            float top = rect.geometry().pos.y;
            float bottom = top + rect.geometry().size.height;
            if ((vec.x > left) && (vec.x <= right) && (vec.y > top) && (vec.y < bottom)) return 1;

            bool onX = (vec.x == left || vec.x == right);
            bool onY = (vec.y == top  || vec.y == bottom);
            if (onX && onY) return 0;

            return -1;
        }

        inline int8_t comparePosInPoint(const Vector2& pos, const Graphics::Point& point) {
            auto pt = point.position();
            if (point.size() < 2) {
                return (point.position().isEqual(pos) ? (int8_t)1 : (int8_t)-1);
            }
            float dx = pos.x - pt.x;
            float dy = pos.y - pt.y;
            float dd = dx * dx + dy * dy;          // 距离平方
            float rr = (static_cast<float>(point.size()) / 2.f) * (static_cast<float>(point.size()) / 2.f);

            if (dd > rr + 1e-5f) return -1;
            if (std::fabs(dd - rr) < 1e-5f) return 0;
            return 1;
        }

        inline int8_t comparePoints(const Graphics::Point& point1, const Graphics::Point& point2) {
            auto p1 = point1.position(), p2 = point2.position();
            if (point1.size() < 2 && point2.size() < 2) {
                return (point1.position().isEqual(point2.position()) ? (int8_t)1 : (int8_t)-1);
            }
            auto r1 = static_cast<float>(point1.size()) / 2.f, r2 = static_cast<float>(point2.size()) / 2.f;
            float dx = p1.x - p2.x;
            float dy = p1.y - p2.y;
            float d  = std::hypot(dx, dy);          // 圆心距
            float rSum = r1 + r2;

            if (d > rSum + 1e-5f) return -1;                // 完全相离
            if (std::fabs(d - rSum) < 1e-5f) return 0;   // 外切（只有一个交点）
            return 1;
        }

        inline int8_t compareRects(const Graphics::Rectangle& rect1, const Graphics::Rectangle& rect2) {
            float x1 = rect1.geometry().pos.x, x2 = rect1.geometry().pos.x + rect1.geometry().size.width,
                  y1 = rect1.geometry().pos.y, y2 = rect1.geometry().pos.y + rect1.geometry().size.height,
                  x3 = rect2.geometry().pos.x, x4 = rect2.geometry().pos.x + rect2.geometry().size.width,
                  y3 = rect2.geometry().pos.y, y4 = rect2.geometry().pos.y + rect2.geometry().size.height;

            if (x2 < x3 || x1 > x4 || y2 < y3 || y1 > y4) return -1;
            if (x3 <= x1 && y3 <= y1 && x4 >= x2 && y4 >= y2) return 1;
            if (x1 <= x3 && y1 <= y3 && x2 >= x4 && y2 >= y4) return 2;
            return 0;
        }
    }
}

#endif //MYENGINE_COLLIDER_H
