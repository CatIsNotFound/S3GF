
#ifndef MYENGINE_FILLEDWIN_H
#define MYENGINE_FILLEDWIN_H
#include "MyEngine"
//using namespace MyEngine;

class FilledWin : public MyEngine::Window {
public:
    explicit FilledWin(const std::string& img_path, MyEngine::Engine* object, const std::string& title, int w = 800, int h = 600)
        : MyEngine::Window(object, title, w, h), _texture(nullptr) {
        _texture = new MyEngine::Texture(img_path, renderer());
        generate();
    }

    ~FilledWin() {
        for (auto& i : _props) {
            delete i;
        }
        delete _texture;
    }


protected:
    void resizeEvent() override {
        MyEngine::Window::resizeEvent();
        generate();
    }

    void paintEvent() override {
        renderer()->drawTexture(_texture->self(), _props);
        renderer()->drawDebugFPS({20, 30});
        MyEngine::Window::paintEvent();
    }

    void generate() {
        auto w = (float)geometry().width, h = (float)geometry().height;
        auto block_size = _texture->property()->size();
        auto cols = w / block_size.width + 1;
        auto rows = h / block_size.height + 1;
        int count = static_cast<int>(rows * cols);
        // if (count <= _props.size()) return;
        for (int i = 0; i < count; ++i) {
            int r = i / (int)cols;
            int c = i % (int)cols;
            if (i >= _props.size()) {
                _props.push_back(new MyEngine::TextureProperty(_texture->property()));
            }
            _props[i]->color_alpha = {
                    (uint8_t)MyEngine::RandomGenerator::randUInt(0, 255),
                    (uint8_t)MyEngine::RandomGenerator::randUInt(0, 255),
                    (uint8_t)MyEngine::RandomGenerator::randUInt(0, 255),
                    255
            };
            _props[i]->setGeomentry((float)c * block_size.width, (float)r * block_size.height, block_size.width, block_size.height);
        }
        _props.resize(count);
        _props.shrink_to_fit();
        MyEngine::Logger::log(std::format("Generated: {} blocks, {:.0f}x{:.0f}",
                                          count, rows, cols), MyEngine::Logger::Info);
    }

    void unloadEvent() override {
        MyEngine::Window::unloadEvent();
    }
private:
    MyEngine::Texture* _texture;
    std::vector<MyEngine::TextureProperty*> _props;
};


#endif //MYENGINE_FILLEDWIN_H
