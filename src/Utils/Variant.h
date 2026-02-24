#pragma once
#ifndef MYENGINE_UTILS_VARIANT_H
#define MYENGINE_UTILS_VARIANT_H
#include "../Libs.h"
namespace MyEngine {
    /**
     * \if EN
     * @brief Variant
     * @details A general-purpose variant container for storing basic data types,
     * supporting integers, floating-point numbers, strings, and pointers.
     * @details Suitable for scenarios that require unified storage of different basic types such as integers,
     * floating-point numbers, strings, and pointers,
     * such as configuration parsing, script parameter passing, and multi-type parameter encapsulation for interfaces.
     * \endif
     */
    class Variant {
        int32_t convert2Int32() const;
        uint32_t convert2UInt32() const;
        int64_t convert2Int64() const;
        uint64_t convert2UInt64() const;
        float convert2Float() const;
        double convert2Double() const;
    public:
        /**
         * \if EN
         * @brief Data type
         * @details Used to indicate the data type stored by the variant
         * \endif
         */
        enum Type {
            Null,
            Bool,
            Int8,
            Int16,
            Int32,
            Int64,
            UInt8,
            UInt16,
            UInt32,
            UInt64,
            Float,
            Double,
            String,
            Pointer
        };
        explicit Variant() : _type(Null), _value(nullptr) {}
        explicit Variant(const Variant& v) : _type(v._type) {
            switch (_type) {
                case Null:
                    _value = nullptr;
                    break;
                case Bool:
                    _value = new bool(*static_cast<bool*>(v._value));
                    break;
                case Int8:
                    _value = new int8_t(*static_cast<int8_t*>(v._value));
                    break;
                case Int16:
                    _value = new int16_t(*static_cast<int16_t*>(v._value));
                    break;
                case Int32:
                    _value = new int32_t(*static_cast<int32_t*>(v._value));
                    break;
                case Int64:
                    _value = new int64_t(*static_cast<int64_t*>(v._value));
                    break;
                case UInt8:
                    _value = new uint8_t(*static_cast<uint8_t*>(v._value));
                    break;
                case UInt16:
                    _value = new uint16_t(*static_cast<uint8_t*>(v._value));
                    break;
                case UInt32:
                    _value = new uint32_t(*static_cast<uint32_t*>(v._value));
                    break;
                case UInt64:
                    _value = new uint64_t(*static_cast<uint64_t*>(v._value));
                    break;
                case Float:
                    _value = new float(*static_cast<float*>(v._value));
                    break;
                case Double:
                    _value = new double(*static_cast<double*>(v._value));
                    break;
                case String:
                    _value = new std::string(*static_cast<std::string*>(v._value));
                    break;
                case Pointer:
                    _value = static_cast<void*>(v._value);
                    break;
            }
        }
        ~Variant() {
            if (!_value) return;
            switch (_type) {
                case Bool:
                    delete (static_cast<bool*>(_value));
                    break;
                case Int8:
                    delete (static_cast<int8_t*>(_value));
                    break;
                case Int16:
                    delete (static_cast<int16_t*>(_value));
                    break;
                case Int32:
                    delete (static_cast<int32_t*>(_value));
                    break;
                case Int64:
                    delete (static_cast<int64_t*>(_value));
                    break;
                case UInt8:
                    delete (static_cast<uint8_t*>(_value));
                    break;
                case UInt16:
                    delete (static_cast<uint16_t*>(_value));
                    break;
                case UInt32:
                    delete (static_cast<uint32_t*>(_value));
                    break;
                case UInt64:
                    delete (static_cast<uint64_t*>(_value));
                    break;
                case Float:
                    delete (static_cast<float*>(_value));
                    break;
                case Double:
                    delete (static_cast<double*>(_value));
                    break;
                case String:
                    delete (static_cast<std::string*>(_value));
                    break;
                case Pointer:
                    if (_deleter) _deleter(_value);
                    break;
                default:
                    break;
            }
            _value = nullptr;
        }
        explicit Variant(bool v) : _type(Bool), _value(new bool(v)) {}
        explicit Variant(int8_t v) : _type(Int8), _value(new int8_t(v)) {}
        explicit Variant(int16_t v) : _type(Int16), _value(new int16_t(v)) {}
        explicit Variant(int32_t v) : _type(Int32), _value(new int32_t(v)) {}
        explicit Variant(int64_t v) : _type(Int64), _value(new int64_t(v)) {}
        explicit Variant(uint8_t v) : _type(UInt8), _value(new uint8_t(v)) {}
        explicit Variant(uint16_t v) : _type(UInt16), _value(new uint16_t(v)) {}
        explicit Variant(uint32_t v) : _type(UInt32), _value(new uint32_t(v)) {}
        explicit Variant(uint64_t v) : _type(UInt64), _value(new uint64_t(v)) {}
        explicit Variant(float v) : _type(Float), _value(new float(v)) {}
        explicit Variant(double v) : _type(Double), _value(new double(v)) {}
        explicit Variant(const char* string) : _type(String), _value(new std::string(string)) {}
        explicit Variant(const std::string& string) : _type(String), _value(new std::string(string)) {}
        explicit Variant(std::string&& string) noexcept : _type(String), _value(new std::string(string)) {}
        explicit Variant(void* pointer, std::function<void(void*)> deleter = {})
            : _type(Pointer), _value(pointer), _deleter(std::move(deleter)) {}
        /**
         * \if EN
         * @brief Get the data type stored in the current variant
         * @return Return an enum of type `Type`
         * \endif
         * @see Type
         * @see typeName
         */
        Type type() const;
        /**
         * \if EN
         * @brief Get the data type name of the current variant
         * @return Return the data type name of a string
         * \endif
         * @see Type
         * @see type
         */
        const char* typeName() const;
        /**
         * \if EN
         * @brief Check whether the current variant has no stored data
         * @return Whether the data for the current variant is empty
         * \endif
         */
        bool isNull() const;
        /**
         * \if EN
         * @brief Check if the current variant is set with a deleter to release the pointer
         * @return Returns `true` if set, otherwise `false`
         * \endif
         * @see setDeleter
         */
        bool hasDeleter() const;
        /**
         * \if EN
         * @brief Set the deleter used for releasing the pointer
         * @param deleter Specify a function for freeing a pointer
         * @details When a variant is destructed, a deleter is automatically called to release the pointer,
         * thereby preventing issues such as memory leaks.
         * @note For reference-counted pointers, there is no need to set a deleter.
         * Otherwise, it may cause the pointer to be deleted twice.
         * \endif
         * @see hasDeleter
         */
        void setDeleter(std::function<void(void*)> deleter);

