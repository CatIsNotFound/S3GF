
#ifndef MYENGINE_HORIZONTALLAYOUT_H
#define MYENGINE_HORIZONTALLAYOUT_H
#include "AbstractLayout.h"

namespace MyEngine {
    namespace Widget {
        class HorizontalLayout : public AbstractLayout {
        public:
            explicit HorizontalLayout(std::string object_name, Window* window);
            explicit HorizontalLayout(std::string object_name, AbstractWidget* widget);
            ~HorizontalLayout() override;

        protected:
            void paintEvent(MyEngine::Renderer *renderer) override;
            void resizeEvent(const MyEngine::Size &size) override;
            void layoutChanged() override;
        };
    }
}

#endif //MYENGINE_HORIZONTALLAYOUT_H
