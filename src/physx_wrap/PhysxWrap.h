#ifndef __PHYSX_WRAP_H__
#define __PHYSX_WRAP_H__

#include <string>
#include <vector>

#ifdef EXPORT_DLL
#define MY_DLL_EXPORT_CLASS __declspec(dllexport)
#define MY_DLL_EXPORT_FUNC extern "C" __declspec(dllexport)
#else
#define MY_DLL_EXPORT_CLASS
#define MY_DLL_EXPORT_FUNC
#endif

namespace PhysxWrap {
    struct MY_DLL_EXPORT_CLASS Vector3 {
        float X;
        float Y;
        float Z;
    };

    struct MY_DLL_EXPORT_CLASS Quat {
        float X;
        float Y;
        float Z;
        float W;
    };

    class PhysxSceneImpl;
    class MY_DLL_EXPORT_CLASS PhysxScene
    {
    public:
        PhysxScene();
        ~PhysxScene();

        bool Init();
        bool CreateScene(const std::string &path);
        void Update(float elapsedTime); // second

        uint64_t CreatePlane(float yAxis);
        uint64_t CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, const Vector3 &scale);
        uint64_t CreateBoxDynamic(const Vector3 &pos, const Vector3 &halfExtents);
        uint64_t CreateBoxKinematic(const Vector3 &pos, const Vector3 &halfExtents);
        uint64_t CreateBoxStatic(const Vector3 &pos, const Vector3 &halfExtents);
        uint64_t CreateSphereDynamic(const Vector3 &pos, float radius);
        uint64_t CreateSphereKinematic(const Vector3 &pos, float radius);
        uint64_t CreateSphereStatic(const Vector3 &pos, float radius);
        uint64_t CreateCapsuleDynamic(const Vector3 &pos, float radius, float halfHeight);
        uint64_t CreateCapsuleKinematic(const Vector3 &pos, float radius, float halfHeight);
        uint64_t CreateCapsuleStatic(const Vector3 &pos, float radius, float halfHeight);
        uint64_t CreateMeshKinematic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib);
        uint64_t CreateMeshStatic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib);

        void RemoveActor(uint64_t id);

        void SetLinearVelocity(uint64_t id, const Vector3 &velocity);
        void AddForce(uint64_t id, const Vector3 &force);
        void ClearForce(uint64_t id);

        Vector3 GetGlobalPostion(uint64_t id);
        Quat GetGlobalRotate(uint64_t id);
        void SetGlobalPostion(uint64_t id, const Vector3 &pos);
        void SetGlobalRotate(uint64_t id, const Quat &rotate);

        bool IsStaticObj(uint64_t id);
        bool IsDynamicObj(uint64_t id);

    private:
        void release();
        PhysxSceneImpl* mImpl;
    };

    MY_DLL_EXPORT_FUNC unsigned GetStaticObjCountInScene(const std::string &path);
    MY_DLL_EXPORT_FUNC bool InitPhysxSDK();
};

#endif
