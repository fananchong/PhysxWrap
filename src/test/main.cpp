#include <physx_wrap.h>
#include <thread>

#ifdef _MSC_VER
#pragma comment(lib, "PhysxWrap.lib")
#endif

void Test1(PhysxScene& scene);
void Test2(PhysxScene& scene, const std::string &path);

int main(int argn, char argv[]) {
    PhysxScene scene;
    scene.InitWithPVD(1.0f / 60.0f);

    //Test1(scene);
    Test2(scene, "../../res/pxscene");

    auto actor = scene.CreateCapsuleKinematic(10, 0, 10, 5, 5);
    int frame = 0;
    while (true)
    {
        if (frame % (30 * 5) == 0)
        {

        }
        frame++;
        scene.Update();
        std::this_thread::sleep_for(33 * std::chrono::milliseconds());
    }

    return 0;
}
