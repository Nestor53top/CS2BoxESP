#pragma once
#include <windows.h>
#include "imgui/imgui.h"

namespace Config {
    inline bool bMenuOpen = true;
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
}
