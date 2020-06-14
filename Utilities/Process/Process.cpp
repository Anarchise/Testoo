//Process.cpp
#include "Process.h"

HWND Process::handle = 0;

std::string Process::getSystemWindowDirectory()
{
	char systemDir[MAX_PATH] = { 0 };
	if (GetSystemDirectoryA(systemDir, MAX_PATH) == 0)
		Sys::exit(778);

	char* end = strrchr(systemDir, '\\');
	if (end == nullptr)
		Sys::exit(779);

	return std::string(systemDir, 0, end + 1 - systemDir);
}

BOOL CALLBACK Process::EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{
	DWORD dwPid = 0;
	GetWindowThreadProcessId(hWnd, &dwPid);

	// Ignore other processes
	if (dwPid != lParam)
		return TRUE;

	// Ignore console window
	if (hWnd == GetConsoleWindow())
		return TRUE;

	// Ignore child windows
	if (GetWindow(hWnd, GW_OWNER) == 0)
		return TRUE;

	// Ignore invisible windows
	if (IsWindowVisible(hWnd))
		return TRUE;

	Process::handle = hWnd;
	return FALSE;
}

HWND Process::getProcessWindow()
{
	if (Process::handle)
		return Process::handle;

	EnumWindows(Process::EnumWindowCallBack, GetCurrentProcessId());

	if (Process::handle == NULL)
		Sys::exit(777);
	return Process::handle;
}