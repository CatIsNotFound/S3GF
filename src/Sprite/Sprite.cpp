
#include "Sprite.h"

S3GF::Sprite::Sprite(S3GF::Texture *texture) : _texture(texture) {
    _property = std::make_unique<TextureProperty>(texture->property());
}

S3GF::Sprite::~Sprite() {

}

void S3GF::Sprite::setTexture(S3GF::Texture *new_texture) {
    _texture = new_texture;
    if (!new_texture) {
        Logger::log("Sprite: You have set 'nullptr' to current texture! It will be thrown error while drawing.",
                    Logger::WARN);
        return;
    }
    _property->reset(*new_texture->property());
}

S3GF::Texture *S3GF::Sprite::texture() const {
    return _texture;
}

void S3GF::Sprite::setPosition(float x, float y) {
    _property->move(x, y);
}

void S3GF::Sprite::setPosition(const S3GF::Vector2 &position) {
    _property->move(position);
}

const S3GF::Vector2 &S3GF::Sprite::position() const {
    return _property->position();
}

void S3GF::Sprite::setSize(float w, float h) {
    _property->resize(w, h);
}

void S3GF::Sprite::setSize(const S3GF::Size &size) {
    _property->resize(size);
}

const S3GF::Size &S3GF::Sprite::size() const {
    return _property->size();
}

void S3GF::Sprite::setScale(float scale) {
    _property->setScale(scale);
}

float S3GF::Sprite::scale() const {
    return _property->scale();
}

void S3GF::Sprite::setRotation(double rotation) {
    _property->rotate_angle = rotation;
}

double S3GF::Sprite::rotation() const {
    return _property->rotate_angle;
}

void S3GF::Sprite::setOpacity(float opcacity) {
    _property->color_alpha.a = static_cast<uint8_t>(255.f * opcacity);
}

float S3GF::Sprite::opacity() const {
    return static_cast<float>(_property->color_alpha.a) / 255.f;
}

void S3GF::Sprite::setVisible(bool visible) {
    _visible = visible;
}

bool S3GF::Sprite::visible() const {
    return _visible;
}

void S3GF::Sprite::setFlipMode(SDL_FlipMode flip_mode) {
    _property->flip_mode = flip_mode;
}

SDL_FlipMode S3GF::Sprite::flipMode() const {
    return _property->flip_mode;
}

void S3GF::Sprite::draw() const {
    if (!_visible) return;
    if (!_texture) {
        Logger::log("Sprite: Current sprite is not valid! Maybe the texture is 'nullptr'?", Logger::FATAL);
        Engine::throwFatalError();
        return;
    }
    _texture->render()->drawTexture(_texture->self(), _property.get());
}

