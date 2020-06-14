#pragma once

#include "stdafx.h"
#include "Game\Game.h"
#include "Hook\Hook.h"

class Application
{
public:
	static void start();
	static void stop();
	static void tick();
	static bool isValidPointer(uintptr_t pointer);
private:
	static void displayConsole();
	static void hook();
	static bool isRunning();
public:
	static uintptr_t BaseAddress;
	static DWORD Size;
	static Game* game;
private:
	static bool running;
};
