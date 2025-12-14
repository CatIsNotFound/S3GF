#include "MyEngine"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    auto win = new Window(&engine, "Hello world!");
    TextureAtlas atlas("./tiles.jpg", win->renderer());
    atlas.property()->move(100, 100);
    atlas.property()->color_alpha = {192, 255, 128, 255};
//    atlas.property()->setScale(0.5f);
    auto size = atlas.property()->size() / 4;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j)
            atlas.setTiles(std::format("test{}", i * 4 + j), GeometryF(size.width * j, size.height * i, size.width, size.height), atlas.property());
    }
    DateTime::setDefaultTimezone("Asia/Shanghai");
    atlas.setClipGeometryOfTiles("test4", GeometryF{0, 0, 200, 200});
    auto start_ts = DateTime::currentTimestamp();
    win->installPaintEvent([&atlas, &start_ts](Renderer* r) {
        r->fillBackground(StdColor::DarkBlue);
        r->drawPixelText(std::format("{}", DateTime::now()), {20, 20});
        auto timest = DateTime::currentTimestamp();
        r->drawPixelText(std::format("TS: {}, NOWTS: {}, DELTA: {}",
                                     start_ts, timest, timest - start_ts), {20, 30});
        static int i = 0;
        static uint64_t ts = DateTime::getTicks(DateTime::Milliseconds);
        auto now = DateTime::getTicks(DateTime::Milliseconds);
        atlas.draw(std::format("test{}", i));
        if (now - ts >= 500) {
            i = (i < 15 ? i + 1 : 0);
            ts = DateTime::getTicks(DateTime::Milliseconds);
        }
    });
    return engine.exec();
}