#include <cassert>
#include "CommandPool.h"

namespace MyEngine {
    namespace RenderCommand {
        template<typename T>
        CommandPool<T>::CommandPool(uint32_t max_commands) : _cmd_max_count(max_commands) {
            _command_deque.resize(max_commands / 4);
        }

        template<typename T>
        CommandPool<T>::~CommandPool() = default;

        template<typename T>
        template<typename... Args>
        T *CommandPool<T>::acquire(Args... args) {
            std::unique_lock<std::mutex> _lock;
            if (_command_deque.empty()) {
                return new T(args...);
            }
            T *ptr = _command_deque.front().release();
            /// Call reset(...) function.
            ptr->reset(args...);
            _command_deque.pop_front();
            return ptr;
        }

        template<typename T>
        void CommandPool<T>::release(std::unique_ptr<T> command) {
            std::unique_lock<std::mutex> _lock;
            if (_command_deque.size() + 1 >= _cmd_max_count) {
                /// Remove from the front of the queue.
                _command_deque.erase(_command_deque.begin());
            }
            _command_deque.push_back(std::move(command));
        }
    }
}