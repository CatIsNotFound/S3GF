
#ifndef MYENGINE_CUSTOMRENDERCOMMAND_H
#define MYENGINE_CUSTOMRENDERCOMMAND_H

#include "MyEngine"

class CustomCommand : public MyEngine::RenderCommand::BaseCommand {
public:
    /// TODO: Need to change the arguments what you need.
    explicit CustomCommand(SDL_Renderer* renderer, SDL_Color color, SDL_BlendMode blend_mode)
        : MyEngine::RenderCommand::BaseCommand(renderer, "You need to modify me") {
        _render_color = color;
        _blend_mode = blend_mode;
        /// TODO: You can add more codes to set the arguments.


    }

    void exec() override {
        /// TODO: Need to execute how to draw in the window, here is a example as below:
        SDL_SetRenderDrawColor(_renderer, _render_color.r, _render_color.g, _render_color.b, _render_color.a);
        SDL_SetRenderDrawBlendMode(_renderer, _blend_mode);

        /// TODO: Add more render function (likes: SDL_Render....())
        /// ...

    }

    /// NOTE: The arguments of current functions must as same as your construct function,
    ///        or it will be crashed when running your engine.
    void reset(SDL_Renderer* renderer, SDL_Color color, SDL_BlendMode blend_mode) {
        /// TODO: To reset all of the arguments.
        _renderer = renderer;
        _render_color = color;
        _blend_mode = blend_mode;

        /// ...

    }

    /// TIPS: You can add more function to improve your custom render command.


private:
    /// TODO: Add some private members below.
    /// SDL_Texture* _texture;
    /// add more....

};

#endif //MYENGINE_CUSTOMRENDERCOMMAND_H
