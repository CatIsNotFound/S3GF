#include "MyEngine"
#include "MyCMD.h"
using namespace MyEngine;

int main() {
    Engine engine;
    engine.setFPS(30);
    auto win = new Window(&engine, engine.applicationName(), 1024, 800, Window::VULKAN);
    win->setResizable(true);
    Texture texture("./tiles.jpg", win->renderer());
    auto size = texture.property()->size();
    texture.property()->move(win->geometry().width / 2 - size.width / 2, win->geometry().height / 2 - size.height / 2);
    texture.property()->setAnchorToCenter();
    texture.property()->color_alpha = RGBAColor::BlueLight;
    TextureAnimation ani("./ani.gif", win->renderer());
    ani.property()->setAnchorToCenter();
    ani.play();
    Logger::log(std::format("Used for engine: {}", SDL_GetRendererName(win->renderer()->self())), MyEngine::Logger::Info);
    RenderCommand::CommandFactory::registerCommand<MyCMD>();
    Graphics::Rectangle rect(300, 300, 100, 100, 4, RGBAColor::Black, RGBAColor::HalfTransparent);
    win->installPaintEvent([&engine, &win, &rect, &texture, &ani](Renderer* r) {
        win->setWindowTitle(std::format("{} FPS: {}", engine.applicationName(), engine.fps()));
        r->fillBackground(0x658823);
        r->drawTexture(texture.self(), texture.property());
        r->drawDebugText("Hello world!", {20, 20});
        static bool tri = false;
        if (!tri) {
            r->setBlendMode(SDL_BLENDMODE_BLEND);
            tri = true;
        }
        auto& angle = texture.property()->rotate_angle;
        if (angle >= 360.f) angle = 0.f; else angle += 1.f;
        auto scale = texture.property()->scale();
        static float d = 0.1f;
        if (scale >= 4.f || scale <= 0.25f) d = -d;
        texture.property()->setScale(scale + d);
        static float f = 1.f;
        auto ro = rect.rotate();
        if (ro >= 360.f || ro < 0.f) f = -f;
        rect.setRotate(ro + f);
        ani.draw();
        r->addCustomCommand<MyCMD>(SDL_FRect(400, 300, 100, 100), StdColor::Black);
        r->drawRectangle(&rect);
        r->drawDebugText(std::format("Rect1: {}, Rect2: {}", rect.rotate(), angle), {20, 40});
    });
    return engine.exec();
}