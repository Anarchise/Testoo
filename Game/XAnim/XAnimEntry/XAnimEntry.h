#pragma once

#include "stdafx.h"
#include "Game\XAnim\XAnimParent\XAnimParent.h"

class XAnimEntry
{
public:
    unsigned __int16 numAnims; // 0x00
    unsigned __int16 parent;   // 0x02
    void* parts;               // 0x04 (XAnimParts*)
    XAnimParent animParent;    // 0x0C
}; // 0x10
