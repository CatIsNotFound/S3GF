
#ifndef MYENGINE_LAYERS_LAYER_H
#define MYENGINE_LAYERS_LAYER_H
#include "../Components.h"
namespace MyEngine {
    class ComponentBase {
    public:
        virtual ~ComponentBase() = default;
        virtual void render() = 0;
    };

    template <typename T>
    class Component : public ComponentBase {
    public:
        explicit Component(T* self, const std::function<void(T*)>& event = {})
                : _self(self), _render_event(event) {}
        explicit Component(T* self, std::function<void(T*)>&& event = {}) noexcept
                : _self(self), _render_event(std::move(event)) {}
        ~Component() override = default;

        T* self() const { return _self; }

        void setComponent(T* self, const std::function<void(T*)>& event = {}) {
            _self = self;
            _render_event = event;
        }
        void setRenderEvent(const std::function<void(T*)>& event) { _render_event = event; }
        void render() override { if (_render_event) _render_event(_self); }
    private:
        T* _self;
        std::function<void(T*)> _render_event;
    };

    class LayerManager;

    class Layer {
        friend class LayerManager;
        struct Node {
            std::unique_ptr<ComponentBase> component;
            std::string class_name;
            std::string name;
            bool visible;
        };
    public:
        explicit Layer(const std::string_view& name) : _object_name(name) {}
        void setLayerName(const std::string_view& name) { _object_name = name; }
        [[nodiscard]] const std::string& layerName() const { return _object_name; }
        template <typename T>
        size_t addComponent(T* component, const std::string_view& class_name, const std::string_view& object_name) {
            _layers.emplace_back(std::make_unique<Component<T>>(component), class_name, object_name, true);
            return _layers.size() - 1;
        }
        template <typename T>
        size_t addComponent(T* component, const std::function<void(T*)>& event,
                    const std::string_view& class_name, const std::string_view& object_name) {
            _layers.emplace_back(std::make_unique<Component<T>>(component, event), class_name, object_name, true);
            return _layers.size() - 1;
        }
        template <typename T>
        size_t addComponent(T* component, std::function<void(T*)>&& event,
                    const std::string_view& class_name, const std::string_view& object_name) noexcept {
            _layers.emplace_back(std::make_unique<Component<T>>(component, std::move(event)), class_name, object_name, true);
            return _layers.size() - 1;
        }
        template <typename T>
        void insertComponent(size_t index, T* component, const std::string_view& class_name,
                        const std::string_view& object_name) {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to insert component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return;
            }
            _layers.emplace(_layers.begin() + index, std::make_unique<Component<T>>(component), class_name, object_name, true);
        }
        void swapComponents(size_t index_1, size_t index_2) {
            if (index_1 >= _layers.size() || index_2 >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to swap component!", _object_name);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return;
            }
            std::swap(_layers[index_1], _layers[index_2]);
        }
        void removeComponent(size_t index) {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to remove component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return;
            }
            _layers.erase(_layers.begin() + index);
        }
        void popComponent() {
            if (!_layers.empty()) _layers.pop_back();
        }
        [[nodiscard]] size_t size() const { return _layers.size(); }
        template <typename T>
        Component<T>& component(size_t index) {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                static Component<T> dummy(nullptr);
                return dummy;
            }
            return *static_cast<Component<T>*>(_layers[index].component.get());
        }

        [[nodiscard]] std::optional<std::string> className(size_t index) const {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return {};
            }
            return _layers[index].class_name;
        }

        [[nodiscard]] std::optional<std::string> objectName(size_t index) const {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return {};
            }
            return _layers[index].name;
        }

        [[nodiscard]] bool isVisible(size_t index) const {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return false;
            }
            return _layers[index].visible;
        }

        void setVisible(size_t index, bool visible) {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return;
            }
            _layers[index].visible = visible;
        }

    protected:
        void paintEvent() {
            for (auto& layer : _layers) {
                if (!layer.visible) continue;
                layer.component->render();
            }
        }

    private:
        std::vector<Node> _layers;
        std::string _object_name;
    };
}

#endif //MYENGINE_LAYERS_LAYER_H