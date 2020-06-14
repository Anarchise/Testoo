#pragma once
#include "stdafx.h"

class Process
{
public:
	static std::string getSystemWindowDirectory();
	static HWND getProcessWindow();
private:
	static BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam);
private:
	static HWND handle;
};
