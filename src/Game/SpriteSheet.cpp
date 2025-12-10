
#include "SpriteSheet.h"
#include "Utils/RGBAColor.h"

MyEngine::SpriteSheet::SpriteSheet(MyEngine::TextureAtlas *textureAtlas) : _atlas(textureAtlas) {
    _global_prop = std::make_shared<TextureProperty>();
    if (!_atlas) {
        Logger::log("SpriteSheet: The current texture atlas is not valid!", Logger::Error);
        return;
    }
    _global_prop->resize(_atlas->property()->size());
    _event_id = IDGenerator::getNewGlobalEventID();
    EventSystem::global()->appendGlobalEvent(_event_id, [this] {
        if (!_animate) return;
        /// If current animation name is null or not in the animation map, skipped!
        if (_cur_ani_name.empty() || !_animation_map.contains(_cur_ani_name)) return;
        if (_start_time == 0) _start_time = SDL_GetTicks();
        auto cur_time = SDL_GetTicks();
        auto& ani = _animation_map.at(_cur_ani_name);
        if (cur_time - _start_time >= ani.duration_per_frame) {
            _cur_frame += 1;
            _start_time = SDL_GetTicks();
            if (_cur_frame >= ani.sequence_list.size()) {
                _cur_frame = 0;
            }
        }
    });
}

MyEngine::SpriteSheet::SpriteSheet(const std::string &path, MyEngine::Renderer *renderer) {
    _atlas = new TextureAtlas(path, renderer);
    _delete_later = true;
    if (!_atlas->isValid()) {
        Logger::log("SpriteSheet: The current texture atlas is not valid!", Logger::Error);
    }
    _global_prop = std::make_shared<TextureProperty>();
    _global_prop->resize(_atlas->property()->size());
    _event_id = IDGenerator::getNewGlobalEventID();
    EventSystem::global()->appendGlobalEvent(_event_id, [this] {
        if (!_animate) return;
        /// If current animation name is null or not in the animation map, skipped!
        if (_cur_ani_name.empty() || !_animation_map.contains(_cur_ani_name)) return;
        if (_start_time == 0) _start_time = SDL_GetTicks();
        auto cur_time = SDL_GetTicks();
        auto& ani = _animation_map.at(_cur_ani_name);
        if (cur_time - _start_time >= ani.duration_per_frame) {
            _cur_frame += 1;
            _start_time = SDL_GetTicks();
            if (_cur_frame >= ani.sequence_list.size()) {
                _cur_frame = 0;
            }
        }
    });
}

MyEngine::SpriteSheet::~SpriteSheet() {
    if (_delete_later) {
        delete _atlas;
    }
    EventSystem::global()->removeGlobalEvent(_event_id);
}

void MyEngine::SpriteSheet::move(float x, float y) {
    _global_prop->move(x, y);
    for (auto& [name, prop] : *_atlas) {
        prop->move(x, y);
    }
}

void MyEngine::SpriteSheet::move(const MyEngine::Vector2 &pos) {
    _global_prop->move(pos);
    for (auto& [name, prop] : *_atlas) {
        prop->move(pos);
    }
}

const MyEngine::Vector2 &MyEngine::SpriteSheet::position() const {
    return _global_prop->position();
}

void MyEngine::SpriteSheet::resize(float w, float h) {
    _global_prop->resize(w, h);
    for (auto& [name, prop] : *_atlas) {
        prop->resize(w, h);
    }
}

void MyEngine::SpriteSheet::resize(const MyEngine::Size &size) {
    _global_prop->resize(size);
    for (auto& [name, prop] : *_atlas) {
        prop->resize(size);
    }
}

const MyEngine::Size &MyEngine::SpriteSheet::size() const {
    return _global_prop->size();
}

void MyEngine::SpriteSheet::setScale(float scale) {
    _global_prop->setScale(scale);
    for (auto& [name, prop] : *_atlas) {
        prop->setScale(scale);
    }
}

float MyEngine::SpriteSheet::scale() const {
    return _global_prop->scale();
}

void MyEngine::SpriteSheet::setColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    _global_prop->color_alpha = { .r = r, .g = g, .b = b, .a = a };
    for (auto& [name, prop] : *_atlas) {
        prop->color_alpha = { .r = r, .g = g, .b = b, .a = a };
    }
}

void MyEngine::SpriteSheet::setColorAlpha(uint64_t hex_code) {
    auto [r, g, b, a] = RGBAColor::RGBAValue2Color(hex_code, true);
    _global_prop->color_alpha = { .r = r, .g = g, .b = b, .a = a };
    for (auto& [name, prop] : *_atlas) {
        prop->color_alpha = { .r = r, .g = g, .b = b, .a = a };
    }
}

void MyEngine::SpriteSheet::setColorAlpha(const SDL_Color &color) {
    _global_prop->color_alpha = color;
    for (auto& [name, prop] : *_atlas) {
        prop->color_alpha = color;
    }
}

const SDL_Color &MyEngine::SpriteSheet::colorAlpha() const {
    return _global_prop->color_alpha;
}

void MyEngine::SpriteSheet::setOpacity(float opacity) {
    _global_prop->color_alpha.a = static_cast<uint8_t>(255.f * opacity);
}

float MyEngine::SpriteSheet::opacity() const {
    return static_cast<float>(_global_prop->color_alpha.a) / 255.f;
}

void MyEngine::SpriteSheet::setVisible(bool visible) {
    _visible = visible;
}

bool MyEngine::SpriteSheet::visible() const {
    return _visible;
}

