#pragma once
#include <stdint.h>

namespace Dynamic
{
	enum class D3D12 : uintptr_t
	{
		SwapChain = 0x123CF618,
		CommandQueue = 0x123CA2A8,
	};

	enum class Functions : uintptr_t
	{
		G_RunFrame = 0x16cf2e0,
		ExecuteSingleCommand = 0x1755cd0,
		Dvar_FindMalleableVar = 0x1930720,
		Dvar_FindVar = 0x1930800,
	};

	namespace Cryptography
	{
		enum class Client : uintptr_t	//	0x3580	//clientinfo_t
		{
			Encrypted = 0x10A2C5C8,	//ENCRYPT_PTR_OFFSET
			DynamicKey = 0x469414F,	//REVERSED_ADDRESS 
			DynamicKeyOffset = 0x13, //LAST_KEY_XOR 
		};

		enum class ClientKeys : uint64_t	//clientinfo_t
		{
			Key0 = 0x0000000000000000,
			Key1 = 0x6F941D46BD74E8BB,
			Key2 = 0x177D16E455DE85C5,
			Key3 = 0x5E378025FDDEFC41,
		};

	}

	enum class Camera : uintptr_t //
	{
		RefDef = 0x0DFEAC10, //  CAMERA_OFFSET, Pointer to Camera Struct, +0x140 to get camera Origin vector3
		REFDEF_OFFSET_1 = 0x10A2E300,	// Encrypted Pointer to RefDef Struct
		LOCAL_INDEX_OFFSET = 0x1588	//// Pointer to LocalInfo of self inside CharacterInfo_t struct, use +0x470 to get IndexNumber
	};

}
