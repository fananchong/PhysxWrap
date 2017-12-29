#include <PhysxWrap.h>
#include <thread>
#include <random>
#include <time.h>
#include <iostream>
#include "util.h"

#ifdef _MSC_VER
#pragma comment(lib, "PhysxWrap.lib")
#endif

void Test1(PhysxScene& scene);
void Test2(PhysxScene& scene, const std::string &path);

int main(int argn, char argv[]) {

    InitPhysxSDK();

    PhysxScene scene1;
    scene1.Init(1.0f / 60.0f);
    Test1(scene1);

    PhysxScene scene2;
    scene2.Init(1.0f / 60.0f);
    Test2(scene2, "../../res/pxscene");

    srand(unsigned int(time(0)));

    auto actor = scene2.CreateSphereDynamic(Vector3{ 10, 25, 10 }, 25);
    scene2.SetLinearVelocity(actor, Vector3{ 0, 0, 1 });
    int frame = 0;
    auto pretTime = GetTimeStamp();
    while (true)
    {
        auto nowTime = GetTimeStamp();
        auto dt = (nowTime - pretTime) / 1000.0f;
        pretTime = nowTime;
        frame++;
        scene2.Update(dt);
        std::this_thread::sleep_for(33 * std::chrono::milliseconds());
        Vector3 pos = scene2.GetGlobalPostion(actor);
        std::cout << "(x, y, z) = (" << pos.X << "," << pos.Y << "," << pos.Z << ")" << std::endl;
    }

    return 0;
}
