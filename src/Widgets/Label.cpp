
#include "Label.h"
#include "Utils/RGBAColor.h"

namespace MyEngine::Widget {

    Label::Label(Window *window) : AbstractWidget(window) {
        NEW_PROPERTY_PTR(this, LABEL_TEXT_POSITION, Vector2)
        NEW_PROPERTY_PTR(this, LABEL_IMAGE_GEOMETRY, GeometryF)
    }

    Label::Label(std::string object_name, Window *window) : AbstractWidget(window) {
        AbstractWidget::_object_name = std::move(object_name);
        NEW_PROPERTY_PTR(this, LABEL_TEXT_POSITION, Vector2)
        NEW_PROPERTY_PTR(this, LABEL_IMAGE_GEOMETRY, GeometryF)
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
            auto text_geo = GET_PROPERTY_PTR(this, LABEL_TEXT_POSITION, GeometryF);

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
        if (!_bg_img) {
            _bg_img = std::make_shared<Texture>(surface, render(), !delete_later);
        } else {
            _bg_img->setImageFromSurface(surface, !delete_later);
        }
    }

    void Label::setBackgroundImage(Texture *texture, bool delete_later) {
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
    }

    void Label::setBackgroundImage(const std::string &image_path) {
        if (!_bg_img) {
            _bg_img = std::make_shared<Texture>(image_path, render());
        } else {
            _bg_img->setImagePath(image_path);
        }
    }

    void Label::setBackgroundImageFillMode(Label::ImageFilledMode filled_mode) {
        _fill_mode = filled_mode;
    }

    const Texture *const Label::backgroundImage() const {
        return _bg_img.get();
    }

    Label::ImageFilledMode Label::backgroundImageFilledMode() const {
        return _fill_mode;
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
        if (_visible_text) {
            renderer->setClipView(toGeometryInt(_trigger_area.geometry()));
            TextSystem::global()->drawText(_text_id, Vector2(), renderer);
            renderer->setClipView({});
        }
    }

    void Label::moveEvent(const Vector2 &position) {
        AbstractWidget::moveEvent(position);
        if (_bg_img) updateBgIMGGeometry();
        if (_text) updateTextGeometry();
    }

    void Label::resizeEvent(const Size &size) {
        AbstractWidget::resizeEvent(size);
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

    }

    void Label::updateBgIMGGeometry() {
        GeometryF* img_geo = GET_PROPERTY_PTR(this, LABEL_IMAGE_GEOMETRY, GeometryF);
        auto con_geo = _trigger_area.geometry();
        switch (_fill_mode) {
            case Stretch:
                img_geo->reset(con_geo);
                break;
            case Center:
                break;
            case Fill:
                break;
        }
    }

    void Label::updateTextGeometry() {
        GeometryF* text_geo = GET_PROPERTY_PTR(this, LABEL_TEXT_POSITION, GeometryF);
        auto con_geo = _trigger_area.geometry();
        switch (_alignment)  {
            case LeftTop:
                text_geo->resetPos(con_geo.pos);
                break;
            case CenterTop:

                break;
            case RightTop:
                break;
            case LeftMiddle:
                break;
            case CenterMiddle:
                text_geo->resetPos(con_geo.pos.x + (con_geo.size.width / 2) - text_geo->size.width / 2,
                                   con_geo.pos.y + (con_geo.size.height / 2) - text_geo->size.height / 2);
                break;
            case RightMiddle:
                break;
            case LeftBottom:
                break;
            case CenterBottom:
                break;
            case RightBottom:
                break;
        }
    }
}
