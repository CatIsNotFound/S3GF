#include "../../../include/MyEngine/MyEngine"

using namespace MyEngine;

class MyWindow : public Window {
    public:
        explicit MyWindow(Engine* engine, const std::string& title, int w, int h)
                : Window(engine, title, w, h) {}

        void setViewer(AudioDecibelMeter* viewer) {
            _viewer = viewer;
        }

    protected:
        void dropEvent(const char *url) override {
            auto bgm = AudioSystem::global()->getBGM("main");
            if (!bgm) return;
            bgm->setPath(url);
            if (_viewer) _viewer->viewBGM(bgm);
            bgm->play(0, true);
        }

    private:
        AudioDecibelMeter* _viewer{};
};

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new MyWindow(&engine, "Audio test", 800, 300);
    AudioSystem::global()->appendBGM("main", FileSystem::getAbsolutePath("./assets/Peace.wav"));
    auto bgm = AudioSystem::global()->getBGM("main");
    Graphics::Rectangle rect_L(100, 100, 0, 20, 0, {}, StdColor::Green);
    Graphics::Rectangle rect_R(100, 120, 0, 20, 0, {}, StdColor::LightGreen);
    Graphics::Rectangle rect_C(100, 100, 240, 40, 0, {}, RGBAColor::BlueLightTrans);
    Graphics::Rectangle rect_peak_L(100, 100, 0, 20, 0, {}, {0, 255, 0, 143});
    Graphics::Rectangle rect_peak_R(100, 120, 0, 20, 0, {}, {144, 238, 144, 143});
    Graphics::Rectangle prg_rect(0, 0, 0, 48, 0, {}, StdColor::LightBlue);
    AudioDecibelMeter mixer_level_viewer(AudioSystem::global()->mixer());
    AudioDecibelMeter track_level_viewer(bgm);
    window->setViewer(&track_level_viewer);
    bgm->play(0, true);
    window->installPaintEvent([&](Renderer* r) {
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        float L = std::clamp(track_level_viewer.leftDecibel(), -40.f, 8.f);
        float R = std::clamp(track_level_viewer.rightDecibel(), -40.f, 8.f);
        float mL = std::clamp(track_level_viewer.leftPeakDecibel(), -40.f, 8.f);
        float mR = std::clamp(track_level_viewer.rightPeakDecibel(), -40.f, 8.f);
        float new_L = std::clamp((L + 40.f) * 5.f, 0.5f, 240.f);
        float new_R = std::clamp((R  + 40.f) * 5.f, 0.5f, 240.f);
        float new_ML = std::clamp((mL + 40.f) * 5.f, 0.5f, 240.f);
        float new_MR = std::clamp((mR + 40.f) * 5.f, 0.5f, 240.f);
        rect_L.resize(new_L, 20.f);
        rect_R.resize(new_R, 20.f);
        rect_peak_L.resize(new_ML, 20.f);
        rect_peak_R.resize(new_MR, 20.f);
        static size_t delay_L = 0, delay_R = 0;
        static bool red_L = false, red_R = false;
        if (track_level_viewer.leftPeakDecibel() >= 0) {
            rect_L.setBackgroundColor(RGBAColor::RedRose);
            rect_peak_L.setBackgroundColor(RGBAColor::RedRoseTrans);
            delay_L = SDL_GetTicks();
            red_L = true;
        } else if (track_level_viewer.leftPeakDecibel() >= -5.f && !red_L) {
            rect_L.setBackgroundColor(StdColor::Orange);
            rect_peak_L.setBackgroundColor({255, 165, 0, 192});
            delay_L = SDL_GetTicks();
        } else if (SDL_GetTicks() - delay_L > 1000) {
            rect_L.setBackgroundColor(StdColor::Green);
            rect_peak_L.setBackgroundColor({0, 255, 0, 143});
            delay_L = SDL_GetTicks();
            red_L = false;
        }
        if (track_level_viewer.rightPeakDecibel() >= 0) {
            rect_R.setBackgroundColor(RGBAColor::RedLightPink);
            rect_peak_R.setBackgroundColor(RGBAColor::RedLightTrans);
            delay_R = SDL_GetTicks();
            red_R = true;
        } else if (track_level_viewer.rightPeakDecibel() >= -5.f && !red_R) {
            rect_R.setBackgroundColor(RGBAColor::MixYellow);
            rect_peak_R.setBackgroundColor(RGBAColor::MixYellowTrans);
            delay_R = SDL_GetTicks();
        } else if (SDL_GetTicks() - delay_R > 1000) {
            rect_R.setBackgroundColor(StdColor::LightGreen);
            rect_peak_R.setBackgroundColor({144, 238, 144, 143});
            delay_R = SDL_GetTicks();
            red_R = false;
        }
        float playing_status = static_cast<float>(bgm->position()) / static_cast<float>(bgm->duration());
        prg_rect.resize(playing_status * static_cast<float>(window->geometry().width), 16);
        r->drawRectangle(&rect_C);
        r->drawRectangle(&rect_peak_L);
        r->drawRectangle(&rect_peak_R);
        r->drawRectangle(&rect_L);
        r->drawRectangle(&rect_R);
        r->drawRectangle(&prg_rect);
        r->drawDebugText(FMT::format("Playing: {}", bgm->path()), {20, 20});
        r->drawDebugText(FMT::format("Status: {} / {}, Vol: {:.0f}%", bgm->position(), bgm->duration(),
            roundf(bgm->volume() * 100.f)), {20, 30});
        r->drawDebugText(FMT::format("[M] mix: {:.2f} dB, L: {:.2f} dB, R: {:.2f} dB",
            mixer_level_viewer.mixDecibel(), mixer_level_viewer.leftDecibel(), mixer_level_viewer.rightDecibel()), {20, 40});
        r->drawDebugText(FMT::format("[P] mix: {:.2f} dB, L: {:.2f} dB, R: {:.2f} dB",
            mixer_level_viewer.mixPeakDecibel(), mixer_level_viewer.leftPeakDecibel(),
            mixer_level_viewer.rightPeakDecibel()), {20, 50});

    });

    EventSystem::global()->appendEvent([&](SDL_Event e) {
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_SPACE)) {
            if (bgm->playStatus() != BGM::Playing) {
                bgm->resume();
                prg_rect.setBackgroundColor(StdColor::LightBlue);
            } else {
                bgm->pause();
                prg_rect.setBackgroundColor(StdColor::DarkBlue);
            }
        }
        if (e.type == SDL_EVENT_KEY_DOWN) {
            if (e.key.key == SDLK_UP) bgm->setVolume(bgm->volume() + 0.1f);
            if (e.key.key == SDLK_DOWN) bgm->setVolume(bgm->volume() - 0.1f);
        }
    });
    window->setDragDropEnabled(true);
    window->show();
    return engine.exec();
}
