
#include "Sprite.h"
#include "Utils/RGBAColor.h"

MyEngine::Sprite::Sprite(MyEngine::Texture *texture) : _texture(texture) {
    _property = std::make_unique<TextureProperty>(texture->property());
}

MyEngine::Sprite::Sprite(const std::string &path, MyEngine::Renderer *renderer) {
    _texture = new Texture(path, renderer);
    _delete_later = true;
    if (!_texture->isValid()) {
        Logger::log("Sprite: Current texture is not valid!", Logger::ERROR);
    }
}

MyEngine::Sprite::~Sprite() {
    if (_delete_later) {
        delete _texture;
    }
}

void MyEngine::Sprite::setTexture(MyEngine::Texture *new_texture) {
    if (_delete_later) {
        delete _texture;
        _delete_later = false;
    }
    _texture = new_texture;
    if (!new_texture) {
        Logger::log("Sprite: You have set 'nullptr' to current texture! It will be thrown error while drawing.",
                    Logger::WARN);
        return;
    }
    _property->reset(*new_texture->property());
}

MyEngine::Texture *MyEngine::Sprite::texture() const {
    return _texture;
}

void MyEngine::Sprite::setPosition(float x, float y) {
    _property->move(x, y);
}

void MyEngine::Sprite::setPosition(const MyEngine::Vector2 &position) {
    _property->move(position);
}

const MyEngine::Vector2 &MyEngine::Sprite::position() const {
    return _property->position();
}

void MyEngine::Sprite::setSize(float w, float h) {
    _property->resize(w, h);
}

void MyEngine::Sprite::setSize(const MyEngine::Size &size) {
    _property->resize(size);
}

const MyEngine::Size &MyEngine::Sprite::size() const {
    return _property->size();
}

void MyEngine::Sprite::setScale(float scale) {
    _property->setScale(scale);
}

float MyEngine::Sprite::scale() const {
    return _property->scale();
}

void MyEngine::Sprite::setRotation(double rotation) {
    _property->rotate_angle = rotation;
}

double MyEngine::Sprite::rotation() const {
    return _property->rotate_angle;
}

void MyEngine::Sprite::setOpacity(float opcacity) {
    _property->color_alpha.a = static_cast<uint8_t>(255.f * opcacity);
}

float MyEngine::Sprite::opacity() const {
    return static_cast<float>(_property->color_alpha.a) / 255.f;
}

void MyEngine::Sprite::setColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _property->color_alpha = { .r = r, .g = g, .b = b, .a = a };
}

void MyEngine::Sprite::setColorAlpha(uint64_t hex_code) {
    auto [r, g, b, a] = RGBAColor::RGBAValue2Color(hex_code, true);
    _property->color_alpha = { .r = r, .g = g, .b = b, .a = a };
}

void MyEngine::Sprite::setColorAlpha(const SDL_Color &color) {
    _property->color_alpha = color;
}

const SDL_Color& MyEngine::Sprite::colorAlpha() const {
    return _property->color_alpha;
}

void MyEngine::Sprite::setVisible(bool visible) {
    _visible = visible;
}

bool MyEngine::Sprite::visible() const {
    return _visible;
}

void MyEngine::Sprite::setFlipMode(SDL_FlipMode flip_mode) {
    _property->flip_mode = flip_mode;
}

SDL_FlipMode MyEngine::Sprite::flipMode() const {
    return _property->flip_mode;
}

void MyEngine::Sprite::draw() const {
    if (!_visible) return;
    if (!_texture) {
        Logger::log("Sprite: Current sprite is not valid! Maybe the texture is 'nullptr'?", Logger::FATAL);
        Engine::throwFatalError();
        return;
    }
    _texture->render()->drawTexture(_texture->self(), _property.get());
}

