#include "Core.h"
#include "Utils/Logger.h"
#include "MultiThread/Components.h"
#include "Utils/RGBAColor.h"
#include "Controls/Button.h"
using namespace S3GF;

int main() {
    Logger::setBaseLogLevel(S3GF::Logger::DEBUG);
    Engine::disabledShowAppInfo();
    Engine engine;
    engine.setFPS(60);
    auto bg_color = RGBAColor::GreenLight;
    auto main_win = new Window(&engine, "Hello world");
    main_win->setResizable(true);
    auto default_fonts = FontDatabase::getSystemDefaultFont();
    auto font1 = default_fonts.empty() ? FontDatabase::FontInfo() : default_fonts[0];
    auto font2 = default_fonts.empty() ? FontDatabase::FontInfo() : default_fonts[1];
    TextSystem::global()->addFont(font1.font_name, font1.font_path, main_win->renderer());
    TextSystem::global()->addFont(font2.font_name, font2.font_path, main_win->renderer());
    Button button("btn", main_win->renderer());
    button.setFont(font1.font_name);
    TextSystem::global()->font(font1.font_name)->setFontSize(24.f);
    button.setText("Button 1");
    button.setKey(SDLK_SPACE);
    button.setActive();
    button.move(60, 60);
    button.resize(120, 70);
    button.setEnabled(false);
    button.setEvent([&bg_color] {
        bg_color = RGBAColor::RedLightPink;
    });
    Texture normal("./back_button_1.png", main_win->renderer());
    Texture hovered("./back_button_2.png", main_win->renderer());
    Texture pressed("./back_button_3.png", main_win->renderer());
    Texture invalid("./back_button_4.png", main_win->renderer());
    normal.property()->color_alpha = RGBAColor::RGBAValue2Color(0xa66b4b);
    hovered.property()->color_alpha = RGBAColor::RGBAValue2Color(0xfff8f8);
    pressed.property()->color_alpha = RGBAColor::BlueDark;
    TextureButton textureButton("test", main_win->renderer());
    textureButton.setTextures(&normal, &hovered, &pressed, &invalid);
    textureButton.move(400.f - textureButton.size().width / 2 + 40, 300.f - textureButton.size().height);
    textureButton.setKey(SDLK_RETURN);
    textureButton.setActive();
    textureButton.setFont(font2.font_name);
    TextSystem::global()->font(font2.font_name)->setFontSize(40.f);
    textureButton.setText("Hello!");
    auto& text_color_status = textureButton.textColor();
    text_color_status.active = RGBAColor::BlueDark;
    text_color_status.pressed = RGBAColor::BlueIce;
    auto size = textureButton.textSize();
    auto s_size = textureButton.geometry().size;
    textureButton.setTextPosition({s_size.width / 2 - size.width / 2, s_size.height / 2 - size.height / 2});
    textureButton.setTextVisible(true);
    textureButton.setEvent([&bg_color]{ bg_color = RGBAColor::BlueLight; });
    Timer timer(3000, [&button, &textureButton] {
        button.setEnabled(!button.enabled());
        textureButton.setEnabled(!textureButton.enabled());
        auto size = button.textSize();
        Logger::log(std::format("Size: {}x{}", size.width, size.height));
    });
    timer.start(0);
    main_win->installPaintEvent([&engine, &bg_color](Renderer* r) {
        r->fillBackground(bg_color);
        r->drawPixelText(std::format("FPS: {}, Button: {}",
                                     engine.fps(), EventSystem::global()->isMouseButtonDown() ? "true" : "false"), {20, 20});
    });
    Logger::log(std::format("Event count: {}", EventSystem::global()->eventCount()), S3GF::Logger::INFO);
    return engine.exec();
}
