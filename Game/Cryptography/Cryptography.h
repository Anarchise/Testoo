#pragma once

#include "stdafx.h"
#include <vector>

class Cryptography
{
public:
	static uintptr_t decrypt(uintptr_t pointer, uint64_t* keys);
};
