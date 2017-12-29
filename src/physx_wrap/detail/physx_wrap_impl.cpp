#include "physx_wrap_impl.h"
#include <PxSceneLock.h>
#include <PxRigidStatic.h>
#include <PxRigidDynamic.h>
#include <extensions/PxExtensionsAPI.h>
#include <cassert>
#include "log.h"
#include "util.h"
#include "scene_info_mgr.h"
#include "physx_sdk.h"

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

#ifdef SCENE_SAFE_THREAD
#define SCENE_LOCK() physx::PxSceneWriteLock scopedLock(*mScene);
#else
#define SCENE_LOCK()
#endif

#define	SAFE_RELEASE(x)	if(x){ x->release(); x = NULL;	}
#define DEFAULT_RIGID_DYNAMIC(ACTOR)                                                \
    ACTOR->setAngularDamping(0.5f);                                                 \
    ACTOR->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, false);             \

#define DEFAULT_RIGID_DYNAMIC_DEBUG(ACTOR)                                          \
    DEFAULT_RIGID_DYNAMIC(ACTOR)                                                    \
    ACTOR->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);                  \

PhysxSceneImpl::PhysxSceneImpl()
    : mScene(nullptr)
    , mCpuDispatcher(nullptr)
    , mTimestep(1.0f / 60.0f)
{

}

PhysxSceneImpl::~PhysxSceneImpl() {
    release();
#ifdef _DEBUG
    INFO("call ~PhysxSceneImpl()");
#endif
}

bool PhysxSceneImpl::Init(float timestep) {
    physx::PxSceneDesc sceneDesc(gPhysxSDKImpl->GetPhysics()->getTolerancesScale());
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
    mScene = gPhysxSDKImpl->GetPhysics()->createScene(sceneDesc);
    if (!mScene) {
        ERROR("[physx] createScene failed!");
        release();
        return false;
    }
#ifdef _DEBUG
    mScene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1.0f);
    mScene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
    physx::PxPvdSceneClient* pvdClient = mScene->getScenePvdClient();
    if (pvdClient)
    {
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }
#endif
    mTimestep = timestep;
    return true;
}

void PhysxSceneImpl::release() {
    {
        SCENE_LOCK();
        for (size_t i = 0; i < mPhysicsActors.size(); ++i)
            mPhysicsActors[i]->release();
        mPhysicsActors.clear();
    }
    SAFE_RELEASE(mScene);
    SAFE_RELEASE(mCpuDispatcher);
}

void PhysxSceneImpl::Update(float dtime) {
    if (mScene != nullptr) {
        while (dtime > 0.0f)
        {
            const float dt = (dtime >= mTimestep ? mTimestep : dtime);
            mScene->simulate(dt);
            mScene->fetchResults();
            dtime -= dt;
        }
    }
}

physx::PxRigidActor* PhysxSceneImpl::CreatePlane(float xNormal, float yNormal, float zNormal, float distance) {
    SCENE_LOCK();
    physx::PxRigidStatic* plane = physx::PxCreatePlane(*gPhysxSDKImpl->GetPhysics(), physx::PxPlane(physx::PxVec3(zNormal, yNormal, zNormal), distance), gPhysxSDKImpl->GetMaterial());
    if (!plane) {
        ERROR("[physx] create plane failed!");
        return nullptr;
    }
    mScene->addActor(*plane);
    mPhysicsActors.push_back(plane);
    return plane;
}

physx::PxRigidActor* PhysxSceneImpl::CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, const Vector3 &scale) {
    physx::PxHeightFieldGeometry hfGeom;
    if (GetHeightFieldGeometry(hfGeom, heightmap, columns, rows, scale) == false) {
        ERROR("[physx] creating heightfield geometry failed");
        return nullptr;
    }
    return CreateHeightField(hfGeom);
}

