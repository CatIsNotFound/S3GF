#include "../../../include/MyEngine/MyEngine"

using namespace MyEngine;

class VSlider {
public:
    explicit VSlider(GeometryF geometry, Window* window)
           : _bar(Algorithm::drawFilledRoundedRectangleSurface((int)geometry.size.width, (int)geometry.size.height,
               (int)geometry.size.width / 2, StdColor::DarkBlue), window->renderer()),
             _slider(0, 0, geometry.size.width, geometry.size.width, 2,
                 StdColor::LightGray, StdColor::White), _geometry(geometry) {
        window->installPaintEvent([this](Renderer* r) {
            if (!_visible) return;
            _bar.draw();
            r->drawEllipse(&_slider);
        });
        EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [this] (SEvent e) {
            if (!_enabled || !_visible) return;
            auto mouse_cur = EventSystem::global()->captureMousePosition();
            bool is_on = Algorithm::comparePosInGeometry(mouse_cur, _bar.property()->geometry()) >= 0;
            if (EventSystem::global()->captureMouse(MouseStatus::Left)) {
                if (is_on && !_is_down) {
                    _is_down = true;
                }
                if (_is_down) {
                    mouse_cur -= _bar.property()->position();
                    if (mouse_cur.y <= _bar.property()->size().height && mouse_cur.y >= 0) {
                        _value = _min_value + (_max_value - _min_value) * (mouse_cur.y / _bar.property()->size().height);
                        updateSlider();
                    }
                }
            } else {
                if (_is_down) {
                    _is_down = false;
                    Cursor::global()->setCursor(Cursor::Busy);
                    if (_func) _func(_value);
                    Cursor::global()->setCursor(Cursor::Default);
                }
            }
        });
        _bar.property()->move(_geometry.pos);
        _slider.move(_geometry.pos.x + _slider.size().width / 2, _geometry.pos.y + _slider.size().height / 2);
        updateSlider();
    }

    void move(float x, float y) {
        _bar.property()->move(x, y);
        _slider.move(x + _slider.size().width / 2, _slider.centerPosition().y);
        _geometry.move(x, y);
        updateSlider();
    }

    void resize(float w, float h) {
        _bar.property()->resize(w, h);
        _slider.resize(w / 2, w / 2);
        _slider.move(_bar.property()->position().x + _slider.size().width / 2, _bar.property()->position().y);
        _geometry.resize(w, h);
        updateSlider();
    }

    void setMinValue(float value) {
        _min_value = value;
        updateSlider();
    }

    void setMaxValue(float value) {
        _max_value = value;
        updateSlider();
    }

    void setValue(float value, bool immediately = true) {
        _value = value;
        updateSlider();
        if (immediately && _func) _func(_value);
    }

    void setValue(float value, float min_value, float max_value, bool immediately = true) {
        _value = std::clamp(value, min_value, max_value);
        _min_value = min_value;
        _max_value = max_value;
        updateSlider();
        if (immediately && _func) _func(_value);
    }

    float minValue() const { return _min_value; }
    float maxValue() const { return _max_value; }
    float value() const { return _value; }
    void setVisible(bool visible) { _visible = visible; }
    bool visible() const { return _visible; }
    void setEnabled(bool enabled) {
        if (_enabled == enabled) return;
        _enabled = enabled;
        _bar.setImageFromSurface(Algorithm::drawFilledRoundedRectangleSurface((int)_geometry.size.width,
                (int)_geometry.size.height, (int)_geometry.size.width / 2,
                _enabled ? StdColor::DarkBlue : StdColor::LightGray));
        _bar.property()->move(_geometry.pos.x, _geometry.pos.y);
    }
    bool enabled() const { return _enabled; }
    void setValueChangedEvent(std::function<void(float)>&& event) { _func = std::move(event); }
    void clearEvent() { _func = {}; }
    VSlider() = delete;
    VSlider(const VSlider&) = delete;
    VSlider(VSlider&&) = delete;
    VSlider& operator=(const VSlider&) = delete;
    VSlider& operator=(VSlider&&) = delete;
private:
    void updateSlider() {
        float max_val = _bar.property()->size().height;
        _slider.move(_slider.centerPosition().x, _bar.property()->position().y + max_val * ((_value - _min_value) / (_max_value - _min_value)));
    }
    Texture _bar;
    Graphics::Ellipse _slider;
    GeometryF _geometry;
    std::function<void(float)> _func{};
    float _min_value{0.f};
    float _max_value{1.f};
    float _value{0.25f};
    bool _visible{true};
    bool _is_down{false};
    bool _enabled{true};
};

