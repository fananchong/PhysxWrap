#include <PhysxWrap.h>
#include <string>
#include <iostream>
#include "util.h"
#include <thread>

using namespace PhysxWrap;

void test(PhysxScene& scene, const std::string &path) {
    auto t1 = GetTimeStamp();
    scene.CreateScene(path);
    auto t2 = GetTimeStamp();
    std::cout << "Create Scene OK. Cost Time = " << t2 - t1 << " ms" << std::endl;
}


void Test1() {
    InitPhysxSDK();

    for (size_t i = 0; i < 10; i++)
    {
        PhysxScene scene;
        scene.Init();
        test(scene, "../../res/pxscene");
    }

    PhysxScene scene;
    scene.Init();
    test(scene, "../../res/pxscene");

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
    ReleasePhysxSDK();
}
