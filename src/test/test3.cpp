#include <PhysxWrap.h>
#include <string>
#include <iostream>
#include "util.h"
#include <thread>
#include <random>
#include <time.h>
#include <iostream>

using namespace PhysxWrap;


#define DEFAULT_TEST_COUNT (100000)

void Test3() {
    InitPhysxSDK();
    int count = 0;
    for (size_t i = 0; i < DEFAULT_TEST_COUNT; i++, count++)
    {
        PhysxScene scene;
        scene.Init();
        scene.CreateScene("../../res/pxscene");
        for (size_t j = 0; j < 1000; j++)
        {
            float x = float(rand() % 1000);
            float y = float(rand() % 1000);
            float z = float(rand() % 1000);
            float r = float(rand() % 20 + 1);
            scene.CreateSphereDynamic(Vector3{ x, y, z }, r);
            scene.CreateBoxStatic(Vector3{ x + 10, y + 10, z + 10 }, Vector3{ 1,1,1 });
        }

        if (count % 100 == 0)
        {
            std::cout << ".";
        }
    }
}