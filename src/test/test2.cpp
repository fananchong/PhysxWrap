#include <PhysxWrap.h>
#include <string>
#include <iostream>
#include "util.h"
#include <thread>
#include <random>
#include <time.h>
#include <iostream>
#include "profiler.h"

using namespace PhysxWrap;


#define DEFAULT_TEST_COUNT (10)

void Test2() {
    InitPhysxSDK();
    
#ifdef _DEBUG
    Profiler::HeapProfilerDump("InitSDK");
#endif

    for (size_t i = 0; i < DEFAULT_TEST_COUNT; i++)
    {
        PhysxScene scene;
        scene.Init();

        for (size_t j = 0; j < 1000; j++)
        {
            float x = float(rand() % 1000);
            float y = float(rand() % 1000);
            float z = float(rand() % 1000);
            float r = float(rand() % 20 + 1);
            scene.CreateSphereDynamic(Vector3{ x, y, z }, r);
            scene.CreateBoxStatic(Vector3{ x + 10, y + 10, z + 10 }, Vector3{ 1,1,1 });
        }
        for (size_t i = 0; i < 500; i++)
        {
            scene.Update(0.016f);
        }

        std::cout << ".";
    }
#ifdef _DEBUG
    Profiler::HeapProfilerDump("EndScene");
#endif
    ReleasePhysxSDK();
    
#ifdef _DEBUG
    Profiler::HeapProfilerDump("ReleaseSDK");
#endif
    std::cout << "exit Test2" << std::endl;
}
