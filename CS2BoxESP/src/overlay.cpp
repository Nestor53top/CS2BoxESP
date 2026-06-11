#include "overlay.h"
#include "config.h"
#include "esp.h"
#include "Utils/Offsets.h"
#include "Utils/Types.h"
#include "Utils/Math.h"
#include <cmath>
#include <cstdlib>
#include <vector>

struct Particle {
    float x, y;
    float vx, vy;
    float life, maxLife;
    float size;
    ImU32 color;
};

static std::vector<Particle> rainDrops;
static std::vector<Particle> snowFlakes;
static std::vector<Particle> sandParticles;
static std::vector<Particle> sparkParticles;
static std::vector<Particle> ashParticles;
static bool particlesInited = false;

static float randf() { return (float)rand() / RAND_MAX; }

static void InitParticles(int w, int h) {
    rainDrops.clear();
    snowFlakes.clear();
    sandParticles.clear();
    sparkParticles.clear();
    ashParticles.clear();

    for (int i = 0; i < 500; i++) {
        Particle p;
        p.x = randf() * w;
        p.y = randf() * h;
        p.vx = -1.5f - randf() * 1.0f;
        p.vy = 12.0f + randf() * 10.0f;
        p.life = randf() * 2.0f;
        p.maxLife = 2.0f;
        p.size = 1.0f;
        p.color = IM_COL32(150, 180, 255, 120);
        rainDrops.push_back(p);
    }

    for (int i = 0; i < 400; i++) {
        Particle p;
        p.x = randf() * w;
        p.y = randf() * h;
        p.vx = -0.3f + randf() * 0.6f;
        p.vy = 1.0f + randf() * 2.0f;
        p.life = randf() * 4.0f;
        p.maxLife = 4.0f;
        p.size = 1.5f + randf() * 2.0f;
        p.color = IM_COL32(220, 220, 255, 180);
        snowFlakes.push_back(p);
    }

    for (int i = 0; i < 300; i++) {
        Particle p;
        p.x = randf() * w;
        p.y = randf() * h;
        p.vx = 3.0f + randf() * 4.0f;
        p.vy = -0.5f + randf() * 1.0f;
        p.life = randf() * 3.0f;
        p.maxLife = 3.0f;
        p.size = 1.0f + randf() * 1.5f;
        p.color = IM_COL32(200, 170, 100, 100);
        sandParticles.push_back(p);
    }

    for (int i = 0; i < 200; i++) {
        Particle p;
        p.x = randf() * w;
        p.y = randf() * h;
        p.vx = -2.0f + randf() * 4.0f;
        p.vy = -3.0f - randf() * 5.0f;
        p.life = randf() * 0.8f;
        p.maxLife = 0.8f;
        p.size = 0.5f + randf() * 1.5f;
        p.color = IM_COL32(255, 200, 50, 255);
        sparkParticles.push_back(p);
    }

    for (int i = 0; i < 350; i++) {
        Particle p;
        p.x = randf() * w;
        p.y = randf() * h;
        p.vx = -0.8f + randf() * 0.4f;
        p.vy = 0.5f + randf() * 1.5f;
        p.life = randf() * 5.0f;
        p.maxLife = 5.0f;
        p.size = 0.8f + randf() * 1.2f;
        p.color = IM_COL32(120, 120, 120, 60);
        ashParticles.push_back(p);
    }

    particlesInited = true;
}

static void UpdateAndDrawRain(ImDrawList* draw, int w, int h, int count) {
    for (int i = 0; i < count && i < (int)rainDrops.size(); i++) {
        Particle& p = rainDrops[i];
        p.x += p.vx;
        p.y += p.vy;
        p.life -= 0.016f;
        if (p.y > h || p.life <= 0) {
            p.x = randf() * w;
            p.y = -5.0f;
            p.life = p.maxLife;
        }
        float alpha = (p.life / p.maxLife);
        ImU32 col = IM_COL32(150, 180, 255, (int)(alpha * 120));
        draw->AddLine(ImVec2(p.x, p.y), ImVec2(p.x + p.vx * 2, p.y + p.vy * 2), col, 1.0f);
    }
}

static void UpdateAndDrawSnow(ImDrawList* draw, int w, int h, int count) {
    for (int i = 0; i < count && i < (int)snowFlakes.size(); i++) {
        Particle& p = snowFlakes[i];
        p.x += p.vx + sinf(p.y * 0.02f) * 0.5f;
        p.y += p.vy;
        p.life -= 0.016f;
        if (p.y > h || p.life <= 0) {
            p.x = randf() * w;
            p.y = -5.0f;
            p.life = p.maxLife;
        }
        float alpha = (p.life / p.maxLife);
        ImU32 col = IM_COL32(220, 220, 255, (int)(alpha * 200));
        draw->AddCircleFilled(ImVec2(p.x, p.y), p.size, col, 6);
    }
}

static void UpdateAndDrawFog(ImDrawList* draw, int w, int h, int alpha) {
    ImU32 col = IM_COL32(180, 190, 200, alpha);
    draw->AddRectFilled(ImVec2(0, 0), ImVec2((float)w, (float)h), col);
    ImU32 col2 = IM_COL32(200, 210, 220, alpha / 2);
    for (int i = 0; i < 3; i++) {
        float y = h * 0.3f + i * h * 0.2f;
        draw->AddRectFilled(ImVec2(0, y), ImVec2((float)w, y + 80), col2);
    }
}

