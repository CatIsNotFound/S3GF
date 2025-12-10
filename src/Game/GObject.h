#ifndef MYENGINE_GAME_GOBJECT_H
#define MYENGINE_GAME_GOBJECT_H
#include "Sprite.h"
#include "SpriteSheet.h"
#include "Collider.h"

namespace MyEngine {
    using Object = std::variant<std::monostate, Sprite*, SpriteSheet*>;
    class GObject {
    public:
        explicit GObject(const std::string& name, Object object, bool delete_later = false);
        explicit GObject(std::string&& name, Object object, bool delete_later = false);
        ~GObject();

        void setObjectName(const std::string& name);
        [[nodiscard]] const std::string& objectName() const;

        void setObject(Object object, bool delete_later = false);
        [[nodiscard]] bool isNull() const;
        [[nodiscard]] Sprite* sprite();
        [[nodiscard]] SpriteSheet* spriteSheet();

        void move(float x, float y);
        void move(const Vector2& pos);
        void resize(float w, float h);
        void resize(const Size& size);
        void setGeometry(float x, float y, float w, float h);
        void setGeometry(const GeometryF& geometry);
        void setVisible(bool visible);
        void setScale(float scale);
        void setOpacity(float opacity);
        void setColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void setColorAlpha(uint64_t color_hex);
        void setColorAlpha(const SDL_Color& color);

        [[nodiscard]] Vector2 position() const;
        [[nodiscard]] Size size() const;
        [[nodiscard]] GeometryF geometryF() const;
        [[nodiscard]] bool visible() const;
        [[nodiscard]] float scale() const;
        [[nodiscard]] float opacity() const;
        [[nodiscard]] SDL_Color colorAlpha() const;
        void draw();

        void setCollider(Collider* collider);
        void setCollider(Collider::Self base);
        Collider* collider();
        void setColliderEnabled(bool enabled);
        [[nodiscard]] bool colliderEnabled() const;
        void addTriggerToCollider(Collider* collider);
        void removeTriggerFromCollider(size_t index);
        void clearTriggerFromCollider();
        bool colliderTriggered(size_t index);

        GObject() = delete;
        GObject(const GObject&) = delete;
        GObject(GObject&&) = delete;
        GObject& operator=(const GObject&) = delete;
        GObject& operator=(GObject&&) = delete;
    private:
        template<typename T>
        bool isTypeOf();
        Object _object;
        Collider* _collider{nullptr};
        std::string _name;
        bool _del_later;
    };
}

#endif //MYENGINE_GAME_GOBJECT_H
