#ifndef __PHYSX_PVD_H__
#define __PHYSX_PVD_H__

#include <pvd/PxPvd.h>
#include <pvd/PxPvdTransport.h>
#include <PxPhysics.h>
#include <string>

namespace PhysxWrap {

    class PhysxPVD
    {
    public:
        PhysxPVD();
        ~PhysxPVD();

        void Init(physx::PxFoundation* foundation, const std::string &ip = "127.0.0.1", unsigned port = 5425, unsigned timeout = 10, bool useFullPvdConnection = true);
        void CreatePvdConnection();
        void Close();

        physx::PxPvd* GetPvdInstance() { return mPvd; }

    private:
        physx::PxPvd* mPvd;
        physx::PxPvdTransport* mTransport;
        physx::PxPvdInstrumentationFlags mPvdFlags;
        std::string mIp;
        unsigned mPort;
        unsigned mTimeout;
        bool mUseFullPvdConnection;

        // 
        physx::PxFoundation* mFoundation;
    };

};

#endif
