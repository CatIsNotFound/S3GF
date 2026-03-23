
#include "include/MyEngine/UI/Label.h"

#include "include/MyEngine/Utils/RGBAColor.h"

namespace MyEngine::UI {
    Label::Label(const std::string_view &object_name, Window *parent)
            : AbstractControl("Label", object_name, parent) {
        setGeometry(0, 0, 120, 40);
        _rectangle = std::make_shared<Graphics::Rectangle>(geometry());
        setProperty(LabelProp_TextID, Variant(static_cast<uint64_t>(0)));
        setProperty(LabelProp_TextPos, Variant(new Vector2(0, 0), Variant::makeDefaultDeleter<Vector2>()));
        setProperty(LabelProp_Font_Ptr, Variant());
        setProperty(LabelProp_Text_Alignment, Variant(Alignment::LeftTop));
        setProperty(LabelProp_Image_FilledMode, Variant(FilledMode::None));
    }

    Label::Label(Window *parent) : AbstractControl("Label", parent) {
        setProperty(LabelProp_TextID, Variant(static_cast<uint64_t>(0)));
        setProperty(LabelProp_TextPos, Variant(new Vector2(0, 0), Variant::makeDefaultDeleter<Vector2>()));
        setProperty(LabelProp_Font_Ptr, Variant());
        setProperty(LabelProp_Text_Alignment, Variant(Alignment::LeftTop));
        setProperty(LabelProp_Image_FilledMode, Variant(FilledMode::None));
    }

    void Label::setFont(Font *font) {
        if (!font) {
            Logger::log(Logger::Error, "{}({}): Can not to set invalid font!",
                            className(), objectName());
            return;
        }
        setProperty(LabelProp_Font_Ptr, Variant(font));
    }

    void Label::setFont(const std::string_view &font_name) {
        if (!TextSystem::global()->isFontContain(font_name.data())) {
            Logger::log(Logger::Error, "{}({}): The specified font '{}' is not exist in TextSystem!",
                            className(), objectName(), font_name);
            return;
        }
        auto font = TextSystem::global()->font(font_name.data());
        setProperty(LabelProp_Font_Ptr, Variant(font, Label_Font_Type_ID));
    }

    void Label::setFont(const std::string_view &font_name, const std::string_view &path, float size) {
        if (TextSystem::global()->isFontContain(font_name.data())) {
            auto font = TextSystem::global()->font(font_name.data());
            font->setFontPath(path.data());
            font->setFontSize(size);
            setProperty(LabelProp_Font_Ptr, Variant(font));
        } else {
            TextSystem::global()->addFont(font_name.data(), path.data(), window()->renderer(), size);
        }
        auto text_id = property(LabelProp_TextID).toUInt64();
        if (!TextSystem::global()->isTextContain(text_id)) {
            uint64_t new_id = IDGenerator::getNewTextID();
            TextSystem::global()->addText(new_id, font_name.data(), "");
            setProperty(LabelProp_TextID, Variant(new_id));
        } else {
            TextSystem::global()->setTextFont(text_id, font_name.data());
        }
    }

    std::optional<Font *> Label::font() const {
        const auto& ptr = property(LabelProp_Font_Ptr);
        if (ptr.type() != Variant::Pointer || ptr.customTypeID() != Label_Font_Type_ID) return {};

        return ptr.toCustomPointer<Font>();
    }

    void Label::setText(const std::string_view &text) {
        _text = text;
    }

    void Label::appendText(const std::string_view &text) {
        _text += text;
    }

    const std::string & Label::text() const {
        return _text;
    }

    void Label::setTextColor(const SDL_Color &color) {
        TextSystem::global()->setTextColor(property(LabelProp_TextID).toUInt64(), color);
    }

    const SDL_Color & Label::textColor() const {
        return TextSystem::global()->indexOfText(property(LabelProp_TextID).toUInt64())->font_color;
    }

    void Label::setBackgroundColor(const SDL_Color &color) {
        _rectangle->setBackgroundColor(color);
    }

    void Label::setBackgroundColor(uint64_t hex_code, bool alpha) {
        _rectangle->setBackgroundColor(RGBAColor::hexCode2RGBA(hex_code, alpha));
    }

    void Label::setBorderColor(const SDL_Color &color) {
        _rectangle->setBorderColor(color);
    }

    void Label::setBorderColor(uint64_t hex_code, bool alpha) {
        _rectangle->setBorderColor(RGBAColor::hexCode2RGBA(hex_code, alpha));
    }

    const SDL_Color& Label::backgroundColor() const {
        return _rectangle->backgroundColor();
    }

