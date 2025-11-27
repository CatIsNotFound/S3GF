
#include "Button.h"

namespace S3GF {

    Button::Button(const std::string &name, Renderer *renderer, AbstractControl *parent)
        : AbstractControl(name, renderer, parent, Graphics::Rectangle()),
          _btn_text(), _is_valid(false) {
        _btn_skin.border = {RGBAColor::MixGrayDark, RGBAColor::BlueDark, RGBAColor::MixGrayDark,
                            RGBAColor::MixGrayLight, RGBAColor::BlueDark};
        _btn_skin.background = {RGBAColor::BlueIce, RGBAColor::BlueLight, RGBAColor::MixGray,
                                RGBAColor::MixGray, RGBAColor::BlueLight};
        _btn_skin.font = {RGBAColor::MixGrayDark, RGBAColor::BlueDark, RGBAColor::MixGrayDark,
                          RGBAColor::MixGrayLight, RGBAColor::BlueDark};
        _geometry.reset(0, 0, 80, 60);
        _rect.reset(_geometry, 1, RGBAColor::Black, RGBAColor::MixGrayLight);
        _click_area.setGraphic(_rect);
    }

    void Button::setFont(const std::string &font_name, const std::string& font_path) {
        if (TextSystem::global()->font(font_name)) {
            _font_name = font_name;
        } else {
            if (!TextSystem::global()->addFont(font_name, font_path, _renderer)) return;
            _font_name = font_name;
        }
        if (!_is_valid) {
            TextSystem::global()->addText(_next_id++, _font_name, _btn_text);
            _id = _next_id - 1;
            _is_valid = true;
        } else {
            TextSystem::global()->setTextFont(_id, font_name);
        }
    }

    const std::string &Button::fontName() const {
        return _font_name;
    }

    void Button::setText(const std::string &text) {
        _btn_text = text;
        if (_is_valid) {
            TextSystem::global()->setText(_id, text);
        } else {
            Logger::log("Button: Current button have not set the font!", Logger::ERROR);
        }
    }

    const std::string &Button::text() const {
        return _btn_text;
    }

    Size Button::textSize() const {
        int w, h;
        TTF_GetTextSize(TextSystem::global()->indexOfText(_id)->self, &w, &h);
        return {(float)w, (float)h};
    }

    Button::ButtonSkin& Button::buttonSkin() {
        return _btn_skin;
    }

    void Button::setEvent(const std::function<void()> &event) {
        _event = event;
    }

    void Button::onEntered() {
        AbstractControl::onEntered();
    }