physx::PxRigidActor* PhysxSceneImpl::CreateHeightField(const physx::PxHeightFieldGeometry &hfGeom) {
    SCENE_LOCK();
    auto columns = hfGeom.heightField->getNbColumns();
    auto rows = hfGeom.heightField->getNbRows();
    physx::PxTransform pose = physx::PxTransform(physx::PxIdentity);
    pose.p = physx::PxVec3(-(float(columns) / 2 * hfGeom.columnScale), 0, -(float(rows) / 2 * hfGeom.rowScale));
    physx::PxRigidStatic* hfActor = gPhysxSDKImpl->GetPhysics()->createRigidStatic(pose);
    if (!hfActor) {
        ERROR("[physx] creating heightfield actor failed");
        return nullptr;
    }
    physx::PxShape* hfShape = physx::PxRigidActorExt::createExclusiveShape(*hfActor, hfGeom, gPhysxSDKImpl->GetMaterial());
    if (!hfShape) {
        ERROR("[physx] creating heightfield shape failed");
        return nullptr;
    }
    mScene->addActor(*hfActor);
    mPhysicsActors.push_back(hfActor);
    return hfActor;
}

physx::PxRigidActor* PhysxSceneImpl::CreateBoxDynamic(const Vector3 &pos, const Vector3 &halfExtents, float density) {
    SCENE_LOCK();
    physx::PxRigidDynamic* box = PxCreateDynamic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxBoxGeometry(halfExtents.X, halfExtents.Y, halfExtents.Z), gPhysxSDKImpl->GetMaterial(), density);
    if (!box) {
        ERROR("[physx] create dynamic box failed!");
        return nullptr;
    }
#ifdef _DEBUG
    DEFAULT_RIGID_DYNAMIC_DEBUG(box);
#else
    DEFAULT_RIGID_DYNAMIC(box);
#endif
    mScene->addActor(*box);
    mPhysicsActors.push_back(box);
    return box;
}

physx::PxRigidActor* PhysxSceneImpl::CreateBoxKinematic(const Vector3 &pos, const Vector3 &halfExtents, float density) {
    SCENE_LOCK();
    physx::PxRigidDynamic* box = PxCreateKinematic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxBoxGeometry(halfExtents.X, halfExtents.Y, halfExtents.Z), gPhysxSDKImpl->GetMaterial(), density);
    if (!box) {
        ERROR("[physx] create kinematic box failed!");
        return nullptr;
    }
    mScene->addActor(*box);
    mPhysicsActors.push_back(box);
    return box;
}

physx::PxRigidActor* PhysxSceneImpl::CreateBoxStatic(const Vector3 &pos, const Vector3 &halfExtents) {
    SCENE_LOCK();
    physx::PxRigidStatic* box = PxCreateStatic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxBoxGeometry(halfExtents.X, halfExtents.Y, halfExtents.Z), gPhysxSDKImpl->GetMaterial());
    if (!box) {
        ERROR("[physx] create static box failed!");
        return nullptr;
    }
    mScene->addActor(*box);
    mPhysicsActors.push_back(box);
    return box;
}

physx::PxRigidActor* PhysxSceneImpl::CreateSphereDynamic(const Vector3 &pos, float radius, float density) {
    SCENE_LOCK();
    physx::PxRigidDynamic* sphere = PxCreateDynamic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxSphereGeometry(radius), gPhysxSDKImpl->GetMaterial(), density);
    if (!sphere) {
        ERROR("[physx] create dynamic sphere failed!");
        return nullptr;
    }
#ifdef _DEBUG
    DEFAULT_RIGID_DYNAMIC_DEBUG(sphere);
#else
    DEFAULT_RIGID_DYNAMIC(sphere);
#endif
    mScene->addActor(*sphere);
    mPhysicsActors.push_back(sphere);
    return sphere;
}

physx::PxRigidActor* PhysxSceneImpl::CreateSphereKinematic(const Vector3 &pos, float radius, float density) {
    SCENE_LOCK();
    physx::PxRigidDynamic* sphere = PxCreateKinematic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxSphereGeometry(radius), gPhysxSDKImpl->GetMaterial(), density);
    if (!sphere) {
        ERROR("[physx] create kinematic sphere failed!");
        return nullptr;
    }
    mScene->addActor(*sphere);
    mPhysicsActors.push_back(sphere);
    return sphere;
}

