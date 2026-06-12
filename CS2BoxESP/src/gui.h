#pragma once
#include "config.h"
#include "imgui/imgui.h"

struct MenuParticle {
    ImVec2 pos;
    ImVec2 vel;
};

namespace MenuSystem {
    void Setup();
    void Shutdown();
    void Render();
    void OnNewFrame();
    void UpdateColors();

    inline bool bMenuOpened = false;
    inline float flAnimAlpha = 0.0f;
    inline bool bAnimSwitch = false;
}

namespace MenuParticles {
    void Init(int count);
    void Update(float dt);
    void Render(ImDrawList* draw, float alpha);
}

void GUI_LoadFont();
void GUI_Init();
void GUI_Render();
