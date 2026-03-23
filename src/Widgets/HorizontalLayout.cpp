
#include "include/MyEngine/Widgets/HorizontalLayout.h"

MyEngine::Widget::HorizontalLayout::HorizontalLayout(std::string object_name, MyEngine::Window *window)
        : AbstractLayout(object_name, window) {}

MyEngine::Widget::HorizontalLayout::HorizontalLayout(std::string object_name, MyEngine::Widget::AbstractWidget *widget)
        : AbstractLayout(object_name, widget) {}

MyEngine::Widget::HorizontalLayout::~HorizontalLayout() {}

void MyEngine::Widget::HorizontalLayout::paintEvent(MyEngine::Renderer *renderer) {
    AbstractLayout::paintEvent(renderer);

}

void MyEngine::Widget::HorizontalLayout::resizeEvent(const MyEngine::Size &size) {
    AbstractLayout::resizeEvent(size);
    layoutChanged();
}

void MyEngine::Widget::HorizontalLayout::layoutChanged() {
    if (_widgets.empty()) return;
    Size avg_size;
    size_t wid_size = _widgets.size();
    if (wid_size == 1)
        avg_size.reset(_padding_geometry.size);
    else if (wid_size > 1)
        avg_size.reset((_padding_geometry.size.width - _padding_h * (float)(wid_size - 1)) / (float)wid_size,
                       _padding_geometry.size.height );
    float next_width = 0;
    for (auto& widget : _widgets) {
        Vector2 new_pos = {_padding_geometry.pos.x + next_width, _padding_geometry.pos.y};
        widget->setGeometry(new_pos, avg_size);
        next_width += avg_size.width + _padding_h;
    }
}


