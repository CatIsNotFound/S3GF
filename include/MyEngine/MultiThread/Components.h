#pragma once
#ifndef MYENGINE_MULTITHREAD_COMPONENTS_H
#define MYENGINE_MULTITHREAD_COMPONENTS_H
#include "../Libs.h"
namespace MyEngine {
    class Timer {
    public:
        Timer() = delete;
        Timer(Timer &&) = delete;
        Timer(const Timer &) = delete;
        Timer &operator=(Timer &&) = delete;
        Timer &operator=(const Timer &) = delete;
        ~Timer();
        explicit Timer(uint64_t delay, const std::function<void()>& event);
        void setDelay(uint64_t delay);
        void start(uint32_t count = 1);
        void stop();
        bool enabled() const;
        uint64_t delay() const;
        void setEvent(const std::function<void()>& event);
        bool isFinished() const;
        uint32_t triggeredCount() const;
    private:
        void running();
        uint32_t _delay;
        std::atomic<bool> _enabled;
        std::function<void()> _function;
        std::thread _thread;
        std::mutex _lock;
        uint32_t _run_count{0};
        uint32_t _finish_count{0};
        uint64_t _start_time{0};
        uint64_t _current_time{0};
        uint64_t _timer_id{0};
    };

    class Trigger {
    public:
        Trigger() = delete;
        Trigger(Trigger &&) = delete;
        Trigger(const Trigger &) = delete;
        Trigger &operator=(Trigger &&) = delete;
        Trigger &operator=(const Trigger &) = delete;

        explicit Trigger(const std::function<bool()>& condition, const std::function<void()>& event);
        ~Trigger();

        void setCondition(const std::function<bool()>& condition);
        void setEvent(const std::function<void()>& event);

        void start(uint32_t count = 1);
        void stop();
        bool enabled() const;
        bool isTriggered() const;
        uint64_t triggeredCount() const;
    private:
        void running();
        std::atomic<bool> _enabled;
        std::function<bool()> _condition_function;
        std::function<void()> _function;
        std::thread _thread;
        std::mutex _mutex;
        uint32_t _run_count{0};
        uint32_t _finish_count{0};
        uint64_t _trigger_id{0};
    };
}

#endif //MYENGINE_MULTITHREAD_COMPONENTS_H
