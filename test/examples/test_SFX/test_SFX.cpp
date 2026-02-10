#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    auto window = new Window(&engine, "Test sound effect");
    window->show();
    window->renderer()->setVSyncMode(Renderer::Enabled);
    SFX sfx(AudioSystem::global()->mixer(), FileSystem::getAbsolutePath("./assets/bell2.wav"));
    Graphics::Rectangle rect, rect2;
    TriggerArea area(GeometryF{100, 100, 100, 100}, window),
                area2(GeometryF(220, 100, 100, 100), window);
    rect.setGeometry(area.geometry());
    rect.setBackgroundColor(StdColor::Yellow);
    rect2.setGeometry(area2.geometry());
    rect2.setBackgroundColor(StdColor::RoyalBlue);
    area.setTriggerEvent([&sfx] {
        auto idx = sfx.findFreeIndex();
        if (idx >= sfx.count()) {
            sfx.play();
            sfx.setDefaultSFX(RandomGenerator::randFloat(0.5f, 5.f));
        } else {
            sfx.setVolume(RandomGenerator::randFloat(0.5f, 5.f), idx);
            sfx.setLRChannel(RandomGenerator::randFloat(0.f, 1.f), RandomGenerator::randFloat(0.f, 1.f), idx);
            sfx.play(idx);
        }
    });
    area.setTriggerKey(SDL_SCANCODE_SPACE);
    area2.setTriggerEvent([&sfx] { sfx.resetAll(); });
    area2.setTriggerKey(SDL_SCANCODE_ESCAPE);
    window->installPaintEvent([&](Renderer* r) {
        r->drawRectangle(&rect);
        r->drawRectangle(&rect2);
        r->drawDebugFPS();
        r->drawDebugText(FMT::format("Playing count: {}, Count: {}", sfx.playingCount(), sfx.count()), {20, 30});
        r->drawDebugText(FMT::format("Memory: {:.2f} MB", SysMemory::getCurProcUsedMemSize() / 1024.f), {20, 40});
    });

    return engine.exec();
}