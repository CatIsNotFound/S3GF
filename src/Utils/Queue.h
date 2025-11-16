#pragma once
#ifndef QUEUE_H
#define QUEUE_H
#include "Libs.h"

namespace S3GF {
    template <typename T>
    class TaskQueue {
    public:
        TaskQueue(TaskQueue &&) = delete;
        TaskQueue(const TaskQueue &) = delete;
        TaskQueue &operator=(TaskQueue &&) = delete;
        TaskQueue &operator=(const TaskQueue &) = delete;
        
        explicit TaskQueue() = default;
        ~TaskQueue() = default;

        bool push(const T& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _cond_var.wait(lock, [this] {
                return _datas_queue.size() <= _max_size || !_running; 
            });
            if (!_running) return false;
            _datas_queue.push_back(data);
            _cond_var.notify_one();
            return true;
        }

        bool push(T&& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _cond_var.wait(lock, [this] {
                return _datas_queue.size() <= _max_size || !_running; 
            });
            if (!_running) return false;
            _datas_queue.push_back(data);
            _cond_var.notify_one();
            return true;
        }

        bool pop(T& data) {
            std::unique_lock<std::mutex> lock(_mutex);
            _cond_var.wait(lock, [this] {
                return !_datas_queue.empty() || !_running;
            });
            if (!_running) return false;
            data = _datas_queue.front();
            _datas_queue.pop_front();
            _cond_var.notify_one();
            return true;
        }

        void clear() {
            std::unique_lock<std::mutex> lock(_mutex);
            if (_deletor) {
                std::for_each(_datas_queue.begin(), _datas_queue.end(), [this](T& data){
                    _deletor(data);
                });
            }
            _datas_queue.clear();
        }

        void start() {
            _running = true;
        }

        void stop() {
            _running = false;
            _cond_var.notify_all();
        }

        bool isRunning() const {
            return _running;
        }

        void setMaxSize(size_t max_size) {
            _max_size = max_size;
        }

        size_t size() const {
            return _datas_queue.size();
        }

        bool empty() const {
            return _datas_queue.empty();
        }

        const T& front() const {
            return _datas_queue.front();
        }

        const T& back() const {
            return _datas_queue.back();
        }
        
        void setDeletor(const std::function<void(T&)>& deletor) {
            _deletor = deletor;
        }
        
    private:
        std::deque<T> _datas_queue;
        std::mutex _mutex;
        std::condition_variable _cond_var;
        std::atomic<bool> _running{false};
        size_t _max_size{50};
        std::function<void(T&)> _deletor;
    };
}

#endif // !QUEUE_H