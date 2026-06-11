#include "Math.h"
#include "Offsets.h"
#include <math.h>

bool WorldToScreen(const Vector3& pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight) {
    float _x = matrix[0] * pos.x + matrix[1] * pos.y + matrix[2] * pos.z + matrix[3];
    float _y = matrix[4] * pos.x + matrix[5] * pos.y + matrix[6] * pos.z + matrix[7];
    float w  = matrix[12] * pos.x + matrix[13] * pos.y + matrix[14] * pos.z + matrix[15];
    if (w < 0.01f) return false;
    float inv_w = 1.0f / w;
    _x *= inv_w;
    _y *= inv_w;
    float x = windowWidth / 2.0f;
    float y = windowHeight / 2.0f;
    x += 0.5f * _x * windowWidth + 0.5f;
    y -= 0.5f * _y * windowHeight + 0.5f;
    screen.x = x;
    screen.y = y;
    return true;
}

uintptr_t GetEntityByIndex(uintptr_t entityList, int index) {
    uintptr_t chunk = *(uintptr_t*)(entityList + 8 * (index >> 9) + 16);
    if (!chunk) return 0;
    return *(uintptr_t*)(chunk + 112 * (index & 0x1FF));
}

bool IsValidBonePos(const Vector3& v) {
    if (v.x == 0.0f && v.y == 0.0f && v.z == 0.0f) return false;
    const float lim = 1e6f;
    if (v.x < -lim || v.x > lim || v.y < -lim || v.y > lim || v.z < -lim || v.z > lim)
        return false;
    return true;
}

static uintptr_t ResolveBoneArray(uintptr_t sceneNode) {
    uintptr_t boneArray = *(uintptr_t*)(sceneNode + offset::m_modelState + offset::m_boneArray);
    if (boneArray) return boneArray;
    boneArray = *(uintptr_t*)(sceneNode + 0x1D0);
    if (boneArray) return boneArray;
    boneArray = *(uintptr_t*)(sceneNode + 0x1E0);
    if (boneArray) return boneArray;
    return *(uintptr_t*)(sceneNode + 0x1F0);
}

Vector3 GetBonePosition(uintptr_t pawn, int boneIndex) {
    Vector3 result = {};
    __try {
        uintptr_t sceneNode = *(uintptr_t*)(pawn + offset::m_pGameSceneNode);
        if (!sceneNode) return result;
        uintptr_t boneArray = ResolveBoneArray(sceneNode);
        if (!boneArray) return result;
        result.x = *(float*)(boneArray + boneIndex * 32 + 0);
        result.y = *(float*)(boneArray + boneIndex * 32 + 4);
        result.z = *(float*)(boneArray + boneIndex * 32 + 8);
        if (!IsValidBonePos(result)) return Vector3{};
    } __except(1) {}
    return result;
}

Vector3 CalculateAngle(const Vector3& from, const Vector3& to) {
    Vector3 delta = { to.x - from.x, to.y - from.y, to.z - from.z };
    float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
    Vector3 angles;
    angles.x = atan2f(-delta.z, hyp) * (180.0f / 3.14159265f);
    angles.y = atan2f(delta.y, delta.x) * (180.0f / 3.14159265f);
    angles.z = 0.0f;
    return angles;
}

float CalculateFov(const Vector3& viewAngles, const Vector3& targetAngle) {
    Vector3 delta = { targetAngle.x - viewAngles.x, targetAngle.y - viewAngles.y, 0.0f };
    NormalizeAngles(delta);
    return sqrtf(delta.x * delta.x + delta.y * delta.y);
}

void NormalizeAngles(Vector3& angles) {
    if (angles.x > 89.0f) angles.x = 89.0f;
    if (angles.x < -89.0f) angles.x = -89.0f;
    while (angles.y > 180.0f) angles.y -= 360.0f;
    while (angles.y < -180.0f) angles.y += 360.0f;
    angles.z = 0.0f;
}

const char* GetWeaponName(uint16_t id) {
    switch (id) {
        case 1: return "Deagle";
        case 2: return "Dual Elites";
        case 3: return "Five-SeveN";
        case 4: return "Glock-18";
        case 7: return "AK-47";
        case 8: return "AUG";
        case 9: return "AWP";
        case 10: return "FAMAS";
        case 11: return "G3SG1";
        case 13: return "Galil AR";
        case 14: return "M249";
        case 16: return "M4A4";
        case 17: return "MAC-10";
        case 19: return "P90";
        case 23: return "MP5-SD";
        case 24: return "UMP-45";
        case 25: return "XM1014";
        case 26: return "PP-Bizon";
        case 27: return "MAG-7";
        case 28: return "Negev";
        case 29: return "Sawed-Off";
        case 30: return "Tec-9";
        case 31: return "Zeus";
        case 32: return "P2000";
        case 33: return "MP7";
        case 34: return "MP9";
        case 35: return "Nova";
        case 36: return "P250";
        case 38: return "SCAR-20";
        case 39: return "SG 553";
        case 40: return "SSG 08";
        case 42: return "Knife";
        case 43: return "Flash";
        case 44: return "HE";
        case 45: return "Smoke";
        case 46: return "Molotov";
        case 47: return "Decoy";
        case 48: return "Inc.";
        case 49: return "C4";
        case 60: return "M4A1-S";
        case 61: return "USP-S";
        case 63: return "CZ75";
        case 64: return "R8 Revolver";
        default: return "?";
    }
}

