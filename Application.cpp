//Application.cpp
#include "Application.h"
#include <Psapi.h>
#include <ctime>

uintptr_t Application::BaseAddress = 0x0;
DWORD Application::Size = 0;
Game* Application::game = nullptr;
bool Application::running = false;

void Application::start()
{
	if (Application::running)
		return;

	// Set a random seed
	srand((unsigned int)std::time(nullptr));

	// Display Console
	Application::displayConsole();

	// Get base address
	Application::BaseAddress = (uintptr_t)GetModuleHandleA(NULL);

	// Get the application size
	MODULEINFO modinfo;
	GetModuleInformation(GetCurrentProcess(), (HMODULE)Application::BaseAddress, &modinfo, sizeof(MODULEINFO));
	Application::Size = modinfo.SizeOfImage;

	// Mark as running
	Application::running = true;

	// Initialise the game
	Application::game = new Game();

	// Hook the program
	Application::hook();
}

void Application::stop()
{
	Application::running = false;

	// Stop the graphics
	Graphics::stop();

	delete Application::game;
}

void Application::displayConsole()
{
	AllocConsole();
	freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	SetConsoleTitleA("Anarchise");
	MoveWindow(GetConsoleWindow(), 50, 50, 720, 500, TRUE);
	ShowWindow(GetConsoleWindow(), SW_SHOW);
}

void Application::hook()
{
	Hook::start();
}

bool Application::isRunning()
{
	return Application::running;
}

void Application::tick()
{
	Application::game->tick();
}

bool Application::isValidPointer(uintptr_t pointer)
{
	return pointer != NULL && pointer >= Application::BaseAddress && pointer <= (Application::BaseAddress + Application::Size);
}
