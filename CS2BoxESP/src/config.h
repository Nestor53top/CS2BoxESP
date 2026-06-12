#pragma once
#include <windows.h>
#include <math.h>
#include "imgui.h"

namespace Config {
    inline bool bMenuOpen = true;
    inline int nMenuKey = VK_INSERT;

    inline bool bBoxESP = true;
    inline int boxType = 1;
    inline ImU32 boxColor = IM_COL32(255, 80, 80, 255);

    inline bool bFOVChanger = false;
    inline int iFOV = 90;

    inline bool bRain = false;
    inline int iRainDensity = 300;
    inline bool bSnow = false;
    inline int iSnowDensity = 200;
    inline bool bFog = false;
    inline int iFogAlpha = 40;
    inline bool bSand = false;
    inline int iSandDensity = 150;
    inline bool bSparks = false;
    inline int iSparksDensity = 100;
    inline bool bAsh = false;
    inline int iAshDensity = 180;

    inline bool bAimbot = false;
    inline float flAimbotFOV = 30.0f;
    inline float flAimbotSmoothing = 5.0f;

    inline bool bAutoBhop = false;
    inline bool bAutoStrafe = false;

    namespace Menu {
        inline bool bDimBackground = true;
        inline bool bParticleBackground = true;
        inline bool bGlowEffect = true;
        inline float flAnimationSpeed = 1.0f;
        inline int nDpiScale = 0;

        inline float colAccent0[3] = { 0.33f, 0.35f, 0.63f };
        inline float colAccent1[3] = { 0.39f, 0.41f, 0.69f };
        inline float colAccent2[3] = { 0.45f, 0.47f, 0.75f };

        inline float colBackground[3] = { 0.08f, 0.08f, 0.12f };
        inline float colText[3] = { 1.0f, 1.0f, 1.0f };
        inline float colDisabled[3] = { 0.75f, 0.75f, 0.75f };
    }
}
