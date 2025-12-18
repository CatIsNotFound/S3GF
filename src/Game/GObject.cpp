
#include "GObject.h"

MyEngine::GObject::GObject(const std::string &name, MyEngine::Object object, bool delete_later)
    : _name(name), _object(object), _del_later(delete_later) {
}

MyEngine::GObject::GObject(std::string &&name, MyEngine::Object object, bool delete_later)
    : _name(std::move(name)), _object(object), _del_later(delete_later) {
}

MyEngine::GObject::~GObject() {
    if (_del_later && !isNull()) {
        if (isTypeOf<Sprite*>()) delete std::get<Sprite*>(_object);
        if (isTypeOf<SpriteSheet*>()) delete std::get<SpriteSheet*>(_object);
    }
    if (_del_later && _collider) {
        delete _collider;
    }
}

void MyEngine::GObject::setObjectName(const std::string &name) {
    _name = name;
}

const std::string &MyEngine::GObject::objectName() const {
    return _name;
}

void MyEngine::GObject::setObject(MyEngine::Object object, bool delete_later) {
    if (_del_later && !isNull()) {
        if (isTypeOf<Sprite*>()) delete std::get<Sprite*>(_object);
        if (isTypeOf<SpriteSheet*>()) delete std::get<SpriteSheet*>(_object);
    }
    _object = object;
    _del_later = delete_later;
}

bool MyEngine::GObject::isNull() const {
    if (_object.index() == 0) return true;
    if (_object.index() == 1) return !std::get<Sprite*>(_object);
    if (_object.index() == 2) return !std::get<SpriteSheet*>(_object);
    return true;
}

MyEngine::Sprite *MyEngine::GObject::sprite() {
    return isTypeOf<Sprite*>() ? std::get<Sprite*>(_object) : nullptr;
}

MyEngine::SpriteSheet *MyEngine::GObject::spriteSheet() {
    return isTypeOf<SpriteSheet*>() ? std::get<SpriteSheet*>(_object) : nullptr;
}

void MyEngine::GObject::move(float x, float y) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->move(x, y);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->move(x, y);
    }
}

void MyEngine::GObject::move(const MyEngine::Vector2 &pos) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        auto sp = sprite();
        sp->move(pos);
        if (_collider) _collider->move(sp->position());
    } else if (isTypeOf<SpriteSheet*>()) {
        auto sp = spriteSheet();
        sp->move(pos);
        if (_collider) _collider->move(sp->position());
    }
}

void MyEngine::GObject::resize(float w, float h) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->resize(w, h);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->resize(w, h);
    }
}

void MyEngine::GObject::resize(const MyEngine::Size &size) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->resize(size);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->resize(size);
    }
}

void MyEngine::GObject::setGeometry(float x, float y, float w, float h) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        auto s = sprite();
        s->move(x, y);
        s->resize(w, h);
    } else if (isTypeOf<SpriteSheet*>()) {
        auto s = spriteSheet();
        s->move(x, y);
        s->resize(w, h);
    }
}

void MyEngine::GObject::setGeometry(const MyEngine::GeometryF &geometry) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        auto s = sprite();
        s->move(geometry.pos);
        s->resize(geometry.size);
        collider()->move(s->position());
    } else if (isTypeOf<SpriteSheet*>()) {
        auto s = spriteSheet();
        s->move(geometry.pos);
        s->resize(geometry.size);
        collider()->move(s->position());
    }
}

void MyEngine::GObject::setVisible(bool visible) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->setVisible(visible);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->setVisible(visible);
    }
}

void MyEngine::GObject::setScale(float scale) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->setScale(scale);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->setScale(scale);
    }
}

void MyEngine::GObject::setOpacity(float opacity) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->setOpacity(opacity);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->setOpacity(opacity);
    }
}

void MyEngine::GObject::setColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->setColorAlpha(r, g, b, a);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->setColorAlpha(r, g, b, a);
    }
}

void MyEngine::GObject::setColorAlpha(uint64_t color_hex) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->setColorAlpha(color_hex);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->setColorAlpha(color_hex);
    }
}

