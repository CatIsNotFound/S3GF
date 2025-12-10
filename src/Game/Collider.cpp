#include "Collider.h"
#include "Algorithm/Collider.h"
MyEngine::Collider::Collider(MyEngine::Collider::Self self, bool delete_later)
    : _base(self), _del_later(delete_later) {
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [this](SDL_Event e) {
        if (!_enabled || isNull() || _colliders.empty()) return;
        Graphics::Point* pt = nullptr;
        Graphics::Rectangle* rect = nullptr;
        if (_base.index() == 1)      pt = std::get<Graphics::Point*>(_base);
        if (_base.index() == 2)    rect = std::get<Graphics::Rectangle*>(_base);
        int8_t tri = 0;
        for (auto& c : _colliders) {
            if ((c.collider->_base.index() == 1) && (pt)) {
                tri = Algorithm::comparePoints(*pt, *std::get<Graphics::Point*>(c.collider->_base));
            } else if ((c.collider->_base.index() == 1) && (rect)) {
                tri = Algorithm::compareCircleRect(*std::get<Graphics::Point*>(c.collider->_base), *rect);
            } else if ((c.collider->_base.index() == 2) && (pt)) {
                tri = Algorithm::compareCircleRect(*pt, *std::get<Graphics::Rectangle*>(c.collider->_base));
            } else if ((c.collider->_base.index() == 2) && (rect)) {
                tri = Algorithm::compareRects(*rect, *std::get<Graphics::Rectangle*>(c.collider->_base));
            }
            c.trigged = (tri > 0);
            tri = 0;
        }
    });
}

MyEngine::Collider::~Collider() {
    if (_del_later && !isNull()) {
        if (isTypeOf<Graphics::Point*>()) delete std::get<Graphics::Point*>(_base);
        if (isTypeOf<Graphics::Rectangle*>()) delete std::get<Graphics::Rectangle*>(_base);
    }
}

void MyEngine::Collider::setSelf(MyEngine::Collider::Self self, bool use_delete) {
    if (use_delete && !isNull()) {
        if (isTypeOf<Graphics::Point*>()) delete std::get<Graphics::Point*>(_base);
        if (isTypeOf<Graphics::Rectangle*>()) delete std::get<Graphics::Rectangle*>(_base);
    }
    _base = self;
}

bool MyEngine::Collider::isNull() const {
    if (_base.index() == 0) return true;
    if (isTypeOf<Graphics::Point*>()) return !std::get<Graphics::Point*>(_base);
    if (isTypeOf<Graphics::Rectangle*>()) return !std::get<Graphics::Rectangle*>(_base);
    return true;
}

MyEngine::Graphics::Point *MyEngine::Collider::point() {
    if (isTypeOf<Graphics::Point*>()) return std::get<Graphics::Point*>(_base);
    return nullptr;
}

MyEngine::Graphics::Rectangle *MyEngine::Collider::rectangle() {
    if (isTypeOf<Graphics::Rectangle*>()) return std::get<Graphics::Rectangle*>(_base);
    return nullptr;
}

void MyEngine::Collider::setEnabled(bool enabled) {
    _enabled = enabled;
}

bool MyEngine::Collider::enabled() const {
    return _enabled;
}

void MyEngine::Collider::draw(Renderer* r) {
    if (!r || isNull()) return;
    size_t idx = _base.index();
    if (idx == 1) {
        r->drawPoint(*std::get<Graphics::Point*>(_base));
    } else if (idx == 2) {
        r->drawRectangle(*std::get<Graphics::Rectangle*>(_base));
    }
}

size_t MyEngine::Collider::appendCollider(MyEngine::Collider *collider) {
    _colliders.emplace_back(collider);
    return _colliders.size() - 1;
}

void MyEngine::Collider::removeCollider(size_t index) {
    if (index >= _colliders.size()) return;
    _colliders.erase(_colliders.begin() + index);
}

void MyEngine::Collider::clearCollider() {
    _colliders.clear();
}

size_t MyEngine::Collider::collidersCount() const {
    return _colliders.size();
}

bool MyEngine::Collider::isTriggered(size_t index) const {
    if (index >= _colliders.size()) return false;
    return _colliders[index].trigged;
}

template<typename T>
bool MyEngine::Collider::isTypeOf() const {
    return std::holds_alternative<T>(_base);
}
