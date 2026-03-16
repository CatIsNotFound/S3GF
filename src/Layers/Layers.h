
#ifndef MYENGINE_LAYERS_H
#define MYENGINE_LAYERS_H
#include "../Components.h"
namespace MyEngine {
    template <typename T>
    class Component {
    public:
        explicit Component(T* self, const std::function<void(T*)>& event = {})
                : _self(self) {}
        ~Component() = default;

        T* self() const { return _self; }

        void setComponent(T* self, const std::function<void(T*)>& event = {}) {
            _self = self;
            _render_event = event;
        }
        void setRenderEvent(const std::function<void(T*)>& event) { _render_event = event; }
        void render() { if (_render_event) _render_event(_self); }
    private:
        T* _self;
        std::function<void(T*)> _render_event;
    };

    class Layer {
        struct Node {
            std::any component;
            std::string class_name;
            std::string name;
            bool visible;
        };
    public:
        explicit Layer(const std::string_view& name) : _object_name(name) {}
        void setLayerName(const std::string_view& name) { _object_name = name; }
        template <typename T>
        size_t addComponent(T* component, const std::string_view& class_name, const std::string_view& object_name) {
            _layers.emplace_back(Component<T>(component), class_name, object_name, true);
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
            _layers.emplace(_layers.begin() + index, Component<T>(component, object_name), class_name, true);
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
        size_t size() const { return _layers.size(); }
        template <typename T>
        Component<T>& component(size_t index) {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return {};
            }
            return std::any_cast<Component<T>>(_layers[index].component);
        }


    private:
        std::vector<Node> _layers;
        std::string _object_name;
    };
}

#endif //MYENGINE_LAYERS_H