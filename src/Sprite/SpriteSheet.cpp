
#include "SpriteSheet.h"

S3GF::SpriteSheet::SpriteSheet(S3GF::TextureAtlas *textureAtlas) : _atlas(textureAtlas) {
}

S3GF::SpriteSheet::SpriteSheet(const std::string &path, S3GF::Renderer *renderer) {

}

S3GF::SpriteSheet::~SpriteSheet() {

}

void S3GF::SpriteSheet::setTextureAtlas(S3GF::TextureAtlas *textureAtlas) {

}

S3GF::TextureAtlas *S3GF::SpriteSheet::textureAtlas() const {
    return nullptr;
}

bool S3GF::SpriteSheet::appendAnimation(const std::string &name, const StringList &sequence_list,
                                        uint64_t duration_per_frame) {
    return false;
}

bool S3GF::SpriteSheet::insertAnimation(size_t index, const std::string &name, const StringList &sequence_list,
                                        uint64_t duration_per_frame) {
    return false;
}

bool S3GF::SpriteSheet::popAnimation() {
    return false;
}

bool S3GF::SpriteSheet::removeAnimation(size_t index, const std::string &name) {
    return false;
}

StringList S3GF::SpriteSheet::sequenceListFromAnimation(const std::string &animation_name) {
    return StringList();
}

size_t S3GF::SpriteSheet::sizeOf() const {
    return 0;
}

StringList S3GF::SpriteSheet::animationList() const {
    return StringList();
}

void S3GF::SpriteSheet::setDurationPerFrame(const std::string &animation_name, uint64_t ms) {

}

uint64_t S3GF::SpriteSheet::durationPerFrameFromAnimation(const std::string &animation_name) {
    return 0;
}

size_t S3GF::SpriteSheet::indexOfAnimation(const std::string &animation_name) {
    return 0;
}

const std::string &S3GF::SpriteSheet::currentAnimation() const {
    return <#initializer#>;
}

void S3GF::SpriteSheet::draw(const std::string &animation_name) {

}