physx::PxRigidActor*  PhysxSceneImpl::CreateSphereStatic(const Vector3 &pos, float radius) {
    SCENE_LOCK();
    physx::PxRigidStatic* sphere = PxCreateStatic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxSphereGeometry(radius), gPhysxSDKImpl->GetMaterial());
    if (!sphere) {
        ERROR("[physx] create static sphere failed!");
        return nullptr;
    }
    mScene->addActor(*sphere);
    mPhysicsActors.push_back(sphere);
    return sphere;
}

physx::PxRigidActor* PhysxSceneImpl::CreateCapsuleDynamic(const Vector3 &pos, float radius, float halfHeight, float density) {
    SCENE_LOCK();
    physx::PxRigidDynamic* capsule = PxCreateDynamic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxCapsuleGeometry(radius, halfHeight), gPhysxSDKImpl->GetMaterial(), density);
    if (!capsule) {
        ERROR("[physx] create dynamic capsule failed!");
        return nullptr;
    }
#ifdef _DEBUG
    DEFAULT_RIGID_DYNAMIC_DEBUG(capsule);
#else
    DEFAULT_RIGID_DYNAMIC(capsule);
#endif
    mScene->addActor(*capsule);
    mPhysicsActors.push_back(capsule);
    return capsule;
}

physx::PxRigidActor* PhysxSceneImpl::CreateCapsuleKinematic(const Vector3 &pos, float radius, float halfHeight, float density) {
    SCENE_LOCK();
    physx::PxRigidDynamic* capsule = PxCreateKinematic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxCapsuleGeometry(radius, halfHeight), gPhysxSDKImpl->GetMaterial(), density);
    if (!capsule) {
        ERROR("[physx] create kinematic capsule failed!");
        return nullptr;
    }
    mScene->addActor(*capsule);
    mPhysicsActors.push_back(capsule);
    return capsule;
}

physx::PxRigidActor* PhysxSceneImpl::CreateCapsuleStatic(const Vector3 &pos, float radius, float halfHeight) {
    SCENE_LOCK();
    physx::PxRigidStatic* capsule = PxCreateStatic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), physx::PxCapsuleGeometry(radius, halfHeight), gPhysxSDKImpl->GetMaterial());
    if (!capsule) {
        ERROR("[physx] create static capsule failed!");
        return nullptr;
    }
    mScene->addActor(*capsule);
    mPhysicsActors.push_back(capsule);
    return capsule;
}

physx::PxRigidActor* PhysxSceneImpl::CreateMeshKinematic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib, float density) {
    physx::PxTriangleMeshGeometry triGeom;
    if (GetMeshGeometry(triGeom, pos, scale, vb, ib) == false) {
        return nullptr;
    }
    return CreateMeshKinematic(pos, triGeom, density);
}

physx::PxRigidActor* PhysxSceneImpl::CreateMeshKinematic(const Vector3 &pos, const physx::PxTriangleMeshGeometry &triGeom, float density) {
    SCENE_LOCK();
    physx::PxRigidDynamic* mesh = PxCreateKinematic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), triGeom, gPhysxSDKImpl->GetMaterial(), density);
    if (!mesh) {
        ERROR("[physx] create kinematic mesh failed!");
        return nullptr;
    }
    mScene->addActor(*mesh);
    mPhysicsActors.push_back(mesh);
    return mesh;
}

physx::PxRigidActor* PhysxSceneImpl::CreateMeshStatic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib) {
    physx::PxTriangleMeshGeometry triGeom;
    if (GetMeshGeometry(triGeom, pos, scale, vb, ib) == false) {
        return nullptr;
    }
    return CreateMeshStatic(pos, triGeom);
}

