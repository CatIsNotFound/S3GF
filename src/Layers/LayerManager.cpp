
#include "LayerManager.h"

namespace MyEngine {
    LayerManager::LayerManager(Window *window) : _window(window) {
        if (!window) {
            Logger::log(Logger::Fatal, "LayerManager: The specified window is not valid!");
            Engine::throwCustomFatalError<InvalidArgumentException>();
            return;
        }
        if (_window->renderer()->layerManager()) {
            Logger::log(Logger::Fatal, "LayerManager: The specified window is already "
                                       "had the layer manager!");
            Engine::throwCustomFatalError<InvalidArgumentException>();
            return;
        }
        _window->renderer()->setLayerManager(this);
        registerEvent();
    }

    LayerManager::~LayerManager() = default;

    void LayerManager::appendLayer(const std::string_view &layer_name) {
        if (contains(layer_name)) {
            Logger::log(Logger::Error, "LayerManager: The specified layer name '{}' is already exist!",
                layer_name);
            return;
        }
        _layers.push_back(std::make_shared<Layer>(layer_name));
    }

    void LayerManager::appendLayer(Layer *layer) {
        if (containsLayer(const_cast<const Layer*&>(layer))) {
            Logger::log(Logger::Error, "LayerManager: The specified layer is already exist!");
            return;
        }
        _layers.emplace_back(layer);
    }

    void LayerManager::insertLayer(size_t index, const std::string_view &layer_name) {
        if (index >= _layers.size()) {
            Logger::log(Logger::Fatal, "LayerManager: The specified index {} is out of range!", index);
            Engine::throwCustomFatalError<InvalidArgumentException>();
        }
        if (contains(layer_name)) {
            Logger::log(Logger::Error, "LayerManager: The specified layer name '{}' is already exist!",
                layer_name);
            return;
        }
        _layers.insert(_layers.begin() + index, std::make_shared<Layer>(layer_name));
    }

    void LayerManager::insertLayer(size_t index, Layer *layer) {
        if (index >= _layers.size()) {
            Logger::log(Logger::Fatal, "LayerManager: The specified index {} is out of range!", index);
            Engine::throwCustomFatalError<InvalidArgumentException>();
        }
        if (containsLayer(const_cast<const Layer*&>(layer))) {
            Logger::log(Logger::Error, "LayerManager: The specified layer is already exist!");
            return;
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
        if (_layers.empty()) return;
        _layers.pop_back();
    }

    void LayerManager::clearLayers() {
        _layers.clear();
    }

    bool LayerManager::swapLayers(size_t index_1, size_t index_2) {
        if (index_1 >= _layers.size() || index_2 >= _layers.size()) {
            Logger::log(Logger::Fatal, "LayerManager: One/All of the specified index is out of range!");
            return false;
        }
        _layers.at(index_1).swap(_layers.at(index_2));
        return true;
    }

    bool LayerManager::swapLayers(const std::string_view &layer_1, const std::string_view &layer_2) {
        auto index_1 = indexOf(layer_1), index_2 = indexOf(layer_2);
        if (!index_1.has_value() || !index_2.has_value()) {
            Logger::log(Logger::Fatal, "LayerManager: One/All of the specified layer name is not found!");
            return false;
        }
        _layers.at(index_1.value()).swap(_layers.at(index_2.value()));
        return true;
    }

    Layer* LayerManager::layer(size_t index) const {
        if (index < _layers.size()) {
            return _layers.at(index).get();
        }
        Logger::log(Logger::Fatal, "LayerManager: The specified index {} is out of range!", index);
        Engine::throwCustomFatalError<InvalidArgumentException>();
        return nullptr;
    }

    Layer *LayerManager::layer(const std::string_view &layer_name) const {
        auto found_layer = std::ranges::find_if(_layers,
            [&layer_name](const std::shared_ptr<Layer>& layer) {
            return layer->layerName() == layer_name;
        });
        if (found_layer == _layers.end()) {
            Logger::log(Logger::Fatal, "LayerManager: The specified layer name {} is not exist!", layer_name);
            Engine::throwCustomFatalError<InvalidArgumentException>();
            return nullptr;
        }
        return found_layer->get();
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

    bool LayerManager::contains(const std::string_view &layer_name) const {
        return std::ranges::find_if(_layers,
            [&layer_name](const std::shared_ptr<Layer>& layer) {
            return layer->layerName() == layer_name;
        }) != _layers.end();
    }

    bool LayerManager::containsLayer(const Layer *&layer) const {
        return std::ranges::find_if(_layers, [&layer](const std::shared_ptr<Layer>& this_layer) {
            return this_layer.get() == layer;
        }) != _layers.end();
    }

    std::optional<size_t> LayerManager::indexOf(const std::string_view &layer_name) const {
        for (size_t index = 0; index < _layers.size(); index++) {
            if (_layers.at(index)->layerName() == layer_name) {
                return index;
            }
        }
        return {};
    }

    std::optional<size_t> LayerManager::indexOf(const Layer *&layer) const {
        for (size_t index = 0; index < _layers.size(); index++) {
            if (_layers[index].get() == layer) {
                return index;
            }
        }
        return {};
    }

    void LayerManager::registerEvent() {
        _window->installPaintEvent([&](Renderer*) {
            for (size_t index = 0; index < _layers.size(); index++) {
                auto layer = _layers.at(_layers.size() - index - 1).get();
                if (layer == nullptr) continue;
                layer->paintEvent();
            }
        });
    }
} // MyEngine