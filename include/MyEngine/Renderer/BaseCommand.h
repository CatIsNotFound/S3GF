
#ifndef MYENGINE_RENDERER_BASECOMMAND_H
#define MYENGINE_RENDERER_BASECOMMAND_H
#include "../Components.h"

namespace MyEngine {
    namespace RenderCommand {
        class BaseCommand {
        public:
            enum class Mode : uint8_t {
                Single,
                Multiple,
                Custom
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

        class BlendModeCMD : public BaseCommand {
        public:
            explicit BlendModeCMD(SDL_Renderer* renderer, SDL_BlendMode blend_mode);

            ~BlendModeCMD() override = default;

            void reset(SDL_Renderer* renderer, SDL_BlendMode blend_mode);

            void exec() override;
        };

        class FillCMD : public BaseCommand {
        public:
            explicit FillCMD(SDL_Renderer *renderer, const SDL_Color &color = {});
            explicit FillCMD(SDL_Renderer *renderer, SDL_Color &&color = {});

            ~FillCMD() override = default;

            void reset(SDL_Renderer *render, const SDL_Color &color);
            void reset(SDL_Renderer *render, SDL_Color &&color);

            void exec() override;
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
                                const std::vector<TextureProperty*>& properties = {},
                                const std::vector<SDL_Texture*>& textures = {});
            ~TextureCMD() override = default;

            void reset(SDL_Renderer *renderer, SDL_Texture *texture, TextureProperty *textureProperty,
                       Mode mode = Mode::Single, uint32_t count = 0,
                       const std::vector<TextureProperty*>& properties = {},
                       const std::vector<SDL_Texture*>& textures = {});

            void exec() override;

            void render(SDL_Texture* texture, TextureProperty* prop);

        private:
            Mode _mode;
            uint32_t _count;
            SDL_Texture *_texture;
            TextureProperty *_property;
            std::vector<SDL_Texture*> _textures;
            std::vector<TextureProperty*> _properties;
        };

        class PointCMD : public BaseCommand {
        public:
            explicit PointCMD(SDL_Renderer *renderer, Graphics::Point* point, Mode mode = Mode::Single,
                              uint32_t count = 0, const std::vector<Graphics::Point*>& point_list = {});

            ~PointCMD() override = default;

            void reset(SDL_Renderer *renderer, Graphics::Point* point, Mode mode = Mode::Single,
                          uint32_t count = 0, const std::vector<Graphics::Point*>& point_list = {});

            void exec() override;

            void render(Graphics::Point* point);

        private:
            Graphics::Point* _point;
            Mode _mode;
            uint32_t _count;
            std::vector<Graphics::Point*> _points;
        };

        class LineCMD : public BaseCommand {
        public:
            explicit LineCMD(SDL_Renderer* renderer, Graphics::Line* line, Mode mode = Mode::Single, size_t count = 0,
                             const std::vector<Graphics::Line*>& line_list = {});

            ~LineCMD() = default;

            void reset(SDL_Renderer* renderer, Graphics::Line* line, Mode mode = Mode::Single,
                       size_t count = 0, const std::vector<Graphics::Line*>& line_list = {});

            void exec() override;

            void render(Graphics::Line* line);

        private:
            Graphics::Line* _line;
            Mode _mode;
            uint32_t _count;
            std::vector<Graphics::Line*> _lines;
        };

        class RectangleCMD : public BaseCommand {
        public:
            explicit RectangleCMD(SDL_Renderer* renderer, Graphics::Rectangle* rect, Mode mode = Mode::Single,
                                  uint32_t count = 0, const std::vector<Graphics::Rectangle*>& rect_list = {});

            ~RectangleCMD() override = default;

            void reset(SDL_Renderer* renderer, Graphics::Rectangle* rect, Mode mode = Mode::Single,
                       uint32_t count = 0, const std::vector<Graphics::Rectangle*>& rect_list = {});
            void exec() override;
            void render(Graphics::Rectangle* rect);
        private:
            Mode _mode;
            Graphics::Rectangle* _rect;
            uint32_t _count;
            std::vector<Graphics::Rectangle*> _rects;
        };

        class TriangleCMD : public BaseCommand {
        public:
            explicit TriangleCMD(SDL_Renderer* renderer, Graphics::Triangle* triangle, Mode mode = Mode::Single,
                                 uint32_t count = 0, const std::vector<Graphics::Triangle*>& triangle_list = {});

            ~TriangleCMD() override = default;

            void reset(SDL_Renderer* renderer, Graphics::Triangle* triangle, Mode mode = Mode::Single,
                       uint32_t count = 0, const std::vector<Graphics::Triangle*>& triangle_list = {});

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
                                uint32_t count = 0, const std::vector<Graphics::Ellipse*>& ellipse_list = {});

            ~EllipseCMD() override = default;

            void reset(SDL_Renderer* renderer, Graphics::Ellipse* ellipse, Mode mode = Mode::Single,
                       uint32_t count = 0, const std::vector<Graphics::Ellipse*>& ellipse_list = {});

            void exec() override;
            void render(Graphics::Ellipse* ellipse);
        private:
            Graphics::Ellipse* _ellipse;
            Mode _mode;
            uint32_t _count;
            std::vector<Graphics::Ellipse*> _ellipses;
        };

        class TextCMD : public BaseCommand {
        public:
            struct Text {
                TTF_Text* text;
                Vector2* position;
            };
            explicit TextCMD(SDL_Renderer* renderer, TTF_Text* text, const Vector2& position, Mode mode = Mode::Single,
                             uint32_t count = 0, const std::vector<Vector2*>& position_list = {},
                             const std::vector<TTF_Text *>& text_list = {});

            ~TextCMD() override = default;

            void reset(SDL_Renderer* renderer, TTF_Text* text, const Vector2& position, Mode mode = Mode::Single,
                       uint32_t count = 0, const std::vector<Vector2*>& position_list = {},
                       std::vector<TTF_Text*> text_list = {});

            void exec() override;

            void render(TTF_Text* text, Vector2* position);

        private:
            TTF_Text* _text;
            Vector2 _pos;
            Mode _mode;
            uint32_t _count;
            std::vector<Vector2*> _positions;
            std::vector<Text> _texts;
        };

        class DebugTextCMD : public BaseCommand {
        public:
            explicit DebugTextCMD(SDL_Renderer* renderer, const std::string& text, const Vector2& position,
                                  const SDL_Color& color, Mode mode = Mode::Single, uint32_t count = 0,
                                  const StringList& text_list = {}, const std::vector<Vector2*>& position_list = {});

            void reset(SDL_Renderer* renderer, const std::string& text, const Vector2& position, const SDL_Color& color,
                       Mode mode = Mode::Single, uint32_t count = 0, const StringList& text_list = {},
                       const std::vector<Vector2*>& position_list = {});

            void exec() override;

            void render(const std::string& text, Vector2* position);

        private:
            std::string _text;
            Vector2 _position;
            Mode _mode;
            uint32_t _count;
            StringList _text_list;
            std::vector<Vector2*> _pos_list;
        };
    }
}

#endif //MYENGINE_RENDERER_BASECOMMAND_H
