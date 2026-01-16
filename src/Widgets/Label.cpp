
#include "Label.h"
#include "Utils/RGBAColor.h"

namespace MyEngine::Widget {

    Label::Label(Window *window) : AbstractWidget(window) {
        _NEW_PROPERTY_PTR(this, ENGINE_PROP_BACKGROUND_IMAGE_ORIGINAL_SIZE, Size);
        _NEW_PROPERTY_PTR(this, ENGINE_PROP_TEXT_COLOR, SDL_Color);
        setProperty(ENGINE_PROP_FONT_NAME);
        setProperty(ENGINE_PROP_FONT_SIZE, 9.f);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_PATH);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_SELF);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_TEXTURE);
    }

    Label::Label(std::string object_name, Window *window) : AbstractWidget(std::move(object_name), window) {
        _NEW_PROPERTY_PTR(this, ENGINE_PROP_BACKGROUND_IMAGE_ORIGINAL_SIZE, Size);
        _NEW_PROPERTY_PTR(this, ENGINE_PROP_TEXT_COLOR, SDL_Color);
        setProperty(ENGINE_PROP_FONT_NAME);
        setProperty(ENGINE_PROP_FONT_SIZE, 9.f);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_PATH);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_SELF);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_TEXTURE);
    }

    Label::~Label() {}

    void Label::setFont(const std::string &font_name, const std::string &font_path, float font_size) {
        if (!TextSystem::global()->isFontContain(font_name)) {
            TextSystem::global()->addFont(font_name, font_path,
                                          AbstractWidget::render(), font_size);
            TextSystem::global()->font(font_name)->setFontPath(font_path);
            TextSystem::global()->font(font_name)->setFontSize(font_size);
        }
        _font = TextSystem::global()->font(font_name);
        if (_text_id > 0) {
            auto font = property(ENGINE_PROP_FONT_NAME)->toString();
            auto f_size = property(ENGINE_PROP_FONT_SIZE)->toFloat();
            if (font != font_name) {
                setProperty(ENGINE_PROP_FONT_NAME, font_name);
                _changer_signal |= ENGINE_SIGNAL_LABEL_FONT_CHANGED;
            }
            if (f_size != font_size) {
                setProperty(ENGINE_PROP_FONT_SIZE, font_size);
                _changer_signal |= ENGINE_SIGNAL_LABEL_FONT_SIZE_CHANGED;
            }
        } else {
            _text_id = IDGenerator::getNewTextID();
            TextSystem::global()->addText(_text_id, font_name, _string);
            bool is_contain = TextSystem::global()->isTextContain(_text_id);
            if (is_contain) {
                _text = TextSystem::global()->indexOfText(_text_id);
            }
            _visible_text = is_contain;
        }
        _changer_signal |= ENGINE_SIGNAL_LABEL_TEXT_COLOR_CHANGED;
    }

    void Label::setFont(const std::string &font_name) {
        if (!TextSystem::global()->isFontContain(font_name)) {
            Logger::log(std::format("Label ({}): The font name '{}' is not contained! "
                    "You need to specified font path.", _object_name, font_name),Logger::Error);
            return;
        }
        _font = TextSystem::global()->font(font_name);
        if (_text_id > 0) {
            _changer_signal |= ENGINE_SIGNAL_LABEL_FONT_CHANGED;
            setProperty(ENGINE_PROP_FONT_NAME, font_name);
        } else {
            _text_id = IDGenerator::getNewTextID();
            if (TextSystem::global()->isTextContain(_text_id)) {
                TextSystem::global()->setTextFont(_text_id, font_name);
            } else {
                TextSystem::global()->addText(_text_id, font_name, _string);
            }
        }
        _text = TextSystem::global()->indexOfText(_text_id);
        _visible_text = true;
        _changer_signal |= ENGINE_SIGNAL_LABEL_TEXT_COLOR_CHANGED;
    }

    std::string_view Label::fontName() const {
        return _text ? _text->font_name.data() : nullptr;
    }

    std::string_view Label::fontPath() const {
        return _font ? _font->fontPath().data() : nullptr;
    }

    void Label::setText(const std::string &text) {
        if (TextSystem::global()->isTextContain(_text_id)) {
            _changer_signal |= ENGINE_SIGNAL_LABEL_TEXT_CHANGED;
            _string = text;
        }
    }

    const std::string &Label::text() const {
        return TextSystem::global()->indexOfText(_text_id)->text;
    }

    void Label::appendText(const std::string &text) {
        if (TextSystem::global()->isTextContain(_text_id)) {
            _string += text;
            _changer_signal |= ENGINE_SIGNAL_LABEL_TEXT_CHANGED;
        }
    }

    void Label::setTextColor(const SDL_Color &color) {
        if (TextSystem::global()->isTextContain(_text_id)) {
            _changer_signal |= ENGINE_SIGNAL_LABEL_TEXT_COLOR_CHANGED;
            auto text_color = _GET_PROPERTY_PTR(this, ENGINE_PROP_TEXT_COLOR, SDL_Color);
            text_color->r = color.r;
            text_color->g = color.g;
            text_color->b = color.b;
            text_color->a = color.a;
        }
    }

    void Label::setTextColor(uint64_t hex_code, bool alpha) {
        auto color = RGBAColor::hexCode2RGBA(hex_code, alpha);
        if (TextSystem::global()->isTextContain(_text_id)) {
            _changer_signal |= ENGINE_SIGNAL_LABEL_TEXT_COLOR_CHANGED;
        }
    }

    void Label::setFontSize(float size) {
        auto font_name = fontName();
        if (TextSystem::global()->isFontContain(font_name.data())) {
            _changer_signal |= ENGINE_SIGNAL_LABEL_FONT_SIZE_CHANGED;
            setProperty(ENGINE_PROP_FONT_SIZE, size);
        }
    }

    const SDL_Color &Label::textColor() const {
        return _text->font_color;
    }

    float Label::fontSize() const {
        return _font->fontSize();
    }

    void Label::setTextAlignment(Alignment alignment) {
        if (_auto_resize_by_text) return;
        _alignment = alignment;
        _changer_signal |= ENGINE_SIGNAL_LABEL_TEXT_ALIGNMENT_CHANGED;
    }

    Alignment Label::textAlignment() const { return _alignment; }

    void Label::setAutoResizedByTextEnabled(bool enabled) {
        _auto_resize_by_text = enabled;
        _changer_signal |= ENGINE_SIGNAL_LABEL_AUTO_RESIZED_TEXT_CHANGED;
    }

    bool Label::isAutoResizedByTextEnabled() const { return _auto_resize_by_text; }

    void Label::setBackgroundVisible(bool visible) {
        _visible_bg = visible;
    }

    void Label::setBackgroundColor(const SDL_Color &back_color) {
        _trigger_area.setBackgroundColor(back_color);
    }

    void Label::setBackgroundColor(uint64_t hex_code, bool alpha) {
        _trigger_area.setBackgroundColor(RGBAColor::hexCode2RGBA(hex_code, alpha));
    }

    bool Label::backgroundVisible() const {
        return _visible_bg;
    }

    const SDL_Color &Label::backgroundColor() const {
        return AbstractWidget::_trigger_area.backgroundColor();
    }

    void Label::setBackgroundImage(SDL_Surface *surface, bool delete_later) {
        if (!surface) {
            Logger::log(std::format("Label ({}): The specified surface is not valid!", _object_name),
                        Logger::Error);
            return;
        }
        if (!_bg_img) {
            _bg_img = std::make_unique<Texture>(surface, render(), !delete_later);
            _visible_img = (_bg_img != nullptr);
            _visible_bg = (!_bg_img);
            auto img_geo = _GET_PROPERTY_PTR(this, ENGINE_PROP_BACKGROUND_IMAGE_ORIGINAL_SIZE, Size);
            img_geo->reset(_bg_img->property()->size());
        } else {
            if (delete_later) _changer_signal |= ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_NEED_DELETE;
            _changer_signal |= ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_CHANGED;
            setProperty(ENGINE_PROP_BACKGROUND_IMAGE_SELF, surface);
        }
    }

    void Label::setBackgroundImage(Texture *texture, bool delete_later) {
        if (!texture) {
            Logger::log(std::format("Label (): The specified surface is not valid!", _object_name),
                        Logger::Error);
            return;
        }
        if (!_bg_img) {
            _bg_img = delete_later ? std::unique_ptr<Texture>(texture)
                    : std::make_unique<Texture>(texture->imagePath(), render());
            _bg_img->property()->reset(*texture->property());
            auto img_geo = _GET_PROPERTY_PTR(this, ENGINE_PROP_BACKGROUND_IMAGE_ORIGINAL_SIZE, Size);
            img_geo->reset(_bg_img->property()->size());
            _visible_img = (_bg_img != nullptr);
            _visible_bg = (!_bg_img);
        } else {
            if (delete_later) _changer_signal |= ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_NEED_DELETE;
            _changer_signal |= ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_CHANGED;
            setProperty(ENGINE_PROP_BACKGROUND_IMAGE_TEXTURE, texture);
        }
    }

    void Label::setBackgroundImage(const std::string &image_path) {
        if (!_bg_img) {
            _bg_img = std::make_unique<Texture>(image_path, render());
            _visible_img = (_bg_img != nullptr);
            _visible_bg = (!_bg_img);
            auto img_geo = _GET_PROPERTY_PTR(this, ENGINE_PROP_BACKGROUND_IMAGE_ORIGINAL_SIZE, Size);
            img_geo->reset(_bg_img->property()->size());
        } else {
            _changer_signal |= ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_CHANGED;
            setProperty(ENGINE_PROP_BACKGROUND_IMAGE_PATH, image_path);
        }
    }

    void Label::setBackgroundImageFillMode(ImageFilledMode filled_mode) {
        _fill_mode = filled_mode;
        _changer_signal |= ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_FILLED_CHANGED;
    }

    void Label::clearBackgroundImage() {
        if (_bg_img) _bg_img.reset();
    }

    void Label::setBackgroundImageVisible(bool visible) {
        _visible_img = visible;
    }

    const Texture *const Label::backgroundImage() const {
        return (_bg_img ? _bg_img.get() : nullptr);
    }

    ImageFilledMode Label::backgroundImageFilledMode() const {
        return _fill_mode;
    }

    bool Label::backgroundImageVisible() const {
        return _visible_bg;
    }

    void Label::loadEvent() {
        AbstractWidget::loadEvent();
        updateBgIMGGeometry();
        updateTextGeometry();
    }

    void Label::unloadEvent() {
        AbstractWidget::unloadEvent();
        clearBackgroundImage();
    }

    void Label::paintEvent(MyEngine::Renderer *renderer) {
        AbstractWidget::paintEvent(renderer);
        bool update_text = false, update_img = false;
        bool size_changed = (_changer_signal & ENGINE_SIGNAL_LABEL_SIZE_CHANGED);
        if (_text) {
            if (_changer_signal & ENGINE_SIGNAL_LABEL_TEXT_CHANGED) {
                TextSystem::global()->setText(_text_id, _string);
                update_text = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LABEL_TEXT_COLOR_CHANGED) {
                auto text_color = *_GET_PROPERTY_PTR(this, ENGINE_PROP_TEXT_COLOR, SDL_Color);
                TextSystem::global()->setTextColor(_text_id, text_color);
                update_text = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LABEL_FONT_CHANGED) {
                auto font = property(ENGINE_PROP_FONT_NAME)->toString();
                TextSystem::global()->setTextFont(_text_id, font);
                update_text = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LABEL_FONT_SIZE_CHANGED) {
                auto size = property(ENGINE_PROP_FONT_SIZE)->toFloat();
                TextSystem::global()->setFontSize(_text->font_name, size);
                update_text = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LABEL_AUTO_RESIZED_TEXT_CHANGED) {
                if (_auto_resize_by_text) {
                    _trigger_area.resize(_text->text_size);
                }
                update_text = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LABEL_TEXT_ALIGNMENT_CHANGED) {
                update_text = true;
            }
            if (size_changed) update_text = true;
        }
        if (_bg_img) {
            if (size_changed) {
                update_img = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_CHANGED) {
                bool is_changed_img = false;
                auto path = property(ENGINE_PROP_BACKGROUND_IMAGE_PATH);
                auto texture = property(ENGINE_PROP_BACKGROUND_IMAGE_TEXTURE);
                auto self = property(ENGINE_PROP_BACKGROUND_IMAGE_SELF);
                if (!path->isNull()) {
                    _bg_img->setImagePath(path->toString());
                    setProperty(ENGINE_PROP_BACKGROUND_IMAGE_PATH);
                    is_changed_img = true;
                }
                if (!texture->isNull()) {
                    auto _p = static_cast<Texture*>(texture->toPointer());
                    if (_changer_signal & ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_NEED_DELETE) {
                        _bg_img.reset(_p);
                    } else {
                        _bg_img->setImagePath(_p->imagePath());
                        _bg_img->property()->reset(*_p->property());
                    }
                    setProperty(ENGINE_PROP_BACKGROUND_IMAGE_TEXTURE);
                    is_changed_img = true;
                }
                if (!self->isNull()) {
                    auto _p = static_cast<SDL_Surface*>(self->toPointer());
                    _bg_img->setImageFromSurface(_p,
                         !(_changer_signal & ENGINE_SIGNAL_LABEL_BACKGROUND_IMAGE_NEED_DELETE));
                    setProperty(ENGINE_PROP_BACKGROUND_IMAGE_SELF);
                    is_changed_img = true;
                }
                if (is_changed_img) {
                    auto img_geo = _GET_PROPERTY_PTR(this, ENGINE_PROP_BACKGROUND_IMAGE_ORIGINAL_SIZE, Size);
                    img_geo->reset(_bg_img->property()->size());
                }
                update_img = true;
            }
        }

        if (update_text) {
            updateTextGeometry();
        }
        if (update_img) {
            updateBgIMGGeometry();
        }
        _changer_signal = 0;
        if (_visible_bg) {
            renderer->drawRectangle(&_trigger_area);
        }

        if (_visible_text || _visible_img) {
            renderer->setClipView(toGeometryInt(_trigger_area.geometry()));
            if (_visible_img && _bg_img) _bg_img->draw();
            if (_visible_text) TextSystem::global()->drawText(_text_id, _text_pos, renderer);
            renderer->setClipView({});
        }
    }

    void Label::moveEvent(const Vector2 &position) {
        AbstractWidget::moveEvent(position);
    }

    void Label::resizeEvent(const Size &size) {
        AbstractWidget::resizeEvent(size);
        _changer_signal |= ENGINE_SIGNAL_LABEL_SIZE_CHANGED;
    }

    void Label::visibleChangedEvent(bool visible) {
        AbstractWidget::visibleChangedEvent(visible);
    }

    void Label::enableChangedEvent(bool enabled) {
        AbstractWidget::enableChangedEvent(enabled);
    }

    void Label::updateBgIMGGeometry() {
        if (!_bg_img || !_visible_img) return;
        Size* img_size = _GET_PROPERTY_PTR(this, ENGINE_PROP_BACKGROUND_IMAGE_ORIGINAL_SIZE, Size);
        auto con_geo = _trigger_area.geometry();
        float scaled{};
        const float ARs = con_geo.size.width / con_geo.size.height;
        const float ARi = img_size->width / img_size->height;
        Size new_size{};
        switch (_fill_mode) {
            case None:
                _bg_img->property()->setGeometry(0, 0, img_size->width, img_size->height);
                break;
            case Stretch:
                _bg_img->property()->setGeometry(0, 0, con_geo.size.width, con_geo.size.height);
                break;
            case Center:
                _bg_img->property()->setGeometry(con_geo.size.width * 0.5f - img_size->width * 0.5f,
                                                 con_geo.size.height * 0.5f - img_size->height * 0.5f,
                                                 img_size->width, img_size->height);
                break;
            case Fit:
                if (ARi > ARs) {
                    scaled = con_geo.size.width / img_size->width;
                } else {
                    scaled = con_geo.size.height / img_size->height;
                }
                _bg_img->property()->resize(img_size->width * scaled, img_size->height * scaled);
                new_size.reset(_bg_img->property()->size());
                _bg_img->property()->move((con_geo.size.width - new_size.width) * 0.5f,
                                          (con_geo.size.height - new_size.height) * 0.5f);
                break;
            case Fill:
                if (ARi > ARs) {
                    scaled = con_geo.size.height / img_size->height;
                } else {
                    scaled = con_geo.size.width / img_size->width;
                }
                _bg_img->property()->resize(img_size->width * scaled, img_size->height * scaled);
                new_size.reset(_bg_img->property()->size());
                _bg_img->property()->move((con_geo.size.width - new_size.width) * 0.5f,
                                          (con_geo.size.height - new_size.height) * 0.5f);
                break;

        }
    }

    void Label::updateTextGeometry() {
        if (_auto_resize_by_text) {
            _text_pos.reset(0, 0);
            _trigger_area.resize(_text->text_size);
            return;
        }
        const GeometryF& GEOMETRY = _trigger_area.geometry();
        const Size& SIZE = _text->text_size;
        switch (_alignment)  {
            case LeftTop:
                _text_pos.reset(0, 0);
                break;
            case CenterTop:
                _text_pos.reset((GEOMETRY.size.width * 0.5f - SIZE.width * 0.5f),0);
                break;
            case RightTop:
                _text_pos.reset((GEOMETRY.size.width - SIZE.width),0);
                break;
            case LeftMiddle:
                _text_pos.reset(0,(GEOMETRY.size.height * 0.5f - SIZE.height * 0.5f));
                break;
            case CenterMiddle:
                _text_pos.reset((GEOMETRY.size.width * 0.5f - SIZE.width * 0.5f),
                                 (GEOMETRY.size.height * 0.5f - SIZE.height * 0.5f));
                break;
            case RightMiddle:
                _text_pos.reset((GEOMETRY.size.width - SIZE.width),
                                 (GEOMETRY.size.height * 0.5f - SIZE.height * 0.5f));
                break;
            case LeftBottom:
                _text_pos.reset(0,(GEOMETRY.size.height - SIZE.height));
                break;
            case CenterBottom:
                _text_pos.reset((GEOMETRY.size.width * 0.5f - SIZE.width * 0.5f),
                                 (GEOMETRY.size.height - SIZE.height));
                break;
            case RightBottom:
                _text_pos.reset((GEOMETRY.size.width - SIZE.width),
                                 (GEOMETRY.size.height - SIZE.height));
                break;
        }
    }

}
