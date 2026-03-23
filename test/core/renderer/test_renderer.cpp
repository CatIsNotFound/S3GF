#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../../../include/MyEngine/MyEngine"
using namespace MyEngine;

extern inline constexpr bool isColorsEqual(SDL_Color color1, SDL_Color color2) noexcept {
    return (color1.r == color2.r) && (color1.g == color2.g) &&
           (color1.b == color2.b) && (color1.a == color2.a);
}

TEST_CASE("Renderer Test", "[Core][Window][Renderer]") {
    Engine engine;
    auto window = new Window(&engine, "Renderer Test");
    window->show();
    auto renderer = window->renderer();
    std::atomic<SSurface*> captured_view{};
    const SColor PT_COLOR = StdColor::Blue;
    const SColor LINE_COLOR = StdColor::Red;
    const SColor TRI_FG_COLOR = StdColor::DarkGreen;
    const SColor TRI_BG_COLOR = StdColor::LightGreen;
    const SColor RECT_FG_COLOR = StdColor::Blue;
    const SColor RECT_BG_COLOR = StdColor::Red;
    const SColor ELLI_BG_COLOR = StdColor::Pink;

    // Render Graphic
    Graphics::Point pt(100, 100, 20, PT_COLOR);
    Graphics::Line line(20.f, 20.f, 80.f, 20.f, 3, LINE_COLOR);
    Graphics::Triangle tri(400, 10, 400, 110, 600, 60, 2,
                           TRI_FG_COLOR, TRI_BG_COLOR);
    Graphics::Rectangle rect(100, 400, 100, 100, 4, RECT_FG_COLOR,
                             RECT_BG_COLOR, 90.f);
    Graphics::Ellipse elli(300, 300, 200, 100, 2, StdColor::Red, ELLI_BG_COLOR, 45.f, 64);
    window->installPaintEvent([&](Renderer* r) {
        r->drawPoint(&pt);
        r->drawLine(&line);
        r->drawTriangle(&tri);
        r->drawRectangle(&rect);
        r->drawEllipse(&elli);
    });

    SECTION("Initialized renderer") {
        REQUIRE(renderer);
        CHECK(renderer->window() == window);
    }

    SECTION("Check basic graphics") {
        // Timer for control
        Timer timer(1000, [&]() {
            static int sec = 0;
            sec++;
            // Exit engine
            if (sec == 2) Engine::exit();
            if (sec == 1) {
                captured_view = renderer->capture();

                if (!captured_view) {
                    Logger::log("Failed to get render capture!", MyEngine::Logger::Warn);
                }
            }
        });
        timer.start(0);

        bool ok;
        engine.exec();

        REQUIRE(captured_view);
        auto pt_color = Algorithm::readPixelFromSurface(captured_view,
                        static_cast<int>(pt.position().x), static_cast<int>(pt.position().y), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(pt_color, PT_COLOR));

        auto st_line_color = Algorithm::readPixelFromSurface(captured_view,
                             static_cast<int>(line.startPosition().x),
                             static_cast<int>(line.startPosition().y), &ok);
        REQUIRE(ok);
        auto ed_line_color = Algorithm::readPixelFromSurface(captured_view,
                                static_cast<int>(line.endPosition().x),
                                static_cast<int>(line.endPosition().y), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(st_line_color, LINE_COLOR));
        CHECK(isColorsEqual(ed_line_color, LINE_COLOR));

        // Test Triangle
        auto triangle_fg_color = Algorithm::readPixelFromSurface(captured_view,
                                    static_cast<int>(tri.position(1).x),
                                    static_cast<int>(tri.position(1).y), &ok);
        REQUIRE(ok);
        auto triangle_bg_color = Algorithm::readPixelFromSurface(captured_view,
                                       static_cast<int>((tri.position(0).x + 50)),
                                       static_cast<int>((tri.position(0).y + 30)), &ok);
        CHECK(isColorsEqual(triangle_fg_color, TRI_FG_COLOR));
        CHECK(isColorsEqual(triangle_bg_color, TRI_BG_COLOR));

        // Test Rect
        auto rect_fg_color = Algorithm::readPixelFromSurface(captured_view,
                                 static_cast<int>(rect.geometry().pos.x),
                                 static_cast<int>(rect.geometry().pos.y), &ok);
        REQUIRE(ok);
        auto rect_bg_color = Algorithm::readPixelFromSurface(captured_view,
                                 static_cast<int>(rect.geometry().pos.x + rect.geometry().size.width / 2),
                                 static_cast<int>(rect.geometry().pos.y + rect.geometry().size.height / 2), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(rect_fg_color, RECT_FG_COLOR));
        CHECK(isColorsEqual(rect_bg_color, RECT_BG_COLOR));

        // Test ellipse
        auto ellipse_bg_color = Algorithm::readPixelFromSurface(captured_view,
                                    static_cast<int>(elli.centerPosition().x),
                                    static_cast<int>(elli.centerPosition().y), &ok);
        REQUIRE(ok);
        CHECK(isColorsEqual(ellipse_bg_color, ELLI_BG_COLOR));

        // Destroy
        SDL_DestroySurface(captured_view);
        captured_view = nullptr;
        CHECK_FALSE(captured_view);
        CHECK_FALSE(renderer->self());
        CHECK_FALSE(window->self());
    }
}
