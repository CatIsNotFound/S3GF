
#ifndef S3GF_DATETIME_H
#define S3GF_DATETIME_H
#include "Libs.h"

namespace S3GF {
    class DateTime {
    public:
        DateTime() = delete;
        ~DateTime() = delete;
        DateTime(const DateTime&) = delete;
        DateTime(DateTime&&) = delete;
        DateTime& operator=(const DateTime&) = delete;
        DateTime& operator=(DateTime&&) = delete;

        struct DT {
            uint32_t year;
            uint8_t month;
            uint8_t day;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
        };

        static DT getNow() {
            auto now = std::chrono::system_clock::now();
            auto zoned = std::chrono::zoned_time(std::chrono::current_zone(), now);
            auto local_days = std::chrono::floor<std::chrono::days>(zoned.get_local_time());
            std::chrono::year_month_day local_ymd(local_days);
            std::chrono::hh_mm_ss<std::chrono::seconds> local_time(std::chrono::floor<std::chrono::seconds>(zoned.get_local_time()));

            return _ret;
        }

        static std::string now() {
            auto now = std::chrono::system_clock::now();
            auto zoned = std::chrono::zoned_time(std::chrono::current_zone(), now);
            return std::format("{}", zoned);
        }

        static uint64_t timestamp() {
            return std::chrono::system_clock::now().time_since_epoch().count();
        }

        static std::string formatDateTime(const std::string& format) {
            auto now = std::chrono::system_clock::now();
            auto zoned = std::chrono::zoned_time(std::chrono::current_zone(), now);
            std::string output;
            size_t p = 0;
            for (; p < format.size();) {
                if (format[p] != '%') {
                    output += format[p++];
                    continue;
                }
                auto sub_str = format.substr(p, 2);
                p += 2;
                if (sub_str == "%Y") {
                    output += std::format("{:%Y}", zoned);
                } else if (sub_str == "%y") {
                    output += std::format("{:%y}", zoned);
                } else if (sub_str == "%m") {
                    output += std::format("{:%m}", zoned);
                } else if (sub_str == "%d") {
                    output += std::format("{:%d}", zoned);
                } else if (sub_str == "%H") {
                    output += std::format("{:%H}", zoned);
                } else if (sub_str == "%p") {
                    output += std::format("{:%p}", zoned);
                } else if (sub_str == "%I") {
                    output += std::format("{:%I}", zoned);
                } else if (sub_str == "%M") {
                    output += std::format("{:%M}", zoned);
                } else if (sub_str == "%S") {
                    output += std::format("{:%S}", zoned);
                } else if (sub_str == "%a") {
                    output += std::format("{:%a}", zoned);
                } else if (sub_str == "%A") {
                    output += std::format("{:%A}", zoned);
                } else if (sub_str == "%b") {
                    output += std::format("{:%b}", zoned);
                } else if (sub_str == "%B") {
                    output += std::format("{:%B}", zoned);
                } else if (sub_str == "%Z") {
                    output += std::format("{:%Z}", zoned);
                } else if (sub_str == "%C") {
                    output += std::format("{:%Y/%m/%d %H:%M:%S}", zoned);
                } else if (sub_str == "%E") {
                    output += std::format("{:%a %b %d %I:%M:%S %p %Z %Y}", zoned);
                }
            }
            return output;
        }

    private:

    };
}

#endif //S3GF_DATETIME_H
