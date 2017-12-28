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

class PhysxSceneImpl
{
public:
    PhysxSceneImpl();
    ~PhysxSceneImpl();

    bool Init(float timestep, bool supportPVD, const std::string &ip, unsigned port, unsigned timeout, bool useFullPvdConnection);
    void CreateScene(const std::string &path);
    void Update();
    physx::PxRigidActor* CreatePlane(float xNormal, float yNormal, float zNormal, float distance);
    physx::PxRigidActor* CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, float columnScale, float rowScale, float heightScale);
    physx::PxRigidActor* CreateBoxDynamic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ, float density);
    physx::PxRigidActor* CreateBoxKinematic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ, float density);
    physx::PxRigidActor* CreateBoxStatic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ);
    physx::PxRigidActor* CreateSphereDynamic(float posX, float posY, float posZ, float radius, float density);
    physx::PxRigidActor* CreateSphereKinematic(float posX, float posY, float posZ, float radius, float density);
    physx::PxRigidActor* CreateSphereStatic(float posX, float posY, float posZ, float radius);
    physx::PxRigidActor* CreateCapsuleDynamic(float posX, float posY, float posZ, float radius, float halfHeight, float density);
    physx::PxRigidActor* CreateCapsuleKinematic(float posX, float posY, float posZ, float radius, float halfHeight, float density);
    physx::PxRigidActor* CreateCapsuleStatic(float posX, float posY, float posZ, float radius, float halfHeight);
    physx::PxRigidActor* CreateMeshKinematic(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const std::vector<float> &vb, const std::vector<uint16_t> &ib, float density);
    physx::PxRigidActor* CreateMeshStatic(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const std::vector<float> &vb, const std::vector<uint16_t> &ib);

    void SetGlobalPostion(physx::PxRigidActor* actor, float posX, float posY, float posZ);
    void SetGlobalRotate(physx::PxRigidActor* actor, float rotateX, float rotateY, float rotateZ, float rotateW);

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