void MyEngine::GObject::setColorAlpha(const SDL_Color &color) {
    if (isNull()) return;
    if (isTypeOf<Sprite*>()) {
        sprite()->setColorAlpha(color);
    } else if (isTypeOf<SpriteSheet*>()) {
        spriteSheet()->setColorAlpha(color);
    }
}

MyEngine::Vector2 MyEngine::GObject::position() const {
    if (_object.index() == 1) {
        return std::get<Sprite*>(_object)->position();
    } else if (_object.index() == 2) {
        return std::get<SpriteSheet*>(_object)->position();
    }
    return Vector2();
}

MyEngine::Size MyEngine::GObject::size() const {
    if (_object.index() == 1) {
        return std::get<Sprite*>(_object)->size();
    } else if (_object.index() == 2) {
        return std::get<SpriteSheet*>(_object)->size();
    }
    return Size();
}

MyEngine::GeometryF MyEngine::GObject::geometryF() const {
    if (_object.index() == 1) {
        auto s = std::get<Sprite*>(_object);
        return GeometryF(s->position(), s->size());
    } else if (_object.index() == 2) {
        auto s = std::get<SpriteSheet*>(_object);
        return GeometryF(s->position(), s->size());
    }
    return GeometryF();
}

bool MyEngine::GObject::visible() const {
    if (_object.index() == 1) {
        return std::get<Sprite*>(_object)->visible();
    } else if (_object.index() == 2) {
        return std::get<SpriteSheet*>(_object)->visible();
    }
    return false;
}

float MyEngine::GObject::scale() const {
    if (_object.index() == 1) {
        return std::get<Sprite*>(_object)->scale();
    } else if (_object.index() == 2) {
        return std::get<SpriteSheet*>(_object)->scale();
    }
    return 0;
}

float MyEngine::GObject::opacity() const {
    if (_object.index() == 1) {
        return std::get<Sprite*>(_object)->opacity();
    } else if (_object.index() == 2) {
        return std::get<SpriteSheet*>(_object)->opacity();
    }
    return 0;
}

SDL_Color MyEngine::GObject::colorAlpha() const {
    if (_object.index() == 1) {
        return std::get<Sprite*>(_object)->colorAlpha();
    } else if (_object.index() == 2) {
        return std::get<SpriteSheet*>(_object)->colorAlpha();
    }
    return SDL_Color();
}

void MyEngine::GObject::draw() {
    if (_object.index() == 1) {
        std::get<Sprite*>(_object)->draw();
    } else if (_object.index() == 2) {
        std::get<SpriteSheet*>(_object)->draw();
    }
}

void MyEngine::GObject::setCollider(MyEngine::Collider *collider) {
    if (_del_later && !_collider) {
        delete _collider;
    }
    _collider = collider;
}

void MyEngine::GObject::setCollider(MyEngine::Collider::Self base) {
    if (_del_later && !_collider) {
        delete _collider;
    }
    _collider = new Collider(base, _del_later);
}

MyEngine::Collider *MyEngine::GObject::collider() {
    return _collider;
}

void MyEngine::GObject::setColliderEnabled(bool enabled) {
    if (_collider) {
        _collider->setEnabled(enabled);
    }
}

bool MyEngine::GObject::colliderEnabled() const {
    return _collider != nullptr && _collider->enabled();
}

void MyEngine::GObject::addTriggerToCollider(MyEngine::Collider *collider) {
    if (!_collider) return;
    _collider->appendCollider(collider);
}

void MyEngine::GObject::removeTriggerFromCollider(size_t index) {
    if (!_collider) return;
    _collider->removeCollider(index);
}

void MyEngine::GObject::clearTriggerFromCollider() {
    if (!_collider) return;
    _collider->clearCollider();
}

bool MyEngine::GObject::colliderTriggered(size_t index) {
    if (!_collider) return false;
    return _collider->isTriggered(index);
}

template<typename T>
bool MyEngine::GObject::isTypeOf() {
    return std::holds_alternative<T>(_object);
}
