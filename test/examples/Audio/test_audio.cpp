#include "MyEngine"

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
    auto window = new MyWindow(&engine, "Audio test", 800, 600);
    AudioSystem::global()->appendBGM("main", FileSystem::getAbsolutePath("./assets/Peace.wav"));
    auto bgm = AudioSystem::global()->getBGM("main");
    Graphics::Rectangle rect_L(100, 100, 0, 20, 0, {}, StdColor::Green);
    Graphics::Rectangle rect_R(100, 120, 0, 20, 0, {}, StdColor::LightGreen);
    Graphics::Rectangle rect_C(100, 100, 240, 40, 0, {}, StdColor::LightGray);
    Graphics::Rectangle rect_peak_L(100, 100, 0, 20, 0, {}, {0, 255, 0, 128});
    Graphics::Rectangle rect_peak_R(100, 120, 0, 20, 0, {}, {144, 238, 144, 128});
    Graphics::Line line_L(100, 100, 100, 120, 2, StdColor::DarkGreen);
    Graphics::Line line_R(100, 120, 100, 140, 2, StdColor::DarkGreen);
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
        r->drawRectangle(&rect_C);
        r->drawRectangle(&rect_peak_L);
        r->drawRectangle(&rect_peak_R);
        r->drawRectangle(&rect_L);
        r->drawRectangle(&rect_R);
        r->drawDebugText(FMT::format("Playing: {}", bgm->path()), {20, 20});
        r->drawDebugText(FMT::format("Status: {}, Vol: {:.0f}%", bgm->position(),
            roundf(bgm->volume() * 100.f)), {20, 30});
        r->drawDebugText(FMT::format("[master] main: {:.2f} dB, L: {:.2f} dB, R: {:.2f} dB",
            mixer_level_viewer.currentDecibel(), mixer_level_viewer.leftDecibel(), mixer_level_viewer.rightDecibel()), {20, 40});
    });

    EventSystem::global()->appendEvent([&](SDL_Event e) {
        if (EventSystem::global()->captureKeyboard(SDL_SCANCODE_SPACE)) {
            if (bgm->playStatus() != BGM::Playing) {
                bgm->resume();
            } else {
                bgm->pause();
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
