#include "physx_sdk.h"
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <foundation/PxFoundationVersion.h>
#include <common/PxTolerancesScale.h>
#include <PxPhysicsVersion.h>
#include <PxSceneDesc.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <PxSceneLock.h>
#include <extensions/PxSimpleFactory.h>
#include <cooking/PxTriangleMeshDesc.h>
#include <extensions/PxDefaultStreams.h>
#include <extensions/PxRigidActorExt.h>
#include <extensions/PxExtensionsAPI.h>
#include <PxMaterial.h>
#include "log.h"

PhysxSDKImpl __gPhysxSDKImpl;
PhysxSDKImpl* gPhysxSDKImpl = &__gPhysxSDKImpl;

#define	SAFE_RELEASE(x)	if(x){ x->release(); x = NULL;	}
physx::PxDefaultAllocator gDefaultAllocatorCallback;
physx::PxDefaultErrorCallback gDefaultErrorCallback;

PhysxSDKImpl::PhysxSDKImpl()
    : mInit(false)
    , mFoundation(nullptr)
    , mPhysicsSDK(nullptr)
    , mCooking(nullptr)
    , mMaterial(nullptr)
{

}

PhysxSDKImpl::~PhysxSDKImpl() {
    release();
#ifdef _DEBUG
    INFO("call ~PhysxSDKImpl()");
#endif
}

bool PhysxSDKImpl::Init() {
    if (mInit.load() == false) {
        mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
        if (!mFoundation) {
            ERROR("[physx] PxCreateFoundation failed!");
            release();
            return false;
        }
#ifdef _DEBUG
        mPVD.Init(mFoundation);
        mPVD.CreatePvdConnection();
#endif
        physx::PxTolerancesScale scale;
        customizeTolerances(scale);
        mPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, scale, false, mPVD.GetPvdInstance());
        if (!mPhysicsSDK) {
            ERROR("[physx] PxCreatePhysics failed!");
            release();
            return false;
        }
#ifdef _DEBUG
        if (!PxInitExtensions(*mPhysicsSDK, mPVD.GetPvdInstance())) {
            ERROR("[physx] PxInitExtensions failed!");
            release();
            return false;
        }
#endif
        mMaterial = mPhysicsSDK->createMaterial(0.5f, 0.5f, 0.1f);
        if (!mMaterial) {
            ERROR("[physx] createMaterial failed!");
            release();
            return false;
        }

        physx::PxCookingParams params(scale);
        params.meshWeldTolerance = 0.001f;
        params.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES);
        mCooking = PxCreateCooking(PX_PHYSICS_VERSION, *mFoundation, params);
        if (!mCooking) {
            ERROR("[physx] PxCreateCooking failed!");
            release();
            return false;
        }
        mInit.store(true);
    }
    return true;
}

void PhysxSDKImpl::release() {
    bool exp = true;
    if (mInit.compare_exchange_strong(exp, false)) {
        SAFE_RELEASE(mMaterial);
        SAFE_RELEASE(mCooking);
#ifdef _DEBUG
        PxCloseExtensions();
#endif
        SAFE_RELEASE(mPhysicsSDK);
        mPVD.Close();
        SAFE_RELEASE(mFoundation);
    }
}
