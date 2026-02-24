#pragma once
#ifndef MYENGINE_ALGORITHM_STRING_H
#define MYENGINE_ALGORITHM_STRING_H
#include "../Basic.h"

namespace MyEngine {
    namespace Algorithm {
        inline static std::string mergeStringList(const StringList& string_list, bool reverse = false) {
            std::string output;
            for (auto& s : string_list) {
                if (reverse) output.assign(s + output);
                else output += s;
            }
            return output;
        }

        inline static std::string multiplicationString(const char* str, uint32_t count = 1) {
            std::string output;
            while (count--) {
                output += str;
            }
            return output;
        }

        inline static StringList splitUTF8(const std::string& utf8_string) {
            StringList _ret;
            size_t char_length = 0;
            for (size_t i = 0; i < utf8_string.size();) {
                const auto byte = static_cast<uint8_t>(utf8_string[i]);
                if ((byte & 0x80) == 0) {
                    char_length = 1;
                } else if ((byte & 0xe0) == 0xc0) {
                    char_length = 2;
                } else if ((byte & 0xf0) == 0xe0) {
                    char_length = 3;
                } else if ((byte & 0xf8) == 0xf0) {
                    char_length = 4;
                } else {
                    Logger::log(Logger::Warn, "splitUTF8: "
                                              "The specified character is not valid! Returned null!");
                    return {};
                }
                _ret.emplace_back(utf8_string.substr(i, char_length));
                i += char_length;
            }
            return _ret;
        }

        inline static BinaryArray number2Bin(size_t number) {
            BinaryArray _ret;
            while (number) {
                _ret.push_back(number & 0xff);
                number >>= 8;
            }
            std::reverse(_ret.begin(), _ret.end());
            return _ret;
        }

        inline static size_t bin2Number(const BinaryArray& number) {
            size_t ret = 0;
            for (size_t i = 0; i < number.size(); ++i) {
                ret |= number[i] << (8 * (number.size() - i - 1));
            }
            return ret;
        }

        inline static BinaryArray stringToBinary(const std::string& string) {
            BinaryArray ret(string.size());
            memcpy(ret.data(), string.data(), ret.size());
            return ret;
        }

        inline static std::string binaryToString(const BinaryArray& binary) {
            std::string ret(binary.size(), '\0');
            memcpy(ret.data(), binary.data(), ret.size());
            return ret;
        }

        inline static BinaryArray int8ToBinary(int8_t number) {
            BinaryArray ret(sizeof(int8_t));
            memcpy(ret.data(), &number, sizeof(int8_t));
            return ret;
        }

        inline static BinaryArray uint8ToBinary(uint8_t number) {
            BinaryArray ret(sizeof(uint8_t));
            memcpy(ret.data(), &number, sizeof(uint8_t));
            return ret;
        }

        inline static BinaryArray int16ToBinary(int16_t number) {
            BinaryArray ret(sizeof(int16_t));
            memcpy(ret.data(), &number, sizeof(int16_t));
            return ret;
        }

        inline static BinaryArray uint16ToBinary(uint16_t number) {
            BinaryArray ret(sizeof(uint16_t));
            memcpy(ret.data(), &number, sizeof(uint16_t));
            return ret;
        }

        inline static BinaryArray int32ToBinary(int32_t number) {
            BinaryArray ret(sizeof(int32_t));
            memcpy(ret.data(), &number, sizeof(int32_t));
            return ret;
        }

        inline static BinaryArray uint32ToBinary(uint32_t number) {
            BinaryArray ret(sizeof(uint32_t));
            memcpy(ret.data(), &number, sizeof(uint32_t));
            return ret;
        }

        inline static BinaryArray int64ToBinary(int64_t number) {
            BinaryArray ret(sizeof(int64_t));
            memcpy(ret.data(), &number, sizeof(int64_t));
            return ret;
        }

        inline static BinaryArray uint64ToBinary(uint64_t number) {
            BinaryArray ret(sizeof(uint64_t));
            memcpy(ret.data(), &number, sizeof(uint64_t));
            return ret;
        }

        inline static BinaryArray floatToBinary(float number) {
            BinaryArray ret(sizeof(float));
            memcpy(ret.data(), &number, sizeof(float));
            return ret;
        }

        inline static BinaryArray doubleToBinary(double number) {
            BinaryArray ret(sizeof(double));
            memcpy(ret.data(), &number, sizeof(double));
            return ret;
        }

        inline static int8_t binaryToInt8(const BinaryArray& number) {
            if (number.size() != sizeof(int8_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (int8_t = 1)");
            }
            int8_t ret = 0;
            memcpy(&ret, number.data(), sizeof(int8_t));
            return ret;
        }

        inline static uint8_t binaryToUInt8(const BinaryArray& number) {
            if (number.size() != sizeof(uint8_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (uint8_t = 1)");
            }
            uint8_t ret = 0;
            memcpy(&ret, number.data(), sizeof(uint8_t));
            return ret;
        }

        inline static int16_t binaryToInt16(const BinaryArray& number) {
            if (number.size() != sizeof(int16_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (int16_t = 2)");
            }
            int16_t ret = 0;
            memcpy(&ret, number.data(), sizeof(int16_t));
            return ret;
        }

        inline static uint16_t binaryToUInt16(const BinaryArray& number) {
            if (number.size() != sizeof(uint16_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (uint16_t = 2)");
            }
            uint16_t ret = 0;
            memcpy(&ret, number.data(), sizeof(uint16_t));
            return ret;
        }

        inline static int32_t binaryToInt32(const BinaryArray& number) {
            if (number.size() != sizeof(int32_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (int32_t = 4)");
            }
            int32_t ret = 0;
            memcpy(&ret, number.data(), sizeof(int32_t));
            return ret;
        }

        inline static uint32_t binaryToUInt32(const BinaryArray& number) {
            if (number.size() != sizeof(uint32_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (uint32_t = 4)");
            }
            uint32_t ret = 0;
            memcpy(&ret, number.data(), sizeof(uint32_t));
            return ret;
        }

        inline static int64_t binaryToInt64(const BinaryArray& number) {
            if (number.size() != sizeof(int64_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (int64_t = 8)");
            }
            int64_t ret = 0;
            memcpy(&ret, number.data(), sizeof(int64_t));
            return ret;
        }

        inline static uint64_t binaryToUInt64(const BinaryArray& number) {
            if (number.size() != sizeof(uint64_t)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (uint64_t = 8)");
            }
            uint64_t ret = 0;
            memcpy(&ret, number.data(), sizeof(uint64_t));
            return ret;
        }

        inline static float binaryToFloat(const BinaryArray& number) {
            if (number.size() != sizeof(float)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (float = 4)");
            }
            float ret = 0;
            memcpy(&ret, number.data(), sizeof(float));
            return ret;
        }

        inline static double binaryToDouble(const BinaryArray& number) {
            if (number.size() != sizeof(double)) {
                throw InvalidArgumentException("binaryToNumber: Invalid number size! (double = 8)");
            }
            double ret = 0;
            memcpy(&ret, number.data(), sizeof(double));
            return ret;
        }
    }
}

#endif //MYENGINE_ALGORITHM_STRING_H
