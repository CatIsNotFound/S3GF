#pragma once
#ifndef MYENGINE_UTILS_LOGGER_H
#define MYENGINE_UTILS_LOGGER_H
#include "../Libs.h"
#include "FileSystem.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif
namespace MyEngine {
    /**
     * @if EN
     * @class Logger
     * @brief A Logging Tool for MyEngine
     * @details A logging tool that supports multiple levels and customizable output
     * @note This is a static class, there is no need to obtain a global singleton.
     * @endif
     */
    class Logger {
    public:
        /**
         * @if EN
         * @enum LogLevel
         * @brief Log Level, defined the severity of logs (from Debug to Fatal)
         * @details Used for log filtering. After setting the global level, logs below this level will be ignored.
         * @endif
         * @see setBaseLogLevel
         */
        enum LogLevel : uint8_t {
            Debug,
            Info,
            Warn,
            Error,
            Fatal
        };

        Logger() = delete;
        Logger(Logger &&) = delete;
        Logger(const Logger &) = delete;
        Logger &operator=(Logger &&) = delete;
        Logger &operator=(const Logger &) = delete;
        ~Logger() = delete;

        /**
         * \if EN
         * @brief Outputs log messages as a string
         * @param message Defines the log message
         * @param level Uses the log level
         * @note Not all log messages can be output to the terminal, it depends on the minimum log output level.
         * @details This will output a line in the terminal in the format `[Runtime] [Log Level] Message Content`.
         * @details If the logging tool allows writing to a log file, the output log message will also be written to the specified log file. This is helpful for viewing log files in real time!
         * \endif
         * @code
         * MyEngine::Logger::log("This is an error message.", MyEngine::Logger::Error);
         * // e.g:
         * //   [3.801925] [ERROR] This is an error message.
         * @endcode
         * @see setBaseLogLevel
         * @see setWriteLogEnabled
         * @see setLogFileName
         * @see log(LogLevel level, std::string_view format, Args... args)
         */
        static void log(const std::string &message, LogLevel level = Debug) {
            std::unique_lock<std::mutex> _lock;
            if (level < _base_level) return;
            _running_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            auto _real_time = static_cast<float>(_running_time - _started_time) / 1e9;
            std::string log;
#ifdef __ANDROID__
            log = FMT::format("[{:.06f}] {}\n", _real_time, message);
            __android_log_print(_logLevelToAndroidLogLevel(level), "MyEngine", "%s", log.data());
#else
            if (level >= Warn) {
                log = FMT::format("[{:.06f}] [{}] {}\n", _real_time, _logLevelToString(level), message);
                std::cerr << log;
            } else {
                log = FMT::format("[{:.06f}] [{}] {}\n", _real_time, _logLevelToString(level), message);
                std::cout << log;
            }
#endif
            if (_write_log) {
                _writeLogToFile(log);
            }
            _last_log_level = level;
            _last_log_info = message;
        }

        /**
         * \if EN
         * @brief Formats and outputs log messages
         * @tparam Args Types of the passed-in arguments
         * @param level Defines the log level
         * @param format Defines the output format string, using `{}` as placeholders for the passed-in arguments, similar to the usage of `FMT::format()`
         * @param args Multiple arguments of different types to be passed in
         * @note Not all log messages can be output to the terminal, it depends on the minimum log output level.
         * @details This will output a line in the terminal in the format `[Runtime] [Log Level] Message Content`.
         * @details If the logging tool allows writing to a log file, the output log message will also be written to the specified log file. This is helpful for viewing log files in real time!
         * \endif
         * @code
         * MyEngine::Logger::log(MyEngine::Logger::Info, "Current memory in KB: {}", SysMemory::getCurProcUsedMemSize());
         * // e.g:
         * //   [3.801925] [INFO] Current memory in KB: 1002770
         * @endcode
         * @see setBaseLogLevel
         * @see setWriteLogEnabled
         * @see setLogFileName
         * @see log(const std::string &message, constLogLevel level)
         */
        template<typename ...Args>
        static void log(LogLevel level, std::string_view format, Args... args) {
            std::unique_lock<std::mutex> _lock;
            if (level < _base_level) return;
            _running_time = std::chrono::high_resolution_clock::now().time_since_epoch().count();
            auto _real_time = (float) (_running_time - _started_time) / 1e9;
#ifdef __ANDROID__
            std::string fmt = FMT::format("[{:.06f}] ", _real_time);
#else
            std::string fmt = FMT::format("[{:.06f}] [{}] ", _real_time, _logLevelToString(level));
#endif
            std::string msg;
            try {
                FMT::vformat_to(std::back_inserter(msg), format, FMT::make_format_args(args...));
#ifdef __ANDROID__
                __android_log_print(_logLevelToAndroidLogLevel(level), "MyEngine",
                                    "%s%s\n", fmt.c_str(), msg.c_str());
#else
                std::cout << fmt << msg << "\n";
#endif
                if (_write_log) {
                    _writeLogToFile(fmt);
                }
                _last_log_level = level;
                _last_log_info = msg;
            } catch (const FMT::format_error &e) {
#ifdef __ANDROID__
                _last_log_info = FMT::format("[{:.06f}] Logger: {}\n",
                                             _real_time, e.what());
                __android_log_print(ANDROID_LOG_ERROR, "MyEngine",
                                    "%s", _last_log_info.c_str());
#else
                _last_log_info = FMT::format("[{:.06f}] [{}] Logger: {}\n",
                                             _real_time, _logLevelToString(Error), e.what());
                std::cerr << _last_log_info;
#endif
                _last_log_level = Error;
            }
        }