physx::PxRigidActor* PhysxSceneImpl::CreateMeshStatic(const Vector3 &pos, const physx::PxTriangleMeshGeometry &triGeom) {
    SCENE_LOCK();
    physx::PxRigidStatic* mesh = PxCreateStatic(*gPhysxSDKImpl->GetPhysics(), physx::PxTransform(pos.X, pos.Y, pos.Z), triGeom, gPhysxSDKImpl->GetMaterial());
    if (!mesh) {
        ERROR("[physx] create static mesh failed!");
        return nullptr;
    }
    mScene->addActor(*mesh);
    mPhysicsActors.push_back(mesh);
    return mesh;
}

void PhysxSceneImpl::SetLinearVelocity(physx::PxRigidActor* actor, const Vector3 &velocity) {
    if (actor->getType() == physx::PxActorType::eRIGID_DYNAMIC) {
        auto dynamicActor = (physx::PxRigidDynamic*)actor;
        dynamicActor->setLinearVelocity(physx::PxVec3{ velocity.X, velocity.Y, velocity.Z });
    }
}

void PhysxSceneImpl::AddForce(physx::PxRigidActor* actor, const Vector3 &force) {
    if (actor->getType() == physx::PxActorType::eRIGID_DYNAMIC) {
        auto dynamicActor = (physx::PxRigidDynamic*)actor;
        dynamicActor->addForce(physx::PxVec3{ force.X, force.Y, force.Z });
    }
}

void PhysxSceneImpl::ClearForce(physx::PxRigidActor* actor) {
    if (actor->getType() == physx::PxActorType::eRIGID_DYNAMIC) {
        auto dynamicActor = (physx::PxRigidDynamic*)actor;
        dynamicActor->clearForce();
    }
}

Vector3 PhysxSceneImpl::GetGlobalPostion(physx::PxRigidActor* actor) {
    auto pose = actor->getGlobalPose();
    return Vector3{ pose.p.x, pose.p.y, pose.p.z };
}

Quat PhysxSceneImpl::GetGlobalRotate(physx::PxRigidActor* actor) {
    auto pose = actor->getGlobalPose();
    return Quat{ pose.q.x, pose.q.y, pose.q.z, pose.q.w };
}

void PhysxSceneImpl::SetGlobalPostion(physx::PxRigidActor* actor, const Vector3 &pos) {
    auto pose = actor->getGlobalPose();
    pose.p.x = pos.X;
    pose.p.y = pos.Y;
    pose.p.z = pos.Z;
    actor->setGlobalPose(pose);
}

void PhysxSceneImpl::SetGlobalRotate(physx::PxRigidActor* actor, const Quat &rotate) {
    auto pose = actor->getGlobalPose();
    pose.q.x = rotate.X;
    pose.q.y = rotate.Y;
    pose.q.z = rotate.Z;
    pose.q.w = rotate.W;
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
            auto actor = CreateHeightField(info.Geom);
            SetGlobalPostion(actor, info.Postion);
            SetGlobalRotate(actor, info.Rotate);
        }
        for (size_t i = 0; i < sceneInfo->Boxs.size(); i++)
        {
            auto &info = sceneInfo->Boxs[i];
            auto actor = CreateBoxStatic(info.Postion, info.Half);
            SetGlobalRotate(actor, info.Rotate);
        }
        for (size_t i = 0; i < sceneInfo->Capsules.size(); i++)
        {
            auto &info = sceneInfo->Capsules[i];
            auto actor = CreateCapsuleStatic(info.Postion, info.Radius, info.HalfHeight);
            SetGlobalRotate(actor, info.Rotate);
        }
        for (size_t i = 0; i < sceneInfo->Meshs.size(); i++)
        {
            auto &info = sceneInfo->Meshs[i];
            auto actor = CreateMeshStatic(info.Postion, info.Scale, info.VB, info.IB);
            SetGlobalRotate(actor, info.Rotate);
        }
        for (size_t i = 0; i < sceneInfo->Spheres.size(); i++)
        {
            auto &info = sceneInfo->Spheres[i];
            auto actor = CreateSphereStatic(info.Postion, info.Radius);
            SetGlobalRotate(actor, info.Rotate);
        }
    }
}