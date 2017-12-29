#include "physx_pvd.h"
#include "log.h"

#define	SAFE_RELEASE(x)	if(x){ x->release(); x = NULL;	}

namespace PhysxWrap {

    PhysxPVD::PhysxPVD()
        : mPvd(nullptr)
        , mTransport(nullptr)
        , mIp("127.0.0.1")
        , mPort(5425)
        , mTimeout(10)
        , mUseFullPvdConnection(true)
        , mFoundation(nullptr)
    {

    }

    PhysxPVD::~PhysxPVD() {
        Close();
    }

    void PhysxPVD::Init(physx::PxFoundation* foundation, const std::string &ip, unsigned port, unsigned timeout, bool useFullPvdConnection) {
        mFoundation = foundation;
        mIp = ip;
        mPort = port;
        mTimeout = timeout;
        mUseFullPvdConnection = useFullPvdConnection;
    }

    void PhysxPVD::CreatePvdConnection() {
        mTransport = physx::PxDefaultPvdSocketTransportCreate(mIp.c_str(), mPort, mTimeout);
        if (mTransport == NULL) {
            ERROR("[physx] CreatePvdConnection fail. #1");
            return;
        }
        mPvdFlags = physx::PxPvdInstrumentationFlag::eALL;
        if (!mUseFullPvdConnection)
        {
            mPvdFlags = physx::PxPvdInstrumentationFlag::ePROFILE;
        }

        mPvd = physx::PxCreatePvd(*mFoundation);
        if (mPvd->connect(*mTransport, mPvdFlags) == false) {
            ERROR("[physx] CreatePvdConnection fail. #2");
            return;
        }
    }

    void PhysxPVD::Close() {
        SAFE_RELEASE(mPvd);
        SAFE_RELEASE(mTransport);
    }
}