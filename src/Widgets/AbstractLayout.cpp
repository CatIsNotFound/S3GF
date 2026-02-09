
#include "AbstractLayout.h"

#include <utility>

namespace MyEngine::Widget {

    AbstractLayout::AbstractLayout(std::string object_name, Window *window)
            : AbstractWidget(std::move(object_name), window), _margin(0, 0, 0, 0) {}

    AbstractLayout::AbstractLayout(std::string object_name, AbstractWidget* parent)
            : AbstractWidget(std::move(object_name), parent), _margin(0, 0, 0, 0) {}

    AbstractLayout::AbstractLayout(Window *window)
            : AbstractWidget(window), _margin(0, 0, 0, 0) {}

    AbstractLayout::~AbstractLayout() = default;

    void AbstractLayout::addWidget(AbstractWidget *widget) {
        if (!widget->parent()) {
            widget->setParent(this);
            _widgets.emplace_back(widget);
        } else if (widget->parent() != this) {
            Logger::log(Logger::Warn, "AbstractLayout ({}): Widget \"{}\" is already had its parent! Skipped adding widget to layout! ", objectName(), widget->objectName());
            return;
        }
        _widgets.emplace_back(widget);
        layoutChanged();
    }

    void AbstractLayout::addWidgets(const std::vector<AbstractWidget *> &widgets) {
        for (auto& w : widgets) {
            if (w->parent()) {
                Logger::log(Logger::Warn, "AbstractLayout ({}): Widget \"{}\" is already had its parent! Skipped adding widget to layout! ", objectName(), w->objectName());
                continue;
            }
            w->setParent(this);

            _widgets.emplace_back(w);
        }
        layoutChanged();
    }

    void AbstractLayout::removeWidget(AbstractWidget *widget) {
        auto idx = indexOf(widget);
        if (!idx) return;
        _widgets.at(idx.value())->setParent(nullptr);
        _widgets.erase(_widgets.begin() + idx.value());
        layoutChanged();
    }

    void AbstractLayout::removeWidget(uint32_t index) {
        _widgets.erase(_widgets.cbegin() + index);
        layoutChanged();
    }

    void AbstractLayout::removeWidget(const std::string &object_name) {
        std::erase_if(_widgets, [&object_name](AbstractWidget* w) { return w->objectName() == object_name; });
        layoutChanged();
    }

    std::optional<AbstractWidget *> AbstractLayout::widget(const std::string &object_name) const {
        auto iter = std::find_if(_widgets.begin(), _widgets.end(), [&object_name](AbstractWidget* w) {
            return w->objectName() == object_name;
        });
        if (iter != _widgets.end()) return *iter;
        return {};
    }

    std::optional<AbstractWidget *> AbstractLayout::widget(uint32_t index) const {
        if (index >= _widgets.size()) return {};
        return _widgets.at(index);
    }

    bool AbstractLayout::swapWidget(AbstractWidget *widget_1, AbstractWidget *widget_2) {
        if (!widget_1 || !widget_2) return false;
        auto idx1 = indexOf(widget_1), idx2 = indexOf(widget_2);
        if (!idx1 || !idx2) return false;
        std::swap(_widgets[idx1.value()], _widgets[idx2.value()]);
        layoutChanged();
        return true;
    }

    bool AbstractLayout::swapWidget(uint32_t index_1, uint32_t index_2) {
        if (index_1 > _widgets.size() || index_2 > _widgets.size()) return false;
        std::swap(_widgets[index_1], _widgets[index_2]);
        layoutChanged();
        return true;
    }

    std::optional<int64_t> AbstractLayout::indexOf(AbstractWidget *widget) {
        for (int64_t i = 0; i < (int64_t)_widgets.size(); ++i) {
            if (_widgets[i] == widget) return i;
        }
        return {};
    }

    void AbstractLayout::setMargin(float margin) {
        _margin = {
                .left = margin,
                .right = margin,
                .top = margin,
                .bottom = margin
        };
        resizeLayout();
        layoutChanged();
    }

    void AbstractLayout::setMargin(float h, float v) {
        _margin = {
                .left = h,
                .right = h,
                .top = v,
                .bottom = v
        };
        resizeLayout();
        layoutChanged();
    }

    void AbstractLayout::setMargin(const Margin &margin) {
        _margin = margin;
        resizeLayout();
        layoutChanged();
    }

    const Margin &AbstractLayout::margin() const {
        return _margin;
    }
    
    void AbstractLayout::setPadding(float padding) {
        _padding_h = padding;
        _padding_v = padding;
        resizeLayout();
        layoutChanged();
    }

    void AbstractLayout::setPadding(float h, float v) {
        _padding_h = h;
        _padding_v = v;
        resizeLayout();
        layoutChanged();
    }
    
    float AbstractLayout::paddingHorizontal() const {
        return _padding_h;
    }

    float AbstractLayout::paddingVertical() const {
        return _padding_v;
    }

    void AbstractLayout::loadEvent() {
    }

    void AbstractLayout::paintEvent(MyEngine::Renderer *renderer) {
        AbstractWidget::paintEvent(renderer);
    }

    void AbstractLayout::enableChangedEvent(bool enabled) {
        for (auto& w : _widgets) {
            w->setEnabled(enabled);
        }
    }

    void AbstractLayout::visibleChangedEvent(bool visible) {
        for (auto& w : _widgets) {
            w->setVisible(visible);
        }
    }

    void AbstractLayout::resizeEvent(const Size &size) {
        AbstractWidget::resizeEvent(size);
        resizeLayout();
        layoutChanged();
    }

    void AbstractLayout::resizeLayout() {
        _padding_geometry.setGeometry(_margin.left, _margin.top,
                                geometry().size.width - _margin.left - _margin.right,
                                geometry().size.height - _margin.top - _margin.bottom);
    }


} // MyEngine::Widget