    const SDL_Color& Label::borderColor() const {
        return _rectangle->borderColor();
    }

    void Label::setBackgroundImage(SDL_Surface *surface, bool delete_later) {
        if (!_texture) {
            _texture = std::make_shared<Texture>(std::forward<SDL_Surface*&&>(surface), window()->renderer(), delete_later);
        } else {
            _texture->setImageFromSurface(std::forward<SDL_Surface*&&>(surface), delete_later);
        }
    }

    void Label::setBackgroundImage(const std::shared_ptr<Texture> &texture) {
        if (_texture) {
            _texture.reset();
        }
        _texture = texture;
    }

    std::optional<Texture *> Label::backgroundImage() const {
        if (_texture) return _texture.get();
        return {};
    }

    std::optional<TextureProperty *> Label::backgroundImageProperty() const {
        if (_texture) return _texture->property();
        return {};
    }

    void Label::setImageFilledMode(FilledMode mode) {
        setProperty(LabelProp_Image_FilledMode, Variant(mode));
    }

    FilledMode Label::imageFilledMode() const {
        return static_cast<FilledMode>(property(LabelProp_Image_FilledMode).toUInt8());
    }

    void Label::setTextAlignment(Alignment alignment) {
        setProperty(LabelProp_Text_Alignment, Variant(alignment));
    }

    Alignment Label::textAlignment() const {
        return static_cast<Alignment>(property(LabelProp_Text_Alignment).toUInt8());
    }

    void Label::loadEvent() {
        auto layer_mgr = window()->renderer()->layerManager();
        if (!layer_mgr) {
            layer_mgr = window()->renderer()->layerManager();
        }
        if (!layer_mgr->contains(layerName())) {
            layer_mgr->insertLayer(0, layerName());
        }
        auto ui_layer = layer_mgr->layer(layerName());
        auto label_component = new Component(this, false);
        label_component->setRenderEvent([](const Label* self) {
            auto render = self->window()->renderer();
            render->drawRectangle(self->_rectangle.get());
            if (self->_texture) self->_texture->draw();
            auto text_id = self->property(LabelProp_TextID).toUInt64();
            if (TextSystem::global()->isTextContain(text_id) > 0) {
                auto text_pos = self->property(LabelProp_TextPos).toCustomPointer<Vector2>();
                TextSystem::global()->drawText(text_id, *text_pos, render);
            }
        });
        ui_layer->addComponent(std::forward<ComponentBase*>(label_component), className(), objectName());
    }

    void Label::unloadEvent() {
        AbstractControl::unloadEvent();
        auto layer_mgr = window()->renderer()->layerManager();
        if (!layer_mgr) return;
        if (!layer_mgr->contains(layerName())) return;
        auto ui_layer = layer_mgr->layer(layerName());
        auto idx = ui_layer->indexOf(objectName());
        if (idx.has_value()) {
            ui_layer->removeComponent(idx.value());
        }
    }

    void Label::enabledEvent() {
    }

    void Label::disabledEvent() {
    }

    void Label::showEvent() {
    }

    void Label::hideEvent() {
    }

    void Label::getFocusEvent() {
    }

    void Label::lostFocusEvent() {
    }

    void Label::activeEvent() {
    }

    void Label::inactiveEvent() {
    }

    void Label::moveEvent(const Vector2 &position) {

    }

    void Label::resizeEvent(const Size &size) {
    }

    void Label::paintEvent() {

    }

    void Label::triggeredEvent() {
    }

    void Label::keyDownEvent(SDL_Scancode) {
    }

    void Label::keyUpEvent(SDL_Scancode) {
    }

    void Label::keyPressedEvent(SDL_Scancode) {
    }

    void Label::hotkeyDownEvent(size_t index) {
    }

    void Label::mouseButtonDownEvent(MouseStatus) {
    }

    void Label::mouseButtonUpEvent(MouseStatus) {
    }

    void Label::mouseMovedEvent(const Vector2 &position, const Vector2 &distance) {
    }

    void Label::mouseMovedInEvent() {
    }

    void Label::mouseMovedOutEvent() {
    }

    void Label::mouseClicked() {
    }

    void Label::customContextMenuRequested() {
    }

    void Label::fingerDownEvent(SDL_FingerID finger_id) {
    }

    void Label::fingerMovedEvent(const Vector2 &position, const Vector2 &distance) {
    }

    void Label::fingerUpEvent(SDL_FingerID finger_id) {
    }

    void Label::fingerTouchedEvent() {
    }

    void Label::fingerMovedInEvent() {
    }

    void Label::fingerMovedOutEvent() {
    }
}
