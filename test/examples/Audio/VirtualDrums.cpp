#include "MyEngine"

using namespace MyEngine;

class PadKey : public TriggerArea {
public:
    explicit PadKey(const GeometryF& geometry, Window* window, const char* key = "?" ) : TriggerArea(geometry, window),
             _rect(geometry, 0, {}, RGBAColor::BlueBaby), _key(key) {
        if (!TextSystem::global()->isFontContain("default")) {
            TextSystem::global()->addFont("default",
                    FontDatabase::getSystemDefaultFont().begin()->font_path, window->renderer(), 24.f);
        }
        auto id = IDGenerator::getNewTextID();
        TextSystem::global()->addText(id, "default", _key);
        _font = TextSystem::global()->font("default");
        _text = TextSystem::global()->indexOfText(id);
        window->installPaintEvent([&](Renderer* r) {
            r->drawRectangle(&_rect);
            r->drawText(_text->self, _text_pos);
        });
        _text_pos = Vector2(geometry.pos.x + (geometry.size.width / 2 - _text->text_size.width / 2),
                        geometry.pos.y + (geometry.size.height / 2 - _text->text_size.height / 2));
    }

    void setSFX(SFX* sfx) { _sfx = sfx; }
    SFX* sfx() { return _sfx; }
protected:
    void mouseDownEvent(MouseStatus button) override {
        if (_sfx) _sfx->play();
        _rect.setBackgroundColor(RGBAColor::BlueDark);
    }
    void mouseUpEvent(MouseStatus button) override {
        _rect.setBackgroundColor(RGBAColor::BlueBaby);
    }
    void mouseMovedOutEvent() override {
        _rect.setBackgroundColor(RGBAColor::BlueBaby);
    }
    void mouseMovedInEvent() override {
        _rect.setBackgroundColor(RGBAColor::BlueDark);
        if (_sfx) _sfx->play();
    }
    void keyDownEvent(SDL_Scancode keycode) override {
        if (triggerKey() == keycode) {
            _rect.setBackgroundColor(RGBAColor::BlueDark);
            if (_sfx) _sfx->play();
        }
    }
    void keyUpEvent(SDL_Scancode keycode) override {
        _rect.setBackgroundColor(RGBAColor::BlueBaby);
    }
    void fingerDownEvent(SDL_FingerID id) override {
        _rect.setBackgroundColor(RGBAColor::BlueDark);
        if (_sfx) _sfx->play();
    }
    void fingerMovedInEvent() override {
        _rect.setBackgroundColor(RGBAColor::BlueDark);
        if (_sfx) _sfx->play();
    }
    void fingerMovedOutEvent() override {
        _rect.setBackgroundColor(RGBAColor::BlueBaby);
    }
    void fingerUpEvent(SDL_FingerID id) override {
        _rect.setBackgroundColor(RGBAColor::BlueBaby);
    }

private:
    Graphics::Rectangle _rect;
    TextSystem::Text* _text;
    Font* _font;
    SFX* _sfx{};
    Vector2 _text_pos{};
    std::string _key{};
};

