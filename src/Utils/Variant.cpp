#include "Variant.h"
#include "Exception.h"
#include "Algorithm/String.h"

namespace MyEngine {
    int64_t Variant::convert2Int64() const {
        if (_type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == UInt64) return *static_cast<int64_t*>(_value);
        if (_type == Float) return static_cast<int64_t>(*static_cast<float*>(_value));
        if (_type == Double) return static_cast<int64_t>(*static_cast<double*>(_value));
        if (_type == UInt8) return *static_cast<uint8_t*>(_value);
        if (_type == UInt16) return *static_cast<uint16_t*>(_value);
        if (_type == UInt32) return *static_cast<uint32_t*>(_value);
        if (_type == Int8) return *static_cast<int8_t*>(_value);
        if (_type == Int16) return *static_cast<int16_t*>(_value);
        if (_type == Int32) return *static_cast<int32_t*>(_value);
        return *static_cast<int64_t*>(_value);
    }

    uint64_t Variant::convert2UInt64() const {
        if (_type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == Int64) return *static_cast<uint64_t*>(_value);
        if (_type == Float) return static_cast<int64_t>(*static_cast<float*>(_value));
        if (_type == Double) return static_cast<int64_t>(*static_cast<double*>(_value));
        if (_type == UInt8) return *static_cast<uint8_t*>(_value);
        if (_type == UInt16) return *static_cast<uint16_t*>(_value);
        if (_type == UInt32) return *static_cast<uint32_t*>(_value);
        if (_type == Int8) return *static_cast<int8_t*>(_value);
        if (_type == Int16) return *static_cast<int16_t*>(_value);
        if (_type == Int32) return *static_cast<int32_t*>(_value);
        return *static_cast<uint64_t*>(_value);
    }

    int32_t Variant::convert2Int32() const {
        if (_type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == Int64 || _type == UInt64) return static_cast<int32_t>(*static_cast<int64_t*>(_value));
        if (_type == Float) return static_cast<int64_t>(*static_cast<float*>(_value));
        if (_type == Double) return static_cast<int64_t>(*static_cast<double*>(_value));
        if (_type == UInt8) return *static_cast<uint8_t*>(_value);
        if (_type == UInt16) return *static_cast<uint16_t*>(_value);
        if (_type == UInt32) return *static_cast<uint32_t*>(_value);
        if (_type == Int8) return *static_cast<int8_t*>(_value);
        if (_type == Int16) return *static_cast<int16_t*>(_value);
        return *static_cast<int32_t*>(_value);
    }

    uint32_t Variant::convert2UInt32() const {
        if (_type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == Int64 || _type == UInt64) return static_cast<uint32_t>(*static_cast<int64_t*>(_value));
        if (_type == Float) return static_cast<int64_t>(*static_cast<float*>(_value));
        if (_type == Double) return static_cast<int64_t>(*static_cast<double*>(_value));
        if (_type == UInt8) return *static_cast<uint8_t*>(_value);
        if (_type == UInt16) return *static_cast<uint16_t*>(_value);
        if (_type == Int8) return *static_cast<int8_t*>(_value);
        if (_type == Int16) return *static_cast<int16_t*>(_value);
        if (_type == Int32) return *static_cast<int32_t*>(_value);
        return *static_cast<uint32_t*>(_value);
    }

    float Variant::convert2Float() const {
        if (_type == Double) return static_cast<float>(*static_cast<double*>(_value));
        if (_type <= Bool || _type >= String) return 0;
        if (_type == Int64) return static_cast<float>(*static_cast<int64_t*>(_value));
        if (_type == UInt64) return static_cast<float>(*static_cast<uint64_t*>(_value));
        if (_type == UInt8) return *static_cast<uint8_t*>(_value);
        if (_type == UInt16) return *static_cast<uint16_t*>(_value);
        if (_type == UInt32) return *static_cast<uint32_t*>(_value);
        if (_type == Int8) return *static_cast<int8_t*>(_value);
        if (_type == Int16) return *static_cast<int16_t*>(_value);
        if (_type == Int32) return *static_cast<int32_t*>(_value);
        return *static_cast<float*>(_value);
    }

