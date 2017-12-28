#ifndef __PHYSX_WRAP_IMPL_H__
#define __PHYSX_WRAP_IMPL_H__

#include <PxPhysics.h>
#include <foundation/PxFoundation.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <cooking/PxCooking.h>
#include <PxScene.h>
#include <PxRigidActor.h>
#include <atomic>
#include <vector>
#include "physx_pvd.h"
#include "../physx_wrap.h"

class PhysxSceneImpl
{
public:
    PhysxSceneImpl();
    ~PhysxSceneImpl();

    bool Init(float timestep, bool supportPVD, const std::string &ip, unsigned port, unsigned timeout, bool useFullPvdConnection);
    void CreateScene(const std::string &path);
    void Update();
    physx::PxRigidActor* CreatePlane(float xNormal, float yNormal, float zNormal, float distance);
    physx::PxRigidActor* CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, const Vector3 &scale);
    physx::PxRigidActor* CreateBoxDynamic(const Vector3 &pos, const Vector3 &halfExtents, float density);
    physx::PxRigidActor* CreateBoxKinematic(const Vector3 &pos, const Vector3 &halfExtents, float density);
    physx::PxRigidActor* CreateBoxStatic(const Vector3 &pos, const Vector3 &halfExtents);
    physx::PxRigidActor* CreateSphereDynamic(const Vector3 &pos, float radius, float density);
    physx::PxRigidActor* CreateSphereKinematic(const Vector3 &pos, float radius, float density);
    physx::PxRigidActor* CreateSphereStatic(const Vector3 &pos, float radius);
    physx::PxRigidActor* CreateCapsuleDynamic(const Vector3 &pos, float radius, float halfHeight, float density);
    physx::PxRigidActor* CreateCapsuleKinematic(const Vector3 &pos, float radius, float halfHeight, float density);
    physx::PxRigidActor* CreateCapsuleStatic(const Vector3 &pos, float radius, float halfHeight);
    physx::PxRigidActor* CreateMeshKinematic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib, float density);
    physx::PxRigidActor* CreateMeshStatic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib);

    void SetLinearVelocity(physx::PxRigidActor* actor, const Vector3 &velocity);
    void AddForce(physx::PxRigidActor* actor, const Vector3 &force);
    void ClearForce(physx::PxRigidActor* actor);

    Vector3 GetGlobalPostion(physx::PxRigidActor* actor);
    Quat GetGlobalRotate(physx::PxRigidActor* actor);
    void SetGlobalPostion(physx::PxRigidActor* actor, const Vector3 &pos);
    void SetGlobalRotate(physx::PxRigidActor* actor, const Quat &rotate);

protected:
    virtual void customizeTolerances(physx::PxTolerancesScale&) {}
    virtual void customizeSceneDesc(physx::PxSceneDesc& desc) {}

private:
    void release();

    physx::PxFoundation* mFoundation;
    physx::PxPhysics* mPhysicsSDK;
    physx::PxCooking* mCooking;
    physx::PxScene* mScene;
    physx::PxDefaultCpuDispatcher* mCpuDispatcher;
    physx::PxMaterial* mMaterial;
    std::vector<physx::PxRigidActor*> mPhysicsActors;
    std::atomic_bool mInit;
    float mTimestep;

    bool mSupportPVD;
    PhysxPVD mPVD;

    std::vector<physx::PxRigidActor*> mTempVec;
    unsigned long mPreUpdateTime;

    friend class PhysxScene;
};


#endif
