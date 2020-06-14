#pragma once

#include "stdafx.h"
#include "Game\Gfx\GfxViewport\GfxViewport.h"
#include "Game\Refdef\RefdefView\RefdefView.h"

class refdef_t
{
public:
    static refdef_t* get();
    static refdef_t* get2();

public:
    GfxViewport displayViewport; // 0x00
    RefdefView view;             // 0x10
    unsigned __int8 unk1[0x3C];  // 0x30
    Vector3 unk2;                // 0x6C
    unsigned __int8 unk3[0x4];   // 0x78
    Vector3 viewOffset;          // 0x7C
    Vector3 viewOffsetPrev;      // 0x88
};
