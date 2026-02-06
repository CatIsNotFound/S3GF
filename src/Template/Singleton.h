#ifndef MYENGINE_TEMPLATE_SINGLETON_H
#define MYENGINE_TEMPLATE_SINGLETON_H
#include "../Libs.h"

namespace MyEngine::Template {
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
    protected:
        Singleton() = default;
        virtual ~Singleton() = default;
    };

    template<class C>
    class LazySingleton {
        inline static std::unique_ptr<C> _instance{};
        inline static std::mutex _mutex{};
    public:
        static C* global() {
            std::lock_guard<std::mutex> lock(_mutex);
            if (!_instance) {
                _instance = std::make_unique<C>();
            }
            return _instance;
        }

        LazySingleton(const LazySingleton&) = delete;
        LazySingleton(LazySingleton&&) = delete;
        LazySingleton& operator=(const LazySingleton&) = delete;
        LazySingleton& operator=(LazySingleton&&) = delete;
    protected:
        LazySingleton() = default;
        virtual ~LazySingleton() = default;
    };

    template<class C>
    class SafeSingleton {
    public:
        static std::shared_ptr<C> global() {
            std::call_once(_once_flag, [&] {
                _instance = std::make_shared<C>();
            });
            return _instance;
        }

        SafeSingleton(const SafeSingleton&) = delete;
        SafeSingleton(SafeSingleton&&) = delete;
        SafeSingleton& operator=(const SafeSingleton&) = delete;
        SafeSingleton& operator=(SafeSingleton&&) = delete;
    protected:
        SafeSingleton() = default;
        virtual ~SafeSingleton() = default;
    private:
        inline static std::shared_ptr<C> _instance{};
        inline static std::once_flag _once_flag{};
    };
}

#endif //MYENGINE_TEMPLATE_SINGLETON_H
