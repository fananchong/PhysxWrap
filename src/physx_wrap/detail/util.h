#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

namespace PhysxWrap {
    unsigned long GetTimeStamp(void);
    std::string GetFileContent(const std::string &filename);
};

#endif
