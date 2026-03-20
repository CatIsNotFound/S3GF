
#include "BaseCommand.h"

namespace MyEngine {
    namespace RenderCommand {
        BlendModeCMD::BlendModeCMD(SDL_Renderer *renderer, SDL_BlendMode blend_mode)
            : BaseCommand(renderer, "BlendMode") {
            _blend_mode = blend_mode;
        }

        void BlendModeCMD::reset(SDL_Renderer *renderer, SDL_BlendMode blend_mode) {
            _renderer = renderer;
            _blend_mode = blend_mode;
        }

        void BlendModeCMD::exec() {
            auto _ret = SDL_SetRenderDrawBlendMode(_renderer, _blend_mode);
            if (!_ret) {
                Logger::log(Logger::Warn, "Renderer: Set render draw blend mode failed! Exception: {}", SDL_GetError());
            }
        }

        FillCMD::FillCMD(SDL_Renderer *renderer, const SDL_Color &color)
            : BaseCommand(renderer, "Fill") {
            _render_color = color;
        }

        FillCMD::FillCMD(SDL_Renderer *renderer, SDL_Color &&color)
            : BaseCommand(renderer, "Fill") {
            _render_color = std::move(color);
        }

        void FillCMD::reset(SDL_Renderer *render, const SDL_Color &color) {
            _renderer = render;
            _render_color = color;
        }

        void FillCMD::reset(SDL_Renderer *render, SDL_Color &&color) {
            _renderer = render;
            _render_color = std::move(color);
        }

        void FillCMD::exec() {
            SDL_SetRenderDrawColor(_renderer, _render_color.r, _render_color.g,
                                   _render_color.b, _render_color.a);
            auto _ret = SDL_RenderClear(_renderer);
            if (!_ret) {
                Logger::log(Logger::Error, "Renderer: Render clear failed! Exception: {}", SDL_GetError());
            }
        }


