//Cryptography.cpp
#include "Cryptography.h"
#include "Application.h"

uintptr_t Cryptography::decrypt(uintptr_t pointer, uint64_t* keys)
{
	#ifdef _DEBUG_ADDRESSES
	std::cout << "Keys[0]: " << std::hex << keys[0] << std::endl;
	std::cout << "Keys[1]: " << std::hex << keys[1] << std::endl;
	std::cout << "Keys[2]: " << std::hex << keys[2] << std::endl;
	std::cout << "Keys[3]: " << std::hex << keys[3] << std::endl;
	#endif
	pointer = pointer * keys[0];
	pointer = pointer ^ (pointer >> 0x1D);
	pointer = pointer ^ (pointer >> 0x3A);
	pointer = pointer * keys[1];
	pointer = pointer ^ (pointer >> 0x7);
	pointer = pointer ^ (pointer >> 0x0E);
	pointer = pointer ^ (pointer >> 0x1C);
	pointer = pointer ^ (pointer >> 0x38);
	pointer = pointer * keys[2];
	pointer = pointer ^ (pointer >> 0x1B);
	pointer = pointer ^ (pointer >> 0x36);
	pointer = pointer * keys[3];
	pointer = pointer ^ (pointer >> 0x0B);
	pointer = pointer ^ (pointer >> 0x16);
	pointer = pointer ^ (pointer >> 0x2C);
	return pointer;
}
