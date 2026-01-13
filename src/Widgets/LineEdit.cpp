
#include "LineEdit.h"

namespace MyEngine::Widget {
    LineEdit::LineEdit(Window *window) : AbstractWidget(window) {
        init();
    }

    LineEdit::LineEdit(std::string object_name, Window *window)
            : AbstractWidget(std::move(object_name), window) {
        init();
    }

    LineEdit::~LineEdit() {}

    void LineEdit::setFont(const std::string &font_name, const std::string &font_path, float font_size) {
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
                _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_FONT_CHANGED;
            }
            if (f_size != font_size) {
                setProperty(ENGINE_PROP_FONT_SIZE, font_size);
                _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_FONT_SIZE_CHANGED;
            }
        } else {
            _text_id = IDGenerator::getNewTextID();
            _place_text_id = IDGenerator::getNewTextID();
            TextSystem::global()->addText(_text_id, font_name, "");
            TextSystem::global()->addText(_place_text_id, font_name, "");
            bool is_contain = TextSystem::global()->isTextContain(_text_id);
            if (is_contain) {
                _text = TextSystem::global()->indexOfText(_text_id);
                _place_text = TextSystem::global()->indexOfText(_place_text_id);
            }
        }
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
    }

    void LineEdit::setFont(const std::string &font_name) {
        if (!TextSystem::global()->isFontContain(font_name)) {
            Logger::log(std::format("LineEdit ({}): The font name '{}' is not contained! "
                                "You need to specified font path.", _object_name, font_name),Logger::Error);
            return;
        }
        _font = TextSystem::global()->font(font_name);
        if (_text_id > 0) {
            _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_FONT_CHANGED;
            setProperty(ENGINE_PROP_FONT_NAME, font_name);
        } else {
            _text_id = IDGenerator::getNewTextID();
            _place_text_id = IDGenerator::getNewTextID();
            if (TextSystem::global()->isTextContain(_text_id)) {
                TextSystem::global()->setTextFont(_text_id, font_name);
            } else {
                TextSystem::global()->addText(_text_id, font_name, "");
                TextSystem::global()->addText(_place_text_id, font_name, "");
            }
        }
        _text = TextSystem::global()->indexOfText(_text_id);
        _place_text = TextSystem::global()->indexOfText(_place_text_id);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
    }

    const std::string &LineEdit::fontName() const {
        return _text ? _text->font_name : _none_str;
    }

    const std::string &LineEdit::fontPath() const {
        auto font = fontName();
        return _text ? _text->font_name : _none_str;
    }

    void LineEdit::setFontSize(float size) {
        setProperty(ENGINE_PROP_FONT_SIZE, size);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_SIZE_CHANGED;
    }

    float LineEdit::fontSize() const {
        return _font->fontSize();
    }

    void LineEdit::setText(const std::string &text) {
        _strings = Algorithm::splitUTF_8(text);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED;
        if (_strings.empty()) {
            _status ^= ENGINE_BOOL_LINE_EDIT_HAS_TEXT;
            if (!placeHolderText().empty()) _status |= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        } else {
            _status |= ENGINE_BOOL_LINE_EDIT_HAS_TEXT;
            if (!placeHolderText().empty()) _status ^= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        }
        textChangedEvent();
    }

    std::string_view LineEdit::text() const {
        return {property(ENGINE_PROP_LINE_EDIT_TEXT)->toString()};
    }

    const StringList &LineEdit::textList() const {
        return _strings;
    }

    void LineEdit::setPlaceHolderText(const std::string &text) {
        setProperty(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT, text);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_CHANGED;
        if (_strings.empty()) {
            _status |= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        }
    }

    std::string_view LineEdit::placeHolderText() const {
        return {property(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT)->toString()};
    }

    void LineEdit::setPasswordEnabled(bool enabled, const std::string &secret) {
        if (enabled) {
            _status |= ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE;
            strncpy(_secret_char, secret.data(), sizeof(char) * 8);
        } else {
            _changer_signal ^= ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE;
        }
    }

    bool LineEdit::passwordEnabled() const {
        return _status & ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE;
    }

    void LineEdit::setTextColor(const SDL_Color &color) {
        auto _color = _GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
    }

    void LineEdit::setPlaceHolderTextColor(const SDL_Color &color) {
        auto _color = _GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_COLOR_CHANGED;
    }

    const SDL_Color &LineEdit::textColor() const {
        return *_GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR, SDL_Color);
    }

    const SDL_Color &LineEdit::placeHolderTextColor() const {
        return *_GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR, SDL_Color);
    }

    void LineEdit::setBackgroundColor(const SDL_Color &color, WidgetStatus status) {
        auto key = getBackgroundColorPropertyKey(status);
        auto _color = _GET_PROPERTY_PTR(this, key, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
    }

    void LineEdit::setBorderColor(const SDL_Color &color, WidgetStatus status) {
        auto key = getBorderColorPropertyKey(status);
        auto _color = _GET_PROPERTY_PTR(this, key, SDL_Color);
        _color->r = color.r; _color->g = color.g; _color->b = color.b; _color->a = color.a;
    }

    const SDL_Color &LineEdit::backgroundColor(WidgetStatus status) const {
        auto key = getBackgroundColorPropertyKey(status);
        return *_GET_PROPERTY_PTR(this, key, SDL_Color);
    }

    const SDL_Color &LineEdit::borderColor(WidgetStatus status) const {
        auto key = getBorderColorPropertyKey(status);
        return *_GET_PROPERTY_PTR(this, key, SDL_Color);
    }

    void LineEdit::setHorizontalPadding(int value) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, value);
        auto geo = toGeometryInt(_trigger_area.geometry());
        _real_area.x = geo.x + value;
        _real_area.width = geo.width - value * 2;
    }

    void LineEdit::setVerticalPadding(int value) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, value);
        auto geo = toGeometryInt(_trigger_area.geometry());
        _real_area.y = geo.y + value;
        _real_area.height = geo.height - value * 2;
    }

    void LineEdit::setPadding(int h, int v) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, h);
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, v);
        auto geo = toGeometryInt(_trigger_area.geometry());
        _real_area.setGeometry(geo.x + h, geo.y + v,
                               geo.width - h * 2, geo.height - v * 2);
    }

    void LineEdit::setPadding(int value) {
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, value);
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, value);
        auto geo = toGeometryInt(_trigger_area.geometry());
        int nx = geo.x + value, ny = geo.y + value,
            nw = geo.width - value * 2, nh = geo.height - value * 2;
        _real_area.setGeometry(nx, ny, nw, nh);
        Logger::log(std::format("ORI: {}, {}, {}, {}", geo.x, geo.y, geo.width, geo.height));
        Logger::log(std::format("REAL: {}, {}, {}, {}", nx, ny, nw, nh));
    }

    int LineEdit::horizontalPadding() const {
        return property(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL)->toInt32();
    }

    int LineEdit::verticalPadding() const {
        return property(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL)->toInt32();
    }

    void LineEdit::loadEvent() {
        AbstractWidget::loadEvent();
        auto status = WidgetStatus::Normal;
        if (isFocusEnabled()) status = WidgetStatus::Active;
        if (!enabled()) status = WidgetStatus::Disabled;
        updateStatus(status);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_COLOR_CHANGED;
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED;
        if (!_strings.empty()) _status |= ENGINE_BOOL_LINE_EDIT_HAS_TEXT;
    }

    void LineEdit::unloadEvent() {
        AbstractWidget::unloadEvent();
    }

    void LineEdit::focusInEvent() {
        if (!(_status & ENGINE_BOOL_LINE_EDIT_HAS_TEXT) && (_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE)) {
            _status ^= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        }
        updateStatus(WidgetStatus::Active);
    }

    void LineEdit::focusOutEvent() {
        if (!(_status & ENGINE_BOOL_LINE_EDIT_HAS_TEXT)) {
            _status |= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        }
        updateStatus(WidgetStatus::Normal);
    }

    void LineEdit::paintEvent(MyEngine::Renderer *renderer) {
        AbstractWidget::paintEvent(renderer);
        renderer->drawRectangle(&_trigger_area);

        if (_text) {
            bool show_text_mode = false, update_text_pos = false;
            if (!(_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE)) {
                show_text_mode = false;     // show text
            } else {
                show_text_mode = true;      // show placeholder text
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED) {
                auto text = Algorithm::mergeStringList(_strings);
                setProperty(ENGINE_PROP_LINE_EDIT_TEXT, text);
                TextSystem::global()->setText(_text_id, text);
                update_text_pos = true;
            }

            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_TEXT_SIZE_CHANGED) {
                TextSystem::global()->setFontSize(_text->font_name,
                                                  property(ENGINE_PROP_FONT_SIZE)->toFloat());
                update_text_pos = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED) {
                auto color = *_GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR, SDL_Color);
                TextSystem::global()->setTextColor(_text_id, color);
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_CHANGED) {
                auto text = property(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT)->toString();
                TextSystem::global()->setText(_place_text_id, text);
                update_text_pos = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_COLOR_CHANGED) {
                auto color = *_GET_PROPERTY_PTR(this, ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR, SDL_Color);
                TextSystem::global()->setTextColor(_place_text_id, color);
            }

            if (update_text_pos) {
                updateTextPosition();
            }

            renderer->setViewport(_real_area);
            if (show_text_mode) {
                TextSystem::global()->drawText(_place_text_id, _text_pos, renderer);
            } else {
                TextSystem::global()->drawText(_text_id, _text_pos, renderer);
            }
            renderer->setViewport({});

            static bool show_cur = false;
            if (!_start_tick) _start_tick = SDL_GetTicks();
            auto now_tick = SDL_GetTicks();
            if (now_tick - _start_tick >= 500) {
                show_cur = !show_cur;
                _start_tick = SDL_GetTicks();
            }
            if (_wid_status == WidgetStatus::Input && show_cur) renderer->drawLine(&_cursor_line);
        }
        _changer_signal = 0;
    }

    void LineEdit::enableChangedEvent(bool enabled) {
        AbstractWidget::enableChangedEvent(enabled);
        updateStatus(enabled ? (isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal)
                                    : WidgetStatus::Disabled);
    }

    void LineEdit::resizeEvent(const Size &size) {
        AbstractWidget::resizeEvent(size);
        setPadding(horizontalPadding(), verticalPadding());
    }

    void LineEdit::keyDownEvent(SDL_Scancode scancode) {
        AbstractWidget::keyDownEvent(scancode);
        if (!isInputModeEnabled()) return;
        switch (scancode) {
            case SDL_SCANCODE_RETURN:
            case SDL_SCANCODE_KP_ENTER:
                setInputModeEnabled(false);
                break;
            case SDL_SCANCODE_BACKSPACE:
                if (!_strings.empty()) {
                    _strings.pop_back();
                    _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED;
                }
                if (_status & ENGINE_BOOL_LINE_EDIT_HAS_TEXT) _status ^= ENGINE_BOOL_LINE_EDIT_HAS_TEXT;
                if (_strings.empty() && !placeHolderText().empty())
                    _status |= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
                break;
            default:
                break;
        }
    }

    void LineEdit::mouseEnteredEvent() {
        AbstractWidget::mouseEnteredEvent();
        if (!isInputModeEnabled())
            updateStatus(WidgetStatus::Hovered);
    }

    void LineEdit::mouseLeftEvent() {
        AbstractWidget::mouseLeftEvent();
        if (!isInputModeEnabled())
            updateStatus(isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal);
    }

    void LineEdit::mouseClickedEvent() {
        AbstractWidget::mouseClickedEvent();
        if (!isInputModeEnabled()) {
            setInputModeEnabled(true);
        }
    }

    void LineEdit::fingerTappedEvent() {
        AbstractWidget::fingerTappedEvent();
        if (!isInputModeEnabled()) {
            setInputModeEnabled(true);
        }
    }

    void LineEdit::startedInputEvent() {
        AbstractWidget::startedInputEvent();
        updateStatus(WidgetStatus::Input);
        _trigger_area.setBorder(4, _trigger_area.borderColor());
    }

    void LineEdit::endedInputEvent() {
        AbstractWidget::endedInputEvent();
        updateStatus(isHovered() ? WidgetStatus::Hovered :
                     (isFocusEnabled() ? WidgetStatus::Active : WidgetStatus::Normal));
        _trigger_area.setBorder(2, _trigger_area.borderColor());
    }

    void LineEdit::inputEvent(const char *string) {
        AbstractWidget::inputEvent(string);
        _strings.emplace_back(string);
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED;
        if (_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE)
            _status ^= ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE;
        _status |= ENGINE_BOOL_LINE_EDIT_HAS_TEXT;
        textChangedEvent();
    }

    void LineEdit::textChangedEvent() {

    }

    void LineEdit::init() {
        setProperty(ENGINE_PROP_FONT_NAME);
        setProperty(ENGINE_PROP_FONT_SIZE, 9.f);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_PATH);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_SELF);
        setProperty(ENGINE_PROP_BACKGROUND_IMAGE_TEXTURE);
        setProperty(ENGINE_PROP_LINE_EDIT_TEXT, "");
        setProperty(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT, "");
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, 0);
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, 0);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_NORMAL,
                                             SDL_Color, RGBAColor::BlueIce);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_NORMAL,
                                             SDL_Color, RGBAColor::MixGrayLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_HOVERED,
                                             SDL_Color, RGBAColor::BlueLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_HOVERED,
                                             SDL_Color, RGBAColor::BlueDark);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_ACTIVE,
                                             SDL_Color, RGBAColor::BlueIce);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_ACTIVE,
                                             SDL_Color, RGBAColor::BlueDeepSky);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_INPUT,
                                             SDL_Color, RGBAColor::White);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_INPUT,
                                             SDL_Color, RGBAColor::MixGrayDark);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_DISABLED,
                                             SDL_Color, RGBAColor::MixGrayLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_BORDER_COLOR_DISABLED,
                                             SDL_Color, RGBAColor::MixGrayLight);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR,
                                             SDL_Color, RGBAColor::MixGrayDark);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_PLACE_HOLDER_TEXT_COLOR,
                                             SDL_Color, RGBAColor::MixGray);
        _NEW_PROPERTY_WITH_DEFAULT_VALUE_PTR(this, ENGINE_PROP_LINE_EDIT_TEXT_COLOR,
                                             SDL_Color, RGBAColor::MixGrayDark);
        setCursor(Cursor::Edit);
        _trigger_area.setBorder(2, RGBAColor::MixGrayDark);
        _cursor_line.setSize(1);
        _cursor_line.setColor(StdColor::Black);
    }

    void LineEdit::updateStatus(WidgetStatus status) {
        _wid_status = status;

        auto bd_color = getBorderColor(status);
        auto bg_color = getBackgroundColor(status);

        _trigger_area.setBorderColor(bd_color);
        _trigger_area.setBackgroundColor(bg_color);

        updateTextPosition();
    }

    void LineEdit::updateTextPosition() {
        auto hp = horizontalPadding();
        float dis = _trigger_area.geometry().size.width - _text->text_size.width;
        Vector2 st_pos, ed_pos;
        st_pos.y += _real_area.y;
        // auto text_height = (_wid_status == WidgetStatus::Input ? _text->text_size.height : _place_text->text_size.height);
        float text_height = 0.f;
        if (_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE) {
            text_height = _place_text->text_size.height;
        } else {
            text_height = _text->text_size.height;
        }


        if (dis - hp * 2 < 0) {
            st_pos.x += _real_area.x + _real_area.width;
            ed_pos.reset(st_pos);
            _text_pos = {_wid_status == WidgetStatus::Input ? dis - hp * 2 : 0,
                         _real_area.height / 2.f - text_height / 2.f};
        } else {
            st_pos.x += _real_area.x + _text->text_size.width;
            ed_pos.reset(st_pos);
            _text_pos = {0, _real_area.height / 2.f - text_height / 2.f};
        }
        ed_pos.y += _real_area.height;

        _cursor_line.setStartPosition(st_pos);
        _cursor_line.setEndPosition(ed_pos);
    }

    std::string LineEdit::getBorderColorPropertyKey(WidgetStatus status) {
        switch (status) {
            case WidgetStatus::Active:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_ACTIVE;
            case WidgetStatus::Disabled:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_DISABLED;
            case WidgetStatus::Hovered:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_HOVERED;
            case WidgetStatus::Input:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_INPUT;
            default:
                return ENGINE_PROP_LINE_EDIT_BORDER_COLOR_NORMAL;
        }
    }

    std::string LineEdit::getBackgroundColorPropertyKey(WidgetStatus status) {
        switch (status) {
            case WidgetStatus::Active:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_ACTIVE;
            case WidgetStatus::Disabled:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_DISABLED;
            case WidgetStatus::Hovered:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_HOVERED;
            case WidgetStatus::Input:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_INPUT;
            default:
                return ENGINE_PROP_LINE_EDIT_BACKGROUND_COLOR_NORMAL;
        }
    }

    SDL_Color LineEdit::getBackgroundColor(WidgetStatus status) {
        auto key = getBackgroundColorPropertyKey(status);
        if (hasProperty(key)) {
            return *_GET_PROPERTY_PTR(this, key, SDL_Color);
        }
        return SDL_Color(0, 0, 0, 255);
    }

    SDL_Color LineEdit::getBorderColor(WidgetStatus status) {
        auto key = getBorderColorPropertyKey(status);
        if (hasProperty(key)) {
            return *_GET_PROPERTY_PTR(this, key, SDL_Color);
        }
        return SDL_Color(0, 0, 0, 255);
    }
}
