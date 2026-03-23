
#include "include/MyEngine/Utils/SysMemory.h"
#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "User32.Lib")
#endif

#ifdef __APPLE__
#include <sys/types.h>
#include <sys/sysctl.h>
#include <mach/mach.h>
#include <mach/mach_host.h>
#endif

#include "include/MyEngine/Utils/Logger.h"

namespace MyEngine {
    SysMemory::SysMemStatus SysMemory::getSystemMemoryStatus(bool* ok) {
        SysMemory::SysMemStatus m_status;
        
#ifdef _WIN32
        MEMORYSTATUSEX mem_stat;
        mem_stat.dwLength = sizeof(mem_stat);
        if (GlobalMemoryStatusEx(&mem_stat)) {
            m_status.total_mem = mem_stat.ullTotalPhys / 1024;
            m_status.available_mem = mem_stat.ullAvailPhys / 1024;
            m_status.used_mem = m_status.total_mem - m_status.available_mem;
        } else {
            if (ok) *ok = false;
            return m_status;
        }
#endif
#ifdef __linux__
        // Linux
        FILE* file = fopen("/proc/meminfo", "r");
        if (!file) {
            Logger::log("SysMemory: Failed to get process memory!", Logger::Warn);
            if (ok) *ok = false;
            return m_status;
        }
        char line[256] = {'\0'};
        while (fgets(line, 256, file)) {
            if (sscanf(line, "MemTotal: %zu kB", &m_status.total_mem) == 1) continue;
            if (sscanf(line, "MemAvailable: %zu kB", &m_status.available_mem) == 1) break;
        }
        m_status.used_mem = m_status.total_mem - m_status.available_mem;
        fclose(file);
#endif
#ifdef __APPLE__
        // MacOS
        // Get physical memory
        int kib[2] = {CTL_HW, HW_MEMSIZE};
        size_t len = sizeof(m_status.total_mem);
        if (sysctl(kib, 2, &m_status.total_mem, &len, nullptr, 0) < 0) {
            if (ok) *ok = false;
            return m_status;
        }

        // Get memories detail
        size_t page_size;
        mach_port_t mach_port = mach_host_self();
        vm_statistics64_data_t vm_stats;
        mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

        if (host_page_size(mach_port, &page_size) != KERN_SUCCESS) {
            if (ok) *ok = false;
            return m_status;
        }
        
        if (host_statistics64(mach_port, HOST_VM_INFO64, 
                (host_info64_t)&vm_stats, &count) != KERN_SUCCESS) {
            if (ok) *ok = false;
            return m_status;
        }

        uint64_t free_mem = (uint64_t)vm_stats.free_count * (uint64_t)page_size;
        uint64_t active_mem = (uint64_t)vm_stats.active_count * (uint64_t)page_size;
        uint64_t inactive_mem = (uint64_t)vm_stats.inactive_count * (uint64_t)page_size;
        uint64_t wired_mem = (uint64_t)vm_stats.wire_count * (uint64_t)page_size;
        uint64_t speculative_mem = (uint64_t)vm_stats.speculative_count * (uint64_t)page_size;
        
        m_status.available_mem = (free_mem + inactive_mem + speculative_mem) / 1024;
        m_status.used_mem = (active_mem + wired_mem) / 1024;
        m_status.total_mem /= 1024;
        mach_port_deallocate(mach_task_self(), mach_port);
#endif
        if (ok) *ok = true;
        return m_status;
    }

    // Get current process used memory size
    size_t SysMemory::getCurProcUsedMemSize(bool* ok) {
        size_t used_mem = 0;
#ifdef _WIN32
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
            used_mem = pmc.WorkingSetSize / 1024;
        } else {
            if (ok) *ok = false;
            return 0;
        }
#endif
#ifdef __linux__
        // Linux
        FILE* file = fopen("/proc/self/status", "r");
        if (!file) {
            if (ok) *ok = false;
            return 0;
        }
        char line[256] = {'\0'};
        while (fgets(line, 256, file)) {
            if (sscanf(line, "VmRSS: %zu kB", &used_mem) == 1) break;
        }
        fclose(file);
#endif
#ifdef __APPLE__
        task_basic_info_data_t task_basic_info;
        mach_msg_type_number_t info_count = TASK_BASIC_INFO_COUNT;
        
        if (task_info(mach_task_self(), TASK_BASIC_INFO, 
                    (task_info_t)&task_basic_info, &info_count) != KERN_SUCCESS) {
            mach_task_basic_info_data_t machtask_info;
            mach_msg_type_number_t machinfo_count = MACH_TASK_BASIC_INFO_COUNT;
            
            if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO,
                        (task_info_t)&machtask_info, &machinfo_count) == KERN_SUCCESS) {
                used_mem = machtask_info.resident_size / 1024;
            } else {
                if (ok) *ok = false;
                return used_mem;
            }
        } else {
            used_mem = task_basic_info.resident_size / 1024;
        }
#endif
        if (ok) *ok = true;
        return used_mem;
    }
    
}

