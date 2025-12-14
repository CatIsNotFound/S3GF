#ifndef MYENGINE_COMMANDPOOL_H
#define MYENGINE_COMMANDPOOL_H
#include "BaseCommand.h"

namespace MyEngine {
    namespace RenderCommand {
        class AbstractCommandPool {
        public:
            explicit AbstractCommandPool() = default;
            virtual ~AbstractCommandPool() = 0;
        };

        template<typename T>
        class CommandPool : public AbstractCommandPool {
        public:
            explicit CommandPool(uint32_t max_commands = 1024);
            ~CommandPool() override;

            template <typename ...Args>
            T *acquire(Args ...args);

            void release(std::unique_ptr<T> command);

        private:
            std::deque<std::unique_ptr<T>> _command_deque;
            std::mutex _mutex;
            uint32_t _cmd_max_count;
        };
    }
}

#endif //MYENGINE_COMMANDPOOL_H
