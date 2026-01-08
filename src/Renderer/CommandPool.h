#ifndef MYENGINE_RENDERER_COMMANDPOOL_H
#define MYENGINE_RENDERER_COMMANDPOOL_H
#include "BaseCommand.h"

namespace MyEngine {
    namespace RenderCommand {
        class AbstractCommandPool {
        public:
            explicit AbstractCommandPool() = default;
            virtual ~AbstractCommandPool() = default;
        };

        class NormalCommandPool : public AbstractCommandPool {
        public:
            explicit NormalCommandPool(uint32_t max_commands = 4095, bool auto_incresement = false)
                : _max_cmds(max_commands), _auto_increasement(auto_incresement) {}

            ~NormalCommandPool() override {
                for (auto &cmd: _cmd_list) cmd.reset();
                _cmd_list.clear();
            }

            template<typename C, typename ...Args>
            C* acquire(Args... args) {
                std::unique_lock<std::mutex> lock(_mutex);
                if (!_cmd_list.empty()) {
                    auto ptr = std::move(_cmd_list.back());
                    _cmd_list.pop_back();
                    ptr.reset(new C(args...));
                    return dynamic_cast<C*>(ptr.release());
                } else {
                    return new C(args...);
                }
            }

            template<typename C>
            void release(std::unique_ptr<C> pointer) {
                std::unique_lock<std::mutex> lock(_mutex);
                if (_cmd_list.size() < _max_cmds) {
                    _cmd_list.push_back(std::move(pointer));
                } else if (_auto_increasement) {
                    _max_cmds *= 2;
                }
            }

        private:
            uint32_t _max_cmds;
            bool _auto_increasement{};
            std::mutex _mutex;
            std::vector<std::unique_ptr<BaseCommand>> _cmd_list;
        };
    }
}

#endif //MYENGINE_RENDERER_COMMANDPOOL_H
