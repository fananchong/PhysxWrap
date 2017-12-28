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
    uint64_t CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, const Vector3 &scale);
    uint64_t CreateBoxDynamic(const Vector3 &pos, const Vector3 &halfExtents);
    uint64_t CreateBoxKinematic(const Vector3 &pos, const Vector3 &halfExtents);
    uint64_t CreateBoxStatic(const Vector3 &pos, const Vector3 &halfExtents);
    uint64_t CreateSphereDynamic(const Vector3 &pos, float radius);
    uint64_t CreateSphereKinematic(const Vector3 &pos, float radius);
    uint64_t CreateSphereStatic(const Vector3 &pos, float radius);
    uint64_t CreateCapsuleDynamic(const Vector3 &pos, float radius, float halfHeight);
    uint64_t CreateCapsuleKinematic(const Vector3 &pos, float radius, float halfHeight);
    uint64_t CreateCapsuleStatic(const Vector3 &pos, float radius, float halfHeight);
    uint64_t CreateMeshKinematic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib);
    uint64_t CreateMeshStatic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib);

    void SetLinearVelocity(uint64_t id, const Vector3 &velocity);
    void AddForce(uint64_t id, const Vector3 &force);
    void ClearForce(uint64_t id);

    Vector3 GetGlobalPostion(uint64_t id);
    Quat GetGlobalRotate(uint64_t id);
    void SetGlobalPostion(uint64_t id, const Vector3 &pos);
    void SetGlobalRotate(uint64_t id, const Quat &rotate);

    ActorInfo GetActorInfo(uint64_t id);
    std::vector<uint64_t> GetActors();

private:
    void release();
    std::shared_ptr<PhysxSceneImpl> mImpl;
};

unsigned GetStaticObjCountInScene(const std::string &path);

#endif
