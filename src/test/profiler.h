#ifndef __PROFILER_H__
#define __PROFILER_H__

#ifdef ENABLE_PROFILER

#include <cstdint>

class Profiler
{
public:
    static void Init();

    // mem
    static double GetMemoryReleaseRate();
    static void SetMemoryReleaseRate(double rate);  // Reasonable rates are in the range [0,10]
    static void ReleaseFreeMemory();

    // heap
    static void HeapProfilerStart(const char *prefix);
    static void HeapProfilerDump(const char *reason);
    static void HeapProfilerStop();
    static bool IsHeapProfilerRunning();
    static void HeapProfilerSetTimeInterval(int64_t interval); // seconds
    static int64_t HeapProfilerGetTimeInterval();
    static void HeapProfilerSetAllocationInterval(int64_t interval); // bytes
    static int64_t HeapProfilerGetAllocationInterval();

    // cpu
    static int ProfilerStart(const char *prefix);
    static void ProfilerStop();
    static void ProfilerFlush();
};

#endif

#endif