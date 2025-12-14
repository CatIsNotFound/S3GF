
#ifndef MYENGINE_BASECOMMAND_H
#define MYENGINE_BASECOMMAND_H
#include "../Components.h"

namespace MyEngine {
    namespace RenderCommand {
        class BaseCommand {
        public:
            enum class Mode {
                Single,
                Multiple
            };
            explicit BaseCommand(SDL_Renderer *renderer, std::string&& cmd_type)
                : _renderer(renderer), _type_name(std::move(cmd_type)) {}
            virtual ~BaseCommand() = default;

            virtual void exec() = 0;

            [[nodiscard]] const char* commandType() const { return _type_name.data(); }
            void resetCommand(std::string&& type) { _type_name = std::move(type); }
            void setRenderColor(const SDL_Color& color) { _render_color = color; }
            void setRenderColor(SDL_Color&& color) { _render_color = std::move(color); }
            void setBlendMode(SDL_BlendMode blend_mode) { _blend_mode = blend_mode; }
        protected:
            SDL_Renderer *_renderer;
            SDL_Color _render_color{};
            SDL_BlendMode _blend_mode{};
            std::string _type_name;
        };

        class FillCMD : public BaseCommand {
        public:
            explicit FillCMD(SDL_Renderer *renderer, const SDL_Color &color = {});
            explicit FillCMD(SDL_Renderer *renderer, SDL_Color &&color = {});

            ~FillCMD() override = default;

            void reset(SDL_Renderer *render, const SDL_Color &color);
            void reset(SDL_Renderer *render, SDL_Color &&color);

            void exec() override;

        private:
            SDL_Color _color;
        };

        class ViewPortCMD : public BaseCommand {
        public:
            explicit ViewPortCMD(SDL_Renderer *renderer, bool reset, const Geometry &geometry);
            explicit ViewPortCMD(SDL_Renderer *renderer, bool reset, Geometry &&geometry);

            ~ViewPortCMD() override = default;

            void reset(SDL_Renderer *renderer, bool reset, const Geometry &geometry);
            void reset(SDL_Renderer *renderer, bool reset, Geometry &&geometry);

            void exec() override;

        private:
            SDL_Rect _rect;
            bool _reset{false};
        };

        class ClipViewCMD : public BaseCommand {
        public:
            explicit ClipViewCMD(SDL_Renderer *renderer, bool reset, const Geometry &geometry);
            explicit ClipViewCMD(SDL_Renderer *renderer, bool reset, Geometry &&geometry);
            ~ClipViewCMD() override = default;

            void reset(SDL_Renderer *renderer, bool reset, const Geometry &geometry);
            void reset(SDL_Renderer *renderer, bool reset, Geometry &&geometry);

            void exec() override;

        private:
            SDL_Rect _rect;
            bool _reset;
        };

        class TextureCMD : public BaseCommand {
        public:
            explicit TextureCMD(SDL_Renderer *renderer, SDL_Texture *texture, TextureProperty *property,
                                Mode mode = Mode::Single, uint32_t count = 0,
                                std::vector<TextureProperty*> properties = {});
            ~TextureCMD() override = default;

            void reset(SDL_Renderer *renderer, SDL_Texture *texture, TextureProperty *textureProperty,
                            Mode mode = Mode::Single, uint32_t count = 0,
                            std::vector<TextureProperty*> properties = {});

            void exec() override;

            void render(SDL_Texture* texture, TextureProperty* prop);

        private:
            Mode _mode;
            uint32_t _count;
            SDL_Texture *_texture;
            TextureProperty *_property;
            std::vector<TextureProperty*> _properties;
        };

        class PointCMD : public BaseCommand {
        public:
            explicit PointCMD(SDL_Renderer *renderer, Graphics::Point* point, Mode mode = Mode::Single,
                              uint32_t count = 0, std::vector<Graphics::Point*> point_list = {});
            ~PointCMD() override = default;

            void reset(SDL_Renderer *renderer, Graphics::Point* point, Mode mode = Mode::Single,
                          uint32_t count = 0, std::vector<Graphics::Point*> point_list = {});

            void exec() override;

            void render(Graphics::Point* point);

        private:
            Graphics::Point* _point;
            Mode _mode;
            uint32_t _count;
            std::vector<Graphics::Point*> _points;
        };

        class RectangleCMD : public BaseCommand {
        public:
            explicit RectangleCMD(SDL_Renderer* renderer, Graphics::RectangleEX* rect, Mode mode = Mode::Single,
                                  uint32_t count = 0, std::vector<Graphics::RectangleEX*> rect_list = {});

            ~RectangleCMD() override = default;

            void reset(SDL_Renderer* renderer, Graphics::RectangleEX* rect, Mode mode = Mode::Single,
                       uint32_t count = 0, std::vector<Graphics::RectangleEX*> rect_list = {});
            void exec() override;
            void render(Graphics::RectangleEX* rect);
        private:
            Mode _mode;
            Graphics::RectangleEX* _rect;
            uint32_t _count;
            std::vector<Graphics::RectangleEX*> _rects;
        };

        class TriangleCMD : public BaseCommand {
        public:
            explicit TriangleCMD(SDL_Renderer* renderer, Graphics::Triangle* triangle, Mode mode = Mode::Single,
                                 uint32_t count = 0, std::vector<Graphics::Triangle*> triangle_list = {});

            ~TriangleCMD() override = default;

            void reset(SDL_Renderer* renderer, Graphics::Triangle* triangle, Mode mode = Mode::Single,
                       uint32_t count = 0, std::vector<Graphics::Triangle*> triangle_list = {});

            void exec() override;
            void render(Graphics::Triangle* triangle);
        private:
            Graphics::Triangle* _triangle;
            Mode _mode;
            uint32_t _count;
            std::vector<Graphics::Triangle*> _triangles;
        };

        class EllipseCMD : public BaseCommand {
        public:
            explicit EllipseCMD(SDL_Renderer* renderer, Graphics::Ellipse* ellipse, Mode mode = Mode::Single,
                                uint32_t count = 0, std::vector<Graphics::Ellipse*> ellipse_list = {});

            ~EllipseCMD() override = default;

            void reset(SDL_Renderer* renderer, Graphics::Ellipse* ellipse, Mode mode = Mode::Single,
                       uint32_t count = 0, std::vector<Graphics::Ellipse*> ellipse_list = {});

            void exec() override;
            void render(Graphics::Ellipse* ellipse);
        private:
            Graphics::Ellipse* _ellipse;
            Mode _mode;
            uint32_t _count;
            std::vector<Graphics::Ellipse*> _ellipses;
        };
    }
}

#endif //MYENGINE_BASECOMMAND_H
