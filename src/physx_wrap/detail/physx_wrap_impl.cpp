#include "physx_wrap_impl.h"
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <foundation/PxFoundationVersion.h>
#include <common/PxTolerancesScale.h>
#include <PxPhysicsVersion.h>
#include <PxSceneDesc.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <PxSceneLock.h>
#include <extensions/PxSimpleFactory.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>
#include <PxRigidBody.h>
#include <cooking/PxTriangleMeshDesc.h>
#include <extensions/PxDefaultStreams.h>
#include <extensions/PxRigidActorExt.h>
#include <geometry/PxHeightFieldSample.h>
#include <geometry/PxHeightFieldDesc.h>
#include <extensions/PxExtensionsAPI.h>
#include <cassert>
#include "../log.h"
#include "../util.h"
#include "scene_info_mgr.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "PhysX3DEBUG_x64.lib")
#pragma comment(lib, "PxFoundationDEBUG_x64.lib")
#pragma comment(lib, "PhysX3ExtensionsDEBUG.lib")
#pragma comment(lib, "PhysX3CommonDEBUG_x64.lib")
#pragma comment(lib, "PhysX3CookingDEBUG_x64.lib")
#pragma comment(lib, "PxPvdSDKDEBUG_x64.lib")
#else
#pragma comment(lib, "PhysX3_x64.lib")
#pragma comment(lib, "PxFoundation_x64.lib")
#pragma comment(lib, "PhysX3Extensions.lib")
#pragma comment(lib, "PhysX3Common_x64.lib")
#pragma comment(lib, "PhysX3Cooking_x64.lib")
#pragma comment(lib, "PxPvdSDK_x64.lib")
#endif
#endif

#define	SAFE_RELEASE(x)	if(x){ x->release(); x = NULL;	}
#define DEFAULT_RIGID_DYNAMIC(ACTOR)                                                \
    ACTOR->setAngularDamping(0.5f);                                                 \
    if (mSupportPVD) {                                                              \
        ACTOR->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);              \
    }                                                                               \
    ACTOR->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);             \

physx::PxDefaultAllocator gDefaultAllocatorCallback;
physx::PxDefaultErrorCallback gDefaultErrorCallback;

PhysxSceneImpl::PhysxSceneImpl()
    : mFoundation(nullptr)
    , mPhysicsSDK(nullptr)
    , mCooking(nullptr)
    , mScene(nullptr)
    , mCpuDispatcher(nullptr)
    , mInit(false)
    , mTimestep(1.0f / 60.0f)
    , mSupportPVD(false)
{
    mPreUpdateTime = GetTimeStamp();
}

PhysxSceneImpl::~PhysxSceneImpl() {
    release();
#ifdef _DEBUG
    INFO("call ~PhysxSceneImpl()");
#endif
}

bool PhysxSceneImpl::Init(float timestep, bool supportPVD, const std::string &ip, unsigned port, unsigned timeout, bool useFullPvdConnection) {
    if (mInit.load() == false) {
        mFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
        if (!mFoundation) {
            ERROR("[physx] PxCreateFoundation failed!");
            release();
            return false;
        }
        mSupportPVD = supportPVD;
        if (mSupportPVD) {
            mPVD.Init(mFoundation, ip, port, timeout, useFullPvdConnection);
            mPVD.CreatePvdConnection();
        }
        physx::PxTolerancesScale scale;
        customizeTolerances(scale);
        mPhysicsSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation, scale, false, mPVD.GetPvdInstance());
        if (!mPhysicsSDK) {
            ERROR("[physx] PxCreatePhysics failed!");
            release();
            return false;
        }
        if (mSupportPVD) {
            if (!PxInitExtensions(*mPhysicsSDK, mPVD.GetPvdInstance())) {
                ERROR("[physx] PxInitExtensions failed!");
                release();
                return false;
            }
        }
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

        physx::PxSceneDesc sceneDesc(mPhysicsSDK->getTolerancesScale());
        sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
        mCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(0);
        if (!mCpuDispatcher) {
            ERROR("PxDefaultCpuDispatcherCreate failed!");
            release();
            return false;
        }
        sceneDesc.cpuDispatcher = mCpuDispatcher;
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
        sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
        sceneDesc.flags |= physx::PxSceneFlag::eSUPPRESS_EAGER_SCENE_QUERY_REFIT;
        customizeSceneDesc(sceneDesc);
        mScene = mPhysicsSDK->createScene(sceneDesc);
        if (!mScene) {
            ERROR("[physx] createScene failed!");
            release();
            return false;
        }
        if (mSupportPVD) {
            mScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
            mScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
            physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
            if (pvdClient)
            {
                pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
                pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
                pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
            }
        }
        mInit.store(true);
        mTimestep = timestep;
    }
    return true;
}

