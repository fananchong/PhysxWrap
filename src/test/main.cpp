#include <physx_wrap.h>
#include <thread>
#include <random>
#include <time.h>
#include <iostream>

#ifdef _MSC_VER
#pragma comment(lib, "PhysxWrap.lib")
#endif

void Test1(PhysxScene& scene);
void Test2(PhysxScene& scene, const std::string &path);

int main(int argn, char argv[]) {

    PhysxScene scene;
#ifdef _DEBUG
    scene.InitWithPVD(1.0f / 60.0f);
#else
    scene.Init(1.0f / 60.0f);
#endif
    //Test1(scene);
    Test2(scene, "../../res/pxscene");

    srand(unsigned int(time(0)));

    auto actor = scene.CreateSphereDynamic(Vector3{ 10, 25, 10 }, 25);
    scene.SetLinearVelocity(actor, Vector3{ 0, 0, 1 });
    int frame = 0;
    while (true)
    {
        frame++;
        scene.Update();
        std::this_thread::sleep_for(33 * std::chrono::milliseconds());
        Vector3 pos = scene.GetGlobalPostion(actor);
        std::cout << "(x, y, z) = (" << pos.X << "," << pos.Y << "," << pos.Z << ")" << std::endl;
    }

    return 0;
}