    void Button::onHovered() {
        AbstractControl::onHovered();
        _rect.setBorderColor(_btn_skin.border.active);
        _rect.setBackgroundColor(_btn_skin.background.active);
        if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.active);
    }

    void Button::onLeft() {
        AbstractControl::onLeft();
        _rect.setBorderColor(_checkable && _is_checked ? _btn_skin.border.checked : _btn_skin.border.normal);
        _rect.setBackgroundColor(_checkable && _is_checked ? _btn_skin.background.checked : _btn_skin.background.normal);
        if (_is_valid) TextSystem::global()->setTextColor(_id, _checkable && _is_checked ?
                                                            _btn_skin.font.checked : _btn_skin.font.normal);
    }

    void Button::onPressed() {
        AbstractControl::onPressed();
        _rect.setBorderColor(_btn_skin.border.pressed);
        _rect.setBackgroundColor(_btn_skin.background.pressed);
        if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.pressed);
    }

    void Button::onReleased() {
        AbstractControl::onReleased();
        _rect.setBorderColor(_btn_skin.border.active);
        _rect.setBackgroundColor(_btn_skin.background.active);
        if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.active);
    }

    void Button::onTriggered() {
        if (_event) _event();
    }

    void Button::onKeyPressed() {
        _rect.setBorderColor(_btn_skin.border.pressed);
        _rect.setBackgroundColor(_btn_skin.background.pressed);
        if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.pressed);
    }

    void Button::onKeyReleased() {
        if (_click_area.isHovered()) {
            _rect.setBorderColor(_btn_skin.border.active);
            _rect.setBackgroundColor(_btn_skin.background.active);
            if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.active);
        } else {
            _rect.setBorderColor(_checkable && _is_checked ?
                                        _btn_skin.border.checked : _btn_skin.border.normal);
            _rect.setBackgroundColor(_checkable && _is_checked ?
                                        _btn_skin.background.checked : _btn_skin.background.normal);
            if (_is_valid) TextSystem::global()->setTextColor(_id, _checkable && _is_checked ?
                                        _btn_skin.font.checked : _btn_skin.font.normal);
        }
    }

    void Button::resizeEvent() {
        AbstractControl::resizeEvent();
        _click_area.getRect(_rect);
    }

    void Button::moveEvent() {
        AbstractControl::moveEvent();
        _click_area.getRect(_rect);
    }

    void Button::onChecked(bool checked) {
        AbstractControl::onChecked(checked);
        if (checked) {
            _rect.setBorderColor(_btn_skin.border.checked);
            _rect.setBackgroundColor(_btn_skin.background.checked);
            if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.checked);
        } else {
            _rect.setBorderColor(_click_area.isHovered() ? _btn_skin.border.active : _btn_skin.border.normal);
            _rect.setBackgroundColor(_click_area.isHovered() ? _btn_skin.background.active
                                                                        : _btn_skin.background.normal);
            if (_is_valid) TextSystem::global()->setTextColor(_id, _click_area.isHovered() ?
                                                    _btn_skin.font.active : _btn_skin.font.normal);
        }
    }

    void Button::enabledChanged(bool enabled) {
        AbstractControl::enabledChanged(enabled);
        if (enabled) {
            _rect.setBorderColor(_btn_skin.border.normal);
            _rect.setBackgroundColor(_btn_skin.background.normal);
            if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.normal);
        } else {
            _rect.setBorderColor(_btn_skin.border.invalid);
            _rect.setBackgroundColor(_btn_skin.background.invalid);
            if (_is_valid) TextSystem::global()->setTextColor(_id, _btn_skin.font.invalid);
        }
    }

    void Button::paintEvent(Renderer *r) {
        AbstractControl::paintEvent(r);
        r->drawRectangle(_rect);
        if (_is_valid) {
            r->setViewport(toGeometryInt(geometry()));
            auto size = AbstractControl::_geometry.size;
            auto text_size = textSize();
            TextSystem::global()->drawText(_id, {size.width / 2 - text_size.width / 2,
                                                 size.height / 2 - text_size.height / 2}, r);

            r->setViewport({});
        }
    }

    TextureButton::TextureButton(const std::string &name, Renderer* renderer,
                                 AbstractControl* parent)
             : AbstractControl(name, renderer, parent, Graphics::Rectangle()) {
        _font.normal = RGBAColor::Black;
        _font.active = RGBAColor::Black;
        _font.pressed = RGBAColor::Black;
        _font.invalid = RGBAColor::MixGrayLight;
        _font.checked = RGBAColor::MixGray;
    }

    TextureButton::~TextureButton() {}
    
    void TextureButton::setTextures(Texture *normal, Texture *active, Texture *pressed,
                                    Texture *invalid, Texture *checked) {
        if (!normal) {
            Logger::log("TextureButton: The specified texture is not valid!", Logger::FATAL);
            Engine::throwFatalError();
        }
        if (!normal->isValid()) {
            Logger::log(std::format("TextureButton: The specified texture path '{}' is not valid!",
                                    normal->imagePath()), Logger::FATAL);
            Engine::throwFatalError();
        }
        if (!_textures) {
            _textures = std::make_unique<TextureStatus>(normal, active, pressed, invalid, checked);
        } else {
            _textures->normal = normal;
            _textures->active = active;
            _textures->pressed = pressed;
            _textures->invalid = invalid;
            _textures->checked = checked;
        }
        if (!_textures_loaded) _textures_loaded = true;
        _cur_texture = normal;
        _geometry.resetSize(normal->property()->size());
        _rect.setGeometry(_geometry);
        _click_area.setGraphic(_rect);
    }

    void TextureButton::setFont(const std::string &font_name, const std::string &font_path) {
        if (TextSystem::global()->font(font_name)) {
            _font_name = font_name;
        } else {
            if (!TextSystem::global()->addFont(font_name, font_path, _renderer)) return;
            _font_name = font_name;
        }
        if (!_font_loaded) {
            TextSystem::global()->addText(_next_id++, _font_name, _btn_text);
            _id = _next_id - 1;
            _font_loaded = true;
        } else {
            TextSystem::global()->setTextFont(_id, font_name);
        }
        TextSystem::global()->setTextColor(_id, _font.normal);

    }

    const std::string &TextureButton::fontName() const {
        return _font_name;
    }

    void TextureButton::setText(const std::string &text) {
        _btn_text = text;
        if (_font_loaded) {
            TextSystem::global()->setText(_id, text);
        } else {
            Logger::log("Button: Current button have not set the font!", Logger::ERROR);
        }
    }

    const std::string &TextureButton::text() const {
        return _btn_text;
    }

    Size TextureButton::textSize() const {
        int w, h;
        TTF_GetTextSize(TextSystem::global()->indexOfText(_id)->self, &w, &h);
        return {(float)w, (float)h};
    }

    void TextureButton::setTextVisible(bool visible) {
        _text_visible = visible;

    }

    bool TextureButton::isTextVisible() const {
        return _text_visible;
    }

    void TextureButton::setTextPosition(const Vector2 &pos) {
        _text_pos = pos;
    }

    const Vector2 &TextureButton::textPosition() const {
        return _text_pos;
    }

    ColorStatus& TextureButton::textColor() {
        return _font;
    }

    void TextureButton::setEvent(const std::function<void()> &event) {
        _event = event;
    }

    void TextureButton::onEntered() {
        AbstractControl::onEntered();
    }

    void TextureButton::onHovered() {
        AbstractControl::onHovered();
        if (_textures_loaded) _cur_texture = _textures->active;
        if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.active);
    }

    void TextureButton::onLeft() {
        AbstractControl::onLeft();
        if (_textures_loaded) _cur_texture = _textures->normal;
        if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.normal);
    }

    void TextureButton::onPressed() {
        AbstractControl::onPressed();
        if (_textures_loaded) _cur_texture = _textures->pressed;
        if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.pressed);
    }

    void TextureButton::onReleased() {
        AbstractControl::onReleased();
        if (_textures_loaded) _cur_texture = _textures->active;
        if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.active);
    }

    void TextureButton::onTriggered() {
        AbstractControl::onTriggered();
        if (_event) _event();
    }

    void TextureButton::onKeyPressed() {
        AbstractControl::onKeyPressed();
        if (_textures_loaded) _cur_texture = _textures->pressed;
        if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.pressed);
    }

    void TextureButton::onKeyReleased() {
        AbstractControl::onKeyReleased();
        if (_click_area.isHovered()) {
            if (_textures_loaded) _cur_texture = _textures->active;
            if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.active);
        } else {
            if (_textures_loaded) _cur_texture = _textures->normal;
            if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.normal);
        }
    }

    void TextureButton::resizeEvent() {
        AbstractControl::resizeEvent();
        if (_textures_loaded) {
            if (_textures->active) _textures->active->property()->resize(size());
            if (_textures->normal) _textures->normal->property()->resize(size());
            if (_textures->invalid) _textures->invalid->property()->resize(size());
            if (_textures->pressed) _textures->pressed->property()->resize(size());
        }
        _click_area.getRect(_rect);
    }

    void TextureButton::moveEvent() {
        AbstractControl::moveEvent();
        if (_textures_loaded) {
            if (_textures->active) _textures->active->property()->move(position());
            if (_textures->normal) _textures->normal->property()->move(position());
            if (_textures->invalid) _textures->invalid->property()->move(position());
            if (_textures->pressed) _textures->pressed->property()->move(position());
        }
        _click_area.getRect(_rect);
    }

    void TextureButton::onChecked(bool checked) {
        AbstractControl::onChecked(checked);
        if (checked) {
            if (_textures_loaded) _cur_texture = _textures->checked;
            if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.checked);
        } else {
            if (_textures_loaded)
                _cur_texture = _click_area.isHovered() ? _textures->active : _textures->normal;
            if (_font_loaded)
                TextSystem::global()->setTextColor(_id,
                                                   _click_area.isHovered() ? _font.active :_font.normal);
        }
    }

    void TextureButton::enabledChanged(bool enabled) {
        AbstractControl::enabledChanged(enabled);
        if (enabled) {
            if (_textures_loaded) _cur_texture = _textures->normal;
            if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.normal);
        } else {
            if (_textures_loaded) _cur_texture = _textures->invalid;
            if (_font_loaded) TextSystem::global()->setTextColor(_id, _font.invalid);
        }
    }

    void TextureButton::paintEvent(Renderer *r) {
        AbstractControl::paintEvent(r);
        if (_textures_loaded) {
            if (_cur_texture->isValid()) {
                r->drawTexture(_cur_texture->self(), _cur_texture->property());
            } else {
                r->drawTexture(_textures->normal->self(), _textures->normal->property());
            }
        }
        if (_font_loaded && _text_visible) {
            r->setViewport(toGeometryInt(_geometry));
            TextSystem::global()->drawText(_id, _text_pos, r);
            r->setViewport({});
        }
    }
} // S3GF