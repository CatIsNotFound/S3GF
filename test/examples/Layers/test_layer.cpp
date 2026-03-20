#include "MyEngine"
using namespace MyEngine;

int main() {
    Engine engine;
    auto window = new Window(&engine, "Test Layer");
    window->renderer()->setVSyncMode(Renderer::Enabled);
    LayerManager layer_manager(window);
    layer_manager.appendLayer("1st");
    layer_manager.appendLayer("2nd");

    auto third_layer = new Layer("3rd");
    layer_manager.appendLayer(third_layer);
    auto first_layer = layer_manager.layer(0);
    auto second_layer = layer_manager.layer("2nd");
    auto re2 = new Component<Graphics::Rectangle>(0, 0, 100, 100, 1, StdColor::Gray, StdColor::Yellow);
    auto text = new Component<Texture>("./assets/tilesn.jpg", window->renderer());
    re2->self()->move(100, 100);
    re2->setRenderEvent([&window](Graphics::Rectangle* r) {
        window->renderer()->drawRectangle(r);
    });
    text->self()->property()->move(120, 120);
    text->setRenderEvent([](Texture* texture) {
        texture->draw();
    });
    window->installPaintEvent([&](Renderer* renderer) {
        renderer->drawDebugFPS();
    });
    first_layer->addComponent(re2, "Component<Graphics::Rectangle>", "re2");
    second_layer->addComponent(text, "Component<Texture>", "text");
    EventSystem::global()->appendEvent([&](SDL_Event e) {
        if (e.type == SDL_EVENT_KEY_DOWN) {
            if (e.key.key == SDLK_1) {
                if (first_layer->visibleComponentsCount() > 0) first_layer->setVisible(false);
                else first_layer->setVisible(true);
            } else if (e.key.key == SDLK_2) {
                if (second_layer->visibleComponentsCount() > 0)
                    second_layer->setVisible(false);
                else
                    second_layer->setVisible(true);
            } else if (e.key.key == SDLK_ESCAPE) {
                Engine::exit();
            } else if (e.key.key == SDLK_R) {
                layer_manager.swapLayers("1st", "2nd");
            }
        }
    });
    window->show();
    return engine.exec();
}

