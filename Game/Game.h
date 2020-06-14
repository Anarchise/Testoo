#pragma once

#include "stdafx.h"
#include "Game\Client\Client.h"
#include "Game\CEntity\centity_s.h"
#include "Game\Refdef\refdef_t.h"

class Game
{
public:
	Game();
	~Game();
	bool initialise();
	void tick();
	bool hasLoaded();
public:
	refdef_t* refdef;
	//refdef_t* refdefa;
	Client* client;
	bool initialised;
	DWORD pebIndex;
};
