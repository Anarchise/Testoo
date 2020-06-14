#pragma once

#include "stdafx.h"

class RefdefView
{
public:
    Vector2 tanHalfFov;         // 0x00
    unsigned __int8 unk1[0xC];  // 0x08
    Vector3 axis[3];            // 0x14
};