        void clearValue();
        void setValue(bool v);
        void setValue(int8_t v);
        void setValue(int16_t v);
        void setValue(int32_t v);
        void setValue(int64_t v);
        void setValue(uint8_t v);
        void setValue(uint16_t v);
        void setValue(uint32_t v);
        void setValue(uint64_t v);
        void setValue(float v);
        void setValue(double v);
        void setValue(const char* string);
        void setValue(std::string& string);
        void setValue(void* pointer, std::function<void(void*)> deleter = {});
        [[nodiscard]] bool toBool() const;
        [[nodiscard]] int8_t toInt8() const;
        [[nodiscard]] int16_t toInt16() const;
        [[nodiscard]] int32_t toInt32() const;
        [[nodiscard]] int64_t toInt64() const;
        [[nodiscard]] uint8_t toUInt8() const;
        [[nodiscard]] uint16_t toUInt16() const;
        [[nodiscard]] uint32_t toUInt32() const;
        [[nodiscard]] uint64_t toUInt64() const;
        [[nodiscard]] float toFloat() const;
        [[nodiscard]] double toDouble() const;
        [[nodiscard]] std::string toString() const;
        [[nodiscard]] void* toPointer() const;

        [[nodiscard]] std::string valueAsString(const std::function<std::string(void*)>& callback = {}) const;
        bool stringToValue(const std::string &string_value, Type var_type, const std::function<bool(void*)>& callback = {});

        [[nodiscard]] BinaryArray valueAsBinary(const std::function<BinaryArray(void*)>& callback = {}) const;
        bool binaryToValue(const BinaryArray& bin_value, Type var_type, const std::function<bool(void*)>& callback = {});

    private:
        Type _type;
        void* _value;
        std::function<void(void*)> _deleter{};
    };
} // MyEngine

template<>
    struct FMT::formatter<MyEngine::Variant> {
    constexpr auto parse(FMT::format_parse_context& context) {
        return context.begin();
    }

    auto format(const MyEngine::Variant& variant, FMT::format_context& context) const {
        if (variant.type() == MyEngine::Variant::Null)
            return FMT::format_to(context.out(), "<NULL>");
        else if (variant.type() == MyEngine::Variant::Bool)
            return FMT::format_to(context.out(), "{}", variant.toBool());
        else if (variant.type() == MyEngine::Variant::Int8)
            return FMT::format_to(context.out(), "{}", variant.toInt8());
        else if (variant.type() == MyEngine::Variant::Int16)
            return FMT::format_to(context.out(), "{}", variant.toInt16());
        else if (variant.type() == MyEngine::Variant::Int32)
            return FMT::format_to(context.out(), "{}", variant.toInt32());
        else if (variant.type() == MyEngine::Variant::Int64)
            return FMT::format_to(context.out(), "{}", variant.toInt64());
        else if (variant.type() == MyEngine::Variant::UInt8)
            return FMT::format_to(context.out(), "{}", variant.toUInt8());
        else if (variant.type() == MyEngine::Variant::UInt16)
            return FMT::format_to(context.out(), "{}", variant.toUInt16());
        else if (variant.type() == MyEngine::Variant::UInt32)
            return FMT::format_to(context.out(), "{}", variant.toUInt32());
        else if (variant.type() == MyEngine::Variant::UInt64)
            return FMT::format_to(context.out(), "{}", variant.toUInt64());
        else if (variant.type() == MyEngine::Variant::Float)
            return FMT::format_to(context.out(), "{}", variant.toFloat());
        else if (variant.type() == MyEngine::Variant::Double)
            return FMT::format_to(context.out(), "{}", variant.toDouble());
        else if (variant.type() == MyEngine::Variant::String)
            return FMT::format_to(context.out(), "{}", variant.toString());
        else if (variant.type() == MyEngine::Variant::Pointer)
            return FMT::format_to(context.out(), "0x{:x}", (size_t)variant.toPointer());
        return context.out();
    }
};

#endif //MYENGINE_UTILS_VARIANT_H
