#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "../../../include/MyEngine/MyEngine"

using namespace MyEngine;

TEST_CASE("Audio System Load audios test", "[Core][Engine][Audio]") {
    Engine engine;
    AudioSystem::global()->appendBGM("bgm", FileSystem::getAbsolutePath("./assets/Peace.wav"));
    AudioSystem::global()->appendSFX("sfx", FileSystem::getAbsolutePath("./assets/SineWave.wav"));
    REQUIRE_NOTHROW(AudioSystem::global()->getBGM("bgm"));
    REQUIRE_NOTHROW(AudioSystem::global()->getSFX("sfx"));
    {
        CHECK(AudioSystem::global()->getBGM("bgm")->isLoaded());
        CHECK(AudioSystem::global()->getSFX("sfx")->isLoaded());
    }

    Timer quit_timer(1000, [] { Engine::exit(); });
    quit_timer.start();
    CHECK_NOFAIL(engine.exec());
    CHECK(AudioSystem::global()->size() == 0);
    CHECK_FALSE(AudioSystem::global()->isValid());
}

TEST_CASE("Audio System Audio Control Test", "[Core][Engine][Audio]") {
    Engine engine;
    AudioSystem::global()->appendBGM("bgm", FileSystem::getAbsolutePath("./assets/Peace.wav"));
    AudioSystem::global()->appendSFX("sfx1", FileSystem::getAbsolutePath("./assets/SineWave.wav"));
    AudioSystem::global()->appendSFX("sfx2", FileSystem::getAbsolutePath("./assets/SineWave.wav"));
    AudioSystem::global()->appendSFX("sfx3", FileSystem::getAbsolutePath("./assets/SineWave.wav"));
    REQUIRE_NOTHROW(AudioSystem::global()->getBGM("bgm"));
    REQUIRE_NOTHROW(AudioSystem::global()->getSFX("sfx1"));
    REQUIRE_NOTHROW(AudioSystem::global()->getSFX("sfx2"));
    REQUIRE_NOTHROW(AudioSystem::global()->getSFX("sfx3"));
    auto bgm = AudioSystem::global()->getBGM("bgm");
    auto sfx1 = AudioSystem::global()->getSFX("sfx1");
    auto sfx2 = AudioSystem::global()->getSFX("sfx2");
    auto sfx3 = AudioSystem::global()->getSFX("sfx3");
    REQUIRE(bgm->isLoaded());
    REQUIRE(sfx1->isLoaded());
    REQUIRE(sfx2->isLoaded());
    REQUIRE(sfx3->isLoaded());

    bool no_failed = true;

    Timer con_timer(1000, [&] {
        static int sec = 5;
        sec--;
        switch (sec) {
            case 4:
                if (!sfx1->play()) {
                    no_failed = false;
                }
                break;
            case 3:
                sfx1->stop(100);
                if (!sfx2->setSpeedAndPitch(1.25f)) no_failed = false;
                if (!sfx2->setVolume(0.5f)) no_failed = false;
                if (!sfx2->setLRChannel(1.f, -1.f)) no_failed = false;
                if (!sfx2->play(false, 100)) no_failed = false;
                break;
            case 2:
                sfx2->stop();
                if (!sfx3->setSpeedAndPitch(1.5f)) no_failed = false;
                if (!sfx3->setVolume(0.5f)) no_failed = false;
                if (!sfx3->set3DPosition(1.f, 0.f, -1.f)) no_failed = false;
                if (!sfx3->play()) no_failed = false;
                break;
            case 1:
                sfx3->stop();
                delay(1);
                if (!bgm->play()) no_failed = false;
                break;
            case 0:
                bgm->stop(1500);
                delayMS(2000);
                Engine::exit();
                break;
            default:
                break;
        }
    });
    con_timer.start(0);
    CHECK_NOFAIL(engine.exec());
    CHECK(no_failed);
}