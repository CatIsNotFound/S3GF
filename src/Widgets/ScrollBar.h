
#ifndef MYENGINE_WIDGETS_SCROLLBAR_H
#define MYENGINE_WIDGETS_SCROLLBAR_H
#include "AbstractWidget.h"
namespace MyEngine {
    namespace Widget {
        class ScrollBar : public AbstractWidget {
        public:
            explicit ScrollBar(std::string object_name, Window* window);
            explicit ScrollBar(Window* window);
            ~ScrollBar();

            void setValue(uint32_t value);
            [[nodiscard]] uint32_t value() const;
            void setMaxValue(uint32_t value);
            [[nodiscard]] uint32_t maxValue() const;

        protected:
            void loadEvent() override;
            void unloadEvent() override;
            void resizeEvent(const MyEngine::Size &size) override;
            void paintEvent(MyEngine::Renderer *renderer) override;
            void mouseDownEvent() override;
            void mouseUpEvent() override;

        private:
            Graphics::Rectangle _slider;
            Texture* _slider_texture{}, *_background{};
            uint8_t _status{};
            uint16_t _changer_signal{};
        };
    }
} // MyEngine

#endif //MYENGINE_WIDGETS_SCROLLBAR_H
