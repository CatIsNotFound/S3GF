#include "MyEngine"

using namespace MyEngine;

#if 0
int main() {
    auto local = std::locale("zh_cn");
    std::locale::global(local);
    std::wcout.imbue(std::locale(""));
    std::cout << "Locale: " << std::cout.getloc().name() << "\n";
    std::cout << "WLocale: " << std::wcout.getloc().name() << "\n";
    std::wcout << L"Hello 你好世界！ world!" << std::endl;
    return 0;
}

#endif

#if 1
int main() {
    Logger::setBaseLogLevel(Logger::Debug);
    Logger::setWriteLogFileEnabled(true);
    Logger::setLogFileName(FMT::format("./{}.log", DateTime::currentTimestamp()));
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Test Voice");
    Graphics::Rectangle rect_L(100, 100, 0, 20, 0, {}, StdColor::Green);
    Graphics::Rectangle rect_R(100, 120, 0, 20, 0, {}, StdColor::LightGreen);
    Graphics::Rectangle rect_C(100, 100, 240, 40, 0, {}, StdColor::LightGray);
    Graphics::Rectangle rect_peak_L(100, 100, 0, 20, 0, {}, {0, 255, 0, 128});
    Graphics::Rectangle rect_peak_R(100, 120, 0, 20, 0, {}, {144, 238, 144, 128});
    int cnt;
    auto r_id_list = SDL_GetAudioRecordingDevices(&cnt);
    for (int i = 0; i < cnt; i++) {
        Logger::log(Logger::Info, "ID: {}", r_id_list[i]);
    }
    auto recorder = new AudioRecorder();
    auto input_spec = recorder->inputAudioSpec();
    auto output_spec = recorder->outputAudioSpec();
    Logger::log(Logger::Info, "[Input] ch: {}, format: {}, freq: {}",
        input_spec.channels, SDL_GetAudioFormatName(input_spec.format), input_spec.freq);
    Logger::log(Logger::Info, "[Output] ch: {}, format: {}, freq: {}",
        output_spec.channels, SDL_GetAudioFormatName(output_spec.format), output_spec.freq);
    AudioDecibelMeter track_level_viewer(recorder);
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
        r->drawRectangle(&rect_C);
        r->drawRectangle(&rect_peak_L);
        r->drawRectangle(&rect_peak_R);
        r->drawRectangle(&rect_L);
        r->drawRectangle(&rect_R);

        r->drawDebugText(FMT::format("[main] M: {:.2f} dB, L: {:.2f} dB, R: {:.2f} dB ",
            track_level_viewer.mixDecibel(), track_level_viewer.leftDecibel(), track_level_viewer.rightDecibel()),
            {20, 20});
    });
    window->show();
    engine.installCleanUpEvent([&recorder] {
        delete recorder;
        recorder = nullptr;
    });
    EventSystem::global()->appendEvent([&](SDL_Event e) {
        if (e.type == SDL_EVENT_KEY_DOWN) {
            if (e.key.key == SDLK_M) {
                if (recorder->isRecording()) {
                    recorder->stopRecord();
                } else {
                    recorder->startRecord();
                }
            }
        }
    });
    return engine.exec();
}
#endif