    double Variant::convert2Double() const {
        if (_type == Float) return static_cast<double>(*static_cast<float*>(_value));
        if (_type <= Bool || _type >= String) return 0;
        if (_type == Int64) return static_cast<double>(*static_cast<int64_t*>(_value));
        if (_type == UInt64) return static_cast<double>(*static_cast<uint64_t*>(_value));
        if (_type == UInt8) return *static_cast<uint8_t*>(_value);
        if (_type == UInt16) return *static_cast<uint16_t*>(_value);
        if (_type == UInt32) return *static_cast<uint32_t*>(_value);
        if (_type == Int8) return *static_cast<int8_t*>(_value);
        if (_type == Int16) return *static_cast<int16_t*>(_value);
        if (_type == Int32) return *static_cast<int32_t*>(_value);
        return *static_cast<double*>(_value);
    }

    Variant::CustomPointer::CustomPointer(void *pointer, const std::function<void(void *)> &deleter)
            : _reference_count(), _pointer(), _custom_type_id(0) {
        initialize(pointer, deleter);
    }

    Variant::CustomPointer::CustomPointer(void *pointer, size_t custom_type_id,
        const std::function<void(void *)> &deleter)
            : _reference_count(), _pointer(), _custom_type_id(custom_type_id) {
        initialize(pointer, deleter);
    }

    Variant::CustomPointer::CustomPointer(const CustomPointer &custom_pointer)
            : _reference_count(), _pointer(), _deleter(), _custom_type_id(custom_pointer._custom_type_id) {
        copy(custom_pointer);
    }

    Variant::CustomPointer::CustomPointer(CustomPointer &&custom_pointer) noexcept
            : _reference_count(), _pointer(), _deleter(), _custom_type_id(custom_pointer._custom_type_id) {
        copy(std::move(custom_pointer));
    }

    Variant::CustomPointer::~CustomPointer() {
        destroy();
    }

    Variant::CustomPointer& Variant::CustomPointer::operator=(const CustomPointer &ptr) {
        copy(ptr);
        return *this;
    }

    Variant::CustomPointer &Variant::CustomPointer::operator=(CustomPointer && ptr) noexcept {
        copy(std::move(ptr));
        return *this;
    }

    void Variant::CustomPointer::reset() {
        destroy();
        _custom_type_id = 0;
    }

    void Variant::CustomPointer::reset(void *pointer, const std::function<void(void *)> &deleter) {
        destroy();
        initialize(pointer, deleter);
        _custom_type_id = 0;
    }

    void Variant::CustomPointer::reset(void *pointer, std::function<void(void *)> &&deleter) noexcept {
        destroy();
        initialize(pointer, deleter);
        _custom_type_id = 0;
    }

    void Variant::CustomPointer::reset(void *pointer, size_t custom_type_id,
                                       const std::function<void(void *)> &deleter) {
        destroy();
        initialize(pointer, deleter);
        _custom_type_id = custom_type_id;
    }

    void Variant::CustomPointer::reset(void *pointer, size_t custom_type_id,
                                       std::function<void(void *)> &&deleter) noexcept {
        destroy();
        initialize(pointer, deleter);
        _custom_type_id = custom_type_id;
    }

    size_t Variant::CustomPointer::usedCount() const {
        return *_reference_count;
    }

    size_t Variant::CustomPointer::customTypeID() const {
        return _custom_type_id;
    }

    void Variant::CustomPointer::setCustomTypeID(size_t type_id) {
        _custom_type_id = type_id;
    }

    void* Variant::CustomPointer::get() const {
        return _pointer;
    }

    void Variant::CustomPointer::initialize(void *pointer, const std::function<void(void *)> &deleter) {
        _pointer = pointer;
        _deleter = deleter;
        _reference_count = new size_t(1);
    }

