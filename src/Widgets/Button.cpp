
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

    void Button::setBackgroundImage(WidgetStatus status, Texture *texture, bool delete_later) {
        Texture* prop = nullptr;
        switch (status) {
            case WidgetStatus::Normal:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_NORMAL)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_NORMAL, Texture)
                    if (delete_later) {
                        prop->setImagePath(texture->imagePath());
                        prop->property()->reset(*texture->property());
                    }
                } else {
                    NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, BACKGROUND_IMAGE_STATUS_NORMAL, Texture, texture->imagePath(), texture->render());
                }
                break;
            case WidgetStatus::Active:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_ACTIVE)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_ACTIVE, Texture)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Disabled:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_DISABLED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_DISABLED, Texture)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Hovered:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_HOVERED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_HOVERED, Texture)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Pressed:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_PRESSED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_PRESSED, Texture)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
                }
                break;
            case WidgetStatus::Checked:
                if (hasProperty(BACKGROUND_IMAGE_STATUS_CHECKED)) {
                    prop = GET_PROPERTY_PTR(this, BACKGROUND_IMAGE_STATUS_CHECKED, Texture)
                    prop->r = color.r; prop->g = color.g; prop->b = color.b; prop->a = color.a;
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
    }

    void Button::focusOutEvent() {
        AbstractWidget::focusOutEvent();
    }

    void Button::mouseEnteredEvent() {
        AbstractWidget::mouseEnteredEvent();
    }

    void Button::mouseLeftEvent() {
        AbstractWidget::mouseLeftEvent();
    }

    void Button::mouseDownEvent() {
        AbstractWidget::mouseDownEvent();
    }

    void Button::mouseUpEvent() {
        AbstractWidget::mouseUpEvent();
    }

    void Button::mouseClickedEvent() {
        AbstractWidget::mouseClickedEvent();
    }

    void Button::mouseDblClickedEvent() {
        AbstractWidget::mouseDblClickedEvent();
    }

    void Button::keyDownEvent(SDL_Scancode scancode) {
        AbstractWidget::keyDownEvent(scancode);
    }

    void Button::keyUpEvent(SDL_Scancode scancode) {
        AbstractWidget::keyUpEvent(scancode);
    }

    void Button::keyPressedEvent() {
        AbstractWidget::keyPressedEvent();
    }

    void Button::hotKeysPressedEvent() {
        AbstractWidget::hotKeysPressedEvent();
    }

    void Button::fingerDownEvent(const Vector2 &position) {
        AbstractWidget::fingerDownEvent(position);
    }

    void Button::fingerUpEvent(const Vector2 &position) {
        AbstractWidget::fingerUpEvent(position);
    }

    void Button::fingerMoveInEvent() {
        AbstractWidget::fingerMoveInEvent();
    }

    void Button::fingerMoveOutEvent() {
        AbstractWidget::fingerMoveOutEvent();
    }

    void Button::fingerTappedEvent() {
        AbstractWidget::fingerTappedEvent();
    }

    void Button::checkedChanged(bool checked) {

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
    }

    void Button::updateStatus() {

    }
}

