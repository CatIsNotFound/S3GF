#pragma once
#ifndef MYENGINE_MULTITHREAD_THREADPOOL_H
#define MYENGINE_MULTITHREAD_THREADPOOL_H
#include "../Libs.h"
#include "../Utils/Logger.h"

namespace MyEngine::MultiThread {
    class ThreadPool {
    public:
        explicit ThreadPool(uint32_t max_waiting, uint32_t max_running = std::thread::hardware_concurrency())
            : _nums_of_threads(max_running), _max_threads_count(max_waiting), _running(true) {
            if (!max_running || max_running > std::thread::hardware_concurrency()) {
                Logger::log("ThreadPool: Argument error: Maximum threads count argument is invalid!", Logger::Fatal);
                throw std::invalid_argument("ThreadPool: Argument error: Maximum threads count argument is invalid!");
            }
            if (!max_waiting) {
                Logger::log("ThreadPool: Argument error: Maximum waiting threads count argument is invalid!!", Logger::Fatal);
                throw std::invalid_argument("ThreadPool: Argument error: Maximum waiting threads count argument is invalid!!");
            }
            while (max_running--) {
                _thread_list.emplace_back([this]{
                    std::function<void()> this_task;
                    while (true) {
                        std::unique_lock<std::mutex> lock(_mutex);
                        _condition_var.wait(lock, [this] {
                            return !_waiting_queue.empty() || !_running;
                        });
                        if (!_running && _waiting_queue.empty()) return;
                        if (_waiting_queue.empty()) continue;
                        this_task = std::move(_waiting_queue.front());
                        _waiting_queue.pop();
                        _running_thread_count += 1;
                        lock.unlock();
                        try {
                            this_task();
                        } catch (const std::exception& e) {
                            Logger::log(FMT::format("ThreadPool: Task failed! "
                                                    "Exception: {}", e.what()), Logger::Error);
                        }
                        lock.lock();
                        _running_thread_count -= 1;
                        _condition_var.notify_all();
                        lock.unlock();
                    }
                });
            }
        }

        ~ThreadPool() {
            stopAll();
        }

        bool append(std::function<void()> function) {
            std::unique_lock<std::mutex> lock(_mutex);
            _condition_var.wait(lock, [this] {
                return _waiting_queue.size() < _max_threads_count || !_running;
            });
            if (!_running) return false;
            _waiting_queue.emplace(std::move(function));
            _condition_var.notify_one();
            return true;
        }

        void startAll() {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _running = true;
            }
            _condition_var.notify_all();
        }

        void wait(bool clear_queue = false) {
            std::unique_lock<std::mutex> lock(_mutex);
            if (clear_queue && !_waiting_queue.empty()) {
                _waiting_queue = {};
                _condition_var.notify_all();
            }
            _condition_var.wait(lock, [this] {
                return (_waiting_queue.empty() && _running_thread_count == 0) || !_running;
            });
        }

        void stopAll() {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                if (!_running) return;
                _running = false;
                _waiting_queue = {};
            }
            _condition_var.notify_all();
            for (auto& th : _thread_list) {
                if (th.joinable()) th.join();
            }
        }

        void restartAll() {
            if (_running) stopAll();

            _thread_list.clear();
            uint32_t nums = _nums_of_threads;
            while (nums--) {
                _thread_list.emplace_back([this]{
                    std::function<void()> this_task;
                    while (true) {
                        std::unique_lock<std::mutex> lock(_mutex);
                        _condition_var.wait(lock, [this] {
                            return !_waiting_queue.empty() || !_running;
                        });
                        if (!_running && _waiting_queue.empty()) return;
                        if (_waiting_queue.empty()) continue;
                        this_task = std::move(_waiting_queue.front());
                        _waiting_queue.pop();
                        _running_thread_count += 1;
                        lock.unlock();
                        try {
                            this_task();
                        } catch (const std::exception& e) {
                            Logger::log(FMT::format("ThreadPool: Task failed! "
                                                    "Exception: {}", e.what()), Logger::Error);
                        }
                        lock.lock();
                        _running_thread_count -= 1;
                        _condition_var.notify_all();
                        lock.unlock();
                    }
                });
            }
            startAll();
        }

        [[nodiscard]] bool isRunning() const { return _running.load(std::memory_order_acquire); }
        [[nodiscard]] uint32_t threadsCount() const { return _nums_of_threads; }
        [[nodiscard]] uint32_t waitingQueueCount() {
            std::unique_lock<std::mutex> lock(_mutex);
            return _waiting_queue.size();
        }
        [[nodiscard]] uint32_t maxWaitingQueueCount() const { return _max_threads_count; }
        [[nodiscard]] uint32_t runningThreadsCount() const { return _running_thread_count; }

    private:
        std::vector<std::thread> _thread_list;
        uint32_t _nums_of_threads;
        uint32_t _max_threads_count;
        std::atomic<uint32_t> _running_thread_count{0};
        std::mutex _mutex;
        std::condition_variable _condition_var;
        std::queue<std::function<void()>> _waiting_queue;
        std::atomic<bool> _running;
    };

    class TasksManager : private ThreadPool {
    public:
        enum Priority {
            Low,
            Medium,
            High
        };

        struct Task {
            uint64_t id;
            Priority priority;
            std::function<void()> function;
        };

        struct TaskCmp {
            bool operator()(Task& t1, Task& t2) {
                if (t1.priority < t2.priority) return true;
                if (t2.priority < t1.priority) return false;
                return t1.id > t2.id;
            }
        };

        explicit TasksManager(size_t max_tasks, size_t max_threads)
            : ThreadPool(max_tasks, max_threads) {}

        void addTask(std::function<void()> function, Priority priority = Low) {
            _tasks_queue.emplace(_next_id++, priority, std::move(function));
        }

        void startAllTasks() {
            while (!_tasks_queue.empty()) {
                auto top = _tasks_queue.top();
                append(top.function);
                _tasks_queue.pop();
            }
        }

        void start() {
            if (_tasks_queue.empty()) return;
            auto top = _tasks_queue.top();
            append(top.function);
            _tasks_queue.pop();
        }

        void stopAllTasks() { ThreadPool::wait(true); }
        void wait() { ThreadPool::wait(); }

        [[nodiscard]] size_t tasksCount() const { return _tasks_queue.size(); }
        [[nodiscard]] size_t runningTasks() const { return runningThreadsCount(); }
    private:
        std::priority_queue<Task, std::vector<Task>, TaskCmp> _tasks_queue;
        uint64_t _next_id{1};
    };
}

#endif //MYENGINE_MULTITHREAD_THREADPOOL_H
