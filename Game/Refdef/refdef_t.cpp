//refdef_t.cpp
#include "refdef_t.h"
#include "Application.h"
refdef_t* refdef_t::get()
{
	return *(refdef_t**)(Application::BaseAddress + (uintptr_t)Dynamic::Camera::RefDef);
}

refdef_t* refdef_t::get2()
{
	return *(refdef_t**)(DecryptRefDef());
}