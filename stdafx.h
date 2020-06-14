#pragma once

#include <windows.h>
#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <string>

#pragma comment(lib, "User32.lib")

// #define _DEBUG_ADDRESSES true

#include "Utilities\Sys\Sys.h"

// Enums
#include "Game\Enums\CharacterStance.h"
#include "Game\Enums\clientConnected_t.h"
#include "Game\Enums\CriticalSection.h"
#include "Game\Enums\errorParm_t.h"
#include "Game\Enums\saveFieldtype_t.h"
#include "Game\Enums\SentientType.h"
#include "Game\Enums\sessionState_t.h"
#include "Game\Enums\team_t.h"
#include "Game\Enums\TraceHitType.h"
#include "Game\Enums\trType_t.h"

#include "Game\Dynamic.h"
#include "Utilities\Math\Vector\Vector2\Vector2.h"
#include "Utilities\Math\Vector\Vector3\Vector3.h"
#include "Utilities\Math\Math.h"
#include "Game\Graphics\Graphics.h"
