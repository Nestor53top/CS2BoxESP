#include "bhop.h"
#include "../Utils/Offsets.h"

#define IN_JUMP (1 << 1)
#define IN_FORWARD (1 << 3)
#define IN_BACK (1 << 4)
#define IN_MOVELEFT (1 << 9)
#define IN_MOVERIGHT (1 << 10)

#define FL_ONGROUND (1 << 0)
#define MOVETYPE_NOCLIP 8
#define MOVETYPE_LADDER 9

void BhopRun(uintptr_t client) {
    if (!Config::bAutoBhop && !Config::bAutoStrafe) return;

    uintptr_t localPawn = *(uintptr_t*)(client + offset::dwLocalPlayerPawn);
    if (!localPawn) return;

    int health = *(int*)(localPawn + offset::m_iHealth);
    if (health <= 0) return;

    int flags = *(int*)(localPawn + offset::m_fFlags);
    int moveType = *(int*)(localPawn + offset::m_MoveType);
    if (moveType == MOVETYPE_NOCLIP || moveType == MOVETYPE_LADDER) return;

    if (Config::bAutoBhop) {
        if (flags & FL_ONGROUND) {
            uintptr_t cmd = *(uintptr_t*)(client + offset::dwCSGOInput);
            if (cmd) {
                uintptr_t inputPtr = *(uintptr_t*)(cmd + 0xF8);
                if (inputPtr) {
                    uintptr_t buttonsPtr = *(uintptr_t*)(inputPtr + offset::m_pInButtonState);
                    if (buttonsPtr) {
                        int buttons = *(int*)(buttonsPtr + offset::m_nButtons);
                        if (buttons & IN_JUMP) {
                            *(int*)(buttonsPtr + offset::m_nButtons) = buttons & ~IN_JUMP;
                        }
                    }
                }
            }
        }
    }

    if (Config::bAutoStrafe) {
        if (!(flags & FL_ONGROUND)) {
            uintptr_t cmd = *(uintptr_t*)(client + offset::dwCSGOInput);
            if (cmd) {
                uintptr_t inputPtr = *(uintptr_t*)(cmd + 0xF8);
                if (inputPtr) {
                    short mouseX = *(short*)(inputPtr + offset::m_nMouseDeltaX);
                    if (mouseX != 0) {
                        float sideMove = mouseX > 0 ? -1.0f : 1.0f;
                        *(float*)(inputPtr + offset::m_flSideMove) = sideMove;
                    }
                }
            }
        }
    }
}
