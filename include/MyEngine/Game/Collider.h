
#ifndef MYENGINE_GAME_COLLIDER_H
#define MYENGINE_GAME_COLLIDER_H
#include "../Components.h"
namespace MyEngine {
    class Collider {
    public:
        using Self = std::variant<std::monostate, Graphics::Point*, Graphics::Rectangle*>;
        explicit Collider(Self self, bool delete_later = false);
        ~Collider();

        void setSelf(Self self, bool use_delete = false);
        [[nodiscard]] bool isNull() const;
        Graphics::Point* point();
        Graphics::Rectangle* rectangle();

        void setEnabled(bool enabled);
        [[nodiscard]] bool enabled() const;
        void move(float x, float y);
        void move(const Vector2& position);
        [[nodiscard]] Vector2 position() const;
        void draw(Renderer* r);

        template<typename T>
        bool isTypeOf() const;

        size_t appendCollider(Collider* collider);
        void removeCollider(size_t index);
        void clearCollider();
        [[nodiscard]] size_t collidersCount() const;
        [[nodiscard]] bool isTriggered(size_t index) const;

        Collider() = delete;
        Collider(const Collider&) = delete;
        Collider(Collider&&) = delete;
        Collider& operator=(const Collider&) = delete;
        Collider& operator=(Collider&&) = delete;
    private:
        struct CTrigger {
            Collider* collider{nullptr};
            bool trigged{false};
        };
        Self _base;
        std::vector<CTrigger> _colliders;
        bool _enabled{false}, _del_later{false};
    };
}

#endif //MYENGINE_GAME_COLLIDER_H
