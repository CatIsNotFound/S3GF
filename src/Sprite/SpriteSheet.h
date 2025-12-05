
#ifndef S3GF_SPRITESHEET_H
#define S3GF_SPRITESHEET_H
#include "Sprite.h"

namespace S3GF {
    class SpriteSheet {
        struct Animation {
            std::string name;
            StringList sequence_list;
            uint64_t duration_per_frame;
        };
    public:
        explicit SpriteSheet(TextureAtlas* textureAtlas);
        explicit SpriteSheet(const std::string& path, Renderer* renderer);
        ~SpriteSheet();

        void setTextureAtlas(TextureAtlas* textureAtlas);
        [[nodiscard]] TextureAtlas* textureAtlas() const;

        bool appendAnimation(const std::string& name, const StringList& sequence_list, uint64_t duration_per_frame);
        bool insertAnimation(size_t index, const std::string& name, const StringList& sequence_list,
                             uint64_t duration_per_frame);
        bool popAnimation();
        bool removeAnimation(size_t index, const std::string& name);
        [[nodiscard]] StringList sequenceListFromAnimation(const std::string& animation_name);
        [[nodiscard]] size_t sizeOf() const;
        [[nodiscard]] StringList animationList() const;

        void setDurationPerFrame(const std::string& animation_name, uint64_t ms);
        [[nodiscard]] uint64_t durationPerFrameFromAnimation(const std::string& animation_name);
        size_t indexOfAnimation(const std::string& animation_name);

        [[nodiscard]] const std::string& currentAnimation() const;
        void draw(const std::string& animation_name);
    private:
        TextureAtlas* _atlas{nullptr};
        std::vector<Animation> _animation_list;
        std::string _cur_ani_name;
    };
}

#endif //S3GF_SPRITESHEET_H
