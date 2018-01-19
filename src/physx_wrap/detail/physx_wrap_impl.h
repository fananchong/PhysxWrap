#ifndef __PHYSX_WRAP_IMPL_H__
#define __PHYSX_WRAP_IMPL_H__

#include <PxPhysics.h>
#include <foundation/PxFoundation.h>
#include <extensions/PxDefaultCpuDispatcher.h>
#include <cooking/PxCooking.h>
#include <PxScene.h>
#include <PxRigidActor.h>
#include <atomic>
#include <unordered_map>
#include "physx_pvd.h"
#include "../PhysxWrap.h"

namespace PhysxWrap {

    class PhysxSceneImpl
    {
    public:
        PhysxSceneImpl();
        ~PhysxSceneImpl();

        bool Init();
        bool CreateScene(const std::string &path);
        void Update(float elapsedTime);
        physx::PxRigidActor* CreatePlane(float xNormal, float yNormal, float zNormal, float distance);
        physx::PxRigidActor* CreateHeightField(const std::vector<int16_t> &heightmap, unsigned columns, unsigned rows, const Vector3 &scale);
        physx::PxRigidActor* CreateHeightField(const physx::PxHeightFieldGeometry &hfGeom);
        physx::PxRigidActor* CreateBoxDynamic(const Vector3 &pos, const Vector3 &halfExtents, float density);
        physx::PxRigidActor* CreateBoxKinematic(const Vector3 &pos, const Vector3 &halfExtents, float density);
        physx::PxRigidActor* CreateBoxStatic(const Vector3 &pos, const Vector3 &halfExtents);
        physx::PxRigidActor* CreateSphereDynamic(const Vector3 &pos, float radius, float density);
        physx::PxRigidActor* CreateSphereKinematic(const Vector3 &pos, float radius, float density);
        physx::PxRigidActor* CreateSphereStatic(const Vector3 &pos, float radius);
        physx::PxRigidActor* CreateCapsuleDynamic(const Vector3 &pos, float radius, float halfHeight, float density);
        physx::PxRigidActor* CreateCapsuleKinematic(const Vector3 &pos, float radius, float halfHeight, float density);
        physx::PxRigidActor* CreateCapsuleStatic(const Vector3 &pos, float radius, float halfHeight);
        physx::PxRigidActor* CreateMeshKinematic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib, float density);
        physx::PxRigidActor* CreateMeshKinematic(const Vector3 &pos, const physx::PxTriangleMeshGeometry &triGeom, float density);
        physx::PxRigidActor* CreateMeshStatic(const Vector3 &pos, const Vector3 &scale, const std::vector<float> &vb, const std::vector<uint16_t> &ib);
        physx::PxRigidActor* CreateMeshStatic(const Vector3 &pos, const physx::PxTriangleMeshGeometry &triGeom);

        void RemoveActor(physx::PxRigidActor* actor);

        void SetLinearVelocity(physx::PxRigidActor* actor, const Vector3 &velocity);
        void AddForce(physx::PxRigidActor* actor, const Vector3 &force);
        void ClearForce(physx::PxRigidActor* actor);

        Vector3 GetGlobalPostion(physx::PxRigidActor* actor);
        Quat GetGlobalRotate(physx::PxRigidActor* actor);
        void SetGlobalPostion(physx::PxRigidActor* actor, const Vector3 &pos);
        void SetGlobalRotate(physx::PxRigidActor* actor, const Quat &rotate);

        bool IsStaticObj(physx::PxRigidActor* actor);
        bool IsDynamicObj(physx::PxRigidActor* actor);

        void SetCurrentMaterial(float staticFriction, float dynamicFriction, float restitution);
        void SetCurrentAngularDamping(float value);

    protected:
        virtual void customizeTolerances(physx::PxTolerancesScale&) {}
        virtual void customizeSceneDesc(physx::PxSceneDesc& desc) {}

    private:
        void release();

        physx::PxScene* mScene;
        physx::PxDefaultCpuDispatcher* mCpuDispatcher;
        physx::PxMaterial* mMaterial;
        void* mScratchBlock;
        float mAngularDamping;
        std::unordered_map<physx::PxRigidActor*, int> mPhysicsActors;

        friend class PhysxScene;
    };

};

#endif
