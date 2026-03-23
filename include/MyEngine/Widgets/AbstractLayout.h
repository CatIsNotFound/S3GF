
#ifndef MYENGINE_WIDGETS_ABSTRACTLAYOUT_H
#define MYENGINE_WIDGETS_ABSTRACTLAYOUT_H

#include "AbstractWidget.h"

namespace MyEngine {
    namespace Widget {
        struct Margin {
            float left, right, top, bottom;
        };

        class AbstractLayout : public AbstractWidget {
        public:
            explicit AbstractLayout(std::string object_name, Window* window);
            explicit AbstractLayout(std::string object_name, AbstractWidget* parent);
            explicit AbstractLayout(Window* window);
            ~AbstractLayout() override;

            void addWidget(AbstractWidget* widget);
            void addWidgets(const std::vector<AbstractWidget*>& widgets);
            void removeWidget(AbstractWidget* widget);
            void removeWidget(uint32_t index);
            void removeWidget(const std::string& object_name);
            [[nodiscard]] std::optional<AbstractWidget *> widget(const std::string& object_name) const;
            [[nodiscard]] std::optional<AbstractWidget *> widget(uint32_t index) const;
            bool swapWidget(AbstractWidget* widget_1, AbstractWidget* widget_2);
            bool swapWidget(uint32_t index_1, uint32_t index_2);
            [[nodiscard]] std::optional<int64_t> indexOf(AbstractWidget* widget);

            void setMargin(float margin);
            void setMargin(float h, float v);
            void setMargin(const Margin& margin);
            [[nodiscard]] const Margin& margin() const;

            void setPadding(float padding);
            void setPadding(float h, float v);
            [[nodiscard]] float paddingHorizontal() const;
            [[nodiscard]] float paddingVertical() const;

        protected:
            void loadEvent() override;
            void paintEvent(MyEngine::Renderer *renderer) override;
            void enableChangedEvent(bool enabled) override;
            void visibleChangedEvent(bool visible) override;
            void resizeEvent(const MyEngine::Size &size) override;
            virtual void layoutChanged() = 0;

            std::vector<AbstractWidget*> _widgets;
            Margin _margin;
            float _padding_h{0}, _padding_v{0};
            GeometryF _padding_geometry;
        private:
            void resizeLayout();
        };
    }
} // MyEngine

#endif //MYENGINE_WIDGETS_ABSTRACTLAYOUT_H
