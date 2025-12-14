#include "MyEngine"

using namespace MyEngine;

class MyCMD : public RenderCommand::BaseCommand {
public:
    explicit MyCMD(SDL_Renderer* renderer, const SDL_FRect rect, const SDL_Color& color)
        : RenderCommand::BaseCommand(renderer, "MyCMD"), _rect(rect), _color(color)  {}

    void reset(SDL_FRect rect, SDL_Color color) { _rect = rect; _color = color; }
    void exec() override {
        SDL_SetRenderDrawColor(_renderer, _color.r, _color.g, _color.b, _color.a);
        SDL_RenderFillRect(_renderer, &_rect);
    }

private:
    SDL_FRect _rect;
    SDL_Color _color;
};

int main() {


    return 0;
}