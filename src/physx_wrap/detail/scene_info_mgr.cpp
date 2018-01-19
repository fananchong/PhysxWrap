#include "scene_info_mgr.h"
#include "physx_sdk.h"
#include "util.h"
#include "log.h"
#include <cassert>
#include <vector>

namespace PhysxWrap {

    SceneInfo::SceneInfo() {

    }

    SceneInfo::~SceneInfo() {

    }

    bool SceneInfo::Load(const std::string path) {
        INFO("load scene ... , path = %s", path.c_str());
        auto t1 = GetTimeStamp();
        std::string content = GetFileContent(path);
        if (content == "") {
            ERROR("load scene fail #1. path = %s", path.c_str());
            return false;
        }
        char* pcontent = (char*)content.c_str();
        assert(content[0] == 'P');
        assert(content[1] == 'X');
        assert(content[2] == 'S');
        assert(content[3] == '\0');
        pcontent += 4;
        uint32_t meshLen = *(uint32_t*)pcontent;
        pcontent += sizeof(uint32_t);

        for (size_t i = 0; i < meshLen; i++)
        {
            parseMesh1(pcontent);
        }
        uint32_t objLen = *(uint32_t*)pcontent;
        pcontent += sizeof(uint32_t);
        for (size_t i = 0; i < objLen; i++)
        {
            uint16_t type = *(uint16_t*)pcontent;
            pcontent += sizeof(uint16_t);
            switch (type)
            {
            case eBoxObj:
            {
                parseBox(pcontent);
            }
            break;
            case eCapsuleObj:
            {
                parseCapsule(pcontent);
            }
            break;
            case eMeshObj:
            {
                parseMesh2(pcontent);
            }
            break;
            case eTerrainObj:
            {
                parseTerrain(pcontent);
            }
            break;
            case eSphereObj:
            {
                parseSphere(pcontent);
            }
            break;
            default:
                assert(false);
                ERROR("load scene fail #2. path = %s", path.c_str());
                return false;
            }
        }
        auto t2 = GetTimeStamp();
        INFO("load scene done. cost time = %u ms", unsigned(t2 - t1));
        return true;
    }

    void SceneInfo::parseMesh1(char* &pcontent) {
        MeshInfo info;
        uint16_t type = *(uint16_t*)pcontent;
        pcontent += sizeof(uint16_t);
        assert(type == eMeshData);
        uint32_t vlen = *(uint32_t*)pcontent;
        pcontent += sizeof(uint32_t);
        for (size_t i = 0; i < vlen; i++)
        {
            float x = *(float*)pcontent;
            pcontent += sizeof(float);
            float y = *(float*)pcontent;
            pcontent += sizeof(float);
            float z = *(float*)pcontent;
            pcontent += sizeof(float);
            info.vb.push_back(x);
            info.vb.push_back(y);
            info.vb.push_back(z);
        }
        uint32_t ilen = *(uint32_t*)pcontent;
        pcontent += sizeof(uint32_t);
        std::vector<uint16_t> ib;
        for (size_t i = 0; i < ilen; i++)
        {
            uint16_t v = *(uint16_t*)pcontent;
            pcontent += sizeof(uint16_t);
            info.ib.push_back(v);
        }
        Meshs.emplace_back(info);
    }

    void SceneInfo::parseBox(char* &pcontent) {
        BoxInfo info;
        parseObjBaseInfo(pcontent, &info);
        info.Half.X = *(float*)pcontent;
        pcontent += sizeof(float);
        info.Half.Y = *(float*)pcontent;
        pcontent += sizeof(float);
        info.Half.Z = *(float*)pcontent;
        pcontent += sizeof(float);
        Boxs.emplace_back(info);
    }

    void SceneInfo::parseCapsule(char* &pcontent) {
        CapsuleInfo info;
        parseObjBaseInfo(pcontent, &info);
        info.Radius = *(float*)pcontent;
        pcontent += sizeof(float);
        info.HalfHeight = *(float*)pcontent;
        pcontent += sizeof(float);
        Capsules.emplace_back(info);
    }

