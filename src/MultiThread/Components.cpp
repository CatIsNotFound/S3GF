
#include "MultiThread/Components.h"
#include "Core.h"
#include "../Utils/Logger.h"
#include "MultiThread/ThreadPool.h"

namespace S3GF {
    Timer::~Timer() {
        stop();
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    Timer::Timer(uint64_t delay, const std::function<void()>& event)
            : _delay(delay), _function(event), _enabled(false) {
        _timer_id = RandomGenerator::randBigUInt(10000, 99999999);
        Logger::log(std::format("Timer ID {} is created", _timer_id));
    }

    void Timer::setDelay(uint64_t delay) {
        _delay = delay;
    }

    void Timer::start(uint32_t count) {
        if (_enabled) {
            Logger::log(std::format("The timer ID {} is already started! Please stop the current timer at first!", _timer_id),
                        Logger::ERROR);
            return;
        }
        _enabled = true;
        _run_count = count;
        _finish_count = 0;
        _current_time = SDL_GetTicks();
        _thread = std::thread(&Timer::running, this);
        Logger::log(std::format("The timer ID {} is started!", _timer_id));
    }

    void Timer::stop() {
        if (!_enabled) return;
        _enabled = false;
        Logger::log(std::format("Preparing to stop the timer ID {}!", _timer_id));
    }

    bool Timer::enabled() const {
        return _enabled;
    }

    uint64_t Timer::delay() const {
        return _delay;
    }

    void Timer::setEvent(const std::function<void()>& event) {
        _function = event;
    }

    bool Timer::isFinished() const {
        return _run_count == 0;
    }

    uint32_t Timer::triggeredCount() const {
        return _finish_count;
    }

    void Timer::running() {
        while (_enabled) {
            _lock.lock();
            SLEEP_MS(1);
            _current_time = SDL_GetTicks();
            auto current_delay = _current_time - _start_time;
            if (current_delay >= _delay) {
                if (_function) {
                    _function();
                    _run_count -= 1;
                    _finish_count += 1;
                    Logger::log(std::format("Triggered the timer event by ID {}! Elapsed triggered count: {}",
                                            _timer_id, _run_count));
                    if (!_run_count) {
                        _enabled = false;
                    }
                    _start_time = SDL_GetTicks();
                }
            }
            _lock.unlock();
        }
        Logger::log(std::format("The timer ID {} is stopped running!", _timer_id));
    }

    Trigger::Trigger(const std::function<bool()>& condition, const std::function<void()>& event)
            : _condition_function(condition), _function(event) {
        _trigger_id = RandomGenerator::randBigUInt(100000, 99999999);
        Logger::log(std::format("Trigger ID {} is loaded!", _trigger_id));
    }

    Trigger::~Trigger() {
        stop();
        if (_thread.joinable()) {
            _thread.join();
        }
    }

    void Trigger::setCondition(const std::function<bool()>& condition) {
        _condition_function = condition;
    }

    void Trigger::setEvent(const std::function<void()>& event) {
        _function = event;
    }

    void Trigger::start(uint32_t count) {
        if (_enabled) {
            Logger::log(std::format("Trigger ID {} is already started! "
                                    "Please stop the current trigger at first", _trigger_id), Logger::ERROR);
            return;
        }
        _enabled = true;
        _run_count = count;
        _finish_count = 0;
        _thread = std::thread(&Trigger::running, this);
        Logger::log(std::format("Trigger ID {} is started!", _trigger_id));
    }

    void Trigger::stop() {
        _enabled = false;
        Logger::log(std::format("Trigger ID {} is preparing to stop the trigger!", _trigger_id));
    }

    bool Trigger::enabled() const {
        return _enabled;
    }

    bool Trigger::isTriggered() const {
        return _finish_count > 0;
    }

    uint64_t Trigger::triggeredCount() const {
        return _finish_count;
    }

    void Trigger::running() {
        while (_enabled) {
            _mutex.lock();
            SLEEP_MS(1);
            bool c_switch = false;
            if (_condition_function) {
                c_switch = _condition_function();
            }
            if (c_switch && _function) {
                _function();
                _run_count -= 1;
                _finish_count += 1;
                if (!_run_count) {
                    _enabled = false;
                }
                Logger::log(std::format("Triggered the trigger event by ID {}, Elapsed count: {}",
                                        _trigger_id, _run_count));
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            _mutex.unlock();
        }
        Logger::log(std::format("Trigger ID {} is stopped!", _trigger_id));
    }
}