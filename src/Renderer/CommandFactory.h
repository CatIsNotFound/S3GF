#ifndef MYENGINE_COMMANDFACTORY_H
#define MYENGINE_COMMANDFACTORY_H
#include "CommandPool.h"

namespace MyEngine {
    namespace RenderCommand {
        class CommandFactory {
        public:
            template<typename T, typename ...Args>
            T* acquire(Args... args) {
                return getPool<T>().acquire(args...);
            }

            template<typename T>
            void release(std::unique_ptr<T> command) {
                getPool<T>().release(std::move(command));
            }

        private:
            template<typename T>
            static CommandPool<T>& getPool() {
                static CommandPool<T> cmd_pool;
                return cmd_pool;
            }
        };
    }
}

#endif //MYENGINE_COMMANDFACTORY_H