int main() {
    Engine engine("Virtual Drums", "v1.0.0", "org.virtualDrums.app");
    auto window = new Window(&engine, engine.applicationName().data());
    window->renderer()->setVSyncMode(Renderer::Enabled);
    window->show();
    AudioSystem::global()->appendSFX("bell2", FileSystem::getAbsolutePath("./assets/Samples/bell2.wav"));
    AudioSystem::global()->appendSFX("kik", FileSystem::getAbsolutePath("./assets/Samples/kik.wav"));
    AudioSystem::global()->appendSFX("open1", FileSystem::getAbsolutePath("./assets/Samples/open1.wav"));
    AudioSystem::global()->appendSFX("snare1", FileSystem::getAbsolutePath("./assets/Samples/snare1.wav"));
    AudioSystem::global()->appendSFX("splash1", FileSystem::getAbsolutePath("./assets/Samples/splash1.wav"));
    AudioSystem::global()->appendSFX("tom1", FileSystem::getAbsolutePath("./assets/Samples/tom1.wav"));
    PadKey key1(GeometryF(100.f, 100.f, 100.f, 100.f), window, "Q");
    PadKey key2(GeometryF(220.f, 100.f, 100.f, 100.f), window, "W");
    PadKey key3(GeometryF(340.f, 100.f, 100.f, 100.f), window, "E");
    PadKey key4(GeometryF(100.f, 220.f, 100.f, 100.f), window, "A");
    PadKey key5(GeometryF(220.f, 220.f, 100.f, 100.f), window, "S");
    PadKey key6(GeometryF(340.f, 220.f, 100.f, 100.f), window, "D");
    std::array<AudioDecibelMeter, 6> meters;
    meters[0].viewSFX(AudioSystem::global()->getSFX("bell2"));
    meters[1].viewSFX(AudioSystem::global()->getSFX("open1"));
    meters[2].viewSFX(AudioSystem::global()->getSFX("splash1"));
    meters[3].viewSFX(AudioSystem::global()->getSFX("snare1"));
    meters[4].viewSFX(AudioSystem::global()->getSFX("kik"));
    meters[5].viewSFX(AudioSystem::global()->getSFX("tom1"));
    struct Meter {
        float peakL{};
        float peakR{};
        float L{};
        float R{};
    };
    Meter keys_mixer;
    key1.setSFX(AudioSystem::global()->getSFX("bell2"));
    key2.setSFX(AudioSystem::global()->getSFX("open1"));
    key3.setSFX(AudioSystem::global()->getSFX("splash1"));
    key4.setSFX(AudioSystem::global()->getSFX("snare1"));
    key5.setSFX(AudioSystem::global()->getSFX("kik"));
    key6.setSFX(AudioSystem::global()->getSFX("tom1"));
    key1.setTriggerKey(SDL_SCANCODE_Q);
    key2.setTriggerKey(SDL_SCANCODE_W);
    key3.setTriggerKey(SDL_SCANCODE_E);
    key4.setTriggerKey(SDL_SCANCODE_A);
    key5.setTriggerKey(SDL_SCANCODE_S);
    key6.setTriggerKey(SDL_SCANCODE_D);

    AudioDecibelMeter mixer_meter(AudioSystem::global()->mixer());
    size_t playing_sum = 0, sum = 0;
    bool show_debug = false;
    EventSystem::global()->appendGlobalEvent(IDGenerator::getNewGlobalEventID(), [&] {
        playing_sum = 0;
        sum = 0;
        playing_sum += key1.sfx()->playingCount();
        playing_sum += key2.sfx()->playingCount();
        playing_sum += key3.sfx()->playingCount();
        playing_sum += key4.sfx()->playingCount();
        playing_sum += key5.sfx()->playingCount();
        playing_sum += key6.sfx()->playingCount();
        sum += key1.sfx()->count();
        sum += key2.sfx()->count();
        sum += key3.sfx()->count();
        sum += key4.sfx()->count();
        sum += key5.sfx()->count();
        sum += key6.sfx()->count();
        auto muted = AudioDecibelMeter::MUTED_DB;
        keys_mixer = {muted, muted, muted, muted};
        for (auto& m : meters) {
            keys_mixer.L = std::max(m.leftDecibel(), keys_mixer.L);
            keys_mixer.R = std::max(m.rightDecibel(), keys_mixer.R);
            keys_mixer.peakL = std::max(m.leftPeakDecibel(), keys_mixer.peakL);
            keys_mixer.peakR = std::max(m.rightPeakDecibel(), keys_mixer.peakR);
        }
        static size_t timer_delay = SDL_GetTicks();
        if (SDL_GetTicks() - timer_delay > 1000) {
            meters[0].viewSFX(AudioSystem::global()->getSFX("bell2"));
            meters[1].viewSFX(AudioSystem::global()->getSFX("open1"));
            meters[2].viewSFX(AudioSystem::global()->getSFX("splash1"));
            meters[3].viewSFX(AudioSystem::global()->getSFX("snare1"));
            meters[4].viewSFX(AudioSystem::global()->getSFX("kik"));
            meters[5].viewSFX(AudioSystem::global()->getSFX("tom1"));
            timer_delay = SDL_GetTicks();
        }
    });
    window->installPaintEvent([&](Renderer* r) {
        if (!show_debug) return;
        static Graphics::Rectangle back(GeometryF(0.f, 0.f,
            (float)window->geometry().width, (float)window->geometry().height),
            0, {}, RGBAColor::HalfTransparent);
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        r->drawRectangle(&back);
        r->setBlendMode(SDL_BLENDMODE_NONE);
        r->drawDebugFPS();
        r->drawDebugText(FMT::format("Playing count: {} / {}", playing_sum, sum), {20, 30});
        r->drawDebugText(FMT::format("[Keys] L: {:.2f} dB, R: {:.2f} dB, peakL: {:.2f} dB, peakR: {:.2f} dB",
            keys_mixer.L, keys_mixer.R, keys_mixer.peakL, keys_mixer.peakR), {20, 40});
        r->drawDebugText(FMT::format("[Mixer] L: {:.2f} dB, R: {:.2f} dB, peakL: {:.2f} dB, peakR: {:.2f} dB",
            mixer_meter.leftDecibel(), mixer_meter.rightDecibel(),
            mixer_meter.leftPeakDecibel(), mixer_meter.rightPeakDecibel()), {20, 50});
    }, true);

    EventSystem::global()->appendEvent([&show_debug](SDL_Event e) {
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_F1)) show_debug = !show_debug;
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_P)) AudioSystem::global()->stopAll();
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_ESCAPE)) Engine::exit();
    });
    return engine.exec();
}