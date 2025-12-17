#ifndef MYENGINE_COMMANDPOOL_H
#define MYENGINE_COMMANDPOOL_H
#include "BaseCommand.h"

namespace MyEngine {
    namespace RenderCommand {
        class AbstractCommandPool {
        public:
            explicit AbstractCommandPool() = default;
            virtual ~AbstractCommandPool() = default;
        };

        template<typename T>
        class CommandPool : public AbstractCommandPool {
        public:
            explicit CommandPool(uint32_t max_commands = 1024) : _cmd_max_count(max_commands) {
                _sub_pool_count = std::thread::hardware_concurrency();
                if (_sub_pool_count == 0) _sub_pool_count = 4;
                std::pmr::pool_options options = {.max_blocks_per_chunk = _sub_pool_count,
                                                  .largest_required_pool_block = sizeof(T)};
                _pool_res = std::make_unique<std::pmr::synchronized_pool_resource>(options);
                for (size_t i = 0; i < _sub_pool_count; ++i) {
                    _pools.emplace_back(std::make_unique<SubPool>());
                }
            }
            
            ~CommandPool() override {
                for (std::unique_ptr<SubPool>& sub_pool : _pools) {
                    std::lock_guard<std::mutex> lock(sub_pool->mutex);
                    for (size_t i = 0; i < sub_pool->cmd_list.size(); ++i) {
                        auto cmd = sub_pool->cmd_list[i].release();
                        _pool_res->deallocate(cmd, sizeof(T), alignof(T));
                    }
                    sub_pool->cmd_list.clear();
                }
            };

            template <typename ...Args>
            T *acquire(Args ...args) {
                uint32_t pool_idx = _cur_sub_pool_idx.fetch_add(1, std::memory_order_relaxed) % _sub_pool_count;
                auto& sub_pool = _pools[pool_idx];
                std::unique_lock<std::mutex> _lock(sub_pool->mutex);
                if (sub_pool->cmd_list.empty()) {
                    /// Use custom memery allocator to create command.
                    void* ptr = _pool_res->allocate(sizeof(T), alignof(T));
                    return new (ptr) T(args...);
                }
                T *cmd = sub_pool->cmd_list.back().release();
                sub_pool->cmd_list.pop_back();
                if (cmd) {
                    /// Call reset function.
                    cmd->reset(args...);
                } else {
                    /// Use custom memery allocator to create command.
                    void* ptr = _pool_res->allocate(sizeof(T), alignof(T));
                    cmd = new (ptr) T(args...);
                }
                return cmd;
            }

            void release(std::unique_ptr<T> command) {
                if (!command) return;
                uint32_t pool_idx = _cur_sub_pool_idx.fetch_add(1, std::memory_order_relaxed) % _sub_pool_count;
                auto& sub_pool = _pools[pool_idx];
                std::unique_lock<std::mutex> _lock(sub_pool->mutex);
                size_t _max_count = _cmd_max_count / _sub_pool_count;
                if (sub_pool->cmd_list.size() + 1 >= _max_count) {
                    /// Remove the command.
                    command->~T();
                    /// Use custom deallocator to release command.
                    _pool_res->deallocate(command.release(), sizeof(T), alignof(T));
                    return;
                }
                sub_pool->cmd_list.push_back(std::move(command));
            }

            void getStatistics(size_t& current_size, size_t& max_size) {
                size_t size = 0;
                for (auto& p : _pools) {
                    size += p->cmd_list.size();
                }
                current_size = size;
                max_size = _cmd_max_count;
            }

        private:
            struct SubPool {
                std::vector<std::unique_ptr<T>> cmd_list{};
                std::mutex mutex{};
            };
            std::unique_ptr<std::pmr::synchronized_pool_resource> _pool_res;
            std::mutex _mutex;
            uint32_t _cmd_max_count, _sub_pool_count{0};
            std::atomic<uint32_t> _cur_sub_pool_idx{0};
            std::vector<std::unique_ptr<SubPool>> _pools;
        };
    }
}

#endif //MYENGINE_COMMANDPOOL_H
