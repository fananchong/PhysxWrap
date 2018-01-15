#include "PhysxWrapGo.h"
#include "../physx_wrap/PhysxWrap.h"

#ifdef _MSC_VER
#pragma comment(lib, "PhysxWrap.lib")
#endif

#ifdef __cplusplus
extern "C" {
#endif

    DLLIMPORT int InitPhysxSDK() {
        return PhysxWrap::InitPhysxSDK() ? 1 : 0;
    }

    DLLIMPORT PxScene CreateScene(const char *path) {
        auto s = new PhysxWrap::PhysxScene();
        if (s && s->Init()) {
            s->CreateScene(path);
            return (PxScene)s;
        }

        if (s) delete s;
        return nullptr;
    }

    DLLIMPORT void DestroyScene(PxScene scene) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        if (s) delete s;
    }

    DLLIMPORT void UpdateScene(PxScene scene, float elapsedTime) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->Update(elapsedTime);
    }

    DLLIMPORT UINT64 CreatePlane(PxScene scene, float yAxis) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreatePlane(yAxis);
    }

    DLLIMPORT UINT64 CreateBoxDynamic(PxScene scene, const Vector3* pos, const Vector3* halfExtents) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateBoxDynamic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, PhysxWrap::Vector3{ halfExtents->X, halfExtents->Y,halfExtents->Z });
    }

    DLLIMPORT UINT64 CreateBoxKinematic(PxScene scene, const Vector3* pos, const Vector3* halfExtents) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateBoxKinematic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, PhysxWrap::Vector3{ halfExtents->X, halfExtents->Y,halfExtents->Z });
    }

    DLLIMPORT UINT64 CreateBoxStatic(PxScene scene, const Vector3* pos, const Vector3* halfExtents) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateBoxStatic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, PhysxWrap::Vector3{ halfExtents->X, halfExtents->Y,halfExtents->Z });
    }

    DLLIMPORT UINT64 CreateSphereDynamic(PxScene scene, const Vector3* pos, float radius) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateSphereDynamic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, radius);
    }

    DLLIMPORT UINT64 CreateSphereKinematic(PxScene scene, const Vector3* pos, float radius) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateSphereKinematic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, radius);
    }

    DLLIMPORT UINT64 CreateSphereStatic(PxScene scene, const Vector3* pos, float radius) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateSphereStatic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, radius);
    }

    DLLIMPORT UINT64 CreateCapsuleDynamic(PxScene scene, const Vector3* pos, float radius, float halfHeight) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateCapsuleDynamic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, radius, halfHeight);
    }

    DLLIMPORT UINT64 CreateCapsuleKinematic(PxScene scene, const Vector3* pos, float radius, float halfHeight) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateCapsuleKinematic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, radius, halfHeight);
    }

    DLLIMPORT UINT64 CreateCapsuleStatic(PxScene scene, const Vector3* pos, float radius, float halfHeight) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateCapsuleStatic(PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z }, radius, halfHeight);
    }

    DLLIMPORT void RemoveActor(PxScene scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->RemoveActor(id);
    }

    DLLIMPORT void SetLinearVelocity(PxScene scene, UINT64 id, const Vector3* velocity) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetLinearVelocity(id, PhysxWrap::Vector3{ velocity->X, velocity->Y, velocity->Z });
    }

    DLLIMPORT void AddForce(PxScene scene, UINT64 id, const Vector3* force) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->AddForce(id, PhysxWrap::Vector3{ force->X, force->Y, force->Z });
    }

    DLLIMPORT void ClearForce(PxScene scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->ClearForce(id);
    }

    DLLIMPORT void GetGlobalPostion(PxScene scene, UINT64 id, Vector3* pos) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        auto p = s->GetGlobalPostion(id);
        pos->X = p.X;
        pos->Y = p.Y;
        pos->Z = p.Z;
    }

    DLLIMPORT void GetGlobalRotate(PxScene scene, UINT64 id, Quat* rotate) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        auto r = s->GetGlobalRotate(id);
        rotate->X = r.X;
        rotate->Y = r.Y;
        rotate->Z = r.Z;
        rotate->W = r.W;
    }

    DLLIMPORT void SetGlobalPostion(PxScene scene, UINT64 id, const Vector3* pos) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetGlobalPostion(id, PhysxWrap::Vector3{ pos->X, pos->Y, pos->Z });
    }

    DLLIMPORT void SetGlobalRotate(PxScene scene, UINT64 id, const Quat* rotate) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetGlobalRotate(id, PhysxWrap::Quat{ rotate->X, rotate->Y, rotate->Z, rotate->W });
    }

    DLLIMPORT bool IsStaticObj(PxScene scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->IsStaticObj(id);
    }

    DLLIMPORT bool IsDynamicObj(PxScene scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->IsDynamicObj(id);
    }


#ifdef __cplusplus
}
#endif