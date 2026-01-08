#ifndef MYENGINE_RENDERER_COMMANDFACTORY_H
#define MYENGINE_RENDERER_COMMANDFACTORY_H
#include "CommandPool.h"

namespace MyEngine {
    namespace RenderCommand {
        class NormalCommandPool;
        class CommandFactory {
        public:
            CommandFactory() = delete;
            ~CommandFactory() = delete;
            CommandFactory(const CommandFactory&) = delete;
            CommandFactory(CommandFactory&&) = delete;
            CommandFactory& operator=(const CommandFactory&) = delete;
            CommandFactory& operator=(CommandFactory&&) = delete;

            template<typename T, typename ...Args>
            static T* acquire(Args... args) {
                return getPool().acquire<T>(args...);
            }

            template<typename T>
            static void release(std::unique_ptr<T> command) {
                getPool().release<T>(std::move(command));
            }

            static void initFactory(uint32_t max_commands, bool auto_incresement) {
                initPool(max_commands, auto_incresement);
            }

        private:
            static NormalCommandPool& initPool(uint32_t max_commands, bool auto_incresement) {
                static NormalCommandPool cmd_pool(max_commands, auto_incresement);
                return cmd_pool;
            }
            static NormalCommandPool& getPool() {
                static NormalCommandPool cmd_pool;
                return cmd_pool;
            }
        };
    }
}

#endif //MYENGINE_RENDERER_COMMANDFACTORY_H