
#ifndef MYENGINE_LAYERS_LAYER_MANAGER_H
#define MYENGINE_LAYERS_LAYER_MANAGER_H
#include "Layer.h"
#include "../Template/Singleton.h"
namespace MyEngine {
    class LayerManager : public Template::SafeSingleton<LayerManager> {
        friend class SafeSingleton;
    public:
        using LayerIterator = std::vector<std::shared_ptr<Layer>>::iterator;
        using LayerConstIterator = std::vector<std::shared_ptr<Layer>>::const_iterator;
        explicit LayerManager(Window* window = nullptr);
        virtual ~LayerManager();

        void appendLayer(const std::string_view& layer_name);
        void appendLayer(Layer* layer);
        void insertLayer(size_t index, const std::string_view& layer_name);
        void insertLayer(size_t index, Layer* layer);
        void removeLayer(size_t index);
        void popLayer();
        void clearLayers();

        [[nodiscard]] Layer* layer(size_t index) const;
        [[nodiscard]] Layer* layer(const std::string_view& layer_name) const;
        [[nodiscard]] LayerIterator begin();
        [[nodiscard]] LayerIterator end();
        [[nodiscard]] LayerConstIterator cbegin() const;
        [[nodiscard]] LayerConstIterator cend() const;
        [[nodiscard]] size_t size() const;
        [[nodiscard]] bool contains(const std::string_view& layer_name) const;
        [[nodiscard]] bool containsLayer(const Layer*& layer) const;
        [[nodiscard]] std::optional<size_t> indexOf(const std::string_view &layer_name) const;
        [[nodiscard]] std::optional<size_t> indexOf(const Layer *&layer) const;

        LayerManager(const LayerManager&) = delete;
        LayerManager(LayerManager&&) = delete;
        LayerManager& operator=(const LayerManager&) = delete;
        LayerManager& operator=(LayerManager&&) = delete;
    private:
        void registerEvent();
        Window* _window;
        std::vector<std::shared_ptr<Layer>> _layers;
    };
} // MyEngine

#endif //MYENGINE_LAYERS_LAYER_MANAGER_H