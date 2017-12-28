#ifndef __PHYSX_SDK_H__
#define __PHYSX_SDK_H__

#include <PxPhysics.h>
#include <foundation/PxFoundation.h>
#include <cooking/PxCooking.h>
#include <atomic>
#include "physx_pvd.h"
#include "physx_sdk.h"

class PhysxSDKImpl
{
public:
    PhysxSDKImpl();
    ~PhysxSDKImpl();

    bool Init();

    inline physx::PxFoundation* GetFoundation() { return mFoundation; }
    inline physx::PxPhysics* GetPhysics() { return mPhysicsSDK; }
    inline physx::PxCooking* GetCooking() { return mCooking; }
    inline physx::PxMaterial& GetMaterial() { return *mMaterial; }
    inline PhysxPVD &GetPVD() { return mPVD; }

protected:
    virtual void customizeTolerances(physx::PxTolerancesScale&) {}

private:
    void release();

    std::atomic_bool mInit;
    physx::PxFoundation* mFoundation;
    physx::PxPhysics* mPhysicsSDK;
    physx::PxCooking* mCooking;
    physx::PxMaterial* mMaterial;
    PhysxPVD mPVD;
};

extern PhysxSDKImpl* gPhysxSDKImpl;

#endif
