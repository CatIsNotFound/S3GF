
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

    std::string_view LineEdit::fontName() const {
        return _text ? _text->font_name.data() : nullptr;
    }

    std::string_view LineEdit::fontPath() const {
        auto font = fontName();
        return _text ? TextSystem::global()->font(font.data())->fontPath().data() : nullptr;
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
        if (_status & ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE) {
            setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD_LENGTH, _strings.size());
            setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD,
                        Algorithm::multiplicationString(_secret_char, _strings.size()));
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
            setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD_LENGTH, _strings.size());
            setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD,
                        Algorithm::multiplicationString(_secret_char, _strings.size()));
        } else {
            setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD_LENGTH, 0);
            _status ^= ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE;
        }
        _changer_signal |= ENGINE_SIGNAL_LINE_EDIT_PASSWORD_CHANGED;
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

    void LineEdit::setBackgroundVisible(bool visible) {
        if (!visible && (_status & ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE)) {
            _status ^= ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE;
        } else {
            _status |= ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE;
        }
        updateStatus(_wid_status);
    }

    void LineEdit::setBorderVisible(bool visible) {
        if (!visible && (_status & ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE)) {
            _status ^= ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE;
        } else {
            _status |= ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE;
        }
        updateStatus(_wid_status);
    }

    bool LineEdit::backgroundVisible() const {
        return (_status & ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE);
    }

    bool LineEdit::borderVisible() const {
        return (_status & ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE);
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
    }

    int LineEdit::horizontalPadding() const {
        return property(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL)->toInt32();
    }

    int LineEdit::verticalPadding() const {
        return property(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL)->toInt32();
    }

    void LineEdit::setTextCursorStyle(uint8_t size, const SDL_Color &color) {
        _cursor_line.setSize(size);
        _cursor_line.setColor(color);
    }

    void LineEdit::setTextCursorStyle(uint8_t size, uint64_t hex_code, bool alpha) {
        _cursor_line.setSize(size);
        _cursor_line.setColor(RGBAColor::hexCode2RGBA(hex_code, alpha));
    }

    uint8_t LineEdit::textCursorSize() const {
        return _cursor_line.size();
    }

    const SDL_Color &LineEdit::textCursorColor() const {
        return _cursor_line.color();
    }

    void LineEdit::loadEvent() {
        AbstractWidget::loadEvent();
        auto status = WidgetStatus::Normal;
        if (isFocusEnabled()) status = WidgetStatus::Active;
        if (!enabled()) status = WidgetStatus::Disabled;
        updateStatus(status);
        _changer_signal |= (ENGINE_SIGNAL_LINE_EDIT_PLACEHOLDER_TEXT_COLOR_CHANGED |
                            ENGINE_SIGNAL_LINE_EDIT_TEXT_COLOR_CHANGED);

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
        if (!(_status & ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE)) {
            _trigger_area.setBackgroundColor(RGBAColor::Transparent);
        }
        if (!(_status & ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE)) {
            _trigger_area.setBorderColor(RGBAColor::Transparent);
        }
        renderer->drawRectangle(&_trigger_area);

        if (_text) {
            bool show_text_mode = false, update_text_pos = false;
            if (!(_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE)) {
                show_text_mode = false;     // show text
            } else {
                show_text_mode = true;      // show placeholder text
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_TEXT_CHANGED) {
                updateText();
                update_text_pos = true;
            }
            if (_changer_signal & ENGINE_SIGNAL_LINE_EDIT_PASSWORD_CHANGED) {
                updateText();
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

            bool show_cur = (_status & ENGINE_BOOL_LINE_EDIT_CURSOR_VISIBLE) > 0;
            if (!_start_tick) _start_tick = SDL_GetTicks();
            auto now_tick = SDL_GetTicks();
            if (now_tick - _start_tick >= 500) {
                show_cur = !show_cur;
                _status ^= ENGINE_BOOL_LINE_EDIT_CURSOR_VISIBLE;
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

    void LineEdit::keyPressedEvent(SDL_Scancode scancode) {
        if ((scancode == SDL_SCANCODE_RETURN ||
            scancode == SDL_SCANCODE_KP_ENTER) && !isInputModeEnabled()) {
            setInputModeEnabled(true);
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
        setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD, "");
        setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD_LENGTH, 0ULL);
        setProperty(ENGINE_PROP_LINE_EDIT_PLACEHOLDER_TEXT, "");
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_HORIZONTAL, 4.f);
        setProperty(ENGINE_PROP_LINE_EDIT_PADDING_VERTICAL, 4.f);
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
        setCursor(Cursor::Edit);
        _trigger_area.setBorder(2, RGBAColor::MixGrayDark);
        _cursor_line.setSize(1);
        _cursor_line.setColor(StdColor::Black);
        _status |= (ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE |
                    ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE);
    }

    void LineEdit::updateStatus(WidgetStatus status) {
        _wid_status = status;

        if (_status & ENGINE_BOOL_LINE_EDIT_BORDER_VISIBLE) {
            auto bd_color = getBorderColor(status);
            _trigger_area.setBorderColor(bd_color);
        }
        if (_status & ENGINE_BOOL_LINE_EDIT_BACKGROUND_VISIBLE) {
            auto bg_color = getBackgroundColor(status);
            _trigger_area.setBackgroundColor(bg_color);
        }
        updateTextPosition();
    }

    void LineEdit::updateTextPosition() {
        auto hp = (float)horizontalPadding();
        float dis = _trigger_area.geometry().size.width - _text->text_size.width;
        Vector2 st_pos, ed_pos;
        auto real_area = toGeometryFloat(_real_area);
        st_pos.y += real_area.pos.y;
        float text_height = 0.f;
        if (_status & ENGINE_BOOL_LINE_EDIT_PLACEHOLDER_TEXT_VISIBLE) {
            text_height = _place_text->text_size.height;
        } else {
            text_height = _text->text_size.height;
        }


        if (dis - hp * 2 < 0) {
            st_pos.x += real_area.pos.x + real_area.size.width;
            ed_pos.reset(st_pos);
            _text_pos = {_wid_status == WidgetStatus::Input ? dis - hp * 2 : 0,
                         real_area.size.height / 2.f - text_height / 2.f};
        } else {
            st_pos.x += real_area.pos.x + _text->text_size.width;
            ed_pos.reset(st_pos);
            _text_pos = {0, real_area.size.height / 2.f - text_height / 2.f};
        }
        ed_pos.y += real_area.size.height;

        _cursor_line.setStartPosition(st_pos);
        _cursor_line.setEndPosition(ed_pos);
    }

    void LineEdit::updateText() {
        auto text = Algorithm::mergeStringList(_strings);
        setProperty(ENGINE_PROP_LINE_EDIT_TEXT, text);
        if (_status & ENGINE_BOOL_LINE_EDIT_PASSWORD_MODE) {
            auto passwd = property(ENGINE_PROP_LINE_EDIT_PASSWORD)->toString();
            auto pass_length = property(ENGINE_PROP_LINE_EDIT_PASSWORD_LENGTH)->toUInt64();
            if (pass_length < _strings.size()) {
                passwd.append(Algorithm::multiplicationString
                                      (_secret_char, _strings.size() - pass_length));
            } else {
                passwd.assign(passwd.substr(0, _strings.size() * strlen(_secret_char)));
            }
            TextSystem::global()->setText(_text_id, passwd);
            setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD, passwd);
            setProperty(ENGINE_PROP_LINE_EDIT_PASSWORD_LENGTH, _strings.size());
        } else {
            TextSystem::global()->setText(_text_id, text);
        }
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
