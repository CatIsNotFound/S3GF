#ifndef MYENGINE_TEMPLATE_SINGLETON_H
#define MYENGINE_TEMPLATE_SINGLETON_H
#include "../Libs.h"

namespace MyEngine::Template {
#define SINGLETON(C) \
friend class Singleton; \
C() = default;
    template<class C>
    class Singleton {
    public:
        static C* global() {
            static C instance;
            return &instance;
        }

        Singleton(Singleton&&) = delete;
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        Singleton& operator=(Singleton&&) = delete;
        // Singleton* operator&() = delete;
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
    };
}

#endif //MYENGINE_TEMPLATE_SINGLETON_H
