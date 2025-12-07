#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Utils/Random.h"
#include "UI/Button.h"
#include "Sprite/SpriteSheet.h"

using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(60);
    AudioSystem::global()->addNewMixer(2);
    Logger::log(std::format("Mixer count: {}", AudioSystem::global()->mixerCount()));
    std::vector<std::shared_ptr<BGM>> bgm_list;
    bgm_list.push_back(std::make_shared<BGM>(AudioSystem::global()->mixer(), "./bgm.mp3"));
    bgm_list.push_back(std::make_shared<BGM>(AudioSystem::global()->mixer(1), "./bgm2.mp3"));
    SFX sfx(AudioSystem::global()->mixer(2), "./sound.oga");
    size_t index = 0;
    auto win = new Window(&engine, "Test window");
    win->setResizable(true);
    SpriteSheet sprite_sheet("./atlas.png", win->renderer());
    sprite_sheet.resize(340, 250);
    StringList sequences;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (i * 4 + j + 1 >= 19) continue;
            auto clip_area = GeometryF{static_cast<float>(j * 340), static_cast<float>(i * 250), 340, 250};
            auto name = std::format("Atlas_{}", std::to_string(i * 4 + j + 1));
            sprite_sheet.appendTiles(name, clip_area);
            sequences.emplace_back(name);
        }
    }
    sprite_sheet.appendAnimation("action", sequences, 50);
    sprite_sheet.setAnimateEnabled(false);
    sprite_sheet.setCurrentAnimation("action");
    ClickArea clickArea(win->windowID(), Graphics::Rectangle(200, 200, 340, 250));

    win->installPaintEvent([&win, &bgm_list, &index, &sfx, &sprite_sheet](Renderer* r) {
        r->fillBackground(StdColor::DarkGray);
        sprite_sheet.draw();
        static float dx = 1.f, dy = 1.f, s = 0.01f;
        auto scale = sprite_sheet.scale();
        if (scale <= 0 || scale >= 2.f) s = -s;
        sprite_sheet.setScale(scale + s);
        auto pos = sprite_sheet.position();
        auto size = sprite_sheet.size();
        if (pos.x > win->geometry().width - size.width || pos.y > win->geometry().height - size.height) sprite_sheet.move(0, 0);
        if (pos.x >= win->geometry().width - size.width || pos.x < 0) dx = -dx;
        if (pos.y >= win->geometry().height - size.height || pos.y < 0) dy = -dy;
        sprite_sheet.move(pos.x + dx, pos.y + dy);
        r->drawPixelText(std::format("{} BGM {}: {} / {} Vol: {:.02f}", bgm_list[0]->playStatusText(), 0,
                                     bgm_list[0]->position(), bgm_list[0]->duration(), bgm_list[0]->volume()), {20, 20});
        r->drawPixelText(std::format("{} BGM {}: {} / {} Vol: {:.02f}", bgm_list[1]->playStatusText(), 1,
                                     bgm_list[1]->position(), bgm_list[1]->duration(), bgm_list[1]->volume()), {20, 30});
        r->drawPixelText(std::format("SFX: {} / {}", sfx.position(), sfx.duration()), {20, 40});
        auto pos1 = bgm_list[0]->get3DPosition();
        auto s1 = bgm_list[0]->getLRChannel();
        r->drawPixelText(std::format("L {:.02f}, R {:.02f}, 3D ({:.02f}, {:.02f}, {:.02f})",
                                     s1.left, s1.right, pos1.x, pos1.y, pos1.z), {20, 60});
        auto pos2 = bgm_list[1]->get3DPosition();
        auto s2 = bgm_list[1]->getLRChannel();
        r->drawPixelText(std::format("L {:.02f}, R {:.02f}, 3D ({:.02f}, {:.02f}, {:.02f})", s2.left, s2.right,
                                     pos2.x, pos2.y, pos2.z), {20, 70});
        auto is_space_down = EventSystem::global()->captureKeyboard(SDL_SCANCODE_SPACE);
        auto is_mouse_down = EventSystem::global()->captureMouse(EventSystem::Left);
        r->drawPixelText(std::format("Get Mouse status: {}, Space: {}", is_mouse_down, is_space_down), {20, 50});
        auto mouse_distance = EventSystem::global()->captureMouseAbsDistance();
        auto mouse_pos = EventSystem::global()->captureMousePosition();
        r->drawPixelText(std::format("Mouse distance: ({}, {}), Mouse pos: ({}, {})",
             mouse_distance.x, mouse_distance.y, mouse_pos.x, mouse_pos.y), {20, 80});

    });
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID() + 1000, [&bgm_list, &sfx, &index, &sprite_sheet, &clickArea](SDL_Event ev) {
        if (ev.key.type == SDL_EVENT_KEY_DOWN) {
            if (ev.key.key == SDLK_2) {
                index = 1;
                bgm_list[0]->stop(5000);
                if (!sprite_sheet.animateEnabled()) sprite_sheet.setAnimateEnabled(true);
                bgm_list[1]->play(0, true, 5000);
            } else if (ev.key.key == SDLK_1) {
                index = 0;
                bgm_list[1]->stop(5000);
                if (!sprite_sheet.animateEnabled()) sprite_sheet.setAnimateEnabled(true);
                bgm_list[0]->play(0, true, 5000);
            } else if (ev.key.key == SDLK_SPACE) {
                if (bgm_list[index]->playStatus() == BGM::Playing) {
                    bgm_list[index]->pause();
                    sprite_sheet.setAnimateEnabled(false);
                } else {
                    bgm_list[index]->play();
                    sprite_sheet.setAnimateEnabled(true);
                }
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
            } else if (ev.key.key == SDLK_X) {
                bgm_list[index]->setLRChannel(1.f, 1.f);
            } else if (ev.key.key == SDLK_C) {
                auto ch = bgm_list[index]->getLRChannel();
                bgm_list[index]->setLRChannel(ch.left - 0.01f, ch.right + 0.01f);
            } else if (ev.key.key == SDLK_LEFT) {
                bgm_list[index]->backward();
            } else if (ev.key.key == SDLK_RIGHT) {
                bgm_list[index]->forward();
            } else if (ev.key.key == SDLK_ESCAPE) {
                bgm_list[index]->stop(5000);
                sprite_sheet.setAnimateEnabled(false);
            } else {
                sfx.play(false);
            }
        }
    });

    engine.exec();
    Logger::log("GoodBye!", MyEngine::Logger::INFO);
    return 0;
}
