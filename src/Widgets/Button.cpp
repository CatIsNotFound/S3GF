
#include "Button.h"
#include "Utils/RGBAColor.h"

namespace MyEngine::Widget {
    Button::Button(std::string object_name, Window *window) : Label(std::move(object_name), window) {
        init();
    }

    Button::Button(Window *window) : Label(window) {
        init();
    }

    Button::~Button() {
    }

    void Button::setChecked(bool checked) {
        _checked = checked;
        checkedChanged(checked);
    }

    bool Button::isChecked() const {
        return _checked;
    }

    void Button::setCheckable(bool checkable) {
        _checkable = checkable;
    }

    bool Button::isCheckable() const {
        return _checkable;
    }

    void Button::setTriggerEvent(const std::function<void()> &event) {
        _event = event;
    }

    void Button::setTriggerEvent(TriggerAction action, std::function<void()> event) {
        if (_triggers.contains(action)) {
            _triggers.at(action) = std::move(event);
        } else {
            _triggers.try_emplace(action, std::move(event));
        }
    }

    void Button::setBackgroundImage(WidgetStatus status, Texture *texture, bool delete_later) {
        if (!texture) {
            Logger::log(std::format("Button ({}): The specified texture is not valid!", _object_name),
                        Logger::Error);
            return;
        }

        auto property_key = getBackgroundImagePropertyKey(status);
        if (property_key.empty()) return;

        if (hasProperty(property_key)) {
            auto* prop = _GET_PROPERTY_PTR(this, property_key, Texture);
            prop->setImagePath(texture->imagePath());
            prop->property()->reset(*texture->property());
        } else {
            if (delete_later) {
                setProperty(property_key,
                           texture,
                           [](void* value) { delete static_cast<Texture*>(value); });
            } else {
                setProperty(property_key,
                           new Texture(texture->imagePath(), texture->renderer()),
                           [](void* value) { delete static_cast<Texture*>(value); });
            }
        }
    }

    void Button::setBackgroundColor(WidgetStatus status, const SDL_Color &color) {
        auto property_key = getBackgroundColorPropertyKey(status);
        if (property_key.empty()) return;

        if (hasProperty(property_key)) {
            auto* prop = _GET_PROPERTY_PTR(this, property_key, SDL_Color);
            prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
        }
    }

    void Button::setTextColor(WidgetStatus status, const SDL_Color &color) {
        auto property_key = getTextColorPropertyKey(status);
        if (property_key.empty()) return;

        if (hasProperty(property_key)) {
            auto* prop = _GET_PROPERTY_PTR(this, property_key, SDL_Color);
            prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
        }
    }

    const Texture *Button::backgroundImage(WidgetStatus status) const {
        auto property_key = getBackgroundImagePropertyKey(status);
        if (property_key.empty() || !hasProperty(property_key)) return nullptr;
        return _GET_PROPERTY_PTR(this, property_key, Texture);
    }

    const SDL_Color &Button::backgroundColor(WidgetStatus status) const {
        auto property_key = getBackgroundColorPropertyKey(status);
        return *_GET_PROPERTY_PTR(this, property_key, SDL_Color);
    }

    const SDL_Color &Button::textColor(WidgetStatus status) const {
        auto property_key = getTextColorPropertyKey(status);
        return *_GET_PROPERTY_PTR(this, property_key, SDL_Color);
    }

    void Button::loadEvent() {
        Label::loadEvent();
        updateStatus(AbstractWidget::isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal);
    }

    void Button::focusInEvent() {
        AbstractWidget::focusInEvent();
        updateStatus(WidgetStatus::Active);
    }

    void Button::focusOutEvent() {
        AbstractWidget::focusOutEvent();
        updateStatus(WidgetStatus::Normal);
    }

    void Button::mouseEnteredEvent() {
        AbstractWidget::mouseEnteredEvent();
        updateStatus(WidgetStatus::Hovered);
    }

    void Button::mouseLeftEvent() {
        AbstractWidget::mouseLeftEvent();
        updateStatus(isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal);
    }

    void Button::mouseDownEvent() {
        AbstractWidget::mouseDownEvent();
        updateStatus(WidgetStatus::Pressed);
    }

    void Button::mouseUpEvent() {
        AbstractWidget::mouseUpEvent();
        updateStatus(isHovered() ? WidgetStatus::Hovered :
                     (isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal));
    }

