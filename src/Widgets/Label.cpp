
#include "Label.h"
#include "Utils/RGBAColor.h"

namespace MyEngine::Widget {

    Label::Label(Window *window) : AbstractWidget(window) {
        NEW_PROPERTY_PTR(this, LABEL_IMAGE_SIZE, Size);
    }

    Label::Label(std::string object_name, Window *window) : AbstractWidget(window) {
        AbstractWidget::_object_name = std::move(object_name);
        NEW_PROPERTY_PTR(this, LABEL_IMAGE_SIZE, Size);
    }

    Label::~Label() {}

    void Label::setFont(const std::string &font_name, const std::string &font_path, float font_size) {
        if (!TextSystem::global()->addFont(font_name, font_path,
                                  AbstractWidget::render(), font_size)) {
            return;
        } else {
            TextSystem::global()->font(font_name)->setFontPath(font_path);
            TextSystem::global()->font(font_name)->setFontSize(font_size);
        }
        _font = TextSystem::global()->font(font_name);
        if (_text_id > 0) {
            TextSystem::global()->setTextFont(_text_id, font_name);
        } else {
            _text_id = IDGenerator::getNewTextID();
            TextSystem::global()->addText(_text_id, font_name, _none_str);
            bool is_contain = TextSystem::global()->isTextContain(_text_id);
            if (is_contain) {
                _text = TextSystem::global()->indexOfText(_text_id);
            }
            _visible_text = is_contain;
        }
    }

    void Label::setFont(const std::string &font_name) {
        if (!TextSystem::global()->isFontContain(font_name)) {
            return;
        }
        _font = TextSystem::global()->font(font_name);
        if (_text_id > 0) {
            TextSystem::global()->setTextFont(_text_id, font_name);
        } else {
            _text_id = IDGenerator::getNewTextID();
            TextSystem::global()->addText(_text_id, font_name, _string);
            bool is_contain = TextSystem::global()->isTextContain(_text_id);
            if (is_contain) {
                _text = TextSystem::global()->indexOfText(_text_id);
            }
            _visible_text = is_contain;
        }
    }

    const std::string &Label::fontName() const {
        return _text ? _text->font_name : _none_str;
    }

    const std::string &Label::fontPath() const {
        return _font ? _font->fontPath() : _none_str;
    }

    void Label::setText(const std::string &text) {
        if (TextSystem::global()->isTextContain(_text_id)) {
            TextSystem::global()->setText(_text_id, text);
            _string = text;

        }
    }

    const std::string &Label::text() const {
        return TextSystem::global()->indexOfText(_text_id)->text;
    }

    void Label::appendText(const std::string &text) {
        if (TextSystem::global()->isTextContain(_text_id)) {
            TextSystem::global()->appendText(_text_id, text);
            updateTextGeometry();
        }
    }

    void Label::setFontColor(const SDL_Color &color) {
        if (TextSystem::global()->isTextContain(_text_id)) {
            TextSystem::global()->setTextColor(_text_id, color);
        }
    }

    void Label::setFontColor(uint64_t hex_code, bool alpha) {
        if (TextSystem::global()->isTextContain(_text_id)) {
            TextSystem::global()->setTextColor(_text_id,
                           RGBAColor::hexCode2RGBA(hex_code, alpha));
        }
    }

    void Label::setFontSize(float size) {
        if (_font) {
            _font->setFontSize(size);
        }
        updateTextGeometry();
    }

    const SDL_Color &Label::fontColor() const {
        return _text->font_color;
    }

    float Label::fontSize() const {
        return _font->fontSize();
    }

    void Label::setTextAlignment(Label::Alignment alignment) {
        _alignment = alignment;
        alignmentChangedEvent(alignment);
    }

    Label::Alignment Label::textAlignment() const { return _alignment; }

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
            Logger::log("Label: The specified surface is not valid!", Logger::Error);
            return;
        }
        if (!_bg_img) {
            _bg_img = std::make_shared<Texture>(surface, render(), !delete_later);
        } else {
            _bg_img->setImageFromSurface(surface, !delete_later);
        }
        auto img_geo = GET_PROPERTY_PTR(this, LABEL_IMAGE_SIZE, Size);
        img_geo->reset(_bg_img->property()->size());
    }

    void Label::setBackgroundImage(Texture *texture, bool delete_later) {
        if (!texture) {
            Logger::log("Label: The specified surface is not valid!", Logger::Error);
            return;
        }
        if (!_bg_img) {
            _bg_img = delete_later ? std::shared_ptr<Texture>(texture)
                    : std::make_shared<Texture>(texture->imagePath(), render());
        } else {
            if (delete_later) {
                _bg_img.reset(texture);
            } else {
                _bg_img->setImagePath(texture->imagePath());
            }
        }
        auto img_geo = GET_PROPERTY_PTR(this, LABEL_IMAGE_SIZE, Size);
        img_geo->reset(_bg_img->property()->size());
    }

    void Label::setBackgroundImage(const std::string &image_path) {
        if (!_bg_img) {
            _bg_img = std::make_shared<Texture>(image_path, render());
        } else {
            _bg_img->setImagePath(image_path);
        }
        auto img_geo = GET_PROPERTY_PTR(this, LABEL_IMAGE_SIZE, Size);
        img_geo->reset(_bg_img->property()->size());
    }

    void Label::setBackgroundImageFillMode(Label::ImageFilledMode filled_mode) {
        _fill_mode = filled_mode;
        imageFillModeChangedEvent(_fill_mode);
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

    Label::ImageFilledMode Label::backgroundImageFilledMode() const {
        return _fill_mode;
    }

    bool Label::backgroundImageVisible() const {
        return _visible_bg;
    }

    void Label::loadEvent() {
        AbstractWidget::loadEvent();

    }

    void Label::unloadEvent() {
        AbstractWidget::unloadEvent();
    }

    void Label::paintEvent(MyEngine::Renderer *renderer) {
        AbstractWidget::paintEvent(renderer);
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
         if (_bg_img) updateBgIMGGeometry();
         if (_text) updateTextGeometry();
    }

    void Label::visibleChangedEvent(bool visible) {
        AbstractWidget::visibleChangedEvent(visible);
    }

    void Label::enableChangedEvent(bool enabled) {
        AbstractWidget::enableChangedEvent(enabled);
    }

    void Label::imageFillModeChangedEvent(Label::ImageFilledMode filled_mode) {
        _fill_mode = filled_mode;
        updateBgIMGGeometry();
    }

    void Label::alignmentChangedEvent(Label::Alignment alignment) {
        updateTextGeometry();
    }

    void Label::updateBgIMGGeometry() {
        if (!_bg_img || !_visible_img) return;
        Size* img_size = GET_PROPERTY_PTR(this, LABEL_IMAGE_SIZE, Size);
        auto con_geo = _trigger_area.geometry();
        float scaled{};
        const float ARs = con_geo.size.width / con_geo.size.height;
        const float ARi = img_size->width / img_size->height;
        Size new_size{};
        Logger::log(std::format("CON_GEO: ({} x {})", con_geo.size.width, con_geo.size.height));
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
        Logger::log(std::format("Update Geometry: {}, {}, {}, {}",
                                _bg_img->property()->geometry().pos.x, _bg_img->property()->geometry().pos.y,
                                _bg_img->property()->geometry().size.width, _bg_img->property()->geometry().size.height));
    }

    void Label::updateTextGeometry() {
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
