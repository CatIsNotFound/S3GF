
#include "LayerManager.h"

namespace MyEngine {
    LayerManager::LayerManager(Window *window) : _window(window) {
        if (!window) {
            Logger::log(Logger::Fatal, "LayerManager: The specified window is not valid!");
            Engine::throwCustomFatalError<InvalidArgumentException>();
            return;
        }
        _window->renderer()->setLayerManager(this);
        registerEvent();
    }

    LayerManager::~LayerManager() = default;

    void LayerManager::appendLayer(const std::string_view &layer_name) {
        _layers.push_back(std::make_shared<Layer>(layer_name));
    }

    void LayerManager::appendLayer(Layer *layer) {
        _layers.emplace_back(layer);
    }

    void LayerManager::insertLayer(size_t index, const std::string_view &layer_name) {
        if (index >= _layers.size()) {
            Logger::log(Logger::Fatal, "LayerManager: The specified index {} is out of range!", index);
            Engine::throwCustomFatalError<InvalidArgumentException>();
        }
        _layers.insert(_layers.begin() + index, std::make_shared<Layer>(layer_name));
    }

    void LayerManager::insertLayer(size_t index, Layer *layer) {
        if (index >= _layers.size()) {
            Logger::log(Logger::Fatal, "LayerManager: The specified index {} is out of range!", index);
            Engine::throwCustomFatalError<InvalidArgumentException>();
        }
        _layers.emplace(_layers.begin() + index, layer);
    }

    void LayerManager::removeLayer(size_t index) {
        if (index >= _layers.size()) {
            Logger::log(Logger::Fatal, "LayerManager: The specified index {} is out of range!", index);
            Engine::throwCustomFatalError<InvalidArgumentException>();
        }
        _layers.erase(_layers.begin() + index);
    }

    void LayerManager::popLayer() {
        _layers.pop_back();
    }

    void LayerManager::clearLayers() {
        _layers.clear();
    }

    Layer* LayerManager::layer(size_t index) const {
        if (index < _layers.size()) {
            return _layers.at(index).get();
        }
        Logger::log(Logger::Fatal, "LayerManager: The specified index {} is out of range!", index);
        Engine::throwCustomFatalError<InvalidArgumentException>();
        return nullptr;
    }

    LayerManager::LayerIterator LayerManager::begin() {
        return _layers.begin();
    }

    LayerManager::LayerIterator LayerManager::end() {
        return _layers.end();
    }

    LayerManager::LayerConstIterator LayerManager::cbegin() const {
        return _layers.cbegin();
    }

    LayerManager::LayerConstIterator LayerManager::cend() const {
        return _layers.cend();
    }

    size_t LayerManager::size() const {
        return _layers.size();
    }

    void LayerManager::registerEvent() {
        _window->installPaintEvent([&](Renderer*) {
            for (auto& _layer : _layers) {
                if (!_layer) continue;
                _layer->paintEvent();
            }
        });
    }
} // MyEngine