
#ifndef MYENGINE_WIDGETS_VERTICALLAYOUT_H
#define MYENGINE_WIDGETS_VERTICALLAYOUT_H

#include "AbstractLayout.h"
namespace MyEngine {
    namespace Widget {
        class VerticalLayout : public AbstractLayout {
        public:
            explicit VerticalLayout(std::string object_name, Window* window);
            explicit VerticalLayout(std::string object_name, AbstractWidget* widget);
            ~VerticalLayout() override;

        protected:
            void paintEvent(MyEngine::Renderer *renderer) override;
            void resizeEvent(const MyEngine::Size &size) override;
            void layoutChanged() override;
        };
    }
}

#endif //MYENGINE_WIDGETS_VERTICALLAYOUT_H
