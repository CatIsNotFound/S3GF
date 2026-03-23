#pragma once
#ifndef MYENGINE_UTILS_DATETIME_H
#define MYENGINE_UTILS_DATETIME_H
#include "Logger.h"

namespace MyEngine {
    /**
     * \if EN
     * @brief DateTime
     * @details Used for handling date and time-related functions,
     * supporting retrieving the current datetime,
     * formatting datetime, and converting between timestamps and datetime.
     * @note This class is a static class and does not require obtaining a global singleton.
     * \endif
     */
    class DateTime {
        inline static std::string timezone{DATE_TZ::current_zone()->name()};
    public:
        DateTime() = delete;
        ~DateTime() = delete;
        DateTime(const DateTime&) = delete;
        DateTime(DateTime&&) = delete;
        DateTime& operator=(const DateTime&) = delete;
        DateTime& operator=(DateTime&&) = delete;

        /**
         * \if EN
         * @brief Describe the days of the week
         * \endif
         */
        enum Weekday {
            Monday = 1,
            Mon = 1,
            Tuesday,
            Tue = 2,
            Wednesday,
            Wed = 3,
            Thursday,
            Thu = 4,
            Friday,
            Fri = 5,
            Saturday,
            Sat = 6,
            Sunday,
            Sun = 7
        };

        /**
         * \if EN
         * @struct MyEngine::DateTime::DT
         * @brief Date and time
         * @details Used to obtain information describing date and time
         * such as year, month, day, hour, minute, and second
         * \endif
         */
        struct DT {
            uint16_t year;
            /// 1~12
            uint8_t month;
            /// 1~31
            uint8_t day;
            /// 0~23
            uint8_t hour;
            /// 0~59
            uint8_t minute;
            /// 0~59
            uint8_t second;
            /// @see Weekday
            Weekday weekday;

            bool operator==(const DT& other) {
                if (this->year != other.year) return false;
                if (this->month != other.month) return false;
                if (this->day != other.day) return false;
                if (this->hour != other.hour) return false;
                if (this->minute != other.minute) return false;
                if (this->second != other.second) return false;
                return true;
            }

            bool operator!=(const DT& other) {
                return !operator==(other);
            }
        };

        enum Month {
            January = 1,
            Jan = 1,
            February,
            Feb = 2,
            March,
            Mar = 3,
            April,
            Apr = 4,
            May,
            June,
            Jun = 6,
            July,
            Jul = 7,
            August,
            Aug = 8,
            September,
            Sep = 9,
            October,
            Oct = 10,
            November,
            Nov = 11,
            December,
            Dec = 12
        };

        /**
         * \if EN
         * @brief unit of time
         * \endif
         */
        enum TimeBase {
            Nanoseconds,
            Microseconds,
            Milliseconds,
            Seconds,
            Minutes,
            Hours,
            Days
        };

        /**
         * \if EN
         * @brief Get the weekday as a string
         * @param weekday       Specified weekday
         * @param short_name    Whether to return the abbreviation (default is `false`)
         * @return Returns a string representing the converted weekday
         * \endif
         * @code
         * auto weekday = MyEngine::DateTime::weekdayStr(MyEngine::DateTime::Sunday, false);
         * auto short_weekday = MyEngine::DateTime::weekdayStr(MyEngine::DateTime::Sun, true);
         * // Output:
         * //       weekday = "Sunday";
         * // short_weekday = "Sun";
         * @endcode
         * @see mouthStr
         */
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

        /**
         * \if EN
         * @brief Get the month as a string
         * @param month         Specified month
         * @param short_name    Whether to return the abbreviation (default is `false`)
         * @return Returns a string representing the converted month
         * \endif
         * @code
         * auto month = MyEngine::DateTime::monthStr(MyEngine::DateTime::December, false);
         * auto short_month = MyEngine::DateTime::monthStr(MyEngine::DateTime::Dec, true);
         * // Output:
         * //       month = "December";
         * // short_month = "Dec";
         * @endcode
         * @see weekdayStr
         */
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

