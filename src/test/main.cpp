#include <physx_wrap.h>
#include <thread>
#include <random>
#include <time.h>

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

    srand(unsigned int(time(0)));

    auto actor = scene.CreateSphereDynamic(Vector3{ 10, 25, 10 }, 25);
    int frame = 0;
    while (true)
    {
        if (frame % (30 * 5) == 0)
        {
            scene.ClearForce(actor);
            scene.AddForce(actor, Vector3{ 0, 0, 1 });
        }
        frame++;
        scene.Update();
        std::this_thread::sleep_for(33 * std::chrono::milliseconds());
    }

    return 0;
}
