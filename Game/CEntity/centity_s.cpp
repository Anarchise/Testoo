//centity_s.cpp
#include "centity_s.h"
#include "Application.h"
#include "Game\Cryptography\Cryptography.h"


bool centity_s::isValid()
{
	return this->infoValid & 1;
}

bool centity_s::isStance(EntityStance stance)
{
	return ((int16_t)this->stance & (int16_t)stance) == (int16_t)stance;
}

bool centity_s::isStanding()
{
	return this->isStance(EntityStance::Standing);
}

bool centity_s::isCrouching()
{
	return this->isStance(EntityStance::Crouching);
}

bool centity_s::isCrawling()
{
	return this->isStance(EntityStance::Crawling);
}

bool centity_s::isDowned()
{
	return this->isStance(EntityStance::Downed);
}

