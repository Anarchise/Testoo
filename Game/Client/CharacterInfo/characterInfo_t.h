#pragma once

#include "stdafx.h"
#include "Game\LerpFrame\lerpFrame_t.h"
#include "Game\Gfx\GfxViewport\GfxViewport.h"
#include "Game\Refdef\RefdefView\RefdefView.h"
#include "Game\XAnim\XAnimTree\XAnimTree.h"
#include "Game\CEntity\centity_s.h"

class characterInfo_tt
{
public:
    bool isValid();
public:
    int entityNum;                // 0x00
    int infoValid;                // 0x04
    int nextValid;                // 0x08
    team_t team;                  // 0x0C
    unsigned __int8 unk1[0x7E0];  // 0x10
    XAnimTree* pXAnimTree;        // 0x7F0
    lerpFrame_t legs;             // 0x7F8
    lerpFrame_t torso;            // 0x850
    float legsYawTurnTimer;       // 0x8A8
    int legsYawTurnDirection;     // 0x8AC
    unsigned __int8 unk2[0x108];  // 0x8B0
    Vector2 angles;               // 0x9B8
    unsigned __int8 unk3[0x3C];   // 0x9C4
    int usingAnimState;           // 0xA00
    int animTreeDirty;            // 0xA04
    unsigned __int8 unk4[0x1B4];  // 0xA08
    CharacterStance stance;       // 0xBBC
    unsigned __int8 unk5[0x3E3];  // 0xBBD
    int stanceTransitionTime;     // 0xFA0
    unsigned __int8 unk6[0x25E4]; // 0xFA4
}; // 0x3588
