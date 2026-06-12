#include "aimbot.h"
#include "../Utils/Offsets.h"
#include "../Utils/Math.h"
#include <math.h>

static float NormalizeYaw(float yaw) {
    while (yaw > 180.0f) yaw -= 360.0f;
    while (yaw < -180.0f) yaw += 360.0f;
    return yaw;
}

static float NormalizePitch(float pitch) {
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    return pitch;
}

void AimbotRun(uintptr_t client) {
    if (!Config::bAimbot) return;

    uintptr_t localPawn = *(uintptr_t*)(client + offset::dwLocalPlayerPawn);
    if (!localPawn) return;

    int localHealth = *(int*)(localPawn + offset::m_iHealth);
    if (localHealth <= 0) return;

    int localTeam = *(int*)(localPawn + offset::m_iTeamNum);

    float viewAngles[3];
    viewAngles[0] = *(float*)(client + offset::dwViewAngles);
    viewAngles[1] = *(float*)(client + offset::dwViewAngles + 4);

    float bestDist = Config::flAimbotFOV;
    int bestBone = -1;
    float bestAngle[2] = {0, 0};

    uintptr_t entityList = *(uintptr_t*)(client + offset::dwEntityList);
    if (!entityList) return;

    uintptr_t localSceneNode = *(uintptr_t*)(localPawn + offset::m_pGameSceneNode);
    if (!localSceneNode) return;
    uintptr_t localBoneArray = *(uintptr_t*)(*(uintptr_t*)(localSceneNode + offset::m_modelState) + offset::m_boneArray);
    if (!localBoneArray) return;

    Vector3 localEye = {};
    localEye.x = *(float*)(localPawn + offset::m_vOldOrigin);
    localEye.y = *(float*)(localPawn + offset::m_vOldOrigin + 4);
    localEye.z = *(float*)(localPawn + offset::m_vOldOrigin + 8);
    localEye.z += *(float*)(localPawn + offset::m_vecViewOffset + 8);

    for (int i = 1; i <= 64; i++) {
        uintptr_t entry = *(uintptr_t*)(entityList + 8 * (i >> 9) + 16);
        if (!entry) continue;
        uintptr_t ctrl = *(uintptr_t*)(entry + 112 * (i & 0x1FF));
        if (!ctrl) continue;

        uint32_t ph = *(uint32_t*)(ctrl + offset::m_hPlayerPawn);
        if (!ph || ph == 0xFFFFFFFF) continue;

        uintptr_t pawn = GetEntityByIndex(entityList, ph & 0x7FFF);
        if (!pawn || pawn == localPawn) continue;

        int health = *(int*)(pawn + offset::m_iHealth);
        if (health <= 0) continue;

        int team = *(int*)(pawn + offset::m_iTeamNum);
        if (team == localTeam) continue;

        bool dormant = *(bool*)(*(uintptr_t*)(pawn + offset::m_pGameSceneNode) + 0x100);
        if (dormant) continue;

        uintptr_t sceneNode = *(uintptr_t*)(pawn + offset::m_pGameSceneNode);
        if (!sceneNode) continue;
        uintptr_t boneArray = *(uintptr_t*)(*(uintptr_t*)(sceneNode + offset::m_modelState) + offset::m_boneArray);
        if (!boneArray) continue;

        int boneToAim = 6;
        Vector3 target = {};
        target.x = *(float*)(boneArray + boneToAim * 32 + 0);
        target.y = *(float*)(boneArray + boneToAim * 32 + 4);
        target.z = *(float*)(boneArray + boneToAim * 32 + 8);

        if (target.x == 0 && target.y == 0 && target.z == 0) continue;

        Vector3 delta = {};
        delta.x = target.x - localEye.x;
        delta.y = target.y - localEye.y;
        delta.z = target.z - localEye.z;

        float hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);
        float pitch = atan2f(-delta.z, hyp) * (180.0f / 3.14159265f);
        float yaw = atan2f(delta.y, delta.x) * (180.0f / 3.14159265f);

        float deltaPitch = pitch - viewAngles[0];
        float deltaYaw = NormalizeYaw(yaw - viewAngles[1]);
        float dist = sqrtf(deltaPitch * deltaPitch + deltaYaw * deltaYaw);

        if (dist < bestDist) {
            bestDist = dist;
            bestAngle[0] = pitch;
            bestAngle[1] = yaw;
        }
    }

    if (bestDist >= Config::flAimbotFOV) return;

    float recoil[2];
    recoil[0] = *(float*)(localPawn + offset::m_aimPunchAngle);
    recoil[1] = *(float*)(localPawn + offset::m_aimPunchAngle + 4);

    float finalPitch = NormalizePitch(bestAngle[0] - recoil[0] * 2.0f);
    float finalYaw = NormalizeYaw(bestAngle[1] - recoil[1] * 2.0f);

    float smoothing = Config::flAimbotSmoothing;
    if (smoothing < 1.0f) smoothing = 1.0f;

    finalPitch = viewAngles[0] + (finalPitch - viewAngles[0]) / smoothing;
    finalYaw = viewAngles[1] + (finalYaw - viewAngles[1]) / smoothing;

    *(float*)(client + offset::dwViewAngles) = NormalizePitch(finalPitch);
    *(float*)(client + offset::dwViewAngles + 4) = NormalizeYaw(finalYaw);
}