void MyEngine::SpriteSheet::appendTiles(const std::string &tiles_name, const MyEngine::GeometryF &clip_geometry) {
    _atlas->setTiles(tiles_name, clip_geometry);
    _atlas->tilesProperty(tiles_name)->move(_global_prop->position());
    _atlas->tilesProperty(tiles_name)->resize(_global_prop->size());
    _atlas->tilesProperty(tiles_name)->setScale(_global_prop->scale());
    _atlas->tilesProperty(tiles_name)->color_alpha = _global_prop->color_alpha;
}

void MyEngine::SpriteSheet::removeTiles(const std::string &tiles_name) {
    _atlas->eraseTiles(tiles_name);
}

const MyEngine::TextureProperty *MyEngine::SpriteSheet::propertyOfTiles(const std::string &tiles_name) const {
    return _atlas->tilesProperty(tiles_name);
}

void MyEngine::SpriteSheet::setTextureAtlas(MyEngine::TextureAtlas *textureAtlas) {
    if (_delete_later) {
        delete _atlas;
    }
    _atlas = textureAtlas;
    if (!_atlas) {
        Logger::log("SpriteSheet: You have set 'nullptr' to current texture atlas! "
                    "It will be thrown error while drawing.",Logger::Warn);
    }
}

MyEngine::TextureAtlas *MyEngine::SpriteSheet::textureAtlas() const {
    return _atlas;
}

bool MyEngine::SpriteSheet::appendAnimation(const std::string &name, const StringList &sequence_list,
                                        uint64_t duration_per_frame) {
    if (_animation_map.contains(name)) {
        Logger::log(std::format("SpriteSheet: The frame animation named {} already exists!", name),
                    Logger::Error);
        return false;
    }
    for (auto& sequence : sequence_list) {
        if (!_atlas->isTilesNameExist(sequence)) {
            Logger::log(std::format("SpriteSheet: The tiles named {} is not in current texture atlas!",
                                    sequence), Logger::Error);
            return false;
        }
    }
    _animation_map.emplace(name, FrameAnimation(sequence_list, duration_per_frame));
    return true;
}

bool MyEngine::SpriteSheet::removeAnimation(const std::string &name) {
    if (!_animation_map.contains(name)) {
        Logger::log(std::format("SpriteSheet: The animation named {} is not exist!", name), Logger::Error);
        return false;
    }
    if (_cur_ani_name == name) {
        Logger::log(std::format("SpriteSheet: Can not remove current animation! "
                    "Please use `SpriteSheet::setCurrentAnimation()` to instead at first!", name), Logger::Error);
        return false;
    }
    _animation_map.erase(name);
    return true;
}

StringList MyEngine::SpriteSheet::sequenceListFromAnimation(const std::string &name) {
    StringList out;
    if (!_animation_map.contains(name)) {
        Logger::log(std::format("SpriteSheet: The animation named {} is not exist!", name), Logger::Error);
        return out;
    }
    for (auto& a : _animation_map.at(name).sequence_list) {
        out.emplace_back(a);
    }
    return out;
}

size_t MyEngine::SpriteSheet::sizeOf() const {
    return _animation_map.size();
}

StringList MyEngine::SpriteSheet::animationList() const {
    StringList out;
    for (auto& a : _animation_map) {
        out.emplace_back(a.first);
    }
    return out;
}

bool MyEngine::SpriteSheet::setDurationPerFrame(const std::string &name, uint64_t ms) {
    if (!_animation_map.contains(name)) {
        Logger::log(std::format("SpriteSheet: The animation named {} is not exist!", name), Logger::Error);
        return false;
    }
    _animation_map.at(name).duration_per_frame = ms;
    return true;
}

uint64_t MyEngine::SpriteSheet::durationPerFrameFromAnimation(const std::string &name) {
    if (!_animation_map.contains(name)) {
        Logger::log(std::format("SpriteSheet: The animation named {} is not exist!", name), Logger::Error);
        return false;
    }
    return _animation_map.at(name).duration_per_frame;
}

bool MyEngine::SpriteSheet::setCurrentAnimation(const std::string &name) {
    if (!_animation_map.contains(name)) {
        Logger::log(std::format("SpriteSheet: The animation named {} is not exist!", name), Logger::Error);
        return false;
    }
    _cur_ani_name = name;
    return true;
}

const std::string &MyEngine::SpriteSheet::currentAnimation() const {
    return _cur_ani_name;
}

void MyEngine::SpriteSheet::draw() {
    if (!_visible) return;
    if (!_animation_map.contains(_cur_ani_name)) {
        Logger::log(std::format("SpriteSheet: Renderer failed! "
                                "The animation named '{}' is not exist! "
                                "Did you forget to use `SpriteSheet::setCurrentAnimation()`?",
                                _cur_ani_name), Logger::Fatal);
        Engine::throwFatalError();
    }
    auto& frame_name = _animation_map.at(_cur_ani_name).sequence_list[_cur_frame];
    if (!_atlas->isTilesNameExist(frame_name)) {
        Logger::log(std::format("SpriteSheet: Renderer failed! "
                                "The animation named '{}' of frame '{}' is not valid! ",
                                _cur_ani_name, frame_name), Logger::Fatal);
        Engine::throwFatalError();
    }
    _atlas->draw(frame_name);
}

void MyEngine::SpriteSheet::setAnimateEnabled(bool animate) {
    _animate = animate;
}

bool MyEngine::SpriteSheet::animateEnabled() const {
    return _animate;
}

