#ifndef __PHYSX_WRAP_H__
#define __PHYSX_WRAP_H__

#include <memory>
#include <vector>
#include "actor_info.h"
#include "vector3.h"
#include "quat.h"

class PhysxSceneImpl;
class PhysxScene
{
public:
    PhysxScene();
    ~PhysxScene();

    bool Init(float timestep);
    bool InitWithPVD(float timestep, const std::string &ip = "127.0.0.1", unsigned port = 5425, unsigned timeout = 10, bool useFullPvdConnection = true);
    void CreateScene(const std::string &path);
    void Update();

    uint64_t CreatePlane(float yAxis);
    uint64_t CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, float columnScale, float rowScale, float heightScale);
    uint64_t CreateBoxDynamic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ);
    uint64_t CreateBoxKinematic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ);
    uint64_t CreateBoxStatic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ);
    uint64_t CreateSphereDynamic(float posX, float posY, float posZ, float radius);
    uint64_t CreateSphereKinematic(float posX, float posY, float posZ, float radius);
    uint64_t CreateSphereStatic(float posX, float posY, float posZ, float radius);
    uint64_t CreateCapsuleDynamic(float posX, float posY, float posZ, float radius, float halfHeight);
    uint64_t CreateCapsuleKinematic(float posX, float posY, float posZ, float radius, float halfHeight);
    uint64_t CreateCapsuleStatic(float posX, float posY, float posZ, float radius, float halfHeight);
    uint64_t CreateMeshKinematic(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const std::vector<float> &vb, const std::vector<uint16_t> &ib);
    uint64_t CreateMeshStatic(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const std::vector<float> &vb, const std::vector<uint16_t> &ib);

    //void SetLinearVelocity(float velocityX, float velocityY, float velocityZ);


    void SetGlobalPostion(uint64_t id, float posX, float posY, float posZ);
    void SetGlobalRotate(uint64_t id, float rotateX, float rotateY, float rotateZ, float rotateW);

    ActorInfo GetActorInfo(uint64_t id);
    std::vector<uint64_t> GetActors();

private:
    void release();
    std::shared_ptr<PhysxSceneImpl> mImpl;
};

unsigned GetStaticObjCountInScene(const std::string &path);

#endif