        /**
         * \if EN
         * @brief Get all supported time zone names
         * @return Return a list consisting of multiple time zone names
         *
         * \endif
         * @see setDefaultTimezone
         * @see currentTimezone
         */
        static std::vector<std::string_view> timezoneList() {
            static std::vector<std::string_view> _out;
            if (_out.empty()) {
                for (auto &zone: DATE_TZ::get_tzdb().zones) {
                    _out.emplace_back(zone.name());
                }
            }
            return _out;
        }

        /**
         * \if EN
         * @brief Set default time zone
         * @param tz    Specify the time zone (usually `Asia/Shanghai`, `America/New_York`, `UTC`, etc.)
         * @return Returns `true` if the default time zone was set successfully, otherwise `false`.
         * \endif
         * @code
         * MyEngine::DateTime::setDefaultTimezone("Asia/Shanghai");
         * @endcode
         * @see currentTimezone
         * @see timezoneList
         */
        static bool setDefaultTimezone(const std::string& tz) {
            try {
                DATE_TZ::locate_zone(tz);
                timezone = tz;
            } catch (const std::runtime_error& e) {
                Logger::log(FMT::format("DateTime: Can't set invalid timezone! Exception: {}", e.what()),
                            Logger::Error);
                return false;
            }
            return true;
        }


        /**
         * \if EN
         * @brief Get the current timezone
         * @return Return the time zone name using a string
         * \endif
         * @see setDefaultTimezone
         * @see timezoneList
         */
        static std::string_view currentTimezone() {
            return timezone;
        }

        /**
         * \if EN
         * @brief Get the current timestamp
         * @param timebase  Specify a time unit (Default: `Seconds`)
         * @return Return the converted timestamp
         * \endif
         * @see TimeBase
         * @see generateTimestamp
         * @see parseFromTimestamp
         */
        static uint64_t currentTimestamp(TimeBase timebase = Seconds) {
            using namespace std::chrono;
            if (timebase == Nanoseconds) {
                return duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
            } else if (timebase == Microseconds) {
                return duration_cast<microseconds>(system_clock::now().time_since_epoch()).count();
            } else if (timebase == Milliseconds) {
                return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
            } else if (timebase == Seconds) {
                return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
            } else if (timebase == Minutes) {
                return duration_cast<minutes>(system_clock::now().time_since_epoch()).count();
            } else if (timebase == Hours) {
                return duration_cast<hours>(system_clock::now().time_since_epoch()).count();
            } else if (timebase == Days) {
                return duration_cast<days>(system_clock::now().time_since_epoch()).count();
            }
            return 0llu;
        }

        /**
         * \if EN
         * @brief Get the current date and time (UTC Time)
         * @return Returns a string type used to represent the current date and time
         * \endif
         * @see formatCurrentDateTime
         * @see currentDateTime
         */
        static std::string now() {
            auto now = std::chrono::system_clock::now();
            auto zoned = DATE_TZ::zoned_time(timezone, now);
            auto sys_time = zoned.get_sys_time();
            // auto day_time = DATE_TZ::floor<DATE_TZ::days>(sys_time);
            // DATE_TZ::year_month_day ymy = day_time;
            // DATE_TZ::hh_mm_ss hms(sys_time - day_time);
            return FMT::format("{}", sys_time);
        }

        /**
         * \if EN
         * @brief Get the current date and time
         * @return Returns a `DT` type used to represent the current date and time
         * \endif
         * @see DT
         * @see now
         * @see formatCurrentDateTime
         */
        static DT currentDateTime() {
            auto now = std::chrono::system_clock::now();
            auto zoned = DATE_TZ::zoned_time(timezone, now);
            auto local_time = zoned.get_local_time();
            auto day_time = floor<DATE_TZ::days>(local_time);
            DATE_TZ::year_month_day ymy{day_time};
            DATE_TZ::hh_mm_ss time{local_time - day_time};
            DATE_TZ::weekday w_day = DATE_TZ::weekday{DATE_TZ::sys_days{ymy}};
            DT _ret{};
            _ret.year = int(ymy.year());
            _ret.month = static_cast<uint8_t>(static_cast<unsigned>(ymy.month()));
            _ret.day = static_cast<uint8_t>(static_cast<unsigned>(ymy.day()));
            _ret.hour = static_cast<uint8_t>(time.hours().count());
            _ret.minute = static_cast<uint8_t>(time.minutes().count());
            _ret.second = static_cast<uint8_t>(time.seconds().count());
            auto real_weekday = (w_day.c_encoding() == 0 ? 7 : w_day.c_encoding());
            _ret.weekday = static_cast<Weekday>(real_weekday);
            return _ret;
        }

