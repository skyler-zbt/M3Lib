#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

extern "C" int benchmark_pin_cpu(int preferred) {
    DWORD_PTR allowed = 0;
    DWORD_PTR system = 0;
    if (!GetProcessAffinityMask(GetCurrentProcess(), &allowed, &system))
        return -1;
    const int bits = static_cast<int>(sizeof(DWORD_PTR) * 8);
    int selected = -1;
    if (preferred >= 0 && preferred < bits && (allowed & (DWORD_PTR{1} << preferred)))
        selected = preferred;
    else
        for (int cpu = 0; cpu < bits; ++cpu)
            if (allowed & (DWORD_PTR{1} << cpu)) { selected = cpu; break; }
    return selected >= 0 && SetThreadAffinityMask(GetCurrentThread(), DWORD_PTR{1} << selected)
        ? selected : -1;
}
#elif defined(__linux__)
#include <sched.h>

extern "C" int benchmark_pin_cpu(int preferred) {
    cpu_set_t allowed;
    CPU_ZERO(&allowed);
    if (sched_getaffinity(0, sizeof(allowed), &allowed) != 0)
        return -1;
    int selected = -1;
    if (preferred >= 0 && preferred < CPU_SETSIZE && CPU_ISSET(preferred, &allowed))
        selected = preferred;
    else
        for (int cpu = 0; cpu < CPU_SETSIZE; ++cpu)
            if (CPU_ISSET(cpu, &allowed)) { selected = cpu; break; }
    cpu_set_t target;
    CPU_ZERO(&target);
    if (selected < 0)
        return -1;
    CPU_SET(selected, &target);
    return sched_setaffinity(0, sizeof(target), &target) == 0 ? selected : -1;
}
#else
extern "C" int benchmark_pin_cpu(int) { return -1; }
#endif
