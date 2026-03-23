
#include "include/MyEngine/Widgets/VerticalLayout.h"

MyEngine::Widget::VerticalLayout::VerticalLayout(std::string object_name, MyEngine::Window *window)
        : AbstractLayout(object_name, window) {}

MyEngine::Widget::VerticalLayout::VerticalLayout(std::string object_name, MyEngine::Widget::AbstractWidget *widget)
        : AbstractLayout(object_name, widget) {}

MyEngine::Widget::VerticalLayout::~VerticalLayout() {}

void MyEngine::Widget::VerticalLayout::paintEvent(MyEngine::Renderer *renderer) {
    AbstractLayout::paintEvent(renderer);

}

void MyEngine::Widget::VerticalLayout::resizeEvent(const MyEngine::Size &size) {
    AbstractLayout::resizeEvent(size);
    layoutChanged();
}

void MyEngine::Widget::VerticalLayout::layoutChanged() {
    if (_widgets.empty()) return;
    Size avg_size;
    size_t wid_size = _widgets.size();
    if (wid_size == 1)
        avg_size.reset(_padding_geometry.size);
    else if (wid_size > 1)
        avg_size.reset(_padding_geometry.size.width,
                       (_padding_geometry.size.height - _padding_v * (float)(wid_size - 1)) / (float)wid_size );
    float next_height = 0;
    for (auto& widget : _widgets) {
        Vector2 new_pos = { _padding_geometry.pos.x,_padding_geometry.pos.y + next_height};
        widget->setGeometry(new_pos, avg_size);
        next_height += avg_size.height + _padding_v;
    }
}
