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

    typedef struct { float X; float Y; float Z; } Vector3;
    typedef struct { float X; float Y;  float Z;  float W; } Quat;
    typedef struct { int _1; } *PxScene;

    DLLIMPORT int InitPhysxSDK();
    DLLIMPORT PxScene CreateScene(const char *path);
    DLLIMPORT void DestroyScene(PxScene scene);
    DLLIMPORT void UpdateScene(PxScene scene, float elapsedTime); // second

    DLLIMPORT UINT64 CreatePlane(PxScene scene, float yAxis);
    DLLIMPORT UINT64 CreateBoxDynamic(PxScene scene, const Vector3* pos, const Vector3* halfExtents);
    DLLIMPORT UINT64 CreateBoxKinematic(PxScene scene, const Vector3* pos, const Vector3* halfExtents);
    DLLIMPORT UINT64 CreateBoxStatic(PxScene scene, const Vector3* pos, const Vector3* halfExtents);
    DLLIMPORT UINT64 CreateSphereDynamic(PxScene scene, const Vector3* pos, float radius);
    DLLIMPORT UINT64 CreateSphereKinematic(PxScene scene, const Vector3* pos, float radius);
    DLLIMPORT UINT64 CreateSphereStatic(PxScene scene, const Vector3* pos, float radius);
    DLLIMPORT UINT64 CreateCapsuleDynamic(PxScene scene, const Vector3* pos, float radius, float halfHeight);
    DLLIMPORT UINT64 CreateCapsuleKinematic(PxScene scene, const Vector3* pos, float radius, float halfHeight);
    DLLIMPORT UINT64 CreateCapsuleStatic(PxScene scene, const Vector3* pos, float radius, float halfHeight);

    DLLIMPORT void SetLinearVelocity(PxScene scene, UINT64 id, const Vector3* velocity);
    DLLIMPORT void AddForce(PxScene scene, UINT64 id, const Vector3* force);
    DLLIMPORT void ClearForce(PxScene scene, UINT64 id);

    DLLIMPORT void GetGlobalPostion(PxScene scene, UINT64 id, Vector3* outPostion);
    DLLIMPORT void GetGlobalRotate(PxScene scene, UINT64 id, Quat* outRotate);
    DLLIMPORT void SetGlobalPostion(PxScene scene, UINT64 id, const Vector3* pos);
    DLLIMPORT void SetGlobalRotate(PxScene scene, UINT64 id, const Quat* rotate);

#ifdef __cplusplus
}
#endif

#endif
