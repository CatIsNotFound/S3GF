#ifndef MYENGINE_RCOMMAND_H
#define MYENGINE_RCOMMAND_H

#include "CommandFactory.h"

namespace MyEngine {
    template<typename T, typename ...Args>
    void Renderer::addCommand(Args... args) {
        auto ptr = RenderCommand::CommandFactory::acquire<T>(args...);
        if (ptr) _cmd_list.push_back(std::unique_ptr<T>(ptr));
    }
    
    template<typename T, typename ...Args>
    void Renderer::addCustomCommand(Args... args) {
        addCommand<T>(_renderer, args...);
    }
}

#endif //MYENGINE_RCOMMAND_H