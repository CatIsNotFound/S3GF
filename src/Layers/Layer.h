
#ifndef MYENGINE_LAYERS_LAYER_H
#define MYENGINE_LAYERS_LAYER_H
#include "../Components.h"
namespace MyEngine {
    class Layer;
    class ComponentBase {
    public:
        explicit ComponentBase() = default;
        ComponentBase(ComponentBase&&) = default;
        virtual ~ComponentBase() = default;
        virtual void render() = 0;
        ComponentBase(const ComponentBase&) = delete;
        ComponentBase& operator=(const ComponentBase&) = delete;
        ComponentBase& operator=(ComponentBase&&) = delete;
    };

    template <typename T>
    class Component : public ComponentBase {
        friend class Layer;
    public:
        explicit Component(T* self, bool delete_later = false, const std::function<void(T*)>& event = {})
                : _self(std::unique_ptr<T>(self)), _delete_this(delete_later), _render_event(event) {}
        template <typename ... Args>
        explicit Component(Args... args)
            : _self(std::make_unique<T>(std::forward<Args>(args)...)), _delete_this(true) {}
        explicit Component(Component&& component) noexcept {
            _self = std::move(component._self);
            _delete_this = component._delete_this;
            _render_event = std::move(component._render_event);
            component._delete_this = false;
        }

        ~Component() override {
            if (!_delete_this) _self.release();
        }

        T* self() const { return _self.get(); }

        void setComponent(T* self, bool delete_later = false) {
            if (!_delete_this) _self.release();
            _self.reset(self);
            _delete_this = delete_later;
        }
        void setComponent(std::unique_ptr<T>&& unique_ptr, bool delete_later = false) {
            if (!_delete_this) _self.release();
            _self = std::move(unique_ptr);
            _delete_this = delete_later;
        }
        template <typename ... Args>
        void setComponent(Args... args) {
            if (!_delete_this) _self.release();
            _self = std::make_unique<T>(std::forward<Args>(args)...);
            _delete_this = true;
        }
        void setRenderEvent(const std::function<void(T*)>& event) { _render_event = event; }
    private:
        void render() override { if (_render_event) _render_event(_self.get()); }
        std::unique_ptr<T> _self;
        bool _delete_this{};
        std::function<void(T*)> _render_event{};
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

        size_t addComponent(ComponentBase* component, const std::string_view& class_name, const std::string_view& object_name) {
            if (indexOf(object_name).has_value()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to add component! "
                                           "The specified object name is already exist!", object_name);
                Engine::throwCustomFatalError<InvalidArgumentException>();
                return 0;
            }
            _layers.emplace_back(std::unique_ptr<ComponentBase>(component),
                class_name.data(), object_name.data(), true);
            return _layers.size() - 1;
        }

        void insertComponent(size_t index, ComponentBase* component, const std::string_view& class_name,
                        const std::string_view& object_name) {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to insert component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return;
            }
            if (indexOf(object_name).has_value()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to add component! "
                                           "The specified object name is already exist!", object_name);
                Engine::throwCustomFatalError<InvalidArgumentException>();
                return;
            }
            _layers.emplace(_layers.begin() + index, std::unique_ptr<ComponentBase>(component),
                class_name.data(), object_name.data(), true);
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
        Component<T> *component(size_t index) {
            if (index >= _layers.size()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component to the index {}.",
                        _object_name, index);
                Engine::throwCustomFatalError<OutOfRangeException>();
                static Component<T>* dummy(nullptr);
                return dummy;
            }
            return static_cast<Component<T>*>(_layers[index].component.get());
        }

        template <typename T>
        Component<T> *component(const std::string_view& object_name) {
            auto idx = indexOf(object_name);
            if (!idx.has_value()) {
                Logger::log(Logger::Fatal, "Layer({}): Failed to get component from the specified object name.",
                        _object_name);
                Engine::throwCustomFatalError<OutOfRangeException>();
                return nullptr;
            }
            return static_cast<Component<T>*>(_layers[idx.value()].component.get());
        }

        [[nodiscard]] std::optional<size_t> indexOf(const std::string_view& object_name) const {
            for (size_t i = 0; i < _layers.size(); ++i) {
                if (_layers.at(i).name == object_name) return i;
            }
            return {};
        }

        [[nodiscard]] std::vector<size_t> indexListFromClassName(const std::string_view& class_name) const {
            std::vector<size_t> _index_list;
            for (size_t i = 0; i < _layers.size(); ++i) {
                if (_layers.at(i).class_name == class_name) {
                    _index_list.emplace_back(i);
                }
            }
            return _index_list;
        }

        [[nodiscard]] bool containsComponent(const std::string_view& object_name) const {
            return std::ranges::find_if(_layers, [&object_name](const Node& node) {
                return node.name == object_name;
            }) != _layers.end();
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

        [[nodiscard]] size_t visibleComponentsCount() const {
            size_t vis_cnt = 0;
            for (auto& layer : _layers) {
                if (layer.visible) ++vis_cnt;
            }
            return vis_cnt;
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

        void setVisible(bool visible) {
            for (auto& layer : _layers) {
                layer.visible = visible;
            }
        }

    protected:
        void paintEvent() const {
            for (size_t i = 0; i < _layers.size(); ++i) {
                const auto& LAYER = _layers.at(_layers.size() - i - 1);
                if (!LAYER.visible) continue;
                LAYER.component->render();
            }
        }

    private:
        std::vector<Node> _layers;
        std::string _object_name;
    };
}

#endif //MYENGINE_LAYERS_LAYER_H
