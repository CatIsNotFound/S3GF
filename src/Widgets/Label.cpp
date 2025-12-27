
#include "Label.h"

namespace MyEngine::Widget {

    Label::Label(Window *window) : AbstractWidget(window) {
        _font_path = FontDatabase::getSystemDefaultFont().front().font_path;
        _font = new Font(_font_path, 9.f);
        if (!_font->self()) {
            Logger::log("Label: Unable to load default font, please manually set the font!",
                        Logger::Error);
            delete _font;
            _font = nullptr;
        }
    }

    Label::~Label() {
        if (_font && _delete_font) {
            delete _font;
        }
    }

    void Label::setFont(const std::string &font_path, float font_size) {
        if (!_font) {
            _font = new Font(font_path, font_size);
            _delete_font = true;
            if (!_font->self()) {
                Logger::log(std::format("Label: Unable to set font! Exception: {}", SDL_GetError()),
                            Logger::Error);
                delete _font;
                _font = nullptr;
            }
        }
    }

    void Label::setFontFromTextSystem(const std::string &font_name) {
        auto font = TextSystem::global()->font(font_name);
        if (!font) return;
        _font = font;
    }

    Font* Label::font() {
        if (_font) return _font;
        Logger::log("Label: Current font is not loaded! Please manually set the font!", Logger::Fatal);
        throw NullPointerException("Label: Current font is not loaded! Please manually set the font!");
    }

    const std::string &Label::fontPath() const {
        return _font_path;
    }

    void Label::setText(const std::string &text) {
        _text = text;
        if (!_font) {
            Logger::log("Label: Current label have not set font!", Logger::Error);
            return;
        }
        if (!_text_self) {
            _text_self = std::make_unique<Texture>(_font->toImage(_text),
                                                   render(), true);
        } else {
            _text_self->setImageFromSurface(_font->toImage(_text), true);
        }
    }

    const std::string &Label::text() const {
        return _text;
    }

    void Label::setImage(SDL_Surface *surface, bool delete_later) {

    }

    void Label::setImage(Texture *texture, bool delete_later) {

    }

    void Label::setImageAnimation(TextureAnimation *animation, bool delete_later) {

    }

    const Texture *const Label::image() const {
        return _texture;
    }

    const TextureAnimation *const Label::imageAnimation() const {
        return _texture_ani;
    }

    void Label::loadEvent() {
        AbstractWidget::loadEvent();

    }

    void Label::unloadEvent() {
        AbstractWidget::unloadEvent();

    }

    void Label::paintEvent(MyEngine::Renderer *renderer) {
        AbstractWidget::paintEvent(renderer);

    }

    void Label::moveEvent(const Vector2 &position) {
        AbstractWidget::moveEvent(position);

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
}
