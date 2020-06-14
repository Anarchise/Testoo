// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Application.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		// Start the application
		Application::start();
		break;
	case DLL_PROCESS_DETACH:
		// Stop the application
		Application::stop();
		break;
	}
	return TRUE;
}