void PhysxSceneImpl::release() {
    bool exp = true;
    if (mInit.compare_exchange_strong(exp, false)) {
        {
            physx::PxSceneWriteLock scopedLock(*mScene);
            for (size_t i = 0; i < mPhysicsActors.size(); ++i)
                mPhysicsActors[i]->release();
            mPhysicsActors.clear();
        }
        SAFE_RELEASE(mScene);
        SAFE_RELEASE(mCpuDispatcher);
        SAFE_RELEASE(mCooking);
        if (mSupportPVD) {
            PxCloseExtensions();
        }
        SAFE_RELEASE(mPhysicsSDK);
        mPVD.Close();
        SAFE_RELEASE(mFoundation);
    }
}

void PhysxSceneImpl::Update() {
    if (mScene != nullptr) {
        const physx::PxReal timestep = mTimestep;
        unsigned long nowTime = GetTimeStamp();
        float dtime = float(nowTime - mPreUpdateTime) / 1000.0f;
        mPreUpdateTime = nowTime;
        while (dtime > 0.0f) {
            const physx::PxReal dt = dtime >= timestep ? timestep : dtime;
            mScene->simulate(dt);
            mScene->fetchResults();
            dtime -= dt;
        }
    }
}

physx::PxRigidActor* PhysxSceneImpl::CreatePlane(float xNormal, float yNormal, float zNormal, float distance) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidStatic* plane = physx::PxCreatePlane(*mPhysicsSDK, physx::PxPlane(physx::PxVec3(zNormal, yNormal, zNormal), distance), *mMaterial);
    if (!plane) {
        ERROR("[physx] create plane failed!");
        return nullptr;
    }
    mScene->addActor(*plane);
    mPhysicsActors.push_back(plane);
    return plane;
}

physx::PxRigidActor* PhysxSceneImpl::CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, float columnScale, float rowScale, float heightScale) {
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

    physx::PxHeightField* heightField = mCooking->createHeightField(hfDesc, mPhysicsSDK->getPhysicsInsertionCallback());
    if (!heightField) {
        ERROR("[physx] creating the heightfield failed");
        free(samples);
        return nullptr;
    }

    physx::PxTransform pose = physx::PxTransform(physx::PxIdentity);
    pose.p = physx::PxVec3(-(float(columns) / 2 * columnScale), 0, -(float(rows) / 2 * rowScale));
    physx::PxRigidStatic* hfActor = mPhysicsSDK->createRigidStatic(pose);
    if (!hfActor) {
        ERROR("[physx] creating heightfield actor failed");
        free(samples);
        return nullptr;
    }
    physx::PxHeightFieldGeometry hfGeom(heightField, physx::PxMeshGeometryFlags(), heightScale, rowScale, columnScale);
    physx::PxShape* hfShape = physx::PxRigidActorExt::createExclusiveShape(*hfActor, hfGeom, *mMaterial);
    if (!hfShape) {
        ERROR("[physx] creating heightfield shape failed");
        free(samples);
        return nullptr;
    }
    mScene->addActor(*hfActor);
    mPhysicsActors.push_back(hfActor);
    free(samples);
    return hfActor;
}

physx::PxRigidActor* PhysxSceneImpl::CreateBoxDynamic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ, float density) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidDynamic* box = PxCreateDynamic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxBoxGeometry(halfExtentsX, halfExtentsY, halfExtentsZ), *mMaterial, density);
    if (!box) {
        ERROR("[physx] create dynamic box failed!");
        return nullptr;
    }
    DEFAULT_RIGID_DYNAMIC(box);
    mScene->addActor(*box);
    mPhysicsActors.push_back(box);
    return box;
}

physx::PxRigidActor* PhysxSceneImpl::CreateBoxKinematic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ, float density) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidDynamic* box = PxCreateKinematic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxBoxGeometry(halfExtentsX, halfExtentsY, halfExtentsZ), *mMaterial, density);
    if (!box) {
        ERROR("[physx] create kinematic box failed!");
        return nullptr;
    }
    mScene->addActor(*box);
    mPhysicsActors.push_back(box);
    return box;
}

physx::PxRigidActor* PhysxSceneImpl::CreateBoxStatic(float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidStatic* box = PxCreateStatic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxBoxGeometry(halfExtentsX, halfExtentsY, halfExtentsZ), *mMaterial);
    if (!box) {
        ERROR("[physx] create static box failed!");
        return nullptr;
    }
    mScene->addActor(*box);
    mPhysicsActors.push_back(box);
    return box;
}