    void SceneInfo::parseMesh2(char* &pcontent) {
        ObjInfoBase baseInfo;
        parseObjBaseInfo(pcontent, &baseInfo);
        float xScale = *(float*)pcontent;
        pcontent += sizeof(float);
        float yScale = *(float*)pcontent;
        pcontent += sizeof(float);
        float zScale = *(float*)pcontent;
        pcontent += sizeof(float);
        uint32_t meshIndex = *(uint32_t*)pcontent;
        pcontent += sizeof(uint32_t);
        assert(meshIndex < Meshs.size());
        if (meshIndex < Meshs.size())
        {
            auto &info = Meshs[meshIndex];
            info.Postion = baseInfo.Postion;
            info.Rotate = baseInfo.Rotate;
            info.Layer = baseInfo.Layer;
            info.scale = Vector3{ xScale,yScale,zScale };
            if (GetMeshGeometry(info.Geom, info.Postion, info.scale, info.vb, info.ib) == false) {
                assert(false);
            }
        }
    }

    void SceneInfo::parseTerrain(char* &pcontent) {
        TerrainInfo info;
        parseObjBaseInfo(pcontent, &info);
        Vector3 size;
        uint32_t d;
        std::vector<int16_t> data;
        size.X = *(float*)pcontent;
        pcontent += sizeof(float);
        size.Y = *(float*)pcontent;
        pcontent += sizeof(float);
        size.Z = *(float*)pcontent;
        pcontent += sizeof(float);
        d = *(uint32_t*)pcontent;
        pcontent += sizeof(uint32_t);
        data.resize(d*d);
        for (size_t i = 0; i < d; i++)
            for (size_t j = 0; j < d; j++)
            {
                float v = *(float*)pcontent;
                pcontent += sizeof(float);
                data[j*d + i] = int16_t(v*size.Y);
            }

        if (GetHeightFieldGeometry(info.Geom, data, d, d, Vector3{ size.X / (d - 1), 1, size.Z / (d - 1) })) {
            Terrains.emplace_back(info);
        }
        else
        {
            assert(false);
        }
    }

    void SceneInfo::parseSphere(char* &pcontent) {
        SphereInfo info;
        parseObjBaseInfo(pcontent, &info);
        info.Radius = *(float*)pcontent;
        pcontent += sizeof(float);
        Spheres.emplace_back(info);
    }

    void SceneInfo::parseObjBaseInfo(char* &pcontent, ObjInfoBase *infobase) {
        infobase->Postion.X = *(float*)pcontent;
        pcontent += sizeof(float);
        infobase->Postion.Y = *(float*)pcontent;
        pcontent += sizeof(float);
        infobase->Postion.Z = *(float*)pcontent;
        pcontent += sizeof(float);
        infobase->Rotate.X = *(float*)pcontent;
        pcontent += sizeof(float);
        infobase->Rotate.Y = *(float*)pcontent;
        pcontent += sizeof(float);
        infobase->Rotate.Z = *(float*)pcontent;
        pcontent += sizeof(float);
        infobase->Rotate.W = *(float*)pcontent;
        pcontent += sizeof(float);
        infobase->Layer = *(unsigned char*)pcontent;
        pcontent += sizeof(unsigned char);
    }

    SceneInfoMgr __gSceneInfoMgr;
    SceneInfoMgr* gSceneInfoMgr = &__gSceneInfoMgr;

    std::shared_ptr<SceneInfo> SceneInfoMgr::Get(const std::string &path) {
        auto it = mScenes.find(path);
        if (it != mScenes.end())
        {
            return it->second;
        }
        return nullptr;
    }

    void SceneInfoMgr::Set(const std::string &path, const std::shared_ptr<SceneInfo> &scene) {
        if (mScenes.find(path) != mScenes.end())
        {
            assert(false);
        }
        mScenes[path] = scene;
    }

    unsigned SceneInfoMgr::GetStaticObjCount(const std::string &path) {
        auto sceneInfo = Get(path);
        if (sceneInfo)
        {
            auto count = sceneInfo->Boxs.size()     \
                + sceneInfo->Capsules.size()        \
                + sceneInfo->Meshs.size()           \
                + sceneInfo->Terrains.size()        \
                + sceneInfo->Spheres.size()         \
                ;
            return unsigned(count);
        }
        return 0;
    }

}
