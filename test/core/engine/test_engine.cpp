#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "MyEngine"

using namespace MyEngine;

TEST_CASE( "Application meta datas test", "[Core][Engine]" ) {
    Engine::disabledShowAppInfo();
    Engine::setApplicationName("Test Engine");
    Engine::setApplicationVersion("v0.0.1");
    Engine::setApplicationID("MyEngine.test.engine.app");
    Engine::setApplicationTypeName("Test");
    Engine::setApplicationAuthor("Anonymous");
    Engine::setApplicationURL("https://www.domain.com");
    Engine::setApplicationCopyright("(c) 2026 Anonymous reserved");
    bool ok;
    Engine::openApplicationURL(&ok);
    CHECK(Engine::applicationName() == std::string_view("Test Engine"));
    CHECK(Engine::applicationVersion() == std::string_view("v0.0.1"));
    CHECK(Engine::applicationID() == std::string_view("MyEngine.test.engine.app"));
    CHECK(Engine::applicationTypeName() == std::string_view("Test"));
    CHECK(Engine::applicationAuthor() == std::string_view("Anonymous"));
    CHECK(Engine::applicationURL() == std::string_view("https://www.domain.com"));
    CHECK(Engine::applicationCopyright() == std::string_view("(c) 2026 Anonymous reserved"));
    CHECK(ok);
}

TEST_CASE( "Engine Initialization test", "[Core][Engine]" ) {
    Engine engine;
    CHECK(engine.isRunning() == true);
    engine.exit(0);
    CHECK(engine.exec() == 0);
}

TEST_CASE( "Windows management test", "[Core][Engine]" ) {
    Engine engine;

    // Create windows
    auto window = new Window(&engine, "Test Window");
    auto second_window = new Window(&engine, "Second Test Window");
    auto third_window = new Window(&engine, "Third Test Window");
    auto forth_window = new Window(&engine, "Forth Test Window");
    auto fifth_window = new Window(&engine, "Fifth Test Window");
    auto sixth_window = new Window(&engine, "Sixth Test Window");

    SECTION("First window is exist" ) {
        REQUIRE(window->self() != nullptr);
        REQUIRE(engine.isWindowExist(window->windowID()));
        CHECK(window->engine() == &engine);
    }
    SECTION("Second window is exist") {
        REQUIRE(second_window->self() != nullptr);
        REQUIRE(engine.isWindowExist(second_window->windowID()));
        CHECK(second_window->engine() == &engine);
    }
    SECTION("Third window is exist") {
        REQUIRE(third_window->self() != nullptr);
        REQUIRE(engine.isWindowExist(third_window->windowID()));
        CHECK(third_window->engine() == &engine);
    }
    SECTION("Forth window is exist") {
        REQUIRE(forth_window->self() != nullptr);
        REQUIRE(engine.isWindowExist(forth_window->windowID()));
        CHECK(forth_window->engine() == &engine);
    }
    SECTION("Fifth window is exist") {
        REQUIRE(fifth_window->self() != nullptr);
        REQUIRE(engine.isWindowExist(fifth_window->windowID()));
        CHECK(fifth_window->engine() == &engine);
    }
    SECTION("Sixth window is exist") {
        REQUIRE(sixth_window->self() != nullptr);
        REQUIRE(engine.isWindowExist(sixth_window->windowID()));
        CHECK(sixth_window->engine() == &engine);
    }
    
    // Check window ID list
    std::vector<SDL_WindowID> expected_window_id_list = {
        window->windowID(),
        second_window->windowID(),
        third_window->windowID(),
        forth_window->windowID(),
        fifth_window->windowID(),
        sixth_window->windowID()
    };
    auto window_id_list = engine.windowIDList();
    SECTION("Windows count is correct") {
        REQUIRE(engine.windowCount() == window_id_list.size());
        REQUIRE(window_id_list.size() == 6);
    }

    SECTION("Check whether every windows is in window ID list") {
        for (auto& id : window_id_list) {
            // Check if each window ID is in the expected list
            REQUIRE(std::find(expected_window_id_list.begin(), expected_window_id_list.end(), id) != expected_window_id_list.end());
        }
    }
    // Tried to remove 2 windows
    third_window->close();
    sixth_window->close();
    auto third_id = third_window->windowID();
    auto sixth_id = sixth_window->windowID();
    // Get window by engine
    SECTION("Get window by engine") {
        REQUIRE(engine.window(window->windowID()) == window);
        REQUIRE(engine.window(second_window->windowID()) == second_window);
        REQUIRE_FALSE(engine.isWindowExist(third_id));
        REQUIRE_FALSE(third_window == nullptr);
        REQUIRE(engine.window(forth_window->windowID()) == forth_window);
        REQUIRE(engine.window(fifth_window->windowID()) == fifth_window);
        REQUIRE_FALSE(engine.isWindowExist(sixth_id));
        REQUIRE_FALSE(sixth_window == nullptr);
    }

    // Exit engine
    engine.exit(0);
    SECTION("Check whether windows are closed after engine exit") {
        REQUIRE_FALSE(engine.exec());
        REQUIRE_FALSE(engine.windowCount());
        REQUIRE_FALSE(engine.isWindowExist(window->windowID()));
        REQUIRE_FALSE(engine.isWindowExist(second_window->windowID()));
        REQUIRE_FALSE(engine.isWindowExist(third_window->windowID()));
        REQUIRE_FALSE(engine.isWindowExist(forth_window->windowID()));
        REQUIRE_FALSE(engine.isWindowExist(fifth_window->windowID()));
        REQUIRE_FALSE(engine.isWindowExist(sixth_window->windowID()));
    }
}