WeaponIconInfo GetWeaponIcon(uint16_t id) {
    switch (id) {
        case 1: return {"DE", IM_COL32(255,200,50,255)};
        case 2: return {"DE", IM_COL32(255,200,50,255)};
        case 3: return {"57", IM_COL32(255,200,50,255)};
        case 4: return {"GL", IM_COL32(255,200,50,255)};
        case 7: return {"AK", IM_COL32(220,60,60,255)};
        case 8: return {"AU", IM_COL32(60,120,255,255)};
        case 9: return {"AW", IM_COL32(180,60,200,255)};
        case 10: return {"FM", IM_COL32(60,120,255,255)};
        case 11: return {"G3", IM_COL32(180,60,200,255)};
        case 13: return {"GA", IM_COL32(60,120,255,255)};
        case 14: return {"MG", IM_COL32(200,150,50,255)};
        case 16: return {"M4", IM_COL32(60,120,255,255)};
        case 17: return {"M1", IM_COL32(60,200,200,255)};
        case 19: return {"90", IM_COL32(60,200,200,255)};
        case 23: return {"M5", IM_COL32(60,200,200,255)};
        case 24: return {"UM", IM_COL32(60,200,200,255)};
        case 25: return {"XM", IM_COL32(200,100,50,255)};
        case 26: return {"BZ", IM_COL32(60,200,200,255)};
        case 27: return {"M7", IM_COL32(200,100,50,255)};
        case 28: return {"NG", IM_COL32(200,150,50,255)};
        case 29: return {"SO", IM_COL32(200,100,50,255)};
        case 30: return {"T9", IM_COL32(255,200,50,255)};
        case 31: return {"ZE", IM_COL32(255,255,100,255)};
        case 32: return {"P2", IM_COL32(255,200,50,255)};
        case 33: return {"M7", IM_COL32(60,200,200,255)};
        case 34: return {"M9", IM_COL32(60,200,200,255)};
        case 35: return {"NV", IM_COL32(200,100,50,255)};
        case 36: return {"P2", IM_COL32(255,200,50,255)};
        case 38: return {"S2", IM_COL32(180,60,200,255)};
        case 39: return {"SG", IM_COL32(60,120,255,255)};
        case 40: return {"S8", IM_COL32(180,60,200,255)};
        case 42: return {"KN", IM_COL32(200,200,200,255)};
        case 43: return {"FL", IM_COL32(100,255,100,255)};
        case 44: return {"HE", IM_COL32(255,100,100,255)};
        case 45: return {"SM", IM_COL32(180,180,180,255)};
        case 46: return {"MO", IM_COL32(255,150,50,255)};
        case 47: return {"DE", IM_COL32(180,180,180,255)};
        case 48: return {"IN", IM_COL32(255,100,50,255)};
        case 49: return {"C4", IM_COL32(255,50,50,255)};
        case 60: return {"M4", IM_COL32(60,120,255,255)};
        case 61: return {"US", IM_COL32(255,200,50,255)};
        case 63: return {"CZ", IM_COL32(255,200,50,255)};
        case 64: return {"R8", IM_COL32(255,200,50,255)};
        default: return {"?", IM_COL32(200,200,200,255)};
    }
}

void itoa_simple(int val, char* buf, int bufSize) {
    int pos = 0;
    if (val == 0) { buf[0] = '0'; buf[1] = 0; return; }
    if (val < 0) { buf[pos++] = '-'; val = -val; }
    int digits[16], dpos = 0;
    while (val > 0 && dpos < 16) { digits[dpos++] = val % 10; val /= 10; }
    for (int i = dpos - 1; i >= 0; i--) { if (pos < bufSize - 1) buf[pos++] = (char)('0' + digits[i]); }
    buf[pos] = 0;
}

void format_time(int totalSec, char* buf, int bufSize) {
    int min = totalSec / 60;
    int sec = totalSec % 60;
    if (min >= 100) min = 99;
    int pos = 0;
    buf[pos++] = (char)('0' + min / 10);
    buf[pos++] = (char)('0' + min % 10);
    buf[pos++] = ':';
    buf[pos++] = (char)('0' + sec / 10);
    buf[pos++] = (char)('0' + sec % 10);
    buf[pos] = 0;
}