void updateTexture(Texture& texture, const Window* window) {
    texture.property()->resize(1280, 720);
    texture.property()->move(static_cast<float>(window->geometry().width / 2) -
                            texture.property()->size().width / 2, static_cast<float>(window->geometry().height / 2) -
                            texture.property()->size().height / 2);
}

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Image Viewer", 1366, 768);
    window->renderer()->setVSyncMode(Renderer::Enabled);
    Texture img(FileSystem::getAbsolutePath("./assets/view.jpg"), window->renderer());
    updateTexture(img, window);
    Texture ch_img(img);
    updateTexture(img, window);
    Graphics::Rectangle rect(15, 25, 615, 150, 0, {}, {255, 255, 255, 128});
    bool hide_img = false, show_welcome = true, show_help_info = false;
    float s_width = window->geometry().width / 2;
    VSlider slider1(GeometryF(16, 128, 16, 128), window);
    VSlider slider2(GeometryF(16, 288, 16, 128), window);
    VSlider slider3(GeometryF(16, 448, 16, 128), window);
    slider1.setEnabled(false);
    slider2.setEnabled(false);
    slider3.setEnabled(false);
    Timer timer(15000, [&show_welcome] { show_welcome = false; });
    timer.start();
    window->setDragDropEnabled(true);
    window->installPaintEvent([&] (Renderer* r) {
        img.draw();
        if (!hide_img) {
            s_width = Cursor::global()->position().x;
            r->setClipView(Geometry(0, 0, s_width, window->geometry().height));
            ch_img.draw();
            r->setClipView({});
        }
        if (!show_help_info && show_welcome) r->drawDebugText("Press any number keys to change effect! Type F1 key for more help! ", {20, 10});
        if (show_help_info) {
            r->setBlendMode(SDL_BLENDMODE_BLEND);
            r->drawRectangle(&rect);
            r->drawDebugText("Press F1 to show/hide help info\n", {20, 20});
            r->drawDebugText("Press Esc to quit the application\n", {20, 30});
            r->drawDebugText("Use mouse to dropped the local picture (which you like) from your system. ", {20, 40});
            r->drawDebugText("--- key - function ---------------------------------------------------------\n", {20, 50});
            r->drawDebugText("      1   Generate inverted color image\n", {20, 60});
            r->drawDebugText("      2   Generate gray images of different levels (need pressed repeatedly)", {20, 70});
            r->drawDebugText("      3   Generate brightness image", {20, 80});
            r->drawDebugText("      4   Generate contrast image (Can be adjusted using the slider)", {20, 90});
            r->drawDebugText("      5   Generate gamma correction image (Can be adjusted using the slider)", {20, 100});
            r->drawDebugText("      6   Generate saturation image (Can be adjusted using the slider)", {20, 110});
            r->drawDebugText("      7   Generate solarize image (Can be adjusted using the slider)", {20, 120});
            r->drawDebugText("      8   Generate posterize image (Can be adjusted using the slider)", {20, 130});
            r->drawDebugText("      9   Generate color balance image (Can be adjusted using the slider)", {20, 140});
            r->drawDebugText("      0   Show/hide the generated image", {20, 150});
            r->setBlendMode(SDL_BLENDMODE_NONE);
        }
    });
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&]
                (SEvent e) {
        static size_t gray_mode = 0;
        if (e.type == SDL_EVENT_KEY_DOWN) {
            Cursor::global()->setCursor(Cursor::Busy);
            switch (e.key.scancode) {
                case SDL_SCANCODE_ESCAPE:
                    Engine::exit();
                    break;
                case SDL_SCANCODE_1:
                    ch_img.setImageFromSurface(Algorithm::applyInvertedColorSurface(img.surface()));
                    slider1.setEnabled(false);
                    slider2.setEnabled(false);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_2:
                    gray_mode += 1;
                    switch (gray_mode % 4) {
                    case 0:
                        ch_img.setImageFromSurface(Algorithm::applyAvgGraySurface(img.surface()));
                        break;
                    case 1:
                        ch_img.setImageFromSurface(Algorithm::applyDarkGraySurface(img.surface()));
                        break;
                    case 2:
                        ch_img.setImageFromSurface(Algorithm::applyLightGraySurface(img.surface()));
                        break;
                    case 3:
                        ch_img.setImageFromSurface(Algorithm::applySepiaToneSurface(img.surface()));
                    default:
                        break;
                    }
                    slider1.setEnabled(false);
                    slider2.setEnabled(false);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_3:
                    slider1.setEnabled(true);
                    slider1.setValueChangedEvent([&img, &ch_img, &window] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applyBrightnessSurface(img.surface(), value));
                        updateTexture(ch_img, window);
                    });
                    slider1.setValue(0, -255, 255);
                    slider2.setEnabled(false);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_4:
                    slider1.setEnabled(true);
                    slider1.setValueChangedEvent([&img, &ch_img, &window] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applyContrastSurface(img.surface(), value));
                        updateTexture(ch_img, window);
                    });
                    slider1.setValue(1.f, 0.f, 3.f);
                    slider2.setEnabled(false);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_5:
                    slider1.setEnabled(true);
                    slider1.setValueChangedEvent([&img, &ch_img, &window] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applyGammaCorrectionSurface(img.surface(), value));
                        updateTexture(ch_img, window);
                    });
                    slider1.setValue(1.f, 0.1f, 5.f);
                    slider2.setEnabled(false);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_6:
                    slider1.setEnabled(true);
                    slider1.setValueChangedEvent([&img, &ch_img, &window] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applySaturationSurface(img.surface(), value));
                        updateTexture(ch_img, window);
                    });
                    slider1.setValue(1.f, 0.f, 3.f);
                    slider2.setEnabled(false);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_7:
                    slider1.setEnabled(true);
                    slider1.setValueChangedEvent([&img, &ch_img, &window, &slider2] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applySolarizeSurface(img.surface(), value, slider2.value()));
                        updateTexture(ch_img, window);
                    });
                    slider1.setMinValue(0.f);
                    slider1.setMaxValue(255.f);
                    slider1.setValue(255.f, false);
                    slider2.setEnabled(true);
                    slider2.setValueChangedEvent([&img, &ch_img, &window, &slider1] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applySolarizeSurface(img.surface(), slider1.value(), value));
                        updateTexture(ch_img, window);
                    });
                    slider2.setValue(0.f);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_8:
                    slider1.setEnabled(true);
                    slider1.setValueChangedEvent([&img, &ch_img, &window] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applyPosterizeSurface(img.surface(), static_cast<int>(value)));
                        updateTexture(ch_img, window);
                    });
                    slider1.setValue(2.f, 2.f, 256.f);
                    slider2.setEnabled(false);
                    slider3.setEnabled(false);
                    break;
                case SDL_SCANCODE_9:
                    slider1.setEnabled(true);
                    slider1.setValueChangedEvent([&img, &ch_img, &window, &slider2, &slider3] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applyColorBalanceSurface(img.surface(),
                            static_cast<int>(value), static_cast<int>(slider2.value()),
                            static_cast<int>(slider3.value())));
                        updateTexture(ch_img, window);
                    });
                    slider1.setValue(0.f, -255.f, 255.f, false);
                    slider2.setEnabled(true);
                    slider2.setValueChangedEvent([&img, &ch_img, &window, &slider1, &slider3] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applyColorBalanceSurface(img.surface(),
                            static_cast<int>(slider1.value()), static_cast<int>(value),
                            static_cast<int>(slider3.value())));
                        updateTexture(ch_img, window);
                    });
                    slider2.setValue(0.f, -255.f, 255.f, false);
                    slider3.setEnabled(true);
                    slider3.setValueChangedEvent([&img, &ch_img, &window, &slider1, &slider2] (float value) {
                        ch_img.setImageFromSurface(Algorithm::applyColorBalanceSurface(img.surface(),
                            static_cast<int>(slider1.value()), static_cast<int>(slider2.value()),
                            static_cast<int>(value)));
                        updateTexture(ch_img, window);
                    });
                    slider3.setValue(0.f, -255.f, 255.f, false);
                    break;
                case SDL_SCANCODE_0:
                    hide_img = !hide_img;
                    break;
                case SDL_SCANCODE_F1:
                    show_help_info = !show_help_info;
                    break;
                default:
                    break;
            }
            updateTexture(ch_img, window);
            Cursor::global()->setCursor(Cursor::Default);
        } else if (e.type == SDL_EVENT_DROP_FILE) {
            auto url = window->droppedInfo();
            Logger::log(Logger::Info, "Loaded image file: {}", url);
            if (!img.setImagePath(url.data())) {
                Logger::log(Logger::Error, "Failed to load image file: {}", url);
                return;
            }
            ch_img.setImagePath(url.data());
            slider1.setEnabled(false);
            slider2.setEnabled(false);
            slider3.setEnabled(false);
            updateTexture(img, window);
            updateTexture(ch_img, window);
        }
    });
    window->show();
    return engine.exec();
}
