#include "physx_sdk.h"
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <foundation/PxFoundationVersion.h>
#include <PxPhysicsVersion.h>
#include <extensions/PxExtensionsAPI.h>
#include "log.h"

namespace PhysxWrap {

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

            physx::PxCookingParams params(scale);
            //params.meshWeldTolerance = 0.001f;
            //params.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES);
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
            SAFE_RELEASE(mCooking);
#ifdef _DEBUG
            PxCloseExtensions();
#endif
            SAFE_RELEASE(mPhysicsSDK);
            mPVD.Close();
            SAFE_RELEASE(mFoundation);
        }
    }


    bool GetHeightFieldGeometry(physx::PxHeightFieldGeometry &geom, const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, const Vector3 &scale) {
        unsigned hfNumVerts = columns*rows;
        physx::PxHeightFieldSample* samples = (physx::PxHeightFieldSample*)malloc(sizeof(physx::PxHeightFieldSample)*hfNumVerts);
        memset(samples, 0, hfNumVerts * sizeof(physx::PxHeightFieldSample));

        for (unsigned row = 0; row < rows; row++)
            for (unsigned col = 0; col < columns; col++)
            {
                int index = col + row*columns;
                samples[index].height = heightmap[index];
                //samples[index].setTessFlag();
                //samples[index].materialIndex0 = 1;
                //samples[index].materialIndex1 = 1;
            }

        physx::PxHeightFieldDesc hfDesc;
        hfDesc.format = physx::PxHeightFieldFormat::eS16_TM;
        hfDesc.nbColumns = columns;
        hfDesc.nbRows = rows;
        hfDesc.samples.data = samples;
        hfDesc.samples.stride = sizeof(physx::PxHeightFieldSample);

        physx::PxHeightField* heightField = gPhysxSDKImpl->GetCooking()->createHeightField(hfDesc, gPhysxSDKImpl->GetPhysics()->getPhysicsInsertionCallback());
        if (!heightField) {
            ERROR("[physx] creating the heightfield failed");
            free(samples);
            return false;
        }
        geom.heightField = heightField;
        geom.columnScale = scale.X;
        geom.heightScale = scale.Y;
        geom.rowScale = scale.Z;
        free(samples);
        return true;
    }


    bool GetMeshGeometry(physx::PxTriangleMeshGeometry& geom, const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib) {
        physx::PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = physx::PxU32(vb.size() / 3);
        meshDesc.triangles.count = physx::PxU32(ib.size() / 3);
        meshDesc.points.stride = sizeof(float) * 3;
        meshDesc.triangles.stride = sizeof(uint16_t) * 3;
        meshDesc.points.data = vb.data();
        meshDesc.triangles.data = ib.data();
        meshDesc.flags |= physx::PxMeshFlag::e16_BIT_INDICES;
        meshDesc.flags |= physx::PxMeshFlag::eFLIPNORMALS;

        physx::PxDefaultMemoryOutputStream streamout;
        bool ok = gPhysxSDKImpl->GetCooking()->cookTriangleMesh(meshDesc, streamout);
        if (!ok) {
            ERROR("[physx] cookTriangleMesh fail.");
            return false;
        }

        physx::PxDefaultMemoryInputData streamin(streamout.getData(), streamout.getSize());
        physx::PxTriangleMesh* triangleMesh = gPhysxSDKImpl->GetPhysics()->createTriangleMesh(streamin);
        if (triangleMesh) {
            physx::PxMeshScale meshScale = physx::PxMeshScale(physx::PxVec3{ scale.X ,scale.Y ,scale.Z }, physx::PxQuat(physx::PxIdentity));
            geom.triangleMesh = triangleMesh;
            geom.scale = meshScale;
            return true;
        }
        else {
            ERROR("[physx] createTriangleMesh fail.");
            return false;
        }
    }

}