    void Button::mouseClickedEvent() {
        AbstractWidget::mouseClickedEvent();
        if (_event) _event();
        if (_triggers.contains(TriggerAction::MouseClicked)) {
            _triggers[TriggerAction::MouseClicked]();
        }
    }

    void Button::mouseDblClickedEvent() {
        AbstractWidget::mouseDblClickedEvent();
        // if (_event) _event();
        if (_triggers.contains(TriggerAction::MouseDblClicked)) {
            _triggers[TriggerAction::MouseDblClicked]();
        }
    }

    void Button::keyDownEvent(SDL_Scancode scancode) {
        AbstractWidget::keyDownEvent(scancode);
        if (isConfirmKey(scancode) && isFocusEnabled())
                updateStatus(WidgetStatus::Pressed);
    }

    void Button::keyUpEvent(SDL_Scancode scancode) {
        AbstractWidget::keyUpEvent(scancode);
        if (isConfirmKey(scancode) && isFocusEnabled()) {
            updateStatus(isHovered() ? WidgetStatus::Hovered : WidgetStatus::Active);
        }
    }

    void Button::keyPressedEvent(SDL_Scancode scancode) {
        AbstractWidget::keyPressedEvent(scancode);
        if (!isConfirmKey(scancode)) return;
        if (_event) _event();
        if (_triggers.contains(TriggerAction::KeyPressed)) {
            _triggers[TriggerAction::KeyPressed]();
        }
    }

    void Button::hotKeysPressedEvent() {
        AbstractWidget::hotKeysPressedEvent();
        if (_event) _event();
        if (_triggers.contains(TriggerAction::HotKeysPressed)) {
            _triggers[TriggerAction::HotKeysPressed]();
        }
    }

    void Button::fingerDownEvent(const Vector2 &position) {
        AbstractWidget::fingerDownEvent(position);
        updateStatus(WidgetStatus::Pressed);
    }

    void Button::fingerUpEvent(const Vector2 &position) {
        AbstractWidget::fingerUpEvent(position);
        updateStatus(isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal);
    }

    void Button::fingerMoveInEvent() {
        AbstractWidget::fingerMoveInEvent();
        updateStatus(WidgetStatus::Pressed);
    }

    void Button::fingerMoveOutEvent() {
        AbstractWidget::fingerMoveOutEvent();
        updateStatus(isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal);
    }

    void Button::fingerTappedEvent() {
        AbstractWidget::fingerTappedEvent();
        if (_event) _event();
        if (_triggers.contains(TriggerAction::FingerTapped)) {
            _triggers[TriggerAction::FingerTapped]();
        }
    }

    void Button::checkedChanged(bool checked) {
        WidgetStatus _ret;
        if (checked) {
            _ret = WidgetStatus::Checked;
        } else {
            if (isHovered()) {
                _ret = WidgetStatus::Hovered;
            } else if (isFocusEnabled()) {
                _ret = WidgetStatus::Active;
            } else {
                _ret = WidgetStatus::Normal;
            }
        }
        updateStatus(_ret);
    }

    void Button::enableChangedEvent(bool enabled) {
        updateStatus(!enabled ? WidgetStatus::Disabled :
                     (isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal));
    }

