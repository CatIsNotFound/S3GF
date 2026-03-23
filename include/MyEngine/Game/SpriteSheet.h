
#ifndef MYENGINE_GAME_SPRITESHEET_H
#define MYENGINE_GAME_SPRITESHEET_H
#include "Sprite.h"

namespace MyEngine {
    class SpriteSheet {
        struct FrameAnimation {
            StringList sequence_list;
            uint64_t duration_per_frame;
        };
    public:
        explicit SpriteSheet(TextureAtlas* textureAtlas);
        explicit SpriteSheet(const std::string& path, Renderer* renderer);
        ~SpriteSheet();

        void move(float x, float y);
        void move(const Vector2& pos);
        [[nodiscard]] const Vector2& position() const;

        void resize(float w, float h);
        void resize(const Size& size);
        [[nodiscard]] const Size& size() const;

        void setScale(float scale);
        [[nodiscard]] float scale() const;

        void setColorAlpha(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
        void setColorAlpha(uint64_t hex_code);
        void setColorAlpha(const SDL_Color& color);
        [[nodiscard]] const SDL_Color& colorAlpha() const;

        void setOpacity(float opacity);
        [[nodiscard]] float opacity() const;

        void setVisible(bool visible);
        [[nodiscard]] bool visible() const;

        void appendTiles(const std::string& tiles_name, const GeometryF& clip_geometry);
        void removeTiles(const std::string& tiles_name);
        [[nodiscard]] const TextureProperty* propertyOfTiles(const std::string& tiles_name) const;

        void setTextureAtlas(TextureAtlas* textureAtlas);
        [[nodiscard]] TextureAtlas* textureAtlas() const;

        bool appendAnimation(const std::string& name, const StringList& sequence_list, uint64_t duration_per_frame);
        bool removeAnimation(const std::string &name);
        [[nodiscard]] StringList sequenceListFromAnimation(const std::string& name);
        [[nodiscard]] size_t sizeOf() const;
        [[nodiscard]] StringList animationList() const;

        bool setDurationPerFrame(const std::string& name, uint64_t ms);
        [[nodiscard]] uint64_t durationPerFrameFromAnimation(const std::string& name);

        bool setCurrentAnimation(const std::string& name);
        [[nodiscard]] const std::string& currentAnimation() const;
        void draw();
        void setAnimateEnabled(bool animate);
        [[nodiscard]] bool animateEnabled() const;
        void setAnimationFinishedEvent(const std::function<void()>& event);

    private:
        TextureAtlas* _atlas{nullptr};
        std::unordered_map<std::string, FrameAnimation> _animation_map;
        std::string _cur_ani_name;
        uint64_t _start_time{0}, _event_id{0};
        uint64_t _cur_frame{0};
        std::shared_ptr<TextureProperty> _global_prop;
        bool _delete_later{false};
        bool _animate{false}, _visible{true};
        std::function<void()> _ani_finished_event{};
    };
}

#endif //MYENGINE_GAME_SPRITESHEET_H