static void UpdateAndDrawSand(ImDrawList* draw, int w, int h, int count) {
    for (int i = 0; i < count && i < (int)sandParticles.size(); i++) {
        Particle& p = sandParticles[i];
        p.x += p.vx;
        p.y += p.vy + sinf(p.x * 0.01f) * 0.3f;
        p.life -= 0.016f;
        if (p.x > w || p.life <= 0) {
            p.x = -5.0f;
            p.y = randf() * h;
            p.life = p.maxLife;
        }
        float alpha = (p.life / p.maxLife);
        ImU32 col = IM_COL32(200, 170, 100, (int)(alpha * 120));
        draw->AddCircleFilled(ImVec2(p.x, p.y), p.size, col, 4);
    }
}

static void UpdateAndDrawSparks(ImDrawList* draw, int w, int h, int count) {
    for (int i = 0; i < count && i < (int)sparkParticles.size(); i++) {
        Particle& p = sparkParticles[i];
        p.x += p.vx;
        p.y += p.vy;
        p.vy += 0.15f;
        p.life -= 0.032f;
        if (p.life <= 0) {
            p.x = w * 0.3f + randf() * w * 0.4f;
            p.y = h * 0.6f + randf() * h * 0.3f;
            p.vx = -2.0f + randf() * 4.0f;
            p.vy = -3.0f - randf() * 5.0f;
            p.life = p.maxLife;
        }
        float alpha = (p.life / p.maxLife);
        ImU32 col = IM_COL32(255, 200 + (int)(55 * alpha), 50, (int)(alpha * 255));
        draw->AddCircleFilled(ImVec2(p.x, p.y), p.size * alpha, col, 4);
        if (randf() > 0.7f) {
            ImU32 trail = IM_COL32(255, 180, 30, (int)(alpha * 100));
            draw->AddLine(ImVec2(p.x, p.y), ImVec2(p.x - p.vx, p.y - p.vy), trail, 0.5f);
        }
    }
}

static void UpdateAndDrawAsh(ImDrawList* draw, int w, int h, int count) {
    for (int i = 0; i < count && i < (int)ashParticles.size(); i++) {
        Particle& p = ashParticles[i];
        p.x += p.vx + sinf(p.y * 0.008f + p.x * 0.005f) * 0.8f;
        p.y += p.vy;
        p.life -= 0.016f;
        if (p.y > h || p.life <= 0) {
            p.x = randf() * w;
            p.y = -5.0f;
            p.life = p.maxLife;
        }
        float alpha = (p.life / p.maxLife);
        ImU32 col = IM_COL32(120, 120, 120, (int)(alpha * 80));
        draw->AddCircleFilled(ImVec2(p.x, p.y), p.size, col, 4);
    }
}

void Overlay::RenderParticles(int screenWidth, int screenHeight) {
    if (!particlesInited)
        InitParticles(screenWidth, screenHeight);

    ImDrawList* draw = ImGui::GetBackgroundDrawList();

    if (Config::bFog)
        UpdateAndDrawFog(draw, screenWidth, screenHeight, Config::iFogAlpha);

    if (Config::bRain)
        UpdateAndDrawRain(draw, screenWidth, screenHeight, Config::iRainDensity);

    if (Config::bSnow)
        UpdateAndDrawSnow(draw, screenWidth, screenHeight, Config::iSnowDensity);

    if (Config::bSand)
        UpdateAndDrawSand(draw, screenWidth, screenHeight, Config::iSandDensity);

    if (Config::bSparks)
        UpdateAndDrawSparks(draw, screenWidth, screenHeight, Config::iSparksDensity);

    if (Config::bAsh)
        UpdateAndDrawAsh(draw, screenWidth, screenHeight, Config::iAshDensity);
}

void Overlay::RenderESP(uintptr_t clientBase, int screenWidth, int screenHeight) {
    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    uintptr_t localPawn = *(uintptr_t*)(clientBase + offset::dwLocalPlayerPawn);
    if (!localPawn) return;
    int localTeam = *(int*)(localPawn + offset::m_iTeamNum);
    uintptr_t entityList = *(uintptr_t*)(clientBase + offset::dwEntityList);
    if (!entityList) return;
    float vm[16];
    memcpy(vm, (void*)(clientBase + offset::dwViewMatrix), sizeof(vm));
    for (int i = 1; i <= 64; i++) {
        uintptr_t entry = *(uintptr_t*)(entityList + 8 * (i >> 9) + 16);
        if (!entry) continue;
        uintptr_t ctrl = *(uintptr_t*)(entry + 112 * (i & 0x1FF));
        if (!ctrl) continue;
        uint32_t ph = *(uint32_t*)(ctrl + offset::m_hPlayerPawn);
        if (!ph || ph == 0xFFFFFFFF) continue;
        uintptr_t pawn = GetEntityByIndex(entityList, ph & 0x7FFF);
        if (!pawn || pawn == localPawn) continue;
        if (*(int*)(pawn + offset::m_iHealth) <= 0) continue;
        if (*(int*)(pawn + offset::m_iTeamNum) == localTeam) continue;
        Vector3 head = GetBonePosition(pawn, 6);
        if (head.x == 0 && head.y == 0 && head.z == 0) continue;
        Vector3 feet = *(Vector3*)(pawn + offset::m_vOldOrigin);
        Vector2 headS, feetS;
        if (!WorldToScreen(head, headS, vm, screenWidth, screenHeight)) continue;
        if (!WorldToScreen(feet, feetS, vm, screenWidth, screenHeight)) continue;
        RenderBox(draw, headS, feetS, Config::boxColor, Config::boxType);
    }
}
