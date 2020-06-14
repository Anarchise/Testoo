//Sys.cpp
#include "Sys.h"

void Sys::exit(int code)
{
	MessageBoxA(NULL, ("Error " + std::to_string(code)).c_str(), "Fatal Error", MB_OK | MB_ICONERROR);
	TerminateProcess(GetCurrentProcess(), code);
}