    void Button::init() {
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_BACKGROUND_COLOR_STATUS_NORMAL, SDL_Color, RGBAColor::BlueIce);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_BACKGROUND_COLOR_STATUS_ACTIVE, SDL_Color, RGBAColor::BlueBaby);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_BACKGROUND_COLOR_STATUS_DISABLED, SDL_Color, RGBAColor::MixGrayLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_BACKGROUND_COLOR_STATUS_HOVERED, SDL_Color, RGBAColor::BlueLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_BACKGROUND_COLOR_STATUS_PRESSED, SDL_Color, RGBAColor::MixGray);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_BACKGROUND_COLOR_STATUS_CHECKED, SDL_Color, RGBAColor::BlueSky);

        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_TEXT_COLOR_STATUS_NORMAL, SDL_Color, RGBAColor::MixGrayDark);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_TEXT_COLOR_STATUS_ACTIVE, SDL_Color, RGBAColor::BlueCobalt);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_TEXT_COLOR_STATUS_DISABLED, SDL_Color, RGBAColor::MixGray);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_TEXT_COLOR_STATUS_HOVERED, SDL_Color, RGBAColor::BlueRoyal);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_TEXT_COLOR_STATUS_PRESSED, SDL_Color, RGBAColor::BluePrussian);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_TEXT_COLOR_STATUS_CHECKED, SDL_Color, RGBAColor::BlueNavy);
    }

    void Button::updateStatus(WidgetStatus status) {

        auto bg_color = getBackgroundColorForStatus(status);
        auto text_color = getTextColorForStatus(status);

        Label::setBackgroundColor(bg_color);
        Label::setTextColor(text_color);

        auto* texture = getBackgroundImageForStatus(status);
        if (texture && !texture->imagePath().empty()) {
            Label::setBackgroundImage(texture->imagePath());
        }
    }

    std::string Button::getBackgroundImagePropertyKey(WidgetStatus status) const {
        switch (status) {
            case WidgetStatus::Normal: return ENGINE_PROP_BACKGROUND_IMAGE_STATUS_NORMAL;
            case WidgetStatus::Active: return ENGINE_PROP_BACKGROUND_IMAGE_STATUS_ACTIVE;
            case WidgetStatus::Disabled: return ENGINE_PROP_BACKGROUND_IMAGE_STATUS_DISABLED;
            case WidgetStatus::Hovered: return ENGINE_PROP_BACKGROUND_IMAGE_STATUS_HOVERED;
            case WidgetStatus::Pressed: return ENGINE_PROP_BACKGROUND_IMAGE_STATUS_PRESSED;
            case WidgetStatus::Checked: return ENGINE_PROP_BACKGROUND_IMAGE_STATUS_CHECKED;
            default: return "";
        }
    }

    std::string Button::getBackgroundColorPropertyKey(WidgetStatus status) {
        switch (status) {
            case WidgetStatus::Normal: return ENGINE_PROP_BACKGROUND_COLOR_STATUS_NORMAL;
            case WidgetStatus::Active: return ENGINE_PROP_BACKGROUND_COLOR_STATUS_ACTIVE;
            case WidgetStatus::Disabled: return ENGINE_PROP_BACKGROUND_COLOR_STATUS_DISABLED;
            case WidgetStatus::Hovered: return ENGINE_PROP_BACKGROUND_COLOR_STATUS_HOVERED;
            case WidgetStatus::Pressed: return ENGINE_PROP_BACKGROUND_COLOR_STATUS_PRESSED;
            case WidgetStatus::Checked: return ENGINE_PROP_BACKGROUND_COLOR_STATUS_CHECKED;
            default: return "";
        }
    }

    std::string Button::getTextColorPropertyKey(WidgetStatus status) {
        switch (status) {
            case WidgetStatus::Normal: return ENGINE_PROP_TEXT_COLOR_STATUS_NORMAL;
            case WidgetStatus::Active: return ENGINE_PROP_TEXT_COLOR_STATUS_ACTIVE;
            case WidgetStatus::Disabled: return ENGINE_PROP_TEXT_COLOR_STATUS_DISABLED;
            case WidgetStatus::Hovered: return ENGINE_PROP_TEXT_COLOR_STATUS_HOVERED;
            case WidgetStatus::Pressed: return ENGINE_PROP_TEXT_COLOR_STATUS_PRESSED;
            case WidgetStatus::Checked: return ENGINE_PROP_TEXT_COLOR_STATUS_CHECKED;
            default: return "";
        }
    }

    SDL_Color Button::getBackgroundColorForStatus(WidgetStatus status) const {
        std::string property_key = getBackgroundColorPropertyKey(status);
        if (hasProperty(property_key)) {
            return *_GET_PROPERTY_PTR(this, property_key, SDL_Color);
        }
        
        property_key = getBackgroundColorPropertyKey(WidgetStatus::Normal);
        if (hasProperty(property_key)) {
            return *_GET_PROPERTY_PTR(this, property_key, SDL_Color);
        }
        
        return SDL_Color{0, 0, 0, 255};
    }

    SDL_Color Button::getTextColorForStatus(WidgetStatus status) const {
        std::string property_key = getTextColorPropertyKey(status);
        if (hasProperty(property_key)) {
            return *_GET_PROPERTY_PTR(this, property_key, SDL_Color);
        }
        
        property_key = getTextColorPropertyKey(WidgetStatus::Normal);
        if (hasProperty(property_key)) {
            return *_GET_PROPERTY_PTR(this, property_key, SDL_Color);
        }
        
        return SDL_Color{255, 255, 255, 255};
    }

    Texture* Button::getBackgroundImageForStatus(WidgetStatus status) const {
        auto property_key = getBackgroundImagePropertyKey(status);
        if (!property_key.empty() && hasProperty(property_key)) {
            return _GET_PROPERTY_PTR(this, property_key, Texture);
        }
        return nullptr;
    }
}

