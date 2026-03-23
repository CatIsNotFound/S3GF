#ifndef MYENGINE_UTILS_SYSMEMORY_H
#define MYENGINE_UTILS_SYSMEMORY_H
#include <cstdio>

namespace MyEngine {
    /**
     * \if EN
     * @class MyEngine::SysMemory
     * @brief System Memory
     * @details Used to obtain the operating system's system memory information in real time
     * @note This class is a static class and does not require obtaining a global singleton.
     * \endif
     */
    class SysMemory {
    public:
        explicit SysMemory() = delete;
        SysMemory(const SysMemory&) = delete;
        SysMemory(SysMemory&&) = delete;
        SysMemory& operator=(const SysMemory&) = delete;
        SysMemory& operator=(SysMemory&&) = delete;
        ~SysMemory() = delete;

        /**
         * \if EN
         * @brief System Memory Status
         * @details Used to describe the memory state of the current operating system
         * \endif
         */
        struct SysMemStatus {
            /**
             * \if EN
             * @brief Total memory usage (in KB)
             * \endif
             */
            size_t total_mem{0};
            /**
             * \if EN
             * @brief Used memory usage (in KB)
             * \endif
             */
            size_t used_mem{0};
            /**
             * \if EN
             * @brief Available memory usage (in KB)
             * \endif
             */
            size_t available_mem{0};
        };

        /**
         * \if EN
         * @brief Get the current system memory status
         * @param ok Optional parameter, indicating whether information can be retrieved normally
         * @return Return a system memory status
         * \endif
         * @see SysMemStatus
         */
        static SysMemStatus getSystemMemoryStatus(bool* ok = nullptr);


        /**
         * \if EN
         * @brief Get the memory usage of the current process
         * @param ok    Optional parameter, indicating whether information can be retrieved normally
         * @return Return a memory size in KB.
         * \endif
         */
        static size_t getCurProcUsedMemSize(bool* ok = nullptr);
    };
}


#endif //MYENGINE_UTILS_SYSMEMORY_H
