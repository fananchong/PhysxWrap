#ifndef __ACTOR_INFO_H__
#define __ACTOR_INFO_H__

#include <memory>
#include <vector>

enum EnumActorType
{
    eSPHERE = 0,
    ePLANE = 1,
    eCAPSULE = 2,
    eBOX = 3,
    eCONVEXMESH = 4,    // no support
    eTRIANGLEMESH = 5,
    eHEIGHTFIELD = 6,
};

class ActorInfo
{
public:
    inline uint64_t GetId() { return id; }
    inline EnumActorType GetType() { return type; }
    inline float GetPosX() { return posX; }
    inline float GetPosY() { return posY; }
    inline float GetPosZ() { return posZ; }
    inline float GetQuatX() { return quatX; }
    inline float GetQuatY() { return quatY; }
    inline float GetQuatZ() { return quatZ; }
    inline float GetQuatW() { return quatW; }
    inline float GetRadius() { return param1; }
    inline float GetHalfExtentsX() { return param1; }
    inline float GetHalfExtentsY() { return param2; }
    inline float GetHalfExtentsZ() { return param3; }
    inline float GetHalfHeight() { return param2; }
    inline std::vector<float> GetVB() { return vb; }
    inline std::vector<uint16_t> GetIB() { return ib; }
    inline uint32_t GetColumns() { return uint32_t(param1); }
    inline uint32_t GetRows() { return uint32_t(param2); }
    inline float GetColumnScale() { return param3; }
    inline float GetRowScale() { return param4; }
    inline float GetHeightScale() { return param5; }
    inline std::vector<int16_t> GetHeightFieldData() { return heightFieldData; }

public:
    inline void SetId(uint64_t v) { id = v; }
    inline void SetType(EnumActorType v) { type = v; }
    inline void SetPosX(float v) { posX = v; }
    inline void SetPosY(float v) { posY = v; }
    inline void SetPosZ(float v) { posZ = v; }
    inline void SetQuatX(float v) { quatX = v; }
    inline void SetQuatY(float v) { quatY = v; }
    inline void SetQuatZ(float v) { quatZ = v; }
    inline void SetQuatW(float v) { quatW = v; }
    inline void SetRadius(float v) { param1 = v; }
    inline void SetHalfExtentsX(float v) { param1 = v; }
    inline void SetHalfExtentsY(float v) { param2 = v; }
    inline void SetHalfExtentsZ(float v) { param3 = v; }
    inline void SetHalfHeight(float v) { param2 = v; }
    inline void SetVB(std::vector<float> &v) { vb.swap(v); }
    inline void SetIB(std::vector<uint16_t> &v) { ib.swap(v); }
    inline void SetColumns(uint32_t v) { param1 = float(v); }
    inline void SetRows(uint32_t v) { param2 = float(v); }
    inline void SetColumnScale(float v) { param3 = v; }
    inline void SetRowScale(float v) { param4 = v; }
    inline void SetHeightScale(float v) { param5 = v; }
    inline void SetHeightFieldData(std::vector<int16_t> &v) { heightFieldData.swap(v); }

private:
    uint64_t id;
    EnumActorType type;
    float posX;
    float posY;
    float posZ;
    float quatX;
    float quatY;
    float quatZ;
    float quatW;

    // sphere¡¢capsule¡¢box
    float param1;
    float param2;
    float param3;
    float param4;
    float param5;

    // triangle mesh
    std::vector<float> vb;
    std::vector<uint16_t> ib;

    // heightfield
    std::vector<int16_t> heightFieldData;
};

#endif
