#pragma once
#ifndef MYENGINE_EXCEPTION_H
#define MYENGINE_EXCEPTION_H
#include "Libs.h"

#define EXCEPTION(C) \
class C : public std::runtime_error {                               \
public:                                                             \
    explicit C(std::string message) : std::runtime_error(message.data()), \
        _msg(std::move(message)) {}                                 \
    [[nodiscard]] const char * what() const noexcept override {     \
        return _msg.data();                                         \
    }                                                               \
private:                                                            \
    std::string _msg;                                               \
}

namespace MyEngine {
    EXCEPTION(EngineException);

    EXCEPTION(RenderException);

    EXCEPTION(BadValueException);

    EXCEPTION(OutOfRangeException);

    EXCEPTION(InvalidArgumentException);

    EXCEPTION(NullPointerException);
}

#endif //MYENGINE_EXCEPTION_H
