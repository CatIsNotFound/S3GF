#include "MyEngine"

using namespace MyEngine;

int main() {
    using RG = RandomGenerator;
    constexpr uint32_t COUNT = 256;
    Engine engine;
    engine.setRenderSetup(1024, true);
    auto window = new Window(&engine, "Render Performance Test");
    window->setResizable(true);
    auto renderer = window->renderer();
    std::vector<Graphics::Point*> points_list(COUNT);
    std::vector<Graphics::Line*> lines_list(COUNT);
    std::vector<Graphics::Triangle*> triangles_list(COUNT);
    std::vector<Graphics::Rectangle*> rectangles_list(COUNT);
    std::vector<Graphics::Ellipse*> ellipses_list(COUNT);
    std::atomic<uint8_t> visible_mode{0};

    window->installPaintEvent([&](Renderer* r) {
        auto WIN_W = static_cast<float>(window->geometry().width);
        auto WIN_H = static_cast<float>(window->geometry().height);
        r->fillBackground(RGBAColor::MixViolet);
        r->setBlendMode(SDL_BLENDMODE_MOD);
        if (visible_mode >= 1) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (points_list[i]) {
                    auto &pt = points_list[i];
                    pt->reset(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                              RG::randUInt(1, 32), RGBAColor::BlueLightTrans);
                } else {
                    auto pt = new Graphics::Point(RG::randFloat(0, WIN_W),
                                                  RG::randFloat(0, WIN_H),
                                                  RG::randUInt(1, 32),
                                                  RGBAColor::MixYellowTrans);
                    points_list[i] = pt;
                }
            }
        }

        if (visible_mode >= 2) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (lines_list[i]) {
                    auto &line = lines_list[i];
                    line->reset(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                RG::randUInt(1, 16), RGBAColor::GreenOliveTrans);
                } else {
                    auto line = new Graphics::Line(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                   RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                   RG::randUInt(1, 16), RGBAColor::GreenOliveTrans);
                    lines_list[i] = line;
                }
            }
        }

        if (visible_mode >= 3) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (triangles_list[i]) {
                    auto &triangle = triangles_list[i];
                    triangle->reset(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                    RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                    RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                    RG::randUInt(1, 4), RGBAColor::BlueDarkTrans,
                                    RGBAColor::BlueLightTrans);
                } else {
                    auto triangle = new Graphics::Triangle(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                           RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                           RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                           RG::randUInt(1, 4), RGBAColor::BlueDarkTrans,
                                                           RGBAColor::BlueLightTrans);
                    triangles_list[i] = triangle;
                }
            }
        }
        if (visible_mode >= 4) {
            for (uint32_t i = 0; i < COUNT; ++i) {
                if (rectangles_list[i]) {
                    auto &rect = rectangles_list[i];
                    rect->move(RG::randFloat(0, 700), RG::randFloat(0, 500));
                    rect->resize(std::clamp(RG::randFloat(10, WIN_W), 10.f, WIN_W - rect->geometry().pos.x),
                                 std::clamp(RG::randFloat(10, WIN_H), 10.f, WIN_H - rect->geometry().pos.y));
                    rect->setBorder(RG::randUInt(0, 4), RGBAColor::RedDarkTrans);
                    rect->setRotate(RG::randFloat(0, 360));
                } else {
                    auto rect = new Graphics::Rectangle();
                    rect->move(RG::randFloat(0, 700), RG::randFloat(0, 500));
                    rect->resize(std::clamp(RG::randFloat(10, WIN_W), 10.f, WIN_W - rect->geometry().pos.x),
                                 std::clamp(RG::randFloat(10, WIN_H), 10.f, WIN_H - rect->geometry().pos.y));
                    rect->setBorder(RG::randUInt(0, 4), RGBAColor::RedDarkTrans);
                    rect->setBackgroundColor(RGBAColor::RedLightTrans);
                    rect->setRotate(RG::randFloat(0, 360));
                    rectangles_list[i] = rect;
                }
            }
        }
        if (visible_mode >= 5) {
            for (uint32_t i = 0; i < COUNT; i++) {
                if (ellipses_list[i]) {
                    auto &elli = ellipses_list[i];
                    elli->setGeometry(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                      RG::randFloat(10, 400), RG::randFloat(10, 400));
                    elli->setBorder(RG::randUInt(0, 4), RGBAColor::GreenLimeTrans);
                } else {
                    auto elli = new Graphics::Ellipse(RG::randFloat(0, WIN_W), RG::randFloat(0, WIN_H),
                                                      RG::randFloat(10, 400), RG::randFloat(10, 400),
                                                      RG::randUInt(0, 4), RGBAColor::GreenLimeTrans,
                                                      RGBAColor::GreenLightTrans);
                    ellipses_list[i] = elli;
                }
            }
        }
        if (visible_mode >= 5) r->drawEllipses(ellipses_list);
        if (visible_mode >= 4) r->drawRectangles(rectangles_list);
        if (visible_mode >= 3) r->drawTriangles(triangles_list);
        if (visible_mode >= 2) r->drawLines(lines_list);
        if (visible_mode >= 1) r->drawPoints(points_list);
        r->drawDebugFPS({20, 20}, StdColor::White);
    });

    window->show();

    std::atomic<uint32_t> render_cnt{}, frames_cnt{}, record_count{};

    Timer recorder(1000, [&engine, &renderer, &render_cnt, &frames_cnt, &record_count] {
        render_cnt += renderer->renderCountInSec();
        frames_cnt += engine.fps();
        record_count++;
    });
    recorder.start(0);

    Timer setter(8000, [&renderer, &render_cnt, &frames_cnt, &record_count, &visible_mode] {
        static int sec = 12;
        sec--;
        uint32_t avg_render_cnt = render_cnt / record_count;
        uint32_t avg_frames_cnt = frames_cnt / record_count;
        switch (sec) {
            case 11:
            case 10:
            case 9:
            case 8:
            case 7:
                Logger::log(Logger::Info, "Render Mode {}: {} Commands, {} FPS",
                            static_cast<uint32_t>(visible_mode), avg_render_cnt, avg_frames_cnt);
                visible_mode++;
                break;
            case 6:
                visible_mode = 1;
                break;
            case 5:
                Logger::log(Logger::Info, "No VSync: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::QuarterRate);
                break;
            case 4:
                Logger::log(Logger::Info, "QuarterRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::ThirdRate);
                break;
            case 3:
                Logger::log(Logger::Info, "ThirdRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::HalfRate);
                break;
            case 2:
                Logger::log(Logger::Info, "HalfRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::Enabled);
                break;
            case 1:
                Logger::log(Logger::Info, "NormalRate: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                renderer->setVSyncMode(Renderer::Adaptive);
                break;
            case 0:
                Logger::log(Logger::Info, "Adaptive: {} Commands, {} FPS", avg_render_cnt, avg_frames_cnt);
                Engine::exit();
                break;
            default:
                return;
        }
        delay(2);
        render_cnt = 0;
        frames_cnt = 0;
        record_count = 0;
    });
    setter.start(12);

    engine.installCleanUpEvent([&] {
        static bool do_clean = false;
        if (do_clean) return;
        do_clean = true;
        for (auto p : points_list) {
            if (p) {
                delete p;
                p = nullptr;
            }
        }
        for (auto l : lines_list) {
            if (l) {
                delete l;
                l = nullptr;
            }
        }
        for (auto t : triangles_list) {
            if (t) {
                delete t;
                t = nullptr;
            }
        }
        for (auto r : rectangles_list) {
            if (r) {
                delete r;
                r = nullptr;
            }
        }
        for (auto e : ellipses_list) {
            if (e) {
                delete e;
                e = nullptr;
            }
        }
    });

    return engine.exec();
}
