#pragma once
#ifndef MYENGINE_UTILS_VARIANT_H
#define MYENGINE_UTILS_VARIANT_H
#include "../Libs.h"
#include "../Exception.h"
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

        struct CustomPointer {
        private:
            size_t* _reference_count;
            void* _pointer;
            std::function<void(void*)> _deleter{};
            uint32_t _custom_type_id;

        public:
            explicit CustomPointer() : _reference_count(), _pointer(), _custom_type_id(0) {}
            explicit CustomPointer(void* pointer, const std::function<void(void*)>& deleter = {});
            explicit CustomPointer(void* pointer, size_t custom_type_id,
                                    const std::function<void(void*)>& deleter = {});
            explicit CustomPointer(const CustomPointer& custom_pointer);
            explicit CustomPointer(CustomPointer&& custom_pointer) noexcept;
            ~CustomPointer();
            CustomPointer& operator=(const CustomPointer&);
            CustomPointer& operator=(CustomPointer&&) noexcept;

            void reset();
            void reset(void* pointer, const std::function<void(void*)> &deleter = {});
            void reset(void* pointer, std::function<void(void*)> &&deleter = {}) noexcept;
            void reset(void* pointer, size_t custom_type_id, const std::function<void(void*)>& deleter = {});
            void reset(void* pointer, size_t custom_type_id, std::function<void(void*)>&& deleter = {}) noexcept;
            [[nodiscard]] size_t usedCount() const;
            [[nodiscard]] size_t customTypeID() const;
            void setCustomTypeID(size_t type_id);
            [[nodiscard]] void* get() const;
        private:
            void initialize(void* pointer, const std::function<void(void*)> &deleter = {});
            void destroy();
            void copy(const CustomPointer& ptr);
            void copy(CustomPointer&& ptr) noexcept;
        };
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
        explicit Variant(const Variant&);
        explicit Variant(Variant&&) noexcept;
        Variant& operator=(const Variant&);
        Variant& operator=(Variant&&) noexcept;
        ~Variant();
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
        explicit Variant(void* pointer, const std::function<void(void*)> &deleter = {})
            : _type(Pointer), _value(), _pointer(pointer, deleter) {}
        explicit Variant(void* pointer, uint32_t custom_type_id, const std::function<void(void*)> &deleter = {})
            : _type(Pointer), _value(), _pointer(pointer, custom_type_id, deleter){}
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

        void setCustomTypeID(uint32_t type_id);
        [[nodiscard]] uint32_t customTypeID() const;

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
        void setValue(void* pointer, uint32_t custom_type_id, const std::function<void(void*)> &deleter = {});
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
        template <typename T>
        [[nodiscard]] T* toCustomPointer() const {
            if (_type != Pointer) {
                throw BadValueException(FMT::format("Variant: The variant can not convert to pointer!"));
            }
            return static_cast<T*>(_pointer.get());
        }
        [[nodiscard]] size_t usedCount() const;

        [[nodiscard]] std::string valueAsString(const std::function<std::string(void *, uint32_t)> &callback = {}) const;
        bool stringToValue(const std::string &string_value, Type var_type, const std::function<bool(void*)>& callback = {});

        [[nodiscard]] BinaryArray valueAsBinary(const std::function<BinaryArray(void *, uint32_t)> &callback = {}) const;
        bool binaryToValue(const BinaryArray& bin_value, Type var_type, const std::function<bool(void*)>& callback = {});

        template <typename T>
        static std::function<void(void*)> makeDefaultDeleter() {
            return [](void* ptr) {
                delete static_cast<T*>(ptr);
            };
        }
    private:
        Type _type;
        void* _value;
        CustomPointer _pointer{};
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
            return FMT::format_to(context.out(), "0x{:x}", reinterpret_cast<size_t>(variant.toPointer()));
        return context.out();
    }
};

#endif //MYENGINE_UTILS_VARIANT_H