        /**
         * \if EN
         * @brief Convert the specified date and time to a timestamp
         * @param datetime      Specified datetime
         * @param time_base     Specified the unit of time
         * @return Return a converted timestamp
         * \endif
         * @see DT
         * @see TimeBase
         * @see parseFromTimestamp
         */
        static uint64_t generateTimestamp(const DT& datetime, TimeBase time_base = Seconds) {
            auto real_year = (datetime.year < 1900 ? datetime.year + 1900 : datetime.year);
            DATE_TZ::year y(real_year);
            DATE_TZ::month m(datetime.month);
            DATE_TZ::day d(datetime.day);
            DATE_TZ::year_month_day ymd(y, m, d);
            if (!ymd.ok()) {
                Logger::log("DateTime: Current date is not valid!", Logger::Error);
                return 0;
            }
            auto days = DATE_TZ::sys_days{ymd};
            if (datetime.hour > 23 || datetime.minute > 59 || datetime.second > 59) {
                Logger::log("DateTime: Invalid hour/minute/second", Logger::Error);
                return 0;
            }
            if (time_base == Days) {
                return days.time_since_epoch().count();
            }
            const auto HH = std::chrono::hours(datetime.hour);
            const auto MM = std::chrono::minutes(datetime.minute);
            const auto SS = std::chrono::seconds(datetime.second);
            auto time = HH + MM + SS;
            if (time_base == Hours) {
                auto time_point = days + HH;
                return time_point.time_since_epoch().count();
            } else if (time_base == Minutes) {
                auto time_point = days + (HH + MM);
                return time_point.time_since_epoch().count();
            }
            auto time_point = days + time;
            auto sec = time_point.time_since_epoch().count();
            switch (time_base) {
                case Nanoseconds:
                    return sec * 1000000000ull;
                case Microseconds:
                    return sec * 1000000ull;
                case Milliseconds:
                    return sec * 1000ull;
                default:
                    return sec;
            }
        }

        /**
         * \if EN
         * @brief Convert timestamp to date and time
         * @param timestamp     Specified timestamp
         * @param timebase      Specified time unit
         * @return Return the converted date and time
         * \endif
         * @see DT
         * @see TimeBase
         * @see generateTimestamp
         */
        static DT parseFromTimestamp(uint64_t timestamp, TimeBase timebase = Seconds) {
            uint64_t nanoseconds_count = 0;
            switch (timebase) {
                case Nanoseconds:
                    nanoseconds_count = timestamp;
                    break;
                case Microseconds:
                    nanoseconds_count = timestamp * 1000;
                    break;
                case Milliseconds:
                    nanoseconds_count = timestamp * 1000000ull;
                    break;
                case Seconds:
                    nanoseconds_count = timestamp * 1000000000ull;
                    break;
                case Minutes:
                    nanoseconds_count = timestamp * 60000000000ull;
                    break;
                case Hours:
                    nanoseconds_count = timestamp * 3600000000000ull;
                    break;
                case Days:
                    nanoseconds_count = timestamp * 86400000000000ull;
            }

            const std::chrono::duration<long long, std::ratio<1, 10000000>> NANO(nanoseconds_count);
            const auto sys_duration = std::chrono::duration_cast<std::chrono::system_clock::duration>(NANO);
            auto now = std::chrono::system_clock::time_point(sys_duration);

            auto day_time = floor<DATE_TZ::days>(now);
            DATE_TZ::year_month_day ymy{day_time};
            DATE_TZ::hh_mm_ss time{now - day_time};
            DATE_TZ::weekday weekday{DATE_TZ::sys_days{ymy}};
            DT _ret{};
            _ret.year = int(ymy.year());
            _ret.month = static_cast<uint8_t>(static_cast<unsigned>(ymy.month()));
            _ret.day = static_cast<uint8_t>(static_cast<unsigned>(ymy.day()));
            _ret.hour = static_cast<uint8_t>(time.hours().count());
            _ret.minute = static_cast<uint8_t>(time.minutes().count());
            _ret.second = static_cast<uint8_t>(time.seconds().count());
            auto real_weekday = (weekday.c_encoding() == 0 ? 7 : weekday.c_encoding());
            _ret.weekday = static_cast<Weekday>(real_weekday);
            return _ret;
        }

