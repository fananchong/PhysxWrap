#include <PhysxWrap.h>
#include <thread>
#include <random>
#include <time.h>
#include <iostream>
#include "util.h"

#ifdef _MSC_VER
#pragma comment(lib, "PhysxWrap.lib")
#endif

void Test(PhysxScene& scene, const std::string &path);

int main(int argn, char argv[]) {

    InitPhysxSDK();

    for (size_t i = 0; i < 10; i++)
    {
        PhysxScene scene;
        scene.Init();
        Test(scene, "../../res/pxscene");
    }

    PhysxScene scene;
    scene.Init();
    Test(scene, "../../res/pxscene");

    srand(unsigned int(time(0)));
    auto &s = scene;
    auto actor = s.CreateSphereDynamic(Vector3{ 10, 25, 10 }, 25);
    s.SetLinearVelocity(actor, Vector3{ 0, 0, 1 });


    auto pretTime = GetTimeStamp();
    while (true)
    {
        std::this_thread::sleep_for(33 * std::chrono::milliseconds());

        auto nowTime = GetTimeStamp();
        auto dt = (nowTime - pretTime) / 1000.0f;
        pretTime = nowTime;

        if (dt <= 0)
        {
            continue;
        }
        std::cout << "nowTime - pretTime = " << dt << std::endl;

        s.Update(dt);
        Vector3 pos = s.GetGlobalPostion(actor);
        std::cout << "(x, y, z) = (" << pos.X << "," << pos.Y << "," << pos.Z << ")" << std::endl;
    }

    return 0;
}
