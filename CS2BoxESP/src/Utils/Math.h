#pragma once
#include <stdint.h>
#include "Types.h"
#include "imgui/imgui.h"

bool WorldToScreen(const Vector3& pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight);
uintptr_t GetEntityByIndex(uintptr_t entityList, int index);
bool IsValidBonePos(const Vector3& v);
Vector3 GetBonePosition(uintptr_t pawn, int boneIndex);

Vector3 CalculateAngle(const Vector3& from, const Vector3& to);
float CalculateFov(const Vector3& viewAngles, const Vector3& targetAngle);
void NormalizeAngles(Vector3& angles);

const char* GetWeaponName(uint16_t id);
struct WeaponIconInfo { const char* code; ImU32 color; };
WeaponIconInfo GetWeaponIcon(uint16_t id);

void itoa_simple(int val, char* buf, int bufSize);
void format_time(int totalSec, char* buf, int bufSize);