        /**
         * \if EN
         * @brief Format the specified datetime and convert it to a string
         * @param datetime  Specified datetime
         * @param format    Specified format (compatible with POSIX formatting rules)
         * @return Return a formatted string
         * @note To output '%', please include '\\%' in string to avoid parsing
         * \endif
         * @code
         * MyEngine::DateTime::DT dt(2025, 12, 28, 10, 24, 30);
         * auto s1 = MyEngine::DateTime::formatDateTime(dt, "Now: %E");
         * auto s2 = MyEngine::DateTime::formatDateTime(dt, "Time: %H:%M:%S");
         * // Output:
         * // - s1: "Now: Sun Dec 24 12:24:48 AM 2025"
         * // - s2: "Time: 12:24:48"
         * @endcode
         * @see now
         * @see formatCurrentDateTime
         */
        static std::string formatDateTime(const DT& datetime, const std::string& format) {
            auto real_year = (datetime.year < 1900 ? datetime.year + 1900 : datetime.year);
            DATE_TZ::year y(real_year);
            DATE_TZ::month m(datetime.month);
            DATE_TZ::day d(datetime.day);
            DATE_TZ::year_month_day ymd(y, m, d);
            if (!ymd.ok()) {
                Logger::log("DateTime: Current date is not valid!", Logger::Error);
                return 0;
            }
            auto t_sys = DATE_TZ::sys_days{ymd};
            auto w_day = DATE_TZ::weekday{t_sys};
            std::string output;
            for (size_t p = 0; p < format.size();) {
                if (format[p] != '%' && format[p] != '\\') {
                    output += format[p++];
                    continue;
                }
                auto sub_str = format.substr(p, 2);
                p += 2;
                if (sub_str[0] == '\\') {
                    output += sub_str[1]; continue;
                }
                if (sub_str == "%Y") {
                    output += std::to_string(real_year);
                } else if (sub_str == "%y") {
                    output += FMT::format("{:02d}", (real_year % 100));
                } else if (sub_str == "%m") {
                    output += FMT::format("{:02d}", datetime.month);
                } else if (sub_str == "%d") {
                    output += FMT::format("{:02d}", datetime.day);
                } else if (sub_str == "%H") {
                    output += FMT::format("{:02d}", datetime.hour);
                } else if (sub_str == "%I") {
                    output += FMT::format("{:02d}", datetime.hour % 12);
                } else if (sub_str == "%M") {
                    output += FMT::format("{:02d}", datetime.minute);
                } else if (sub_str == "%S") {
                    output += FMT::format("{:02d}", datetime.second);
                } else if (sub_str == "%a") {
                    output += FMT::format("{}",
                          weekdayStr(static_cast<Weekday>(w_day.iso_encoding()), true));
                } else if (sub_str == "%A") {
                    output += FMT::format("{}",
                          weekdayStr(static_cast<Weekday>(w_day.iso_encoding()), false));
                } else if (sub_str == "%b") {
                    output += FMT::format("{}",
                          monthStr(static_cast<Month>(datetime.month), true));
                } else if (sub_str == "%B") {
                    output += FMT::format("{}",
                          monthStr(static_cast<Month>(datetime.month), false));
                } else if (sub_str == "%C") {
                    output += FMT::format("{:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}",
                                          real_year, datetime.month, datetime.day,
                                          datetime.hour, datetime.minute, datetime.second);
                } else if (sub_str == "%E") {
                    output += FMT::format("{} {} {:02d} {:02d}:{:02d}:{:02d} {} {:04d}",
                          weekdayStr(static_cast<Weekday>(w_day.iso_encoding()), true),
                             monthStr(static_cast<Month>(datetime.month), true),
                             datetime.day,
                             datetime.hour,
                             datetime.minute,
                             datetime.second,
                             (datetime.hour / 12 ? "PM" : "AM"),
                             real_year);
                }
            }
            return output;
        }

