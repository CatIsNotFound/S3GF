#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Utils/Random.h"
#include "UI/Button.h"
#include "Game/SpriteSheet.h"
#include "Game/Collider.h"
#include "Game/GObject.h"

using namespace MyEngine;

int main() {
    Logger::setBaseLogLevel(MyEngine::Logger::Debug);
    Engine engine("MyEngine Test", "v1.0.0", "org.Myengine.test");
    engine.setFPS(60);
    auto win = new Window(&engine, engine.applicationName());
    BGM bgm(AudioSystem::global()->mixer(), "bgm2.mp3");
    Sprite sprite("./back_button_1.png", win->renderer());
    sprite.move(100, 200);
    GObject obj("obj", &sprite);
    Graphics::Rectangle rect(100, 200, sprite.size().width, sprite.size().height, 2, RGBAColor::BlueDark, {});
    obj.setCollider(&rect);
    Graphics::Rectangle pgbar(0, 0, 0, 32, 0, {}, RGBAColor::RedFuchsia);
    Graphics::Point base(100, 200, 128, StdColor::Orange);
    Graphics::Point pt(100, 100, 20, RGBAColor::MixYellowTrans, 24);
    Graphics::Rectangle rt(100, 100, 40, 40, 10, RGBAColor::GreenMossTrans);
    Collider cl(&base), cl2(&rt);
    cl.setEnabled(true);
    cl2.setEnabled(true);
    obj.addTriggerToCollider(&cl2);
    obj.setColliderEnabled(true);
    cl2.appendCollider(&cl);
    win->installPaintEvent([&cl, &cl2, &sprite, &bgm, &obj, &pgbar](Renderer* r) {
        r->fillBackground(RGBAColor::MixBrownDark);
        auto per = (float)bgm.position() / (float)bgm.duration();
        pgbar.resize(800 * per, 20);
        r->drawRectangle(pgbar);
        r->drawPixelText(std::format("SFX: {} / {} speed: {:.2f}", bgm.position(), bgm.duration(), bgm.speedAndPitch()), { 20, 20 });
        obj.draw();
        obj.collider()->draw(r);
        cl.draw(r);
        cl2.draw(r);
    });
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&bgm, &rt, &obj](SDL_Event ev) {
        if (ev.key.down && ev.key.key == SDLK_SPACE) {
            bgm.play();
        }
        if (ev.key.down && ev.key.key == SDLK_UP) {
            bgm.setSpeedAndPitch(bgm.speedAndPitch() + 0.01f);
        }
        if (ev.key.down && ev.key.key == SDLK_DOWN) {
            bgm.setSpeedAndPitch(bgm.speedAndPitch() - 0.01f);
        }
        if (ev.key.down && ev.key.key == SDLK_ESCAPE) {
            bgm.stop(1000);
        }
        rt.move(EventSystem::global()->captureMousePosition());
        if (obj.colliderTriggered(0)) {
            rt.setBackgroundColor(RGBAColor::RedCherryBlossom);
        } else {
            rt.setBackgroundColor(RGBAColor::RedLightPink);
        }
    });

    return engine.exec();
}