        /**
         * \if EN
         * @brief Set the minimum level for output logs
         * @param level Specify the minimum log level
         * \endif
         */
        static void setBaseLogLevel(LogLevel level) {
            _base_level = level;
        }

        /**
         * \if EN
         * @brief Get the last log error
         * @return Returns a log error message with a log level of `Error` or higher
         * \endif
         */
        static std::string lastError() {
            return (_last_log_level >= Error ? _last_log_info : "");
        }

        /**
         * \if EN
         * @brief Sets the path for the output log file (Only used when writing logs is allowed)
         * @param file_name Sets the output file path
         * \endif
         * @code
         * MyEngine::Logger::setLogFileName("/path/to/your.log");
         *
         * @endcode
         * @see setWriteLogFileEnabled
         * @see writeLogFileEnabled
         * @see currentLogFileName
         */
        static void setLogFileName(const std::string& file_name) {
            _output_file_name = file_name;
        }

        /**
         * \if EN
         * @brief Get the current output log file path
         * @return Returns the output log file path, with the default file name being `App.log`
         * \endif
         * @see setWriteLogFileEnabled
         * @see writeLogFileEnabled
         * @see setLogFileName
         */
        static std::string currentLogFileName() {
            return _output_file_name;
        }

        /**
         * \if EN
         * @brief Set whether writing to the log file is allowed
         * @param enabled Whether it is allowed, disabled by default
         * \endif
         * @see writeLogFileEnabled
         * @see setLogFileName
         * @see currentLogFileName
         */
        static void setWriteLogFileEnabled(bool enabled) {
            _write_log = enabled;
        }

        /**
         * \if EN
         * @brief Whether writing to the log file is currently allowed
         * @return Returns `true` or `false` indicating whether writing to the log file is allowed
         * \endif
         * @see setWriteLogFileEnabled
         * @see setLogFileName
         * @see currentLogFileName
         */
        static bool writeLogFileEnabled() {
            return _write_log;
        }

    private:
        static std::string _logLevelToString(LogLevel level) {
            switch (level) {
                case Debug:
                    return "DEBUG";
                case Info:
                    return "INFO";
                case Warn:
                    return "WARN";
                case Error:
                    return "ERROR";
                case Fatal:
                    return "FATAL";
                default:
                    return "UNKNOWN";
            }
        }

#ifdef __ANDROID__
        static int _logLevelToAndroidLogLevel(LogLevel level) {
            switch (level) {
                case Debug:
                    return ANDROID_LOG_DEBUG;
                case Info:
                    return ANDROID_LOG_INFO;
                case Warn:
                    return ANDROID_LOG_WARN;
                case Error:
                    return ANDROID_LOG_ERROR;
                case Fatal:
                    return ANDROID_LOG_FATAL;
                default:
                    return ANDROID_LOG_UNKNOWN;
            }
        }
#endif

        static void _writeLogToFile(const std::string& log) {
            FILE* file = fopen(_output_file_name.c_str(), "a+");
            if (!file) {
                _write_log = false;
                Logger::log(FMT::format("Logger: Can't open file '{}' to write log! ", _output_file_name), Warn);
            }
            fprintf(file, "%s", log.c_str());
            fflush(file);
            fclose(file);
        }

        inline static LogLevel _base_level{Logger::Info};
        inline static LogLevel _last_log_level{Logger::Debug};
        inline static std::string _last_log_info{};
        inline static uint64_t _started_time
            {static_cast<uint64_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count())};
        inline static uint64_t _running_time{};
        inline static std::string _output_file_name{"./App.log"};
        inline static bool _write_log{false};
    };
}
#endif //MYENGINE_UTILS_LOGGER_H
