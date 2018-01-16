#ifndef __PHYSX_SDK_H__
#define __PHYSX_SDK_H__

#include <PxPhysics.h>
#include <foundation/PxFoundation.h>
#include <cooking/PxCooking.h>
#include <geometry/PxHeightFieldGeometry.h>
#include <geometry/PxHeightFieldSample.h>
#include <geometry/PxHeightFieldDesc.h>
#include <geometry/PxHeightField.h>
#include <geometry/PxConvexMeshGeometry.h>
#include <atomic>
#include <vector>
#include "physx_pvd.h"
#include "physx_sdk.h"
#include "../PhysxWrap.h"

namespace PhysxWrap {
    class PhysxSDKImpl
    {
    public:
        PhysxSDKImpl();
        ~PhysxSDKImpl();

        bool Init();

        inline physx::PxFoundation* GetFoundation() { return mFoundation; }
        inline physx::PxPhysics* GetPhysics() { return mPhysicsSDK; }
        inline physx::PxCooking* GetCooking() { return mCooking; }
        inline PhysxPVD &GetPVD() { return mPVD; }

    protected:
        virtual void customizeTolerances(physx::PxTolerancesScale&) {}

    private:
        void release();

        std::atomic_bool mInit;
        physx::PxFoundation* mFoundation;
        physx::PxPhysics* mPhysicsSDK;
        physx::PxCooking* mCooking;
        PhysxPVD mPVD;
    };


    bool GetHeightFieldGeometry(physx::PxHeightFieldGeometry &geom, const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, const Vector3 &scale);
    bool GetMeshGeometry(physx::PxTriangleMeshGeometry& geom, const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib);

    extern PhysxSDKImpl* gPhysxSDKImpl;

};

#endif
