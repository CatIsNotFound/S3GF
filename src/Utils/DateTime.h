#pragma once
#ifndef MYENGINE_DATETIME_H
#define MYENGINE_DATETIME_H
#include "Logger.h"

namespace MyEngine {
    class DateTime {
        static std::string timezone;
    public:
        DateTime() = delete;
        ~DateTime() = delete;
        DateTime(const DateTime&) = delete;
        DateTime(DateTime&&) = delete;
        DateTime& operator=(const DateTime&) = delete;
        DateTime& operator=(DateTime&&) = delete;

        struct DT {
            uint16_t year;
            uint8_t month;
            uint8_t day;
            uint8_t hour;
            uint8_t minute;
            uint8_t second;
        };

        enum Weekday {
            Monday,
            Mon = 0,
            Tuesday,
            Tue = 1,
            Wednesday,
            Wed = 2,
            Thursday,
            Thu = 3,
            Friday,
            Fri = 4,
            Saturday,
            Sat = 5,
            Sunday,
            Sun = 6
        };

        enum Month {
            January,
            Jan = 0,
            February,
            Feb = 1,
            March,
            Mar = 2,
            April,
            Apr = 3,
            May,
            June,
            Jun = 5,
            July,
            Jul = 6,
            August,
            Aug = 7,
            September,
            Sep = 8,
            October,
            Oct = 9,
            November,
            Nov = 10,
            December,
            Dec = 11
        };

        static const char* weekdayStr(Weekday weekday, bool short_name = false) {
            switch (weekday) {
                case Mon:
                    return (short_name ? "Mon" : "Monday");
                case Tue:
                    return (short_name ? "Tue" : "Tuesday");
                case Wed:
                    return (short_name ? "Wed" : "Wednesday");
                case Thu:
                    return (short_name ? "Thur" : "Thursday");
                case Fri:
                    return (short_name ? "Fri" : "Friday");
                case Sat:
                    return (short_name ? "Sat" : "Saturday");
                case Sun:
                    return (short_name ? "Sun" : "Sunday");
                default:
                    return "";
            }
        }

        static const char* monthStr(Month month, bool short_name = false) {
            switch (month) {
                case Jan:
                    return (short_name ? "Jan" : "January");
                case Feb:
                    return (short_name ? "Feb" : "February");
                case Mar:
                    return (short_name ? "Mar" : "March");
                case Apr:
                    return (short_name ? "Apr" : "April");
                case May:
                    return "May";
                case Jun:
                    return (short_name ? "Jun" : "June");
                case Jul:
                    return (short_name ? "Jul" : "July");
                case Aug:
                    return (short_name ? "Aug" : "August");
                case Sep:
                    return (short_name ? "Sep" : "September");
                case Oct:
                    return (short_name ? "Oct" : "October");
                case Nov:
                    return (short_name ? "Nov" : "November");
                case Dec:
                    return (short_name ? "Dec" : "December");
                default:
                    return "";
            }
        }

        static bool setDefaultTimezone(const std::string& tz) {
            try {
                std::chrono::locate_zone(tz);
                timezone = tz;
            } catch (const std::runtime_error& e) {
                Logger::log(std::format("DateTime: Can't set invalid timezone: {}", tz), Logger::ERROR);
                return false;
            }
            return true;
        }

        static std::string_view currentTimezone() {
            return timezone;
        }

        static uint64_t currentTimestamp() {
            auto now = std::chrono::system_clock::now();
            return now.time_since_epoch().count();
        }

        static std::string now() {
            auto now = std::chrono::system_clock::now();
            auto zoned = std::chrono::zoned_time(timezone, now);
            return std::format("{}", zoned);
        }

        static DT currentDateTime() {
            using namespace std::chrono;
            auto now = system_clock::now();
            auto zoned = zoned_time(timezone, now);
            auto local_time = zoned.get_local_time();
            auto day_time = floor<days>(local_time);
            year_month_day ymy{day_time};
            hh_mm_ss time{local_time - day_time};
            DT _ret{};
            _ret.year = int(ymy.year());
            _ret.month = static_cast<uint8_t>(static_cast<unsigned>(ymy.month()));
            _ret.day = static_cast<uint8_t>(static_cast<unsigned>(ymy.day()));
            _ret.hour = static_cast<uint8_t>(time.hours().count());
            _ret.minute = static_cast<uint8_t>(time.minutes().count());
            _ret.second = static_cast<uint8_t>(time.seconds().count());
            return _ret;
        }

