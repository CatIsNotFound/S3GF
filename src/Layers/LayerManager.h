
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

        Layer* layer(size_t index) const;
        [[nodiscard]] LayerIterator begin();
        [[nodiscard]] LayerIterator end();
        [[nodiscard]] LayerConstIterator cbegin() const;
        [[nodiscard]] LayerConstIterator cend() const;
        [[nodiscard]] size_t size() const;
    private:
        void registerEvent();
        Window* _window;
        std::vector<std::shared_ptr<Layer>> _layers;
    };
} // MyEngine

#endif //MYENGINE_LAYERS_LAYER_MANAGER_H