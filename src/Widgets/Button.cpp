
#include "Button.h"
#include "Utils/RGBAColor.h"

namespace MyEngine::Widget {
    Button::Button(std::string object_name, Window *window) : Label(std::move(object_name), window) {
        initStatus();
    }

    Button::Button(Window *window) : Label(window) {
        initStatus();
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

    void Button::setTriggerEvent(MyEngine::Widget::TriggerAction action, std::function<void()> event) {
        if (_triggers.contains(action)) {
            _triggers.at(action) = std::move(event);
        } else {
            _triggers.try_emplace(action, std::move(event));
        }
    }

    void Button::setBackgroundImage(WidgetStatus status, Texture *texture, bool delete_later) {
        Texture* prop = nullptr;
        switch (status) {
            case WidgetStatus::Normal:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_NORMAL)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_NORMAL, Texture)
                    if (!delete_later) {
                        prop->setImagePath(texture->imagePath());
                        prop->property()->reset(*texture->property());
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_NORMAL,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    }
                } else {
                    if (delete_later) {
                        setProperty(BACKGROUND_IMAGE_STATUS_NORMAL,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_NORMAL,
                                    new Texture(texture->imagePath(), texture->renderer()));
                    }
                }
                break;
            case WidgetStatus::Active:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_NORMAL)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_ACTIVE, Texture)
                    if (!delete_later) {
                        prop->setImagePath(texture->imagePath());
                        prop->property()->reset(*texture->property());
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_ACTIVE,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    }
                } else {
                    if (delete_later) {
                        setProperty(BACKGROUND_IMAGE_STATUS_ACTIVE,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_ACTIVE,
                                    new Texture(texture->imagePath(), texture->renderer()));
                    }
                }
                break;
            case WidgetStatus::Disabled:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_DISABLED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_DISABLED, Texture)
                    if (!delete_later) {
                        prop->setImagePath(texture->imagePath());
                        prop->property()->reset(*texture->property());
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_DISABLED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    }
                } else {
                    if (delete_later) {
                        setProperty(BACKGROUND_IMAGE_STATUS_DISABLED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_DISABLED,
                                    new Texture(texture->imagePath(), texture->renderer()));
                    }
                }
                break;
            case WidgetStatus::Hovered:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_HOVERED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_HOVERED, Texture)
                    if (!delete_later) {
                        prop->setImagePath(texture->imagePath());
                        prop->property()->reset(*texture->property());
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_HOVERED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    }
                } else {
                    if (delete_later) {
                        setProperty(BACKGROUND_IMAGE_STATUS_HOVERED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_HOVERED,
                                    new Texture(texture->imagePath(), texture->renderer()));
                    }
                }
                break;
            case WidgetStatus::Pressed:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_PRESSED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_PRESSED, Texture)
                    if (!delete_later) {
                        prop->setImagePath(texture->imagePath());
                        prop->property()->reset(*texture->property());
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_PRESSED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    }
                } else {
                    if (delete_later) {
                        setProperty(BACKGROUND_IMAGE_STATUS_PRESSED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_PRESSED,
                                    new Texture(texture->imagePath(), texture->renderer()));
                    }
                }
                break;
            case WidgetStatus::Checked:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_CHECKED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_CHECKED, Texture)
                    if (!delete_later) {
                        prop->setImagePath(texture->imagePath());
                        prop->property()->reset(*texture->property());
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_CHECKED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    }
                } else {
                    if (delete_later) {
                        setProperty(BACKGROUND_IMAGE_STATUS_CHECKED,
                                    new Texture(texture->imagePath(), texture->renderer()),
                                    [](void* value) { delete static_cast<Texture*>(value); });
                    } else {
                        setProperty(BACKGROUND_IMAGE_STATUS_CHECKED,
                                    new Texture(texture->imagePath(), texture->renderer()));
                    }
                }
                break;
        }
    }

    void Button::setBackgroundColor(WidgetStatus status, const SDL_Color &color) {
        SDL_Color* prop = nullptr;
        switch (status) {
            case WidgetStatus::Normal:
                if (hasProperty(BACKGROUND_COLOR_STATUS_NORMAL)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_NORMAL, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Active:
                if (hasProperty(BACKGROUND_COLOR_STATUS_ACTIVE)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_ACTIVE, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Disabled:
                if (hasProperty(BACKGROUND_COLOR_STATUS_DISABLED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_DISABLED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Hovered:
                if (hasProperty(BACKGROUND_COLOR_STATUS_HOVERED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_HOVERED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Pressed:
                if (hasProperty(BACKGROUND_COLOR_STATUS_PRESSED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_PRESSED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Checked:
                if (hasProperty(BACKGROUND_COLOR_STATUS_CHECKED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_CHECKED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
        }
    }

    void Button::setTextColor(WidgetStatus status, const SDL_Color &color) {
        SDL_Color* prop = nullptr;
        switch (status) {
            case WidgetStatus::Normal:
                if (hasProperty(TEXT_COLOR_STATUS_NORMAL)) {
                    prop = GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_NORMAL, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Active:
                if (hasProperty(TEXT_COLOR_STATUS_ACTIVE)) {
                    prop = GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_ACTIVE, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Disabled:
                if (hasProperty(TEXT_COLOR_STATUS_DISABLED)) {
                    prop = GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_DISABLED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Hovered:
                if (hasProperty(TEXT_COLOR_STATUS_HOVERED)) {
                    prop = GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_HOVERED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Pressed:
                if (hasProperty(TEXT_COLOR_STATUS_PRESSED)) {
                    prop = GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_PRESSED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Checked:
                if (hasProperty(TEXT_COLOR_STATUS_CHECKED)) {
                    prop = GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_CHECKED, SDL_Color)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
        }
    }

    const Texture *Button::backgroundImage(WidgetStatus status) const {
        switch (status) {
            case WidgetStatus::Normal:
                if (!hasProperty(BACKGROUND_IMAGE_STATUS_NORMAL)) return nullptr;
                return GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_NORMAL, Texture)
            case WidgetStatus::Active:
                if (!hasProperty(BACKGROUND_IMAGE_STATUS_ACTIVE)) return nullptr;
                return GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_ACTIVE, Texture)
            case WidgetStatus::Disabled:
                if (!hasProperty(BACKGROUND_IMAGE_STATUS_DISABLED)) return nullptr;
                return GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_DISABLED, Texture)
            case WidgetStatus::Hovered:
                if (!hasProperty(BACKGROUND_IMAGE_STATUS_HOVERED)) return nullptr;
                return GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_HOVERED, Texture)
            case WidgetStatus::Pressed:
                if (!hasProperty(BACKGROUND_IMAGE_STATUS_PRESSED)) return nullptr;
                return GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_PRESSED, Texture)
            case WidgetStatus::Checked:
                if (!hasProperty(BACKGROUND_IMAGE_STATUS_CHECKED)) return nullptr;
                return GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_CHECKED, Texture)
        }
    }

    const SDL_Color &Button::backgroundColor(WidgetStatus status) const {
        switch (status) {
            case WidgetStatus::Normal:
                return *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_NORMAL, SDL_Color)
            case WidgetStatus::Active:
                return *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_ACTIVE, SDL_Color)
            case WidgetStatus::Disabled:
                return *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_DISABLED, SDL_Color)
            case WidgetStatus::Hovered:
                return *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_HOVERED, SDL_Color)
            case WidgetStatus::Pressed:
                return *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_PRESSED, SDL_Color)
            case WidgetStatus::Checked:
                return *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_CHECKED, SDL_Color)
        }
    }

    const SDL_Color &Button::textColor(WidgetStatus status) const {
        switch (status) {
            case WidgetStatus::Normal:
                return *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_NORMAL, SDL_Color)
            case WidgetStatus::Active:
                return *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_ACTIVE, SDL_Color)
            case WidgetStatus::Disabled:
                return *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_DISABLED, SDL_Color)
            case WidgetStatus::Hovered:
                return *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_HOVERED, SDL_Color)
            case WidgetStatus::Pressed:
                return *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_PRESSED, SDL_Color)
            case WidgetStatus::Checked:
                return *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_CHECKED, SDL_Color)
        }
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
        if (isFocusEnabled()) {
            updateStatus(WidgetStatus::Pressed);
        }
    }

    void Button::keyUpEvent(SDL_Scancode scancode) {
        AbstractWidget::keyUpEvent(scancode);
        if (isFocusEnabled()) {
            updateStatus(isHovered() ? WidgetStatus::Hovered : WidgetStatus::Active);
        }
    }

    void Button::keyPressedEvent(SDL_Scancode scancode) {
        AbstractWidget::keyPressedEvent(SDL_SCANCODE_KP_00);
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

    void Button::initStatus() {
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, BACKGROUND_COLOR_STATUS_NORMAL, SDL_Color, RGBAColor::BlueIce)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, BACKGROUND_COLOR_STATUS_ACTIVE, SDL_Color, RGBAColor::BlueBaby)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, BACKGROUND_COLOR_STATUS_DISABLED, SDL_Color, RGBAColor::MixGrayLight)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, BACKGROUND_COLOR_STATUS_HOVERED, SDL_Color, RGBAColor::BlueLight)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, BACKGROUND_COLOR_STATUS_PRESSED, SDL_Color, RGBAColor::MixGray)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, BACKGROUND_COLOR_STATUS_CHECKED, SDL_Color, RGBAColor::BlueSky)

        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, TEXT_COLOR_STATUS_NORMAL, SDL_Color, RGBAColor::MixGrayDark)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, TEXT_COLOR_STATUS_ACTIVE, SDL_Color, RGBAColor::BlueCobalt)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, TEXT_COLOR_STATUS_DISABLED, SDL_Color, RGBAColor::MixGray)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, TEXT_COLOR_STATUS_HOVERED, SDL_Color, RGBAColor::BlueRoyal)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, TEXT_COLOR_STATUS_PRESSED, SDL_Color, RGBAColor::BluePrussian)
        NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, TEXT_COLOR_STATUS_CHECKED, SDL_Color, RGBAColor::BlueNavy)
        updateStatus(WidgetStatus::Normal);
        Label::setFontColor(RGBAColor::MixGrayDark);

    }

    void Button::updateStatus(WidgetStatus status) {
        SDL_Color back_color{}, text_color{};
        Texture* _img = nullptr;
        switch (status) {
            case WidgetStatus::Normal:
                back_color = *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_NORMAL, SDL_Color)
                text_color = *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_NORMAL, SDL_Color)
                if (hasProperty(BACKGROUND_IMAGE_STATUS_NORMAL)) {
                    _img = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_NORMAL, Texture)
                }
                break;
            case WidgetStatus::Active:
                back_color = *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_ACTIVE, SDL_Color)
                text_color = *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_ACTIVE, SDL_Color)
                if (hasProperty(BACKGROUND_IMAGE_STATUS_ACTIVE)) {
                    _img = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_ACTIVE, Texture)
                }
                break;
            case WidgetStatus::Disabled:
                back_color = *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_DISABLED, SDL_Color)
                text_color = *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_DISABLED, SDL_Color)
                if (hasProperty(BACKGROUND_IMAGE_STATUS_DISABLED)) {
                    _img = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_DISABLED, Texture)
                }
                break;
            case WidgetStatus::Hovered:
                back_color = *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_HOVERED, SDL_Color)
                text_color = *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_HOVERED, SDL_Color)
                if (hasProperty(BACKGROUND_IMAGE_STATUS_HOVERED)) {
                    _img = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_HOVERED, Texture)
                }
                break;
            case WidgetStatus::Pressed:
                back_color = *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_PRESSED, SDL_Color)
                text_color = *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_PRESSED, SDL_Color)
                if (hasProperty(BACKGROUND_IMAGE_STATUS_PRESSED)) {
                    _img = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_PRESSED, Texture)
                }
                break;
            case WidgetStatus::Checked:
                back_color = *GET_PROPERTY_PTR(this, BACKGROUND_COLOR_STATUS_CHECKED, SDL_Color)
                text_color = *GET_PROPERTY_PTR(this, TEXT_COLOR_STATUS_CHECKED, SDL_Color)
                if (hasProperty(BACKGROUND_IMAGE_STATUS_CHECKED)) {
                    _img = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_CHECKED, Texture)
                }
                break;
        }
        Label::setBackgroundColor(back_color);
        Label::setFontColor(text_color);
        if (_img) {
            Label::setBackgroundImage(_img);
        }
    }
}