    void Variant::CustomPointer::destroy() {
        if (_reference_count) {
            *_reference_count -= 1;
            if (*_reference_count == 0 && _deleter) {
                try {
                    _deleter(_pointer);
                    delete _reference_count;
                } catch (const std::exception&) {
                    throw BadValueException(FMT::format("Variant::CustomPointer: "
                                                      "The custom pointer (0x{:x}) has deleted at least twice!",
                                                      reinterpret_cast<size_t>(_pointer)));
                }
            }
        }
        _reference_count = nullptr;
        _pointer = nullptr;
        _deleter = {};
        _custom_type_id = 0;
    }

    void Variant::CustomPointer::copy(const CustomPointer &ptr) {
        _pointer = ptr._pointer;
        _deleter = ptr._deleter;
        _reference_count = ptr._reference_count;
        _custom_type_id = ptr._custom_type_id;
        if (!_reference_count) {
            Logger::log(Logger::Fatal, "Variant::CustomPointer: Copy failed! "
                                       "The specified custom pointer is not valid!");
            throw InvalidArgumentException(FMT::format("Variant::CustomPointer: "
                                           "The specified custom pointer (0x{:x}) is not valid!",
                                           reinterpret_cast<size_t>(ptr._pointer)));
        }
        *_reference_count += 1;
    }

    void Variant::CustomPointer::copy(CustomPointer &&ptr) noexcept {
        _pointer = ptr._pointer;
        _deleter = ptr._deleter;
        _reference_count = ptr._reference_count;
        _custom_type_id = ptr._custom_type_id;
        if (!_reference_count) {
            Logger::log(Logger::Fatal, "Variant::CustomPointer: Copy failed! "
                                       "The specified custom pointer is not valid!");
            std::terminate();
        }
    }

    Variant::Variant(const Variant& v) : _type(v._type), _value() {
        switch (_type) {
            case Null:
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
                _pointer = v._pointer;
                break;
        }
    }

    Variant::Variant(Variant && v) noexcept : _type(v._type), _value() {
        switch (_type) {
            case Null:
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
                _value = new std::string(std::move(*static_cast<std::string*>(v._value)));
                break;
            case Pointer:
                _pointer = std::move(v._pointer);
                break;
        }
        v._type = Null;
        v._value = nullptr;
    }

