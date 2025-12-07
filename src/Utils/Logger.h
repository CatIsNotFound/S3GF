#pragma once
#ifndef MYENGINE_LOGGER_H
#define MYENGINE_LOGGER_H
#include "../Libs.h"

namespace MyEngine {
    class Logger {
    public:
        enum LogLevel {
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        Logger() = delete;

        Logger(Logger &&) = delete;

        Logger(const Logger &) = delete;

        Logger &operator=(Logger &&) = delete;

        Logger &operator=(const Logger &) = delete;

        ~Logger() = delete;

        static void log(const std::string &message, LogLevel level = DEBUG) {
            _running_time = std::chrono::system_clock::now().time_since_epoch().count();
            if (level < _base_level) return;
            auto _real_time = (float) (_running_time - _started_time) / 1e9;
            if (level >= WARN) {
                std::cerr << std::format("[{:.06f}] [{}] {}\n", _real_time, _logLevelToString(level), message);
            } else {
                std::cout << std::format("[{:.06f}] [{}] {}\n", _real_time, _logLevelToString(level), message);
            }
            _last_log_level = level;
            _last_log_info = message;
        }

        static void setBaseLogLevel(LogLevel level) {
            _base_level = level;
        }

        static std::string lastError() {
            return (_last_log_level >= ERROR ? _last_log_info : "");
        }

    private:
        static std::string _logLevelToString(LogLevel level) {
            switch (level) {
                case DEBUG:
                    return "DEBUG";
                case INFO:
                    return "INFO";
                case WARN:
                    return "WARN";
                case ERROR:
                    return "ERROR";
                case FATAL:
                    return "FATAL";
                default:
                    return "UNKNOWN";
            }
        }

        static LogLevel _base_level;
        static LogLevel _last_log_level;
        static std::string _last_log_info;
        static uint64_t _started_time;
        static uint64_t _running_time;
    };

    inline Logger::LogLevel Logger::_base_level = Logger::INFO;
    inline Logger::LogLevel Logger::_last_log_level = Logger::DEBUG;
    inline uint64_t Logger::_started_time = std::chrono::system_clock::now().time_since_epoch().count();
    inline uint64_t Logger::_running_time = 0;
    inline std::string Logger::_last_log_info;
}
#endif //MYENGINE_LOGGER_H
