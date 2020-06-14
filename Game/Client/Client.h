#pragma once

#include "stdafx.h"
#include "Game\Client\Local\Local.h"
#include "Game\Client\CharacterInfo\characterInfo_t.h"
uint64_t DecryptRefDef();
//uint64_t decrypt_clientinfo();

class Client
{
public:
	static Client* get();
	static uint64_t* getDecryptionKeys();

	characterInfo_tt* getCharacterInfo(int characterIndex);
	characterInfo_tt* getLocalCharacterInfo();
public:
	unsigned __int8 unk1[0x84848];          // 0x00
	Local* local;                           // 0x84848
	unsigned __int8 unk2[0x12F08];          // 0x84850
	characterInfo_tt (*characterInfos)[155]; // 0x97758
private:
	static uint64_t keys[4];
};
