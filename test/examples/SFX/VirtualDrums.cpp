#include "MyEngine"

using namespace MyEngine;

class PadKey : public TriggerArea {
public:
    explicit PadKey(const GeometryF& geometry, Window* window)
        : TriggerArea(geometry, window), _rect(geometry, 0, {}, RGBAColor::BlueBaby) {
        window->installPaintEvent([&](Renderer* r) {
            r->drawRectangle(&_rect);
        });
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
    SFX* _sfx{};
};

int main() {
    Engine engine("Virtual Drums", "v1.0.0", "org.virtualDrums.app");
    auto window = new Window(&engine, engine.applicationName().data());
    window->show();
    AudioSystem::global()->appendSFX("bell2", FileSystem::getAbsolutePath("./assets/Samples/bell2.wav"));
    AudioSystem::global()->appendSFX("kik", FileSystem::getAbsolutePath("./assets/Samples/kik.wav"));
    AudioSystem::global()->appendSFX("open1", FileSystem::getAbsolutePath("./assets/Samples/open1.wav"));
    AudioSystem::global()->appendSFX("snare1", FileSystem::getAbsolutePath("./assets/Samples/snare1.wav"));
    AudioSystem::global()->appendSFX("splash1", FileSystem::getAbsolutePath("./assets/Samples/splash1.wav"));
    AudioSystem::global()->appendSFX("tom1", FileSystem::getAbsolutePath("./assets/Samples/tom1.wav"));
    PadKey key1(GeometryF(100.f, 100.f, 100.f, 100.f), window);
    PadKey key2(GeometryF(220.f, 100.f, 100.f, 100.f), window);
    PadKey key3(GeometryF(340.f, 100.f, 100.f, 100.f), window);
    PadKey key4(GeometryF(100.f, 220.f, 100.f, 100.f), window);
    PadKey key5(GeometryF(220.f, 220.f, 100.f, 100.f), window);
    PadKey key6(GeometryF(340.f, 220.f, 100.f, 100.f), window);
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
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_F1)) show_debug = !show_debug;
    });
    window->installPaintEvent([&playing_sum, &sum, &show_debug, &window](Renderer* r) {
        if (!show_debug) return;
        static Graphics::Rectangle back(GeometryF(0.f, 0.f, (float)window->geometry().width, (float)window->geometry().height),
            0, {}, RGBAColor::HalfTransparent);
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        r->drawRectangle(&back);
        r->setBlendMode(SDL_BLENDMODE_NONE);
        r->drawDebugFPS();
        r->drawDebugText(FMT::format("Playing count: {} / {}", playing_sum, sum), {20, 30});
    }, true);
    return engine.exec();
}