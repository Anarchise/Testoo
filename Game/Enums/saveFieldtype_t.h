#pragma once

enum class saveFieldtype_t
{
    SF_NONE = 0x0,
    SF_STRING = 0x1,
    SF_ENTITY = 0x2,
    SF_ENTHANDLE = 0x3,
    SF_CLIENT = 0x5,
    SF_VEHICLE = 0x6,
    SF_THREAD = 0x7,
    SF_SENTIENT = 0x8,
    SF_SENTIENTHANDLE = 0x9,
    SF_PATHNODE = 0xA,
    SF_ANIMTREE = 0xB,
    SF_AGENT = 0x11,
};
