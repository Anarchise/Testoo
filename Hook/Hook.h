#pragma once

#include "stdafx.h"
#include <dxgi1_4.h>

typedef void (*DetourFunction)();
typedef HRESULT(__fastcall* PresentFunction) (IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
typedef enum class _MEMORY_INFORMATION_CLASS { MemoryBasicInformation } MEMORY_INFORMATION_CLASS, * PMEMORY_INFORMATION_CLASS;
typedef NTSTATUS(WINAPI* NtQueryVirtualMemoryFunction) (HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);
typedef SIZE_T(*VirtualQueryFunction) (LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);

typedef __int64(__fastcall* fG_RunFrame) (__int64 a1);
typedef __int64(__fastcall* fCmd_ExecuteSingleCommand) (__int64 text);
typedef void*(__fastcall* fDvar_FindMalleableVar) (__int64 dvarName);
typedef void*(__fastcall* fDvar_FindVar) (const char* dvarName);

struct aHook
{
	DetourFunction detour;
	uint64_t address;
	void* original;
};

class Hook
{
public:
	static void start();
	static void hookWndProc();
private:
	static HRESULT __fastcall Present(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
	static NTSTATUS WINAPI NtQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength);
	static SIZE_T VirtualQuery(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength);
	static LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static __int64 __fastcall G_RunFrame(__int64 a1);
	static __int64 __fastcall Cmd_ExecuteSingleCommand(__int64 test);
	static void* __fastcall Dvar_FindMalleableVar(__int64 dvarName);
	static void* __fastcall Dvar_FindVar(const char* dvarName);

	static void hookPresent();
	static void unhookPresent();

	template <typename T>
	static inline T write(void* base, int64_t index, T hook)
	{
		uintptr_t* vTable = *(uintptr_t**)base;
		DWORD oldProtect = 0;
		VirtualProtect(&vTable[index], sizeof(uintptr_t), PAGE_READWRITE, &oldProtect);

		uintptr_t original = vTable[index];
		vTable[index] = (uintptr_t)hook;

		VirtualProtect(&vTable[index], sizeof(uintptr_t), oldProtect, &oldProtect);

		return (T)original;
	}
public:
	static PresentFunction oPresent;
private:
	static NtQueryVirtualMemoryFunction oNtQueryVirtualMemory;
	static VirtualQueryFunction oVirtualQuery;
	static WNDPROC oWndProc;
	static fG_RunFrame oG_RunFrame;
	static fCmd_ExecuteSingleCommand oCmd_ExecuteSingleCommand;
	static fDvar_FindMalleableVar oDvar_FindMalleableVar;
	static fDvar_FindVar oDvar_FindVar;
};
