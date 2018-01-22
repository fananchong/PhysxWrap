#include "util.h"
#include <string>
#include <fstream>

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

}