        ViewPortCMD::ViewPortCMD(SDL_Renderer *renderer, bool reset, const Geometry &geometry)
            : BaseCommand(renderer, "Viewport"), _reset(reset) {
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        ViewPortCMD::ViewPortCMD(SDL_Renderer *renderer, bool reset, Geometry &&geometry)
            : BaseCommand(renderer, "Viewport"), _reset(reset) {
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        void ViewPortCMD::reset(SDL_Renderer *renderer, bool reset, const Geometry &geometry) {
            _renderer = renderer;
            _reset = reset;
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        void ViewPortCMD::reset(SDL_Renderer *renderer, bool reset, Geometry &&geometry) {
            _renderer = renderer;
            _reset = reset;
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        void ViewPortCMD::exec() {
            bool _ret = SDL_SetRenderViewport(_renderer, (_reset ? nullptr : &_rect));
            if (!_ret) {
                Logger::log(Logger::Warn, "Renderer: Set renderer viewport failed! Exception: {}", SDL_GetError());
            }
        }

        ClipViewCMD::ClipViewCMD(SDL_Renderer *renderer, bool reset, const Geometry &geometry)
            : BaseCommand(renderer, "ClipView"), _reset(reset) {
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        ClipViewCMD::ClipViewCMD(SDL_Renderer *renderer, bool reset, Geometry &&geometry)
            : BaseCommand(renderer, "ClipView"), _reset(reset) {
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        void ClipViewCMD::reset(SDL_Renderer *renderer, bool reset, const Geometry &geometry) {
            _renderer = renderer;
            _reset = reset;
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        void ClipViewCMD::reset(SDL_Renderer *renderer, bool reset, Geometry &&geometry) {
            _renderer = renderer;
            _reset = reset;
            _rect = {geometry.x, geometry.y, geometry.width, geometry.height};
        }

        void ClipViewCMD::exec() {
            bool _ret = SDL_SetRenderViewport(_renderer, (_reset ? nullptr : &_rect));
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set renderer viewport failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
        }


        TextureCMD::TextureCMD(SDL_Renderer *renderer, SDL_Texture *texture, TextureProperty *property,
                               BaseCommand::Mode mode, uint32_t count,
                               const std::vector<TextureProperty*>& properties,
                               const std::vector<SDL_Texture*>& textures)
                           : BaseCommand(renderer, "Texture"), _texture(texture), _property(property),
                                _mode(mode), _count(count), _properties(properties), _textures(textures) {
            if (_mode == Mode::Single) {
                assert(_texture != nullptr && _property != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_texture != nullptr && _count > 0 && _properties.size() == _count);
            } else if (_mode == Mode::Custom) {
                assert(_count > 0 && _properties.size() == _count && _textures.size() == _count);
            }
        }

        void TextureCMD::reset(SDL_Renderer *renderer, SDL_Texture *texture, TextureProperty *textureProperty,
                       BaseCommand::Mode mode, uint32_t count, const std::vector<TextureProperty*>& properties,
                       const std::vector<SDL_Texture*>& textures) {
            _renderer = renderer;
            _texture = texture;
            _property = textureProperty;
            _mode = mode;
            _count = count;
            _properties = properties;
            _textures = textures;
            if (_mode == Mode::Single) {
                assert(_texture != nullptr && _property != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_texture != nullptr && _count > 0 && _properties.size() == _count);
            } else if (_mode == Mode::Custom) {
                assert(_count > 0 && _properties.size() == _count && _textures.size() == _count);
            }
        }

        void TextureCMD::exec() {
            if (_mode == Mode::Single) {
                render(_texture, _property);
            } else if (_mode == Mode::Multiple) {
                for (int i = 0; i < _count; ++i) {
                    render(_texture, _properties[i]);
                }
            } else if (_mode == Mode::Custom) {
                for (int i = 0; i < _count; ++i) {
                    render(_textures[i], _properties[i]);
                }
            }
        }
        
        void TextureCMD::render(SDL_Texture *texture, TextureProperty* prop) {
            auto color = prop->color_alpha;
            auto _ret = SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set texture color failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
            _ret = SDL_SetTextureAlphaMod(texture, color.a);

            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set texture alpha failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
            auto scaled = prop->scaledGeometry();
            auto scaled_pos = scaled.pos;
            auto scaled_size = scaled.size;
            SDL_FRect rect_dest = {scaled_pos.x, scaled_pos.y,
                                   scaled_size.width, scaled_size.height};
            auto anchor = prop->scaledAnchor();
            SDL_FPoint center = {anchor.x, anchor.y};
            _ret = SDL_RenderTextureRotated(_renderer, texture,
                                            prop->clip_mode ? &prop->clip_area : nullptr,
                                            &rect_dest, prop->rotate_angle, &center, prop->flip_mode);
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set render texture failed! Exception: {}",
                                        SDL_GetError()), Logger::Error);
                return;
            }
        }


        PointCMD::PointCMD(SDL_Renderer *renderer, Graphics::Point *point, BaseCommand::Mode mode, uint32_t count,
                           const std::vector<Graphics::Point*>& point_list)
               : BaseCommand(renderer, "Point"), _point(point),
                 _mode(mode), _count(count), _points(point_list) {
            if (_mode == Mode::Single) {
                assert(_point != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && point_list.size() == _count);
            }
        }

        void PointCMD::reset(SDL_Renderer *renderer, Graphics::Point *point, BaseCommand::Mode mode, uint32_t count,
                             const std::vector<Graphics::Point*>& point_list) {
            _renderer = renderer;
            _point = point;
            _mode = mode;
            _count = count;
            _points = point_list;
            if (_mode == Mode::Single) {
                assert(_point != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && point_list.size() == _count);
            }
        }

        void PointCMD::exec() {
            if (_mode == Mode::Single) {
                render(_point);
            } else if (_mode == Mode::Multiple) {
                for (int i = 0; i < _count; ++i) {
                    render(_points[i]);
                }
            }
        }
        
        void PointCMD::render(Graphics::Point *point) {
            const auto color = point->color();
            const auto pos = point->position();
            auto _ret = SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set renderer draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
            if (point->size() == 1) {
                _ret = SDL_RenderPoint(_renderer, pos.x, pos.y);
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render point failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            } else {
                _ret = SDL_RenderGeometry(_renderer, nullptr, point->vertices(),
                                          point->verticesCount(),point->indices(),
                                          point->indicesCount());
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render geometry failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            }
        }

        LineCMD::LineCMD(SDL_Renderer *renderer, Graphics::Line *line, BaseCommand::Mode mode, size_t count,
                         const std::vector<Graphics::Line*>& line_list)
                 : BaseCommand(renderer, "Line"), _line(line), _mode(mode),
                    _count(count), _lines(line_list) {
            if (_mode == Mode::Single) {
                assert(_line != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _lines.size() == _count);
            }
        }

        void LineCMD::reset(SDL_Renderer *renderer, Graphics::Line *line, BaseCommand::Mode mode, size_t count,
                            const std::vector<Graphics::Line*>& line_list) {
            _renderer = renderer;
            _line = line;
            _mode = mode;
            _count = count;
            _lines = line_list;
            if (_mode == Mode::Single) {
                assert(_line != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _lines.size() == _count);
            }
        }

        void LineCMD::exec() {
            if (_mode == Mode::Single) {
                render(_line);
            } else if (_mode == Mode::Multiple) {
                for (int i = 0; i < _count; ++i) {
                    render(_lines[i]);
                }
            }
        }

        void LineCMD::render(Graphics::Line *line) {
            const auto SIZE = line->size();
            const auto START = line->startPosition();
            const auto END = line->endPosition();
            if (!SIZE) return;
            const auto color = line->color();
            auto _ret = SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set render draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
            if (SIZE == 1) {
                _ret = SDL_RenderLine(_renderer, START.x, START.y,
                                      END.x, END.y);
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render line failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            } else {
                _ret = SDL_RenderGeometry(_renderer, nullptr, line->vertices(),
                                          line->vertexCount(), line->indices(), line->indicesCount());
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render geometry failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            }
        }

        RectangleCMD::RectangleCMD(SDL_Renderer *renderer, Graphics::Rectangle *rect, BaseCommand::Mode mode,
                                   uint32_t count, const std::vector<Graphics::Rectangle*>& rect_list)
                                   : BaseCommand(renderer, "Rectangle"), _rect(rect),
                                     _mode(mode), _count(count), _rects(rect_list) {
            if (_mode == Mode::Single) {
                assert(rect != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _rects.size() == _count);
            }
        }

        void RectangleCMD::reset(SDL_Renderer *renderer, Graphics::Rectangle *rect, BaseCommand::Mode mode,
                                 uint32_t count, const std::vector<Graphics::Rectangle*>& rect_list) {
            _renderer = renderer;
            _rect = rect;
            _mode = mode;
            _count = count;
            _rects = rect_list;
            if (_mode == Mode::Single) {
                assert(rect != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _rects.size() == _count);
            }
        }

        void RectangleCMD::exec() {
            if (_mode == Mode::Single) {
                render(_rect);
            } else if (_mode == Mode::Multiple) {
                for (int i = 0; i < _count; ++i) {
                    render(_rects[i]);
                }
            }
        }

        void RectangleCMD::render(Graphics::Rectangle *rect) {
            auto back_color = rect->backgroundColor();
            bool border = (rect->borderSize() > 0) && (rect->borderColor().a > 0);
            if (back_color.a > 0) {
                auto& color = rect->backgroundColor();
                auto _ret = SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render draw color failed! Exception: {}",
                                            SDL_GetError()), Logger::Warn);
                }
                _ret = SDL_RenderGeometry(_renderer, nullptr, rect->vertices(), rect->verticesCount(),
                                          rect->indices(), rect->indicesCount());
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render geometry failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            }
            if (!border) return;
            auto& color = rect->backgroundColor();
            auto _ret = SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set render draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
            _ret = SDL_RenderGeometry(_renderer, nullptr, rect->borderVertices(),
                                      rect->borderVerticesCount(),rect->borderIndices(),
                                      rect->borderIndicesCount());
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set render geometry failed! Exception: {}",
                                        SDL_GetError()), Logger::Error);
            }
        }

        TriangleCMD::TriangleCMD(SDL_Renderer *renderer, Graphics::Triangle *triangle, BaseCommand::Mode mode,
                                 uint32_t count, const std::vector<Graphics::Triangle*>& triangle_list)
             : BaseCommand(renderer, "Triangle"), _triangle(triangle), _mode(mode), _count(count), 
               _triangles(triangle_list) {
            if (_mode == Mode::Single) {
                assert(_triangle != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _triangles.size() == _count);
            }
        }

        void TriangleCMD::reset(SDL_Renderer *renderer, Graphics::Triangle *triangle,
                                MyEngine::RenderCommand::BaseCommand::Mode mode, uint32_t count,
                                const std::vector<Graphics::Triangle*>& triangle_list) {
            _renderer = renderer;
            _triangle = triangle;
            _mode = mode;
            _count = count;
            _triangles = triangle_list;
            if (_mode == Mode::Single) {
                assert(_triangle != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _triangles.size() == _count);
            }
        }

        void TriangleCMD::exec() {
            if (_mode == Mode::Single) {
                render(_triangle);
            } else if (_mode == Mode::Multiple) {
                for (int i = 0; i < _count; ++i) {
                    render(_triangles[i]);
                }
            }
        }

        void TriangleCMD::render(Graphics::Triangle *triangle) {
            bool filled = (triangle->backgroundColor().a > 0);
            bool bordered = (triangle->borderSize() > 0 && triangle->borderColor().a > 0);
            bool _ret = false;
            if (filled) {
                _ret = SDL_RenderGeometry(_renderer, nullptr, triangle->vertices(), 3,
                                          triangle->indices(), 3);
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render geometry failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            }
            if (bordered) {
                const auto SIZE = triangle->borderSize();
                const auto color = triangle->borderColor();
                _ret = SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render draw color failed! Exception: {}",
                                            SDL_GetError()), Logger::Warn);
                }
                int err_cnt = 0;
                if (SIZE == 1) {
                    auto p1 = triangle->position(0),
                            p2 = triangle->position(1),
                            p3 = triangle->position(2);
                    err_cnt += SDL_RenderLine(_renderer, p1.x, p1.y, p2.x, p2.y);
                    err_cnt += SDL_RenderLine(_renderer, p3.x, p3.y, p2.x, p2.y);
                    err_cnt += SDL_RenderLine(_renderer, p1.x, p1.y, p3.x, p3.y);
                    if (err_cnt < 3) {
                        Logger::log(FMT::format("Renderer: Set render triangle failed! Exception: {}",
                                                SDL_GetError()), Logger::Error);
                    }
                } else {
                    err_cnt += SDL_RenderGeometry(_renderer, nullptr, triangle->borderVertices1(), triangle->borderVerticesCount(),
                                                  triangle->borderIndices1(), triangle->borderIndicesCount());
                    err_cnt += SDL_RenderGeometry(_renderer, nullptr, triangle->borderVertices2(), triangle->borderVerticesCount(),
                                                  triangle->borderIndices2(), triangle->borderIndicesCount());
                    err_cnt += SDL_RenderGeometry(_renderer, nullptr, triangle->borderVertices3(), triangle->borderVerticesCount(),
                                                  triangle->borderIndices3(), triangle->borderIndicesCount());
                    if (err_cnt < 3) {
                        Logger::log(FMT::format("Renderer: Set render triangle failed! Exception: {}",
                                                SDL_GetError()), Logger::Error);
                    }
                }
            }
        }

        EllipseCMD::EllipseCMD(SDL_Renderer *renderer, Graphics::Ellipse *ellipse, BaseCommand::Mode mode,
                               uint32_t count, const std::vector<Graphics::Ellipse*>& ellipse_list)
               : BaseCommand(renderer, "Ellipse"), _ellipse(ellipse), 
                 _mode(mode), _count(count), _ellipses(ellipse_list){
            if (_mode == Mode::Single) {
                assert(ellipse != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _ellipses.size() == _count);
            }
        }

        void
        EllipseCMD::reset(SDL_Renderer *renderer, Graphics::Ellipse *ellipse, BaseCommand::Mode mode, uint32_t count,
                          const std::vector<Graphics::Ellipse*>& ellipse_list) {
            _renderer = renderer;
            _ellipse = ellipse;
            _mode = mode;
            _count = count;
            _ellipses = ellipse_list;
            if (_mode == Mode::Single) {
                assert(ellipse != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_count > 0 && _ellipses.size() == _count);
            }
        }

        void EllipseCMD::exec() {
            if (_mode == Mode::Single) {
                render(_ellipse);
            } else if (_mode == Mode::Multiple) {
                for (int i = 0; i < _count; ++i) {
                    render(_ellipses[i]);
                }
            }
        }

        void EllipseCMD::render(Graphics::Ellipse *ellipse) {
            bool filled = (ellipse->backgroundColor().a > 0);
            bool bordered = (ellipse->borderSize() > 0 && ellipse->borderColor().a > 0);
            bool _ret = false;
            if (filled) {
                _ret = SDL_RenderGeometry(_renderer, nullptr, ellipse->vertices(),
                                          ellipse->vertexCount(),ellipse->indices(),
                                          ellipse->indicesCount());
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render geometry failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            }
            if (bordered) {
                _ret = SDL_RenderGeometry(_renderer, nullptr, ellipse->borderVertices(),
                                          ellipse->borderVerticesCount(),ellipse->borderIndices(),
                                          ellipse->borderIndicesCount());
                if (!_ret) {
                    Logger::log(FMT::format("Renderer: Set render geometry failed! Exception: {}",
                                            SDL_GetError()), Logger::Error);
                }
            }
        }

        TextCMD::TextCMD(SDL_Renderer *renderer, TTF_Text *text, const Vector2& position, BaseCommand::Mode mode,
                         uint32_t count, const std::vector<Vector2 *> &position_list,
                         const std::vector<TTF_Text *>& text_list)
             : BaseCommand(renderer, "Text"), _text(text), _pos(position), _mode(mode),
               _count(count) {
            if (_mode == Mode::Single) {
                assert(_text != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_text != nullptr && _count > 0 && position_list.size() == _count);
                _positions = position_list;
            } else if (_mode == Mode::Custom) {
                assert(_count > 0 && position_list.size() == _count && text_list.size() == _count);
                for (uint32_t i = 0; i < _count; ++i) {
                    _texts.emplace_back(text_list[i], position_list[i]);
                }
            }
        }

        void TextCMD::reset(SDL_Renderer *renderer, TTF_Text *text, const Vector2& position, BaseCommand::Mode mode,
                            uint32_t count, const std::vector<Vector2 *> &position_list,
                            std::vector<TTF_Text *> text_list) {
            _renderer = renderer;
            _text = text;
            _pos = position;
            _mode = mode;
            _count = count;
            if (_mode == Mode::Single) {
                assert(_text != nullptr);
            } else if (_mode == Mode::Multiple) {
                assert(_text != nullptr && _count > 0 && position_list.size() == _count);
                _positions = position_list;
            } else if (_mode == Mode::Custom) {
                assert(_count > 0 && position_list.size() == _count && text_list.size() == _count);
                for (uint32_t i = 0; i < _count; ++i) {
                    _texts.emplace_back(text_list[i], position_list[i]);
                }
            }
        }

        void TextCMD::exec() {
            if (_mode == Mode::Single) {
                render(_text, &_pos);
            } else if (_mode == Mode::Multiple) {
                for (uint32_t i = 0; i < _count; ++i) {
                    render(_text, _positions[i]);
                }
            } else if (_mode == Mode::Custom) {
                for (uint32_t i = 0; i < _count; ++i) {
                    auto& text = _texts[i];
                    render(text.text, text.position);
                }
            }
        }

        void TextCMD::render(TTF_Text *text, Vector2 *position) {
            bool _ret = TTF_DrawRendererText(text, position->x, position->y);
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set render text failed! Exception: {}",
                                        SDL_GetError()), Logger::Error);
            }
        }


        DebugTextCMD::DebugTextCMD(SDL_Renderer *renderer, const std::string &text, const Vector2 &position,
                                   const SDL_Color& color, BaseCommand::Mode mode, uint32_t count,
                                   const StringList &text_list, const std::vector<Vector2 *> &position_list)
               : BaseCommand(renderer, "Debug"), _text(text), _position(position),
                 _mode(mode), _count(count), _text_list(text_list), _pos_list(position_list) {
            _render_color = color;
            if (_mode == Mode::Multiple) {
                assert(_count > 0 && (_text_list.size() == _count) && (_pos_list.size() == _count));
            }
        }

        void DebugTextCMD::reset(SDL_Renderer *renderer, const std::string &text, const Vector2 &position,
                                 const SDL_Color& color, BaseCommand::Mode mode, uint32_t count,
                                 const StringList &text_list, const std::vector<Vector2 *> &position_list) {
            _renderer = renderer;
            _render_color = color;
            _text = text;
            _position = position;
            _mode = mode;
            _count = count;
            _text_list = text_list;
            _pos_list = position_list;
            if (_mode == Mode::Multiple) {
                assert(_count > 0 && (_text_list.size() == _count) && (_pos_list.size() == _count));
            }
        }

        void DebugTextCMD::exec() {
            if (_mode == Mode::Single) {
                render(_text, &_position);
            } else if (_mode == Mode::Multiple) {
                for (uint32_t i = 0; i < _count; ++i) {
                    render(_text_list[i], _pos_list[i]);
                }
            }
        }

        void DebugTextCMD::render(const std::string &text, Vector2 *position) {
            auto _ret = SDL_SetRenderDrawColor(_renderer, _render_color.r, _render_color.g,
                                               _render_color.b, _render_color.a);
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set render draw color failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
            _ret = SDL_RenderDebugText(_renderer, position->x, position->y, text.c_str());
            if (!_ret) {
                Logger::log(FMT::format("Renderer: Set render debug text failed! Exception: {}",
                                        SDL_GetError()), Logger::Warn);
            }
        }
    }
}