    Variant& Variant::operator=(const Variant& v) {
        if (this == &v) return *this;
        clearValue();

        _type = v._type;
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
                _value = nullptr;
                _pointer = v._pointer;
                break;
        }
        return *this;
    }

    Variant &Variant::operator=(Variant && v) noexcept {
        if (this == &v) return *this;
        clearValue();

        _type = v._type;
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
                _value = new std::string(std::move(*static_cast<std::string*>(v._value)));
                break;
            case Pointer:
                _value = nullptr;
                _pointer = std::move(v._pointer);
                break;
        }
        v._type = Null;
        v._value = nullptr;
        return *this;
    }

    Variant::~Variant() {
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
                _pointer.reset();
                break;
            default:
                break;
        }
        _value = nullptr;
    }

    Variant::Type Variant::type() const {
        return _type;
    }

    const char* Variant::typeName() const {
        switch (_type) {
            case Null:
                return "Null";
            case Bool:
                return "Bool";
            case Int8:
                return "Int8";
            case Int16:
                return "Int16";
            case Int32:
                return "Int32";
            case Int64:
                return "Int64";
            case UInt8:
                return "UInt8";
            case UInt16:
                return "UInt16";
            case UInt32:
                return "UInt32";
            case UInt64:
                return "UInt64";
            case Float:
                return "Float";
            case Double:
                return "Double";
            case String:
                return "String";
            case Pointer:
                return "Pointer";
        }
        return "Unknown";
    }

    bool Variant::isNull() const {
        return _type == Null;
    }

    void Variant::setCustomTypeID(uint32_t type_id) {
        if (_type == Pointer) _pointer.setCustomTypeID(type_id);
    }

    uint32_t Variant::customTypeID() const {
        return _pointer.customTypeID();
    }

    void Variant::clearValue() {
        if (_value) {
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
                default:
                    break;
            }
        } else if (_type == Pointer) _pointer.reset();
        _type = Null;
        _value = nullptr;
    }

    void Variant::setValue(bool v) {
        if (_type == Bool) {
            *static_cast<bool*>(_value) = v;
        } else {
            clearValue();
            _type = Bool;
            _value = new bool(v);
        }
    }

    void Variant::setValue(int8_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<int8_t*>(_value) = static_cast<int8_t>(v);
        } else {
            clearValue();
            _type = Int8;
            _value = new int8_t(v);
        }
    }

    void Variant::setValue(int16_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<int16_t*>(_value) = static_cast<int16_t>(v);
        } else {
            clearValue();
            _type = Int16;
            _value = new int16_t(v);
        }
    }

    void Variant::setValue(int32_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<int32_t *>(_value) = static_cast<int32_t>(v);
        } else {
            clearValue();
            _type = Int32;
            _value = new int32_t(v);
        }
    }

    void Variant::setValue(int64_t v) {
        if (_type == Int64 || _type == UInt64) {
            *static_cast<int64_t*>(_value) = static_cast<int64_t>(v);
        } else {
            clearValue();
            _type = Int64;
            _value = new int64_t(v);
        }
    }

    void Variant::setValue(uint8_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<uint8_t*>(_value) = static_cast<uint8_t>(v);
        } else {
            clearValue();
            _type = UInt8;
            _value = new uint8_t(v);
        }
    }

    void Variant::setValue(uint16_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<uint16_t*>(_value) = static_cast<uint16_t>(v);
        } else {
            clearValue();
            _type = UInt16;
            _value = new uint16_t(v);
        }
    }

    void Variant::setValue(uint32_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<uint32_t*>(_value) = static_cast<uint32_t>(v);
        } else {
            clearValue();
            _type = UInt32;
            _value = new uint32_t(v);
        }
    }

    void Variant::setValue(uint64_t v) {
        if (_type == UInt64 || _type == Int64) {
            *static_cast<uint64_t*>(_value) = static_cast<uint64_t>(v);
        } else {
            clearValue();
            _type = UInt64;
            _value = new uint64_t(v);
        }
    }

    void Variant::setValue(float v) {
        if (_type == Float) {
            *static_cast<float*>(_value) = static_cast<float>(v);
        } else {
            clearValue();
            _type = Float;
            _value = new float(v);
        }
    }

    void Variant::setValue(double v) {
        if (_type == Double) {
            *static_cast<double*>(_value) = static_cast<double>(v);
        } else {
            clearValue();
            _type = Double;
            _value = new double(v);
        }
    }

    void Variant::setValue(const char* string) {
        if (_type == String) {
            *static_cast<std::string*>(_value) = string;
        } else {
            clearValue();
            _type = String;
            _value = new std::string(string);
        }
    }

    void Variant::setValue(std::string& string) {
        if (_type == String) {
            *static_cast<std::string*>(_value) = string;
        } else {
            clearValue();
            _type = String;
            _value = new std::string(string);
        }
    }

    void Variant::setValue(void* pointer, std::function<void(void*)> deleter) {
        if (_type != Pointer) {
            clearValue();
            _type = Pointer;
        }
        _pointer.reset(pointer, deleter);
    }

    void Variant::setValue(void* pointer, uint32_t custom_type_id, const std::function<void(void*)> &deleter) {
        if (_type != Pointer) {
            clearValue();
            _type = Pointer;
        }
        _pointer.reset(pointer, custom_type_id, deleter);
    }

    bool Variant::toBool() const {
        if (_type == Bool) {
            return (_value != nullptr) && *static_cast<bool *>(_value);
        }
        if (_type == Pointer) {
            return _value != nullptr;
        }
        throw BadValueException(FMT::format("Variant: The variant can not convert to bool!"));
    }

    int8_t Variant::toInt8() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to int8!"));
        }
        if (_type == Int8) {
            return *static_cast<int8_t *>(_value);
        }
        return static_cast<int8_t>(convert2Int32());
    }

    int16_t Variant::toInt16() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to int16!"));
        }
        if (_type == Int16) {
            return *static_cast<int16_t*>(_value);
        }
        return static_cast<int16_t>(convert2Int32());
    }

    int32_t Variant::toInt32() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to int32!"));
        }
        if (_type == Int32) {
            return *static_cast<int32_t *>(_value);
        }
        return convert2Int32();
    }

    int64_t Variant::toInt64() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to int64!"));
        }
        if (_type == Int64) {
            return *static_cast<int64_t *>(_value);
        }
        return convert2Int64();
    }

    uint8_t Variant::toUInt8() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to uint8!"));
        }
        if (_type == UInt8) {
            return *static_cast<uint8_t *>(_value);
        }
        return static_cast<uint8_t>(convert2UInt32());
    }

    uint16_t Variant::toUInt16() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to uint16!"));
        }
        if (_type == UInt16) {
            return *static_cast<uint16_t *>(_value);
        }
        return static_cast<uint16_t>(convert2UInt32());
    }

    uint32_t Variant::toUInt32() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to uint32!"));
        }
        if (_type == UInt32) {
            return *static_cast<uint32_t *>(_value);
        }
        return convert2UInt32();
    }

    uint64_t Variant::toUInt64() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to uint64!"));
        }
        if (_type == UInt64) {
            return *static_cast<uint64_t *>(_value);
        }
        return convert2UInt64();
    }

    float Variant::toFloat() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to float!"));
        }
        if (_type == Float) {
            return *static_cast<float *>(_value);
        }
        return convert2Float();
    }

    double Variant::toDouble() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to double!"));
        }
        if (_type == Double) {
            return *static_cast<double *>(_value);
        }
        return convert2Double();
    }

    std::string Variant::toString() const {
        if (_type != String) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to string!"));
        }
        return *static_cast<std::string*>(_value);
    }

    void* Variant::toPointer() const {
        if (_type != Pointer) {
            throw BadValueException(FMT::format("Variant: The variant can not convert to pointer!"));
        }
        return _pointer.get();
    }

    size_t Variant::usedCount() const {
        return _type == Pointer ? _pointer.usedCount() : 0;
    }

    std::string Variant::valueAsString(const std::function<std::string(void *, uint32_t)> &callback) const {
        switch (_type) {
            case Null: break;
            case Bool:
                return std::to_string(*static_cast<bool *>(_value));
            case Int8:
                return std::to_string(*static_cast<int8_t *>(_value));
            case Int16:
                return std::to_string(*static_cast<int16_t *>(_value));
            case Int32:
                return std::to_string(*static_cast<int32_t *>(_value));
            case Int64:
                return std::to_string(*static_cast<int64_t *>(_value));
            case UInt8:
                return std::to_string(*static_cast<uint8_t *>(_value));
            case UInt16:
                return std::to_string(*static_cast<uint16_t *>(_value));
            case UInt32:
                return std::to_string(*static_cast<uint32_t *>(_value));
            case UInt64:
                return std::to_string(*static_cast<uint64_t *>(_value));
            case Float:
                return std::to_string(*static_cast<float *>(_value));
            case Double:
                return std::to_string(*static_cast<double *>(_value));
            case String:
                return *static_cast<std::string *>(_value);
            case Pointer:
                if (callback) return callback(_pointer.get(), _pointer.customTypeID());
                break;
        }
        return {};
    }

    bool Variant::stringToValue(const std::string &string_value, Type var_type, const std::function<bool(void *)> &callback) {
        if (_type != Null) return false;
        auto old_type = _type;
        _type = var_type;
        switch (var_type) {
            case Null: break;
            case Bool:
                _value = new bool(std::stoi(string_value) > 0);
                return true;
            case Int8:
                _value = new int8_t(std::stoi(string_value));
                return true;
            case Int16:
                _value = new int16_t(std::stoi(string_value));
                return true;
            case Int32:
                _value = new int32_t(std::stol(string_value));
                return true;
            case Int64:
                _value = new int64_t(std::stoll(string_value));
                return true;
            case UInt8:
                _value = new uint8_t(std::stoul(string_value));
                return true;
            case UInt16:
                _value = new uint16_t(std::stoul(string_value));
                return true;
            case UInt32:
                _value = new uint32_t(std::stoul(string_value));
                return true;
            case UInt64:
                _value = new uint64_t(std::stoull(string_value));
                return true;
            case Float:
                _value = new float(std::stof(string_value));
                return true;
            case Double:
                _value = new double(std::stod(string_value));
                return true;
            case String:
                _value = new std::string(string_value);
                return true;
            case Pointer:
                if (callback) return callback(_pointer.get());
                break;
        }
        _type = old_type;
        return false;
    }

    BinaryArray Variant::valueAsBinary(const std::function<BinaryArray(void *, uint32_t)> &callback) const {
        switch (_type) {
            case Null: break;
            case Bool:
                return {*static_cast<bool *>(_value)};
            case Int8:
                return Algorithm::int8ToBinary(*static_cast<int8_t *>(_value));
            case Int16:
                return Algorithm::int16ToBinary(*static_cast<int16_t *>(_value));
            case Int32:
                return Algorithm::int32ToBinary(*static_cast<int32_t *>(_value));
            case Int64:
                return Algorithm::int64ToBinary(*static_cast<int64_t *>(_value));
            case UInt8:
                return Algorithm::uint8ToBinary(*static_cast<uint8_t *>(_value));
            case UInt16:
                return Algorithm::uint16ToBinary(*static_cast<uint16_t *>(_value));
            case UInt32:
                return Algorithm::uint32ToBinary(*static_cast<uint32_t *>(_value));
            case UInt64:
                return Algorithm::uint64ToBinary(*static_cast<uint64_t *>(_value));
            case Float:
                return Algorithm::floatToBinary(*static_cast<float *>(_value));
            case Double:
                return Algorithm::doubleToBinary(*static_cast<double *>(_value));
            case String:
                return Algorithm::stringToBinary(*static_cast<std::string *>(_value));
            case Pointer:
                if (callback) return callback(_pointer.get(), _pointer.customTypeID());
                break;
        }
        return {};
    }

    bool Variant::binaryToValue(const BinaryArray &bin_value, Type var_type, const std::function<bool(void *)> &callback) {
        if (_type != Null) return false;
        auto old_type = _type;
        _type = var_type;
        switch (var_type) {
            case Null: break;
            case Bool:
                _value = new bool(bin_value[0] > 0);
                return true;
            case Int8:
                _value = new int8_t(Algorithm::binaryToInt8(bin_value));
                return true;
            case Int16:
                _value = new int16_t(Algorithm::binaryToInt16(bin_value));
                return true;
            case Int32:
                _value = new int32_t(Algorithm::binaryToInt32(bin_value));
                return true;
            case Int64:
                _value = new int64_t(Algorithm::binaryToInt64(bin_value));
                return true;
            case UInt8:
                _value = new uint8_t(Algorithm::binaryToUInt8(bin_value));
                return true;
            case UInt16:
                _value = new uint16_t(Algorithm::binaryToUInt16(bin_value));
                return true;
            case UInt32:
                _value = new uint32_t(Algorithm::binaryToUInt32(bin_value));
                return true;
            case UInt64:
                _value = new uint64_t(Algorithm::binaryToUInt64(bin_value));
                return true;
            case Float:
                _value = new float(Algorithm::binaryToFloat(bin_value));
                return true;
            case Double:
                _value = new double(Algorithm::binaryToDouble(bin_value));
                return true;
            case String:
                _value = new std::string(Algorithm::binaryToString(bin_value));
                return true;
            case Pointer:
                if (callback) return callback(_pointer.get());
                break;
        }
        _type = old_type;
        return false;
    }
}
