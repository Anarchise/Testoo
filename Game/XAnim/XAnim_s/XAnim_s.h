#pragma once

#include "stdafx.h"
#include "Game\XAnim\XAnimEntry\XAnimEntry.h"

class XAnim_s
{
public:
    unsigned int size;     // 0x00
    XAnimEntry entries[1]; // 0x04
}; // 0x18