physx::PxRigidActor* PhysxSceneImpl::CreateSphereDynamic(float posX, float posY, float posZ, float radius, float density) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidDynamic* sphere = PxCreateDynamic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxSphereGeometry(radius), *mMaterial, density);
    if (!sphere) {
        ERROR("[physx] create dynamic sphere failed!");
        return nullptr;
    }
    DEFAULT_RIGID_DYNAMIC(sphere);
    mScene->addActor(*sphere);
    mPhysicsActors.push_back(sphere);
    return sphere;
}

physx::PxRigidActor* PhysxSceneImpl::CreateSphereKinematic(float posX, float posY, float posZ, float radius, float density) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidDynamic* sphere = PxCreateKinematic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxSphereGeometry(radius), *mMaterial, density);
    if (!sphere) {
        ERROR("[physx] create kinematic sphere failed!");
        return nullptr;
    }
    mScene->addActor(*sphere);
    mPhysicsActors.push_back(sphere);
    return sphere;
}

physx::PxRigidActor*  PhysxSceneImpl::CreateSphereStatic(float posX, float posY, float posZ, float radius) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidStatic* sphere = PxCreateStatic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxSphereGeometry(radius), *mMaterial);
    if (!sphere) {
        ERROR("[physx] create static sphere failed!");
        return nullptr;
    }
    mScene->addActor(*sphere);
    mPhysicsActors.push_back(sphere);
    return sphere;
}

physx::PxRigidActor* PhysxSceneImpl::CreateCapsuleDynamic(float posX, float posY, float posZ, float radius, float halfHeight, float density) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidDynamic* capsule = PxCreateDynamic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxCapsuleGeometry(radius, halfHeight), *mMaterial, density);
    if (!capsule) {
        ERROR("[physx] create dynamic capsule failed!");
        return nullptr;
    }
    DEFAULT_RIGID_DYNAMIC(capsule);
    mScene->addActor(*capsule);
    mPhysicsActors.push_back(capsule);
    return capsule;
}

physx::PxRigidActor* PhysxSceneImpl::CreateCapsuleKinematic(float posX, float posY, float posZ, float radius, float halfHeight, float density) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidDynamic* capsule = PxCreateKinematic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxCapsuleGeometry(radius, halfHeight), *mMaterial, density);
    if (!capsule) {
        ERROR("[physx] create kinematic capsule failed!");
        return nullptr;
    }
    mScene->addActor(*capsule);
    mPhysicsActors.push_back(capsule);
    return capsule;
}

physx::PxRigidActor* PhysxSceneImpl::CreateCapsuleStatic(float posX, float posY, float posZ, float radius, float halfHeight) {
    physx::PxSceneWriteLock scopedLock(*mScene);
    physx::PxRigidStatic* capsule = PxCreateStatic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), physx::PxCapsuleGeometry(radius, halfHeight), *mMaterial);
    if (!capsule) {
        ERROR("[physx] create static capsule failed!");
        return nullptr;
    }
    mScene->addActor(*capsule);
    mPhysicsActors.push_back(capsule);
    return capsule;
}

physx::PxRigidActor* PhysxSceneImpl::CreateMeshKinematic(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const std::vector<float> &vb, const std::vector<uint16_t> &ib, float density) {
    physx::PxSceneWriteLock scopedLock(*mScene);
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
    bool ok = mCooking->cookTriangleMesh(meshDesc, streamout);
    if (!ok) {
        ERROR("[physx] cookTriangleMesh fail.");
        return nullptr;
    }

    physx::PxDefaultMemoryInputData streamin(streamout.getData(), streamout.getSize());
    physx::PxTriangleMesh* triangleMesh = mPhysicsSDK->createTriangleMesh(streamin);
    if (triangleMesh) {
        physx::PxMeshScale meshScale = physx::PxMeshScale(physx::PxVec3{ scaleX ,scaleY ,scaleZ }, physx::PxQuat(physx::PxIdentity));
        physx::PxTriangleMeshGeometry triGeom;
        triGeom.triangleMesh = triangleMesh;
        triGeom.scale = meshScale;
        physx::PxRigidDynamic* mesh = PxCreateKinematic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), triGeom, *mMaterial, density);
        if (!mesh) {
            ERROR("[physx] create kinematic mesh failed!");
            return nullptr;
        }
        mScene->addActor(*mesh);
        mPhysicsActors.push_back(mesh);
        return mesh;
    }
    else
    {
        ERROR("[physx] createTriangleMesh fail.");
        return nullptr;
    }
}

