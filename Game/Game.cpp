//Game.cpp
#include "Game.h"
#include "Cryptography\Cryptography.h"
#include "ESP\ESP.h"
#include "Application.h"
#include "vendors\ida\defs.h"

Game::Game()
{
	this->initialised = false;
	this->refdef = nullptr;
	//this->refdefa = nullptr;
	this->client = nullptr;
	this->pebIndex = 0;
}

Game::~Game()
{

}

bool Game::initialise()
{
	this->pebIndex = (DWORD)(__readgsqword(0x60) >> 0xC) & 0xF;

	this->refdef = refdef_t::get2();

	// Failed to initialise if refdef is invalid
	if ((uintptr_t)this->refdef == NULL)
		return false;

	#ifdef _DEBUG_ADDRESSES
	std::cout << "RefDef Address: " << std::hex << this->refdef << std::endl;
	#endif

	this->client = Client::get();
	#ifdef _DEBUG_ADDRESSES
	std::cout << "Client Address: " << std::hex << this->client << std::endl;
	#endif

	this->initialised = true;
	return true;
}
void Game::tick()
{
	/*if (!this->hasLoaded())
		return;*/

	// Try initialise game
	if (!this->initialised)
	{
		if (!this->initialise())
			return;
	}

	// Get my character
	characterInfo_tt* me = this->client->getLocalCharacterInfo();

	// If I exist...
	if (me->infoValid)
		ESP::update();
}
