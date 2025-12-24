#include "FilledWin.h"
#include "MyEngine"
using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new FilledWin("./tiny_block.png", &engine, engine.applicationName(), 1024, 800);
    window->setResizable(true);
    Graphics::Rectangle back((float)window->geometry().width - 300, 0, 300, 200, 0, {}, {0, 0, 0, 160});
    Graphics::Rectangle fps_rect(back.geometry().pos.x + 20, 20, 260, 20, 1, StdColor::Green, {});
    std::array<Vector2, 128> pos_list;
    std::array<Graphics::Line, 127> line_list;
    auto startX = back.geometry().pos.x + 20.f;
    auto endX = (float)window->geometry().width - 20.f;
    auto startY = 60;
    auto endY = back.geometry().size.height - 20.f;
    auto offset = (endX - startX) / 127.f;
    for (int i = 0; i < 128; ++i) {
        pos_list[i].reset({startX + offset * i, RandomGenerator::randFloat(startY, endY)});
    }
    for (int i = 0; i < 127; ++i) {
        line_list[i].reset(pos_list[i], pos_list[i + 1], 1, RGBAColor::BlueSea);
    }

    Timer timer(100, [&line_list, &pos_list, &back, &window, &startX, &endX, &startY, &endY, &offset] {
        startX = back.geometry().pos.x + 20.f;
        endX = (float)window->geometry().width - 20.f;
        startY = 60.f;
        endY = back.geometry().size.height - 20.f;
        offset = (endX - startX) / 127.f;
        for (int i = 0; i < 128; ++i) {
            pos_list[i].reset({startX + offset * i, RandomGenerator::randFloat(startY, endY)});
        }
        for (int i = 0; i < 127; ++i) {
            line_list[i].reset(pos_list[i], pos_list[i + 1], 1, RGBAColor::BlueSea);
        }
    });

    window->installPaintEvent([&back, &fps_rect, &window, &line_list](Renderer* r) {
        r->drawDebugText(std::format("RCount: {}", r->renderCountInSec()), {20, 50});
        r->drawDebugText(std::format("Memory: {:.2f}GB", (float)SysMemory::getCurProcUsedMemSize() / 1024.f / 1024.f), {20, 60});
        r->setBlendMode(SDL_BLENDMODE_BLEND);
        r->drawRectangle(&back);
        back.setGeometry((float)window->geometry().width - 300, 0, 300, 200);
        fps_rect.setGeometry(back.geometry().pos.x + 20, 20, 260.f * ((float)window->engine()->fps() / 60.f), 20);
        r->drawRectangle(&fps_rect);
        for (auto& l : line_list) {
            r->drawLine(&l);
        }
    });
    auto aud_sys = AudioSystem::global();
    aud_sys->appendBGM("See", "./bgm.mp3");
    aud_sys->appendBGM("See2", "./bgm.mp3");
    auto bgm = aud_sys->getBGM("See");
    bgm->play(5000, true);
    auto bgm2 = aud_sys->getBGM("See2");
    bgm2->play(10000, true, 5000);
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&aud_sys](SDL_Event e) {
        static float volume = aud_sys->mixerVolume();
        auto vol = aud_sys->mixerVolume();
        if (e.key.key == SDLK_M && e.key.down) {
            if (vol > 0.f) {
                volume = vol;
                aud_sys->setMixerVolume(0);
            } else {
                aud_sys->setMixerVolume(volume);
            }
        }
        if (e.key.key == SDLK_ESCAPE && e.key.down) {
            aud_sys->stopAll();
        }

    });
    timer.start(0);
    return engine.exec();
}