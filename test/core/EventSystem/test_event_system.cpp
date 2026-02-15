#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "MyEngine"
using namespace MyEngine;

TEST_CASE("EventSystem Register Test", "[Core][Engine][Events]") {
    Engine engine;
    constexpr size_t LENGTH = 1024;
    std::array<uint64_t, LENGTH> expected_ev_id_list{};
    std::array<uint64_t, LENGTH> expected_g_ev_id_list{};
    size_t new_ev_cnt{}, new_g_ev_cnt{};

    // Initilization ID list
    for (auto& i : expected_ev_id_list) {
        i = IDGenerator::getNewEventID();
    }

    for (auto& i : expected_g_ev_id_list) {
        i = IDGenerator::getNewGlobalEventID();
    }

    // Added timer to exit the engine
    Timer exit_engine_timer(1000, [] { Engine::exit(); });
    Timer events_listener(500, [&new_ev_cnt, &new_g_ev_cnt]{
        new_ev_cnt = EventSystem::global()->eventCount();
        new_g_ev_cnt = EventSystem::global()->globalEventCount();
    });
    exit_engine_timer.start();
    events_listener.start();
    SECTION("Append and remove events") {
        // Added the event
        for (auto& id : expected_ev_id_list) {
            EventSystem::global()->appendEvent(id, [&id](SDL_Event) {
                Logger::log(Logger::Info, "Running Event #{} by EventSystem", id);
            });
        }
        auto updated_event_cnt = EventSystem::global()->eventCount();
        CHECK(updated_event_cnt == LENGTH);

        // Added the global event
        for (auto& id : expected_g_ev_id_list) {
            EventSystem::global()->appendGlobalEvent(id, [&id] {
                Logger::log(Logger::Info, "Running global event #{} by EventSystem", id);
            });
        }
        auto updated_g_event_cnt = EventSystem::global()->globalEventCount();
        CHECK(updated_g_event_cnt == LENGTH);

        // Remove the event and global event
        for (size_t i = LENGTH / 2; i < LENGTH; i++) {
            EventSystem::global()->removeEvent(i);
            EventSystem::global()->removeGlobalEvent(i);
        }
        CHECK_NOFAIL(engine.exec());
        CHECK(new_ev_cnt == LENGTH / 2);
        CHECK(new_g_ev_cnt == LENGTH / 2);
        CHECK(EventSystem::global()->eventCount() == 0);
        CHECK(EventSystem::global()->globalEventCount() == 0);
    }
}

TEST_CASE("EventSystem Input Device Test", "[Core][Engine][Events]") {
    Engine engine;
    std::vector<SDL_Scancode> codes{};
    MouseStatus mouse_buttons{};
    std::string keys_char{}, mouse_str{};
    auto win = new Window(&engine, "Please press the keyboard and mouse...");
    win->installPaintEvent([&keys_char, &mouse_str] (Renderer* r) {
        r->drawDebugText(FMT::format("Keyboard: {}", keys_char), {20, 20});
        r->drawDebugText(FMT::format("Mouse: {}", mouse_str), {20, 30});
    });
    win->show();
    EventSystem::global()->appendEvent(IDGenerator::getNewEventID(), [&](SDL_Event e) {
        mouse_buttons = EventSystem::global()->captureMouseStatus();
        codes = EventSystem::global()->captureKeyboardStatus();
        keys_char.clear();
        for (auto& key : codes) {
            keys_char += SDL_GetKeyName(SDL_GetKeyFromScancode(key, SDL_Keymod(), true));
            keys_char += ' ';
        }
        mouse_str = EventSystem::mouseStatusName(mouse_buttons);
    });
    CHECK_NOFAIL(engine.exec());
}