        static uint64_t generateTimestamp(const DT& datetime) {
            using namespace std::chrono;
            year y(datetime.year);
            month m(datetime.month);
            day d(datetime.day);
            year_month_day ymd(y, m, d);
            auto days = sys_days{ymd};
            if (!ymd.ok()) {
                Logger::log("DateTime: Current date is not valid!", Logger::ERROR);
                return 0;
            }
            auto time = hours(datetime.hour) + minutes(datetime.minute) + seconds(datetime.second);
            auto time_point = days + time;
            return time_point.time_since_epoch().count();
        }

        static DT parseFromTimestamp(uint64_t timestamp) {
            using namespace std::chrono;
            auto now = system_clock::time_point(seconds(timestamp));

            auto day_time = floor<days>(now);
            year_month_day ymy{day_time};
            hh_mm_ss time{now - day_time};
            DT _ret{};
            _ret.year = int(ymy.year());
            _ret.month = static_cast<uint8_t>(static_cast<unsigned>(ymy.month()));
            _ret.day = static_cast<uint8_t>(static_cast<unsigned>(ymy.day()));
            _ret.hour = static_cast<uint8_t>(time.hours().count());
            _ret.minute = static_cast<uint8_t>(time.minutes().count());
            _ret.second = static_cast<uint8_t>(time.seconds().count());
            return _ret;
        }

        static std::string formatDateTime(const DT& datetime, const std::string& format) {
            using namespace std::chrono;
            year y(datetime.year);
            month m(datetime.month);
            day d(datetime.day);
            year_month_day ymd(y, m, d);
            if (!ymd.ok()) {
                Logger::log("DateTime: Current date is not valid!", Logger::ERROR);
                return 0;
            }
            auto t_sys = sys_days{ymd};
            weekday w_day = weekday{t_sys};
            std::string output;
            for (size_t p = 0; p < format.size();) {
                if (format[p] != '%') {
                    output += format[p++];
                    continue;
                }
                auto sub_str = format.substr(p, 2);
                p += 2;
                if (sub_str == "%Y") {
                    output += std::to_string(datetime.year);
                } else if (sub_str == "%y") {
                    output += std::format("{:02d}", (datetime.year % 100));
                } else if (sub_str == "%m") {
                    output += std::format("{:02d}", datetime.month);
                } else if (sub_str == "%d") {
                    output += std::format("{:02d}", datetime.day);
                } else if (sub_str == "%H") {
                    output += std::format("{:02d}", datetime.hour);
                } else if (sub_str == "%I") {
                    output += std::format("{:02d}", datetime.hour % 12);
                } else if (sub_str == "%M") {
                    output += std::format("{:02d}", datetime.minute);
                } else if (sub_str == "%S") {
                    output += std::format("{:02d}", datetime.second);
                } else if (sub_str == "%a") {
                    output += std::format("{}", weekdayStr(static_cast<Weekday>(w_day.iso_encoding()), true));
                } else if (sub_str == "%A") {
                    output += std::format("{}", weekdayStr(static_cast<Weekday>(w_day.iso_encoding()), false));
                } else if (sub_str == "%b") {
                    output += std::format("{}", monthStr(static_cast<Month>(datetime.month), true));
                } else if (sub_str == "%B") {
                    output += std::format("{}", monthStr(static_cast<Month>(datetime.month), false));
                } else if (sub_str == "%C") {
                    output += std::format("{:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}",
                                          datetime.year, datetime.month, datetime.day,
                                          datetime.hour, datetime.minute, datetime.second);
                } else if (sub_str == "%E") {
                    output += std::format("{} {} {:02d} {:02d}:{:02d}:{:02d} {} {:04d}",
                          weekdayStr(static_cast<Weekday>(w_day.iso_encoding()), true),
                             monthStr(static_cast<Month>(datetime.month), true),
                             datetime.day,
                             datetime.hour,
                             datetime.minute,
                             datetime.second,
                             (datetime.hour / 12 ? "PM" : "AM"),
                             datetime.year);
                }
            }
            return output;
        }

        static std::string formatCurrentDateTime(const std::string& format) {
            auto now = std::chrono::system_clock::now();
            auto zoned = std::chrono::zoned_time(timezone, now);
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


    };

    inline std::string DateTime::timezone{std::chrono::current_zone()->name()};
}

#endif //MYENGINE_DATETIME_H
