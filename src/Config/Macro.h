
#ifndef MYENGINE_UTILS_MACRO_H
#define MYENGINE_UTILS_MACRO_H
#include "../Libs.h"
#define MACRO_MAKE_CONFIG(NAME, ...)        \
class NAME {                                \
public:                                     \
    NAME() = default;                       \
    ~NAME() = default;                      \
    NAME(const NAME&) = default;            \
    NAME(NAME&&)      = default;            \
    NAME& operator=(const NAME&) = default; \
    NAME& operator=(NAME&&)    = default;   \
                                            \
    __VA_ARGS__                             \
                                            \
}

#define MACRO_MAKE_PROPERTY(TYPE, NAME, ...) \
private:                                     \
    TYPE _##NAME{__VA_ARGS__};               \
public:                                      \
    void set_##NAME(const TYPE& value) {     \
        _##NAME = value;                     \
    }                                        \
                                             \
    void set_##NAME(TYPE&& value) noexcept {  \
        _##NAME = std::move(value);          \
    }                                        \
                                             \
    TYPE get_##NAME() const {                \
        return _##NAME;                      \
    }                                        \
                                             \
    const TYPE& NAME() const {               \
        return _##NAME;                      \
    }                                        \

#define MACRO_MAKE_PTR_PROPERTY(TYPE, NAME)         \
private:                                            \
    TYPE* _##NAME{};                                \
public:                                             \
    void set_##NAME(TYPE*&& pointer) {              \
        _##NAME = std::move(pointer);               \
    }                                               \
                                                    \
    void copy_to_##NAME(TYPE* pointer) {            \
        _##NAME = new TYPE(*pointer);               \
    }                                               \
                                                    \
    TYPE* const& NAME() const {                     \
        return _##NAME;                             \
    }                                               \
                                                    \
    TYPE* get_##NAME() const {                      \
        return _##NAME;                             \
    }                                               \
                                                    \
    TYPE* release_##NAME() {                        \
        return _##NAME ? std::move(_##NAME) : nullptr;  \
    }                                               \
                                                    \
    void reset_##NAME() {                           \
        if (_##NAME) {                              \
            delete _##NAME;                         \
            _##NAME = nullptr;                      \
        }                                           \
    }                                               \

#define MACRO_MAKE_CONFIG_OBJECT(NAME, ...)             \
MyEngine::Config::ConfigObject NAME;                    \
__VA_ARGS__

#define MACRO_APPEND_PROP_TO_OBJECT(OBJECT, CONFIG, PROPERTY)    \
OBJECT.emplace(#PROPERTY, CONFIG.get_##PROPERTY());


#endif //MYENGINE_UTILS_MACRO_H