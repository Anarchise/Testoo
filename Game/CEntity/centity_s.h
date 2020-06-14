#pragma once

#include "stdafx.h"
#include "Game\Enums\EntityStance.h"

typedef void(__stdcall* fGetEntityOrigin)(uintptr_t a1, Vector3* origin);

class centity_s
{
	public:
	bool isValid();
	bool isStance(EntityStance stance);
	bool isStanding();
	bool isCrouching();
	bool isCrawling();
	bool isDowned();

    unsigned __int8 unk1[0x84];  // 0x00
	int32_t infoValid;           // 0x84
	unsigned __int8 unk2[0xCC];  // 0x88
	int32_t type;                // 0x154
	int32_t state;               // 0x158
	unsigned __int8 unk3[0x88];  // 0x15C
	EntityStance stance;         // 0x1E4
	unsigned __int8 unk4[0x6A];  // 0x1E6
	uint64_t originXor1;         // 0x250
	uint64_t originXor2;        // 0x258
	uint64_t originFunc;         // 0x260
	unsigned __int8 unk5[0xF0];  // 0x268

	static uint64_t keys[16][4];
}; // 0x358
