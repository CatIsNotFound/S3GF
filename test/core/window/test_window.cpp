#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../../../include/MyEngine/MyEngine"
using namespace MyEngine;

TEST_CASE("Window Test", "[Core][Windows]") {
    Engine engine;
    auto window = new Window(&engine, "Windows Test");

    SECTION("Window Properties") {
        SECTION("Check window position and size") {
            window->move(200, 100);
            window->resize(1024, 800);
            CHECK(window->geometry().x == 200);
            CHECK(window->geometry().y == 100);
            CHECK(window->geometry().width == 1024);
            CHECK(window->geometry().height == 800);
        }

        SECTION("Check window title") {
            window->setWindowTitle("Changed the window title");
            CHECK(window->windowTitle() == std::string_view("Changed the window title"));
        }

        SECTION("Check window icon") {
            window->setWindowIcon(FileSystem::getAbsolutePath("./assets/block.png"));
            CHECK(window->windowIcon());
        }
    }

    SECTION("Windows Status") {
        SECTION("Check window visible") {
            window->show();
            CHECK(window->visible());
            delay(1);
            window->hide();
            CHECK_FALSE(window->visible());
            delay(1);
            window->show();
            CHECK(window->visible());
            delay(1);
        }

        SECTION("Check other window status") {
            window->minimizeWindow();
            CHECK(window->isMinimizedWindow());
            delay(1);
            window->setResizable(true);
            CHECK(window->resizable());
            window->maximizeWindow();
            CHECK(window->isMaximizedWindow());
            delay(1);
            window->restoreWindow();
            CHECK(window->isRestoredWindow());
            delay(1);
            window->setResizable(false);
            CHECK_FALSE(window->resizable());
            window->maximizeWindow();
            CHECK_FALSE(window->isMaximizedWindow());
            delay(1);
        }

        SECTION("Check fullscreen mode") {
            window->setFullScreen(true, false);
            CHECK(window->fullScreen());
            delay(1);
            window->setFullScreen(false, true);
            CHECK_FALSE(window->fullScreen());
            delay(1);
        }
    }
}


