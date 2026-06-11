#pragma once
#include "imgui/imgui.h"

namespace Overlay {
    void RenderESP(uintptr_t clientBase, int screenWidth, int screenHeight);
    void RenderParticles(int screenWidth, int screenHeight);
}
