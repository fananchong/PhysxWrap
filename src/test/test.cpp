#include <PhysxWrap.h>
#include <string>
#include <iostream>
#include "util.h"

void Test(PhysxScene& scene, const std::string &path) {
    auto t1 = GetTimeStamp();
    scene.CreateScene(path);
    auto t2 = GetTimeStamp();
    std::cout << "Create Scene OK. Cost Time = " << t2 - t1 << " ms" << std::endl;
}
