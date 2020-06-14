//Client.cpp
#include "Client.h"
#include "Application.h"
#include "Game\Cryptography\Cryptography.h"

uint64_t Client::keys[4];
uint64_t refDefKey = 0;

class refdefKeyStruct {
public:
	DWORD ref0;      // 0x00
	DWORD ref1;      // 0x04
	DWORD ref2;		// 0x08
};
uint64_t DecryptRefDef()
{
	if (refDefKey != 0) {
		return refDefKey;
	}
	refdefKeyStruct crypt = *(refdefKeyStruct*)(Application::BaseAddress + (uintptr_t)Dynamic::Camera::REFDEF_OFFSET_1);
	DWORD lower = crypt.ref0 ^ (crypt.ref2 ^ (unsigned __int64)(Application::BaseAddress + (uintptr_t)Dynamic::Camera::REFDEF_OFFSET_1)) * ((crypt.ref2 ^ (unsigned __int64)(Application::BaseAddress + (uintptr_t)Dynamic::Camera::REFDEF_OFFSET_1)) + 2);
	DWORD upper = crypt.ref1 ^ (crypt.ref2 ^ (unsigned __int64)(Application::BaseAddress + (uintptr_t)Dynamic::Camera::REFDEF_OFFSET_1 + 0x4)) * ((crypt.ref2 ^ (unsigned __int64)(Application::BaseAddress + (uintptr_t)Dynamic::Camera::REFDEF_OFFSET_1 + 0x4)) + 2);
	refDefKey = (uint64_t)upper << 32 | lower; // Merge Both DWORD into QWORD

	return refDefKey;
}
uint64_t* Client::getDecryptionKeys()
{
	if (Client::keys[0] != 0)
		return (uint64_t*)Client::keys;

	// Get dynamic key from memory
	uintptr_t reversed = *(uintptr_t*)(Application::BaseAddress + (uintptr_t)Dynamic::Cryptography::Client::DynamicKey);
	uintptr_t address = _byteswap_uint64(reversed);

	#ifdef _DEBUG_ADDRESSES
	std::cout << "Dynamic Client Encryption Key Address: " << std::hex << address << std::endl;
	#endif

	uint64_t dynamicKey = *(uint64_t*)(address + (uintptr_t)Dynamic::Cryptography::Client::DynamicKeyOffset);
	#ifdef _DEBUG_ADDRESSES
	std::cout << "Dynamic Client Encryption Key: " << std::hex << dynamicKey << std::endl;
	#endif

	Client::keys[0] = (uint64_t)Dynamic::Cryptography::ClientKeys::Key0;
	Client::keys[1] = (uint64_t)Dynamic::Cryptography::ClientKeys::Key1;
	Client::keys[2] = (uint64_t)Dynamic::Cryptography::ClientKeys::Key2;
	Client::keys[3] = (uint64_t)Dynamic::Cryptography::ClientKeys::Key3;

	return (uint64_t*)Client::keys;
}
Client* Client::get()
{
	uint64_t* keys = Client::getDecryptionKeys();

	uintptr_t encryptedAddress = *(uintptr_t*)(Application::BaseAddress + (uintptr_t)Dynamic::Cryptography::Client::Encrypted);
	#ifdef _DEBUG_ADDRESSES
		std::cout << "Encrypted Client Address: " << std::hex << encryptedAddress << std::endl;
	#endif
	
	return (Client*)Cryptography::decrypt(encryptedAddress, keys);
}

characterInfo_tt* Client::getCharacterInfo(int characterIndex)
{
	return &((*this->characterInfos)[characterIndex]);
}

characterInfo_tt* Client::getLocalCharacterInfo()
{
	return this->getCharacterInfo(this->local->characterIndex);
}

/*
uint64_t decrypt_clientinfo() {			// (the same as client::get)
	uintptr_t encryptedPtr = *(uintptr_t*)(Application::BaseAddress + (uintptr_t)Dynamic::Cryptography::Client::Encrypted);

	if (encryptedPtr) {
		uint64_t* keys = Client::getDecryptionKeys();

		uintptr_t realPtr = Cryptography::decrypt(encryptedPtr, keys);
		return realPtr;
	}

	return 0;
}
*/