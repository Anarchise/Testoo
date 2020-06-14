#pragma once

#include "stdafx.h"
#include "Game\XAnim\XAnim_s\XAnim_s.h"

class __declspec(align(8)) XAnimTree
{
public:
    XAnim_s* anims;             // 0x00
    unsigned __int16 children;  // 0x08
    unsigned __int8 unk1[0x36]; // 0x0A
    Vector3 origin;             // 0x40
}; // 0x4C
