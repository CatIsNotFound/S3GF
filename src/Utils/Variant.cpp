#include "Variant.h"
#include "Exception.h"

namespace MyEngine {
    int64_t Variant::convert2Int64() const {
        if (_type == Null || _type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == Int64 || _type == UInt64) return *static_cast<int64_t*>(_value);
        if (_type == Float) {
            return static_cast<int64_t>(*static_cast<float*>(_value));
        }
        if (_type == Double) {
            return static_cast<int64_t>(*static_cast<double*>(_value));
        }
        return static_cast<int64_t>(*static_cast<int32_t*>(_value));
    }

    uint64_t Variant::convert2UInt64() const {
        if (_type == Null || _type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == Int64 || _type == UInt64) return *static_cast<uint64_t*>(_value);
        if (_type == Float) {
            return static_cast<int64_t>(*static_cast<float*>(_value));
        }
        if (_type == Double) {
            return static_cast<int64_t>(*static_cast<double*>(_value));
        }
        return static_cast<uint64_t>(*static_cast<int32_t*>(_value));
    }

    int32_t Variant::convert2Int32() const {
        if (_type == Null || _type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == Int64 || _type == UInt64) return static_cast<int32_t>(*static_cast<int64_t*>(_value));
        if (_type == Float) {
            return static_cast<int64_t>(*static_cast<float*>(_value));
        }
        if (_type == Double) {
            return static_cast<int64_t>(*static_cast<double*>(_value));
        }
        return *static_cast<int32_t*>(_value);
    }

    uint32_t Variant::convert2UInt32() const {
        if (_type == Null || _type >= String) return 0;
        if (_type == Bool) return *static_cast<bool*>(_value);
        if (_type == Int64 || _type == UInt64) return static_cast<uint32_t>(*static_cast<int64_t*>(_value));
        if (_type == Float) {
            return static_cast<int64_t>(*static_cast<float*>(_value));
        }
        if (_type == Double) {
            return static_cast<int64_t>(*static_cast<double*>(_value));
        }
        return *static_cast<uint32_t*>(_value);
    }

    float Variant::convert2Float() const {
        if (_type == Float) return *static_cast<float*>(_value);
        if (_type == Double) return static_cast<float>(*static_cast<double*>(_value));
        if (_type <= Bool || _type >= String) return 0;
        if (_type == Int64) {
            return static_cast<float>(*static_cast<int64_t*>(_value));
        } else if (_type == UInt64) {
            return static_cast<float>(*static_cast<uint64_t*>(_value));
        }
        if (_type >= Int8 && _type <= Int32) {
            return static_cast<float>(*static_cast<int32_t*>(_value));
        } else {
            return static_cast<float>(*static_cast<uint32_t*>(_value));
        }
    }

