#pragma once

#include "imgui/imgui.h"
#include "config.h"
#include "Utils/Types.h"

void RenderBox(ImDrawList* draw, const Vector2& head, const Vector2& feet, ImU32 color, int boxType);