        /**
         * \if EN
         * @brief Format the current datetime and convert it to a string
         * @param format    Specified format (compatible with POSIX formatting rules)
         * @return Return a formatted string
         * @note To output `%`, please include `\\%` in string to avoid parsing
         * \endif
         * @code
         * auto now = MyEngine::DateTime::formatCurrentDateTime("Now: %C");
         * // output:
         * //    - now = "Now: 2025/12/24 12:34:56 UTC"
         * @endcode
         * @see formatDateTime
         * @see now
         */
        static std::string formatCurrentDateTime(const std::string& format) {
            auto zoned = DATE_TZ::zoned_time(timezone, std::chrono::system_clock::now());
            auto local_time = zoned.get_local_time();
            auto day_time = DATE_TZ::floor<DATE_TZ::days>(local_time);
            DATE_TZ::year_month_day ymy(day_time);
            DATE_TZ::hh_mm_ss hms(local_time - day_time);
            DATE_TZ::weekday weekday(day_time);
            std::string output;
            size_t p = 0;
            for (; p < format.size();) {
                if (format[p] != '%' && format[p] != '\\') {
                    output += format[p++];
                    continue;
                }
                auto sub_str = format.substr(p, 2);
                p += 2;
                if (sub_str[0] == '\\') {
                    output += sub_str[1]; continue;
                }
                if (sub_str == "%Y") {
                    output += FMT::format("{:04d}", (int)ymy.year());
                } else if (sub_str == "%y") {
                    output += FMT::format("{:02d}", (int)ymy.year() % 100);
                } else if (sub_str == "%m") {
                    output += FMT::format("{:02d}", (unsigned)ymy.month());
                } else if (sub_str == "%d") {
                    output += FMT::format("{:02d}", (unsigned)ymy.day());
                } else if (sub_str == "%H") {
                    output += FMT::format("{:02d}", (unsigned)hms.hours().count());
                } else if (sub_str == "%p") {
                    output += FMT::format("{}", hms.hours().count() > 12 ? "PM" : "AM");
                } else if (sub_str == "%I") {
                    output += FMT::format("{:02d}", hms.hours().count() % 12);
                } else if (sub_str == "%M") {
                    output += FMT::format("{:02d}", hms.minutes().count());
                } else if (sub_str == "%S") {
                    output += FMT::format("{:02d}", hms.seconds().count());
                } else if (sub_str == "%a") {
                    output += FMT::format("{}", weekdayStr(Weekday(weekday.iso_encoding()), true));
                } else if (sub_str == "%A") {
                    output += FMT::format("{}", weekdayStr(Weekday(weekday.iso_encoding())));
                } else if (sub_str == "%b") {
                    output += FMT::format("{}", monthStr(Month((unsigned)ymy.month()), true));
                } else if (sub_str == "%B") {
                    output += FMT::format("{}", monthStr(Month((unsigned)ymy.month())));
                } else if (sub_str == "%Z") {
                    output += FMT::format("{}", zoned.get_info().abbrev);
                } else if (sub_str == "%C") {
                    output += FMT::format("{:04d}/{:02d}/{:02d} {:02d}:{:02d}:{:02d}",
                                          (int)ymy.year(), (unsigned)ymy.month(), (unsigned)ymy.day(),
                                          hms.hours().count(), hms.minutes().count(), hms.seconds().count());
                } else if (sub_str == "%E") {
                    output += FMT::format("{} {} {:02d} {:02d}:{:02d}:{:02d} {} {:04d}",
                                          weekdayStr(static_cast<Weekday>(weekday.iso_encoding()), true),
                                          monthStr(Month((unsigned)ymy.month()), true),
                                          (unsigned)ymy.day(),
                                          hms.hours().count(),
                                          hms.minutes().count(),
                                          hms.seconds().count(),
                                          (hms.hours().count() / 12 ? "PM" : "AM"),
                                          (int)ymy.year());
                }
            }
            return output;
        }
    };
}

#endif //MYENGINE_UTILS_DATETIME_H
