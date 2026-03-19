#include "MyEngine"

using namespace MyEngine;


int main() {
    Engine engine;
    engine.setFPS(60);
    auto window = new Window(&engine, "Test Layer");
    TextureAtlas atlas(FileSystem::getAbsolutePath("./assets/tilesn.jpg"), window->renderer());
    LayerManager layer_manager(window);
    layer_manager.appendLayer("1st");
    layer_manager.appendLayer("2nd");
    auto third_layer = new Layer("3rd");
    layer_manager.appendLayer(third_layer);
    auto first_layer = layer_manager.layer(0);
    auto second_layer = layer_manager.layer("2nd");
    /// first_layer->addComponent<TextureAtlas>(&atlas, "TextureAtlas", "tiles");
    Logger::log(Logger::Info, "ID: {}", typeid(TextureAtlas).name());

    window->show();
    return engine.exec();
}

