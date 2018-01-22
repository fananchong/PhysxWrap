#ifndef __PXSCENE_H__
#define __PXSCENE_H__

#ifdef _WIN32
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT
#endif

#define UINT64 unsigned long long

#ifdef __cplusplus
extern "C" {
#endif

    DLLIMPORT int InitPhysxSDK();
    DLLIMPORT void* CreateScene(const char *path);
    DLLIMPORT void DestroyScene(void *scene);
    DLLIMPORT void UpdateScene(void *scene, float elapsedTime); // second

    DLLIMPORT UINT64 CreatePlane(void *scene, float yAxis);
    DLLIMPORT UINT64 CreateBoxDynamic(void *scene, float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ);
    DLLIMPORT UINT64 CreateBoxKinematic(void *scene, float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ);
    DLLIMPORT UINT64 CreateBoxStatic(void *scene, float posX, float posY, float posZ, float halfExtentsX, float halfExtentsY, float halfExtentsZ);
    DLLIMPORT UINT64 CreateSphereDynamic(void *scene, float posX, float posY, float posZ, float radius);
    DLLIMPORT UINT64 CreateSphereKinematic(void *scene, float posX, float posY, float posZ, float radius);
    DLLIMPORT UINT64 CreateSphereStatic(void *scene, float posX, float posY, float posZ, float radius);
    DLLIMPORT UINT64 CreateCapsuleDynamic(void *scene, float posX, float posY, float posZ, float radius, float halfHeight);
    DLLIMPORT UINT64 CreateCapsuleKinematic(void *scene, float posX, float posY, float posZ, float radius, float halfHeight);
    DLLIMPORT UINT64 CreateCapsuleStatic(void *scene, float posX, float posY, float posZ, float radius, float halfHeight);

    DLLIMPORT void RemoveActor(void *scene, UINT64 id);

    DLLIMPORT void SetLinearVelocity(void *scene, UINT64 id, float velocityX, float velocityY, float velocityZ);
    DLLIMPORT void AddForce(void *scene, UINT64 id, float forceX, float forceY, float forceZ);
    DLLIMPORT void ClearForce(void *scene, UINT64 id);

    DLLIMPORT void GetGlobalPostion(void *scene, UINT64 id, void *outPostionX, void *outPostionY, void *outPostionZ);
    DLLIMPORT void GetGlobalRotate(void *scene, UINT64 id, void *outRotateX, void *outRotateY, void *outRotateZ, void *outRotateW);
    DLLIMPORT void SetGlobalPostion(void *scene, UINT64 id, float posX, float posY, float posZ);
    DLLIMPORT void SetGlobalRotate(void *scene, UINT64 id, float rotateX, float rotateY, float rotateZ, float rotateW);

    DLLIMPORT int IsStaticObj(void *scene, UINT64 id);
    DLLIMPORT int IsDynamicObj(void *scene, UINT64 id);

    DLLIMPORT void SetCurrentMaterial(void *scene, float staticFriction, float dynamicFriction, float restitution);
    DLLIMPORT void SetCurrentAngularDamping(void *scene, float value);

#ifdef __cplusplus
}
#endif

#endif
