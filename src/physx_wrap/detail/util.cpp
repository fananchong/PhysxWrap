#include "util.h"
#include <string>
#include <fstream>
#include <foundation/PxPreprocessor.h>
#include <stdio.h>
#include <malloc.h>

#if defined(_MSC_VER)
#include <Windows.h>
#else
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#endif

namespace PhysxWrap {

    unsigned long GetTimeStamp(void)
    {
        unsigned long current_time;
#ifdef WIN32
        current_time = GetTickCount();
#else
        struct timeval current;
        gettimeofday(&current, NULL);
        current_time = current.tv_sec * 1000 + current.tv_usec / 1000;
#endif
        return current_time;
    }

    std::string GetFileContent(const std::string &filename)
    {
        std::string ret;
        std::ifstream in(filename, std::ios::in | std::ios::binary | std::ios::ate);
        if (in.is_open() == false)
        {
            return "";
        }
        ret.resize((unsigned)in.tellg());
        in.seekg(0, std::ios::beg);
        in.read((char*)ret.data(), ret.size());
        in.close();
        return std::move(ret);
    }

#if defined(WIN32)
    // on win32 we only have 8-byte alignment guaranteed, but the CRT provides special aligned allocation
    // fns
#include <malloc.h>
#include <crtdbg.h>

    void* platformAlignedAlloc(size_t size)
    {
        return _aligned_malloc(size, 16);
    }

    void platformAlignedFree(void* ptr)
    {
        _aligned_free(ptr);
    }
#elif PX_LINUX_FAMILY
    void* platformAlignedAlloc(size_t size)
    {
        return ::memalign(16, size);
    }

    void platformAlignedFree(void* ptr)
    {
        ::free(ptr);
    }
#else

    // on Win64 we get 16-byte alignment by default
    void* platformAlignedAlloc(size_t size)
    {
        void *ptr = ::malloc(size);
        PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15) == 0);
        return ptr;
    }

    void platformAlignedFree(void* ptr)
    {
        ::free(ptr);
    }
#endif

}