physx::PxRigidActor* PhysxSceneImpl::CreateMeshStatic(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, const std::vector<float> &vb, const std::vector<uint16_t> &ib) {
    physx::PxSceneWriteLock scopedLock(*mScene);
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
    bool ok = mCooking->cookTriangleMesh(meshDesc, streamout);
    if (!ok) {
        ERROR("[physx] cookTriangleMesh fail.");
        return nullptr;
    }

    physx::PxDefaultMemoryInputData streamin(streamout.getData(), streamout.getSize());
    physx::PxTriangleMesh* triangleMesh = mPhysicsSDK->createTriangleMesh(streamin);
    if (triangleMesh) {
        physx::PxMeshScale meshScale = physx::PxMeshScale(physx::PxVec3{ scaleX ,scaleY ,scaleZ }, physx::PxQuat(physx::PxIdentity));
        physx::PxTriangleMeshGeometry triGeom;
        triGeom.triangleMesh = triangleMesh;
        triGeom.scale = meshScale;
        physx::PxRigidStatic* mesh = PxCreateStatic(*mPhysicsSDK, physx::PxTransform(posX, posY, posZ), triGeom, *mMaterial);
        if (!mesh) {
            ERROR("[physx] create static mesh failed!");
            return nullptr;
        }
        mScene->addActor(*mesh);
        mPhysicsActors.push_back(mesh);
        return mesh;
    }
    else
    {
        ERROR("[physx] createTriangleMesh fail.");
        return nullptr;
    }
}

void PhysxSceneImpl::SetGlobalPostion(physx::PxRigidActor* actor, float posX, float posY, float posZ) {
    auto pose = actor->getGlobalPose();
    pose.p.x = posX;
    pose.p.y = posY;
    pose.p.z = posZ;
    actor->setGlobalPose(pose);
}

void PhysxSceneImpl::SetGlobalRotate(physx::PxRigidActor* actor, float rotateX, float rotateY, float rotateZ, float rotateW) {
    auto pose = actor->getGlobalPose();
    pose.q.x = rotateX;
    pose.q.y = rotateY;
    pose.q.z = rotateZ;
    pose.q.w = rotateW;
    actor->setGlobalPose(pose);
}


void PhysxSceneImpl::CreateScene(const std::string &path) {
    auto sceneInfo = gSceneInfoMgr->Get(path);
    if (sceneInfo == nullptr)
    {
        sceneInfo = std::make_shared<SceneInfo>();
        if (sceneInfo->Load(path)) {
            gSceneInfoMgr->Set(path, sceneInfo);
        }
        else {
            sceneInfo = nullptr;
        }
    }
    if (sceneInfo != nullptr)
    {
        for (size_t i = 0; i < sceneInfo->Terrains.size(); i++)
        {
            auto &info = sceneInfo->Terrains[i];
            auto actor = CreateHeightField(info.data, info.d, info.d, info.xSize / (info.d - 1), info.zSize / (info.d - 1), 1);
            SetGlobalPostion(actor, info.xPostion, info.yPostion, info.zPostion);
            SetGlobalRotate(actor, info.xRotate, info.yRotate, info.zRotate, info.wRotate);
        }
        for (size_t i = 0; i < sceneInfo->Boxs.size(); i++)
        {
            auto &info = sceneInfo->Boxs[i];
            auto actor = CreateBoxStatic(info.xPostion, info.yPostion, info.zPostion, info.xHalf, info.yHalf, info.zHalf);
            SetGlobalRotate(actor, info.xRotate, info.yRotate, info.zRotate, info.wRotate);
        }
        for (size_t i = 0; i < sceneInfo->Capsules.size(); i++)
        {
            auto &info = sceneInfo->Capsules[i];
            auto actor = CreateCapsuleStatic(info.xPostion, info.yPostion, info.zPostion, info.radius, info.halfHeight);
            SetGlobalRotate(actor, info.xRotate, info.yRotate, info.zRotate, info.wRotate);
        }
        for (size_t i = 0; i < sceneInfo->Meshs.size(); i++)
        {
            auto &info = sceneInfo->Meshs[i];
            auto actor = CreateMeshStatic(info.xPostion, info.yPostion, info.zPostion, info.xScale, info.yScale, info.zScale, info.vb, info.ib);
            SetGlobalRotate(actor, info.xRotate, info.yRotate, info.zRotate, info.wRotate);
        }
        for (size_t i = 0; i < sceneInfo->Spheres.size(); i++)
        {
            auto &info = sceneInfo->Spheres[i];
            auto actor = CreateSphereStatic(info.xPostion, info.yPostion, info.zPostion, info.radius);
            SetGlobalRotate(actor, info.xRotate, info.yRotate, info.zRotate, info.wRotate);
        }
    }
}