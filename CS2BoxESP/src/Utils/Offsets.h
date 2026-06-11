#pragma once
#include <stdint.h>

namespace offset {
    constexpr ptrdiff_t dwJumpButton = 0x2064FA0;
    constexpr ptrdiff_t dwCSGOInput = 0x2356240;
    constexpr ptrdiff_t dwGlowManager = 0x233DF50;
    constexpr ptrdiff_t dwEntityList = 0x24E76A0;
    constexpr ptrdiff_t dwViewMatrix = 0x2346B30;
    constexpr ptrdiff_t dwLocalPlayerPawn = 0x2341698;

    constexpr ptrdiff_t m_hPlayerPawn = 0x90C;
    constexpr ptrdiff_t m_iHealth = 0x34C;
    constexpr ptrdiff_t m_lifeState = 0x354;
    constexpr ptrdiff_t m_fFlags = 0x3F8;
    constexpr ptrdiff_t m_MoveType = 0x525;
    constexpr ptrdiff_t m_pMovementServices = 0x1220;
    constexpr ptrdiff_t m_iTeamNum = 0x3EB;
    constexpr ptrdiff_t m_vOldOrigin = 0x1390;
    constexpr ptrdiff_t m_pGameSceneNode = 0x330;
    constexpr ptrdiff_t m_modelState = 0x150;
    constexpr ptrdiff_t m_boneArray = 0x80;
    constexpr ptrdiff_t m_vecAbsOrigin = 0xC8;

    constexpr ptrdiff_t m_sSanitizedPlayerName = 0x860;
    constexpr ptrdiff_t m_ArmorValue = 0x1C7C;

    constexpr ptrdiff_t m_pWeaponServices = 0x11E0;
    constexpr ptrdiff_t m_hActiveWeapon = 0x60;
    constexpr ptrdiff_t m_AttributeManager = 0x1180;
    constexpr ptrdiff_t m_Item = 0x50;
    constexpr ptrdiff_t m_iItemDefinitionIndex = 0x1BA;

    constexpr ptrdiff_t m_entitySpottedState = 0x1C38;
    constexpr ptrdiff_t m_bSpotted = 0x8;

    constexpr ptrdiff_t m_Glow = 0xDD8;
    constexpr ptrdiff_t m_iGlowType = 0x30;
    constexpr ptrdiff_t m_nGlowRange = 0x38;
    constexpr ptrdiff_t m_nGlowRangeMin = 0x3C;
    constexpr ptrdiff_t m_glowColorOverride = 0x40;
    constexpr ptrdiff_t m_bFlashing = 0x44;
    constexpr ptrdiff_t m_bEligibleForScreenHighlight = 0x50;
    constexpr ptrdiff_t m_bGlowing = 0x51;

    constexpr ptrdiff_t dwGlobalVars = 0x20616D0;
    constexpr ptrdiff_t curtime = 0x18;

    constexpr ptrdiff_t dwPlantedC4 = 0x234FF98;
    constexpr ptrdiff_t dwWeaponC4 = 0x22BED20;
    constexpr ptrdiff_t m_bBombTicking = 0x1160;
    constexpr ptrdiff_t m_nBombSite = 0x1164;
    constexpr ptrdiff_t m_flC4Blow = 0x1190;
    constexpr ptrdiff_t m_flTimerLength = 0x1198;
    constexpr ptrdiff_t m_bBeingDefused = 0x119C;
    constexpr ptrdiff_t m_flDefuseLength = 0x11AC;
    constexpr ptrdiff_t m_flDefuseCountDown = 0x11B0;

    constexpr ptrdiff_t m_pInGameMoneyServices = 0x808;
    constexpr ptrdiff_t m_iAccount = 0x40;
    constexpr ptrdiff_t m_pActionTrackingServices = 0x818;
    constexpr ptrdiff_t m_iScore = 0x934;
    constexpr ptrdiff_t m_nKillCount = 0x941;

    constexpr ptrdiff_t dwGameRules = 0x2341158;
    constexpr ptrdiff_t m_fRoundStartTime = 0x70;

    constexpr ptrdiff_t dwViewAngles = 0x23568C8;

    constexpr ptrdiff_t m_iIDEntIndex = 0x33FC;
    constexpr ptrdiff_t m_aimPunchAngle = 0x301C;

    constexpr ptrdiff_t m_flFlashMaxAlpha = 0x13FC;
    constexpr ptrdiff_t m_flFlashDuration = 0x1400;

    constexpr ptrdiff_t m_iClip1 = 0x16D8;
    constexpr ptrdiff_t m_pReserveAmmo = 0x16E0;

    constexpr ptrdiff_t m_hOwnerEntity = 0x520;
    constexpr ptrdiff_t m_pEntity = 0x10;
    constexpr ptrdiff_t m_designerName = 0x20;

    constexpr ptrdiff_t dwGameEntitySystem = 0x24E76A0;
    constexpr ptrdiff_t dwGameEntitySystem_highestEntityIndex = 0x2090;

    constexpr ptrdiff_t m_bombsiteCenterA = 0x648;
    constexpr ptrdiff_t m_bombsiteCenterB = 0x654;

    constexpr ptrdiff_t m_pCameraServices = 0x1218;
    constexpr ptrdiff_t m_iFOV = 0x290;
    constexpr ptrdiff_t m_iFOVStart = 0x294;
    constexpr ptrdiff_t m_flFOVRate = 0x298;
    constexpr ptrdiff_t m_flViewmodelFOV = 0x1B7C;
    constexpr ptrdiff_t m_bIsScoped = 0x1C50;

    constexpr ptrdiff_t dwLocalPlayerController = 0x2320720;
    constexpr ptrdiff_t m_iDesiredFOV = 0x78C;
    constexpr ptrdiff_t m_flStamina = 0x69C;
}