TEST_CASE( "FPS controller test", "[Core][Engine][Performance]" ) {
    Engine engine;
    auto window = new Window(&engine, "FPS Test");
    SDL_Color color_changer = StdColor::Black;
    std::atomic<uint64_t> sum_fps{0}, record_fps_count{0};
    std::atomic<bool> test_failed{false}, delay_enabled{false};
    window->installPaintEvent([&color_changer](Renderer* r) {
        r->fillBackground(color_changer);
        r->drawDebugFPS({20, 20}, StdColor::LightBlue);
        static uint8_t k = -1;
        if (color_changer.r == 255 || color_changer.r == 0) k = -k;
        color_changer.r += k;
        color_changer.g += k;
        color_changer.b += k;
    });
    window->show();

    // Used timer to test different FPS
    Timer timer_changer_1(10000, [&engine, &sum_fps, &record_fps_count, &test_failed, &delay_enabled] {
        static uint8_t adjust_count = 7;
        adjust_count--;
        uint64_t avg_cnt = sum_fps / record_fps_count;
        sum_fps = 0;
        record_fps_count = 0;
        delay_enabled = true;
        switch (adjust_count) {
            case 0:
                if (avg_cnt > 144) test_failed = true;
                Logger::log(Logger::Info, "144 FPS -> {} - {}", avg_cnt, test_failed ? "FAILED" : "PASSED");
                MyEngine::Engine::exit();
                break;
            case 1:
                if (avg_cnt > 90) test_failed = true;
                Logger::log(Logger::Info, "90 FPS -> {} - {}", avg_cnt, test_failed ? "FAILED" : "PASSED");
                engine.setFPS(144);
                break;
            case 2:
                if (avg_cnt > 60) test_failed = true;
                Logger::log(Logger::Info, "60 FPS -> {} - {}", avg_cnt, test_failed ? "FAILED" : "PASSED");
                engine.setFPS(90);
                break;
            case 3:
                if (avg_cnt > 30) test_failed = true;
                Logger::log(Logger::Info, "30 FPS -> {} - {}", avg_cnt, test_failed ? "FAILED" : "PASSED");
                engine.setFPS(60);
                break;
            case 4:
                if (avg_cnt > 20) test_failed = true;
                Logger::log(Logger::Info, "20 FPS -> {} - {}", avg_cnt, test_failed ? "FAILED" : "PASSED");
                engine.setFPS(30);
                break;
            case 5:
                if (avg_cnt > 15) test_failed = true;
                Logger::log(Logger::Info, "15 FPS -> {} - {}", avg_cnt, test_failed ? "FAILED" : "PASSED");
                engine.setFPS(20);
                break;
            case 6:
                if (avg_cnt <= 0) test_failed = true;
                Logger::log(Logger::Info, "NO FPS -> {} - {}", avg_cnt, test_failed ? "FAILED" : "PASSED");
                engine.setFPS(15);
                break;
            default:
                break;
        }
    });
    timer_changer_1.start(7);
    Timer timer_changer_2(1000, [&engine, &sum_fps, &record_fps_count, &delay_enabled] {
        if (delay_enabled) {
            delay(2);
            delay_enabled = false;
        }
        sum_fps += engine.fps();
        record_fps_count += 1;
    });
    timer_changer_2.start(0);
    CHECK_FALSE(engine.exec());
    REQUIRE_FALSE(test_failed);
}


