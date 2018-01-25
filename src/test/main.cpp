#include <random>
#include <time.h>
#include "profiler.h"

#ifdef _MSC_VER
#pragma comment(lib, "PhysxWrap.lib")
#endif


void Test1();
void Test2();
void Test3();

int main(int argn, char *argv[]) {

#ifdef _DEBUG
    Profiler::HeapProfilerStart("test.pprof");
#endif

#ifdef _MSC_VER
    srand(unsigned int(time(0)));
#else
    srand(time(0));
#endif

    //Test1();
    Test2();
    //Test3();

#ifdef _DEBUG
    Profiler::HeapProfilerDump("exit");
    Profiler::HeapProfilerStop();
#endif

    return 0;
}
