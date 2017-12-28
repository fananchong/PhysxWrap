#include <physx_wrap.h>
#include <iostream>
#include <fstream>
#include "util.h"

void createTestHeightField(PhysxScene& scene, const std::string &path) {
    std::string content = GetFileContent(path);
    unsigned columns = *(uint32_t *)(content.data());
    unsigned rows = *(uint32_t *)(content.data() + sizeof(uint32_t));
    int16_t *pdata = (int16_t *)(content.data() + 2 * sizeof(uint32_t));
    std::vector<int16_t> data;
    for (size_t i = 0; i < columns* rows; i++)
        data.push_back(pdata[i]);
    scene.CreateHeightField(data, columns, rows, Vector3{ 1.0f / 6.0f, 1.0f / 6.0f, 1 / 12.0f });
}

void createTestMesh(PhysxScene& scene, const Vector3 &pos) {
    std::vector<float> vdata;
    std::vector<uint16_t> vindex;

    // dot0
    vdata.push_back(0);
    vdata.push_back(1);
    vdata.push_back(0);

    // dot1
    vdata.push_back(0.5f);
    vdata.push_back(0);
    vdata.push_back(0);

    // dot2
    vdata.push_back(-0.5f);
    vdata.push_back(0);
    vdata.push_back(0);

    // dot3
    vdata.push_back(0);
    vdata.push_back(0);
    vdata.push_back(0.5f);

    vindex.push_back(0);
    vindex.push_back(3);
    vindex.push_back(1);

    vindex.push_back(1);
    vindex.push_back(0);
    vindex.push_back(2);

    vindex.push_back(2);
    vindex.push_back(3);
    vindex.push_back(0);

    vindex.push_back(2);
    vindex.push_back(3);
    vindex.push_back(1);

    scene.CreateMeshKinematic(pos, Vector3{ 1, 1, 1 }, vdata, vindex);
}

void Test1(PhysxScene& scene) {
    //scene.CreatePlane(0);
    createTestHeightField(scene, "../../res/hightmap.data");
    scene.CreateBoxDynamic(Vector3{ 0, 20, 0 }, Vector3{ 0.5f, 0.5f, 0.5f });
    scene.CreateSphereDynamic(Vector3{ 0, 20, 0 }, 0.5);
    scene.CreateCapsuleDynamic(Vector3{ -1.5, 20, 0 }, 0.5, 0.5);
    createTestMesh(scene, Vector3{ 1.5f, 20, 0 });
}