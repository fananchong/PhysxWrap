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

    DLLIMPORT void * CreateScene(const char *path) {
        auto s = new PhysxWrap::PhysxScene();
        if (s && s->Init()) {
            s->CreateScene(path);
            return (void *)s;
        }

        if (s) delete s;
        return nullptr;
    }

    DLLIMPORT void DestroyScene(void *scene) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        if (s) delete s;
    }

    DLLIMPORT void UpdateScene(void *scene, float elapsedTime) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->Update(elapsedTime);
    }

    DLLIMPORT UINT64 CreatePlane(void *scene, float yAxis) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreatePlane(yAxis);
    }

    DLLIMPORT UINT64 CreateBoxDynamic(void *scene, float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateBoxDynamic(PhysxWrap::Vector3{ posX, posY, posZ }, PhysxWrap::Vector3{ halfExtentsX, halfExtentsY,halfExtentsZ });
    }

    DLLIMPORT UINT64 CreateBoxKinematic(void *scene, float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateBoxKinematic(PhysxWrap::Vector3{ posX, posY, posZ }, PhysxWrap::Vector3{ halfExtentsX, halfExtentsY,halfExtentsZ });
    }

    DLLIMPORT UINT64 CreateBoxStatic(void *scene, float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateBoxStatic(PhysxWrap::Vector3{ posX, posY, posZ }, PhysxWrap::Vector3{ halfExtentsX, halfExtentsY,halfExtentsZ });
    }

    DLLIMPORT UINT64 CreateSphereDynamic(void *scene, float posX, float posY, float posZ, float radius) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateSphereDynamic(PhysxWrap::Vector3{ posX, posY, posZ }, radius);
    }

    DLLIMPORT UINT64 CreateSphereKinematic(void *scene, float posX, float posY, float posZ, float radius) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateSphereKinematic(PhysxWrap::Vector3{ posX, posY, posZ }, radius);
    }

    DLLIMPORT UINT64 CreateSphereStatic(void *scene, float posX, float posY, float posZ, float radius) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateSphereStatic(PhysxWrap::Vector3{ posX, posY, posZ }, radius);
    }

    DLLIMPORT UINT64 CreateCapsuleDynamic(void *scene, float posX, float posY, float posZ, float radius, float halfHeight) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateCapsuleDynamic(PhysxWrap::Vector3{ posX, posY, posZ }, radius, halfHeight);
    }

    DLLIMPORT UINT64 CreateCapsuleKinematic(void *scene, float posX, float posY, float posZ, float radius, float halfHeight) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateCapsuleKinematic(PhysxWrap::Vector3{ posX, posY, posZ }, radius, halfHeight);
    }

    DLLIMPORT UINT64 CreateCapsuleStatic(void *scene, float posX, float posY, float posZ, float radius, float halfHeight) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->CreateCapsuleStatic(PhysxWrap::Vector3{ posX, posY, posZ }, radius, halfHeight);
    }

    DLLIMPORT void RemoveActor(void *scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->RemoveActor(id);
    }

    DLLIMPORT void SetLinearVelocity(void *scene, UINT64 id, float velocityX, float velocityY, float velocityZ) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetLinearVelocity(id, PhysxWrap::Vector3{ velocityX, velocityY, velocityZ });
    }

    DLLIMPORT void AddForce(void *scene, UINT64 id, float forceX, float forceY, float forceZ) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->AddForce(id, PhysxWrap::Vector3{ forceX, forceY, forceZ });
    }

    DLLIMPORT void ClearForce(void *scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->ClearForce(id);
    }

    DLLIMPORT void GetGlobalPostion(void *scene, UINT64 id, void *outPostionX, void *outPostionY, void *outPostionZ) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        auto p = s->GetGlobalPostion(id);
        *(float*)outPostionX = p.X;
        *(float*)outPostionY = p.Y;
        *(float*)outPostionZ = p.Z;
    }

    DLLIMPORT void GetGlobalRotate(void *scene, UINT64 id, void *outRotateX, void *outRotateY, void *outRotateZ, void *outRotateW) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        auto r = s->GetGlobalRotate(id);
        *(float*)outRotateX = r.X;
        *(float*)outRotateY = r.Y;
        *(float*)outRotateX = r.Z;
        *(float*)outRotateW = r.W;
    }

    DLLIMPORT void SetGlobalPostion(void *scene, UINT64 id, float posX, float posY, float posZ) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetGlobalPostion(id, PhysxWrap::Vector3{ posX, posY, posZ });
    }

    DLLIMPORT void SetGlobalRotate(void *scene, UINT64 id, float rotateX, float rotateY, float rotateZ, float rotateW) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetGlobalRotate(id, PhysxWrap::Quat{ rotateX, rotateY, rotateZ, rotateW });
    }

    DLLIMPORT int IsStaticObj(void *scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->IsStaticObj(id) ? 1 : 0;
    }

    DLLIMPORT int IsDynamicObj(void *scene, UINT64 id) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        return s->IsDynamicObj(id) ? 1 : 0;
    }

    DLLIMPORT void SetCurrentMaterial(void *scene, float staticFriction, float dynamicFriction, float restitution) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetCurrentMaterial(staticFriction, dynamicFriction, restitution);
    }

    DLLIMPORT void SetCurrentAngularDamping(void *scene, float value) {
        auto s = (PhysxWrap::PhysxScene*)scene;
        s->SetCurrentAngularDamping(value);
    }

#ifdef __cplusplus
}
#endif