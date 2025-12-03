#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Utils/Random.h"
#include "Controls/Button.h"

using namespace S3GF;

int main() {
    Logger::setBaseLogLevel(S3GF::Logger::DEBUG);
    Engine::disabledShowAppInfo();
    Engine engine;
    engine.setFPS(60);
    AudioSystem::global()->appendNewMixer(2);
    Logger::log(std::format("Mixer count: {}", AudioSystem::global()->mixerCount()));
    std::vector<std::shared_ptr<BGM>> bgm_list;
    bgm_list.push_back(std::make_shared<BGM>(AudioSystem::global()->mixer(), "./bgm.mp3"));
    bgm_list.push_back(std::make_shared<BGM>(AudioSystem::global()->mixer(1), "./bgm2.mp3"));
    SFX sfx(AudioSystem::global()->mixer(2), "./sound.oga");
    size_t index = 0;
    auto win = new Window(&engine, "Test window");
//    win->setBorderless(true);
    Cursor::global()->setCursor(Cursor::Hand);
    win->installPaintEvent([&bgm_list, &index, &sfx](Renderer* r) {
        r->fillBackground(StdColor::DarkGray);
        r->drawPixelText(std::format("{} BGM {}: {} / {} Vol: {:.02f}", bgm_list[0]->playStatusText(), 0,
                                     bgm_list[0]->position(), bgm_list[0]->duration(), bgm_list[0]->volume()), {20, 20});
        r->drawPixelText(std::format("{} BGM {}: {} / {} Vol: {:.02f}", bgm_list[1]->playStatusText(), 1,
                                     bgm_list[1]->position(), bgm_list[1]->duration(), bgm_list[1]->volume()), {20, 30});
        r->drawPixelText(std::format("SFX: {} / {}", sfx.position(), sfx.duration()), {20, 40});
        auto pos1 = bgm_list[0]->get3DPosition();
        auto s1 = bgm_list[0]->getLRChannel();
        r->drawPixelText(std::format("L {}, R {}, 3D ({:.02f}, {:.02f}, {:.02f})",
                                     s1.left, s1.right, pos1.x, pos1.y, pos1.z), {20, 60});
        auto pos2 = bgm_list[1]->get3DPosition();
        auto s2 = bgm_list[1]->getLRChannel();
        r->drawPixelText(std::format("L {}, R {}, 3D ({:.02f}, {:.02f}, {:.02f})", s2.left, s2.right,
                                     pos2.x, pos2.y, pos2.z), {20, 70});
        auto is_space_down = EventSystem::global()->captureKeyboard(SDL_SCANCODE_SPACE);
        auto is_mouse_down = EventSystem::global()->captureMouse(EventSystem::Left);
        r->drawPixelText(std::format("Get Mouse status: {}, Space: {}", is_mouse_down, is_space_down), {20, 50});

    });
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&bgm_list, &sfx, &index](SDL_Event ev) {
        if (ev.key.type == SDL_EVENT_KEY_DOWN) {
            if (ev.key.key == SDLK_2) {
                index = 1;
                bgm_list[0]->stop(5000);
                bgm_list[1]->play(0, true, 5000);
            } else if (ev.key.key == SDLK_1) {
                index = 0;
                bgm_list[1]->stop(5000);
                bgm_list[0]->play(0, true, 5000);
            } else if (ev.key.key == SDLK_SPACE) {
                if (bgm_list[index]->playStatus() == BGM::Playing)
                    bgm_list[index]->pause();
                else
                    bgm_list[index]->play();
            } else if (ev.key.key == SDLK_UP) {
                bgm_list[index]->setVolume(bgm_list[index]->volume() + 0.01f);
            } else if (ev.key.key == SDLK_DOWN) {
                bgm_list[index]->setVolume(bgm_list[index]->volume() - 0.01f);
            } else if (ev.key.key == SDLK_A) {
                auto pos = bgm_list[index]->get3DPosition();
                bgm_list[index]->set3DPosition(pos.x - 0.01f, pos.y, pos.z);
            } else if (ev.key.key == SDLK_W) {
                auto pos = bgm_list[index]->get3DPosition();
                bgm_list[index]->set3DPosition(pos.x, pos.y + 0.01f, pos.z);
            } else if (ev.key.key == SDLK_D) {
                auto pos = bgm_list[index]->get3DPosition();
                bgm_list[index]->set3DPosition(pos.x + 0.01f, pos.y, pos.z);
            } else if (ev.key.key == SDLK_S) {
                auto pos = bgm_list[index]->get3DPosition();
                bgm_list[index]->set3DPosition(pos.x, pos.y - 0.01f, pos.z);
            } else if (ev.key.key == SDLK_Q) {
                auto pos = bgm_list[index]->get3DPosition();
                bgm_list[index]->set3DPosition(pos.x, pos.y, pos.z - 0.01f);
            } else if (ev.key.key == SDLK_E) {
                auto pos = bgm_list[index]->get3DPosition();
                bgm_list[index]->set3DPosition(pos.x, pos.y, pos.z + 0.01f);
            } else if (ev.key.key == SDLK_R) {
                bgm_list[index]->setMuted(!bgm_list[index]->isMuted());
            } else if (ev.key.key == SDLK_Z) {
                auto ch = bgm_list[index]->getLRChannel();
                bgm_list[index]->setLRChannel(ch.left + 0.01f, ch.right - 0.01f);
            } else if (ev.key.key == SDLK_C) {
                auto ch = bgm_list[index]->getLRChannel();
                bgm_list[index]->setLRChannel(ch.left - 0.01f, ch.right + 0.01f);
            } else if (ev.key.key == SDLK_LEFT) {
                bgm_list[index]->backward();
            } else if (ev.key.key == SDLK_RIGHT) {
                bgm_list[index]->forward();
            } else if (ev.key.key == SDLK_ESCAPE) {
                bgm_list[index]->stop(5000);
            } else {
                sfx.play(false);
            }
        }
    });


    return engine.exec();
}
