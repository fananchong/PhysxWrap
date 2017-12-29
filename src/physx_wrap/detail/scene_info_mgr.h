#ifndef __SCENE_INFO_MGR_H__
#define __SCENE_INFO_MGR_H__

#include <unordered_map>
#include <string>
#include <memory>
#include <geometry/PxHeightFieldGeometry.h>
#include <geometry/PxConvexMeshGeometry.h>
#include "../PhysxWrap.h"

namespace PhysxWrap {

    enum {
        eMeshData = 1,
        eBoxObj = 2,
        eCapsuleObj = 3,
        eMeshObj = 4,
        eTerrainObj = 5,
        eSphereObj = 6,
    };

    struct ObjInfoBase {
        Vector3 Postion;
        Quat Rotate;
        unsigned char Layer;
    };

    class MeshInfo : public ObjInfoBase
    {
        std::vector<float> vb;
        std::vector<uint16_t> ib;
        Vector3 scale;
    public:
        physx::PxTriangleMeshGeometry Geom;

        friend class SceneInfo;
    };

    class BoxInfo : public ObjInfoBase
    {
    public:
        Vector3 Half;
    };

    class CapsuleInfo : public ObjInfoBase
    {
    public:
        float Radius;
        float HalfHeight;
    };

    class TerrainInfo : public ObjInfoBase
    {
    public:
        physx::PxHeightFieldGeometry Geom;
    };

    class SphereInfo : public ObjInfoBase
    {
    public:
        float Radius;
    };

    class SceneInfo
    {
    public:
        SceneInfo();
        ~SceneInfo();

        bool Load(const std::string path);

        std::vector<MeshInfo> Meshs;
        std::vector<BoxInfo> Boxs;
        std::vector<CapsuleInfo> Capsules;
        std::vector<TerrainInfo> Terrains;
        std::vector<SphereInfo> Spheres;

    private:
        void parseMesh1(char* &pcontent);
        void parseBox(char* &pcontent);
        void parseCapsule(char* &pcontent);
        void parseMesh2(char* &pcontent);
        void parseTerrain(char* &pcontent);
        void parseObjBaseInfo(char* &pcontent, ObjInfoBase *infobase);
        void parseSphere(char* &pcontent);

        std::string mPath;
    };

    class SceneInfoMgr
    {
    public:
        std::shared_ptr<SceneInfo> Get(const std::string &path);
        void Set(const std::string &path, const std::shared_ptr<SceneInfo> &scene);
        unsigned GetStaticObjCount(const std::string &path);

    private:
        std::unordered_map<std::string, std::shared_ptr<SceneInfo>> mScenes;
    };

    extern SceneInfoMgr* gSceneInfoMgr;

};

#endif
