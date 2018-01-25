
#ifdef ENABLE_PROFILER

#include "profiler.h"
#include <string>
#include <cassert>

#ifndef _MSC_VER
#include <gperftools/tcmalloc.h>
#include <gperftools/malloc_extension.h>
#include <gperftools/heap-profiler.h>
#include <gperftools/profiler.h>
#endif

void Profiler::Init()
{
}

double Profiler::GetMemoryReleaseRate()
{
#ifndef _MSC_VER
    return MallocExtension::instance()->GetMemoryReleaseRate();
#else
    return 0;
#endif
}

void Profiler::SetMemoryReleaseRate(double rate)
{
#ifndef _MSC_VER
    MallocExtension::instance()->SetMemoryReleaseRate(rate);
#endif
}

void Profiler::ReleaseFreeMemory()
{
#ifndef _MSC_VER
    MallocExtension::instance()->ReleaseFreeMemory();
#endif
}

void Profiler::HeapProfilerStart(const char *prefix)
{
#ifndef _MSC_VER
    ::HeapProfilerStart(prefix);
#endif
}

void Profiler::HeapProfilerDump(const char *reason)
{
#ifndef _MSC_VER
    ::HeapProfilerDump(reason);
#endif
}

void Profiler::HeapProfilerStop()
{
#ifndef _MSC_VER
    ::HeapProfilerStop();
#endif
}

bool Profiler::IsHeapProfilerRunning()
{
#ifndef _MSC_VER
    return ::IsHeapProfilerRunning() != 0;
#else
    return false;
#endif
}

#ifndef _MSC_VER
namespace FLAG__namespace_do_not_use_directly_use_DECLARE_int64_instead
{
    extern int64_t FLAGS_heap_profile_time_interval;
}
#endif

void Profiler::HeapProfilerSetTimeInterval(int64_t interval)
{
#ifndef _MSC_VER
    FLAG__namespace_do_not_use_directly_use_DECLARE_int64_instead::FLAGS_heap_profile_time_interval = interval;
#endif
}

int64_t Profiler::HeapProfilerGetTimeInterval()
{
#ifndef _MSC_VER
    return FLAG__namespace_do_not_use_directly_use_DECLARE_int64_instead::FLAGS_heap_profile_time_interval;
#else
    return -1;
#endif
}



#ifndef _MSC_VER
namespace FLAG__namespace_do_not_use_directly_use_DECLARE_int64_instead
{
    extern int64_t FLAGS_heap_profile_allocation_interval;
}
#endif

void Profiler::HeapProfilerSetAllocationInterval(int64_t interval)
{
#ifndef _MSC_VER
    FLAG__namespace_do_not_use_directly_use_DECLARE_int64_instead::FLAGS_heap_profile_allocation_interval = interval;
#endif
}

int64_t Profiler::HeapProfilerGetAllocationInterval()
{
#ifndef _MSC_VER
    return FLAG__namespace_do_not_use_directly_use_DECLARE_int64_instead::FLAGS_heap_profile_allocation_interval;
#else
    return -1;
#endif
}

int Profiler::ProfilerStart(const char *prefix)
{
#ifndef _MSC_VER
    return ::ProfilerStart(prefix);
#else
    return -1;
#endif
}

void Profiler::ProfilerStop()
{
#ifndef _MSC_VER
    ::ProfilerStop();
#endif
}

void Profiler::ProfilerFlush()
{
#ifndef _MSC_VER
    ::ProfilerFlush();
#endif
}

#endif