    double Variant::convert2Double() const {
        if (_type == Double) return *static_cast<double*>(_value);
        if (_type == Float) return static_cast<double>(*static_cast<float*>(_value));
        if (_type <= Bool || _type >= String) return 0;
        if (_type == Int64) {
            return static_cast<double>(*static_cast<int64_t*>(_value));
        } else if (_type == UInt64) {
            return static_cast<double>(*static_cast<uint64_t*>(_value));
        }
        if (_type >= Int8 && _type <= Int32) {
            return static_cast<double>(*static_cast<int32_t*>(_value));
        } else {
            return static_cast<double>(*static_cast<uint32_t*>(_value));
        }
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

    bool Variant::hasDeleter() const {
        return (_deleter != nullptr);
    }

    void Variant::setDeleter(std::function<void(void*)> deleter) {
        if (_type == Pointer) _deleter = std::move(deleter);
    }

    void Variant::setValue() {
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
                case Pointer:
                    if (_deleter) _deleter(_value);
                    break;
                default:
                    break;
            }
        }
        _type = Null;
        _value = nullptr;
        _deleter = {};
    }

    void Variant::setValue(bool v) {
        if (_type == Bool) {
            *static_cast<bool*>(_value) = v;
        } else {
            setValue(); 
            _type = Bool;
            _value = new bool(v);
        }
    }

    void Variant::setValue(int8_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<int8_t*>(_value) = static_cast<int8_t>(v);
        } else {
            setValue();
            _type = Int8;
            _value = new int8_t(v);
        }
    }

    void Variant::setValue(int16_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<int16_t*>(_value) = static_cast<int16_t>(v);
        } else {
            setValue();
            _type = Int16;
            _value = new int16_t(v);
        }
    }

    void Variant::setValue(int32_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<int32_t *>(_value) = static_cast<int32_t>(v);
        } else {
            setValue();
            _type = Int32;
            _value = new int32_t(v);
        }
    }

    void Variant::setValue(int64_t v) {
        if (_type == Int64 || _type == UInt64) {
            *static_cast<int64_t*>(_value) = static_cast<int64_t>(v);
        } else {
            setValue();
            _type = Int64;
            _value = new int64_t(v);
        }
    }

    void Variant::setValue(uint8_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<uint8_t*>(_value) = static_cast<uint8_t>(v);
        } else {
            setValue();
            _type = UInt8;
            _value = new uint8_t(v);
        }
    }

    void Variant::setValue(uint16_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<uint16_t*>(_value) = static_cast<uint16_t>(v);
        } else {
            setValue();
            _type = UInt16;
            _value = new uint16_t(v);
        }
    }

    void Variant::setValue(uint32_t v) {
        if ((_type > Int8 && _type <= Int32) || (_type > UInt8 && _type <= UInt32)) {
            *static_cast<uint32_t*>(_value) = static_cast<uint32_t>(v);
        } else {
            setValue();
            _type = UInt32;
            _value = new uint32_t(v);
        }
    }

    void Variant::setValue(uint64_t v) {
        if (_type == UInt64 || _type == Int64) {
            *static_cast<uint64_t*>(_value) = static_cast<uint64_t>(v);
        } else {
            setValue();
            _type = UInt64;
            _value = new uint64_t(v);
        }
    }

    void Variant::setValue(float v) {
        if (_type == Float) {
            *static_cast<float*>(_value) = static_cast<float>(v);
        } else {
            setValue();
            _type = Float;
            _value = new float(v);
        }
    }

    void Variant::setValue(double v) {
        if (_type == Double) {
            *static_cast<double*>(_value) = static_cast<double>(v);
        } else {
            setValue();
            _type = Double;
            _value = new double(v);
        }
    }

    void Variant::setValue(const char* string) {
        if (_type == String) {
            *static_cast<std::string*>(_value) = string;
        } else {
            setValue();
            _type = String;
            _value = new std::string(string);
        }
    }

    void Variant::setValue(std::string& string) {
        if (_type == String) {
            *static_cast<std::string*>(_value) = string;
        } else {
            setValue();
            _type = String;
            _value = new std::string(string);
        }
    }

    void Variant::setValue(void* pointer, std::function<void(void*)> deleter) {
        if (_type == Pointer) {
            if (_value != pointer) {
                if (_deleter) {
                    _deleter(_value);
                }
                _value = pointer;
                _deleter = std::move(deleter);
            } else {
                _deleter = std::move(deleter);
            }
        } else {
            setValue();
            _type = Pointer;
            _value = pointer;
            _deleter = std::move(deleter);
        }
    }

    bool Variant::toBool() const {
        if (_type == Bool) {
            return (_value != nullptr) && *static_cast<bool *>(_value);
        } else if (_type == Pointer) {
            return _value != nullptr;
        } else {
            throw BadValueException(std::format("Variant: The variant can not convert to bool!"));
        }
    }

    int8_t Variant::toInt8() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to int8!"));
        }
        if (_type == Int8) {
            return *static_cast<int8_t *>(_value);
        } else {
            return static_cast<int8_t>(convert2Int32());
        }
    }

    int16_t Variant::toInt16() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to int16!"));
        }
        if (_type == Int16) {
            return *static_cast<int16_t*>(_value);
        } else {
            return static_cast<int16_t>(convert2Int32());
        }
    }

    int32_t Variant::toInt32() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to int32!"));
        }
        if (_type == Int32) {
            return *static_cast<int32_t *>(_value);
        } else {
            return convert2Int32();
        }
    }

    int64_t Variant::toInt64() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to int64!"));
        }
        if (_type == Int64) {
            return *static_cast<int64_t *>(_value);
        } else {
            return convert2Int64();
        }
    }

    uint8_t Variant::toUInt8() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to uint8!"));
        }
        if (_type == UInt8) {
            return *static_cast<uint8_t *>(_value);
        } else {
            return static_cast<uint8_t>(convert2UInt32());
        }
    }

    uint16_t Variant::toUInt16() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to uint16!"));
        }
        if (_type == UInt16) {
            return *static_cast<uint16_t *>(_value);
        } else {
            return static_cast<uint16_t>(convert2UInt32());
        }
    }

    uint32_t Variant::toUInt32() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to uint32!"));
        }
        if (_type == UInt32) {
            return *static_cast<uint32_t *>(_value);
        } else {
            return convert2UInt32();
        }
    }

    uint64_t Variant::toUInt64() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to uint64!"));
        }
        if (_type == UInt64) {
            return *static_cast<uint64_t *>(_value);
        } else {
            return convert2UInt64();
        }
    }

    float Variant::toFloat() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to float!"));
        }
        if (_type == Float) {
            return *static_cast<float *>(_value);
        } else {
            return convert2Float();
        }
    }

    double Variant::toDouble() const {
        if (_type == Null || _type == String || _type == Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to double!"));
        }
        if (_type == Double) {
            return *static_cast<double *>(_value);
        } else {
            return convert2Double();
        }
    }

    std::string Variant::toString() const {
        if (_type != String) {
            throw BadValueException(std::format("Variant: The variant can not convert to string!"));
        }
        return *static_cast<std::string*>(_value);
    }

    void* Variant::toPointer() const {
        if (_type != Pointer) {
            throw BadValueException(std::format("Variant: The variant can not convert to pointer!"));
        }
        return _value;
    }

}
