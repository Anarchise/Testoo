//Hook.cpp
#include "Hook.h"
#include "Application.h"
#include "vendors/minhook/include/MinHook.h"
#include "Utilities\Process\Process.h"

PresentFunction Hook::oPresent = nullptr;

NtQueryVirtualMemoryFunction Hook::oNtQueryVirtualMemory = nullptr;
VirtualQueryFunction Hook::oVirtualQuery = nullptr;
WNDPROC Hook::oWndProc = nullptr;

fG_RunFrame Hook::oG_RunFrame = nullptr;
fCmd_ExecuteSingleCommand Hook::oCmd_ExecuteSingleCommand = nullptr;
fDvar_FindMalleableVar Hook::oDvar_FindMalleableVar = nullptr;
fDvar_FindVar Hook::oDvar_FindVar = nullptr;

void Hook::start()
{
	// Hook present
	Hook::hookPresent();

	// Initialise MinHook
	if (MH_Initialize() != MH_OK)
		Sys::exit(100);

	// Hook G_RunFrame
	fG_RunFrame* G_RunFrameTarget = (fG_RunFrame*)(Application::BaseAddress + (uintptr_t)Dynamic::Functions::G_RunFrame);
	if (MH_CreateHook(G_RunFrameTarget, &Hook::G_RunFrame, (LPVOID*)(&Hook::oG_RunFrame)) != MH_OK || MH_EnableHook(G_RunFrameTarget) != MH_OK)
		Sys::exit(74);

	// Hook Cmd_ExecuteSingleCommand
	fCmd_ExecuteSingleCommand* Cmd_ExecuteSingleCommandTarget = (fCmd_ExecuteSingleCommand*)(Application::BaseAddress + (uintptr_t)Dynamic::Functions::ExecuteSingleCommand);
	if (MH_CreateHook(Cmd_ExecuteSingleCommandTarget, &Hook::Cmd_ExecuteSingleCommand, (LPVOID*)(&Hook::oCmd_ExecuteSingleCommand)) != MH_OK || MH_EnableHook(Cmd_ExecuteSingleCommandTarget) != MH_OK)
		Sys::exit(75);

	// Hook Dvar_FindMalleableVar
	fDvar_FindMalleableVar* Dvar_FindMalleableVarTarget = (fDvar_FindMalleableVar*)(Application::BaseAddress + (uintptr_t)Dynamic::Functions::Dvar_FindMalleableVar);
	if (MH_CreateHook(Dvar_FindMalleableVarTarget, &Hook::Dvar_FindMalleableVar, (LPVOID*)(&Hook::oDvar_FindMalleableVar)) != MH_OK || MH_EnableHook(Dvar_FindMalleableVarTarget) != MH_OK)
		Sys::exit(76);

	// Hook Dvar_FindVar
	fDvar_FindVar* Dvar_FindVarTarget = (fDvar_FindVar*)(Application::BaseAddress + (uintptr_t)Dynamic::Functions::Dvar_FindVar);
	if (MH_CreateHook(Dvar_FindVarTarget, &Hook::Dvar_FindVar, (LPVOID*)(&Hook::oDvar_FindVar)) != MH_OK || MH_EnableHook(Dvar_FindVarTarget) != MH_OK)
		Sys::exit(77);

	// Hook NtQueryVirtualMemory
	NtQueryVirtualMemoryFunction* NtQueryVirtualMemoryTarget = (NtQueryVirtualMemoryFunction*)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryVirtualMemory");
	if (MH_CreateHook(NtQueryVirtualMemoryTarget, &Hook::NtQueryVirtualMemory, (LPVOID*)(&Hook::oNtQueryVirtualMemory)) != MH_OK || MH_EnableHook(NtQueryVirtualMemoryTarget) != MH_OK)
		Sys::exit(21);

	// Hook VirtualQuery
	VirtualQueryFunction* VirtualQueryTarget = (VirtualQueryFunction*)GetProcAddress(GetModuleHandleA("kernel32.dll"), "VirtualQuery");
	if (MH_CreateHook(VirtualQueryTarget, &Hook::VirtualQuery, (LPVOID*)(&Hook::oVirtualQuery)) != MH_OK || MH_EnableHook(VirtualQueryTarget) != MH_OK)
		Sys::exit(22);
}

void Hook::hookPresent()
{
	uintptr_t swapChain = *(uintptr_t*)(Application::BaseAddress + (uintptr_t)Dynamic::D3D12::SwapChain);
	if (swapChain == 0)
		Sys::exit(1337);

	Hook::oPresent = Hook::write<PresentFunction>((void*)swapChain, 8, Hook::Present);
}

void Hook::unhookPresent()
{
	uintptr_t swapChain = *(uintptr_t*)(Application::BaseAddress + (uintptr_t)Dynamic::D3D12::SwapChain);
	if (swapChain == 0)
		Sys::exit(1339);

	Hook::write<PresentFunction>((void*)swapChain, 8, Hook::oPresent);
}

void Hook::hookWndProc()
{
	Hook::oWndProc = (WNDPROC)SetWindowLongPtr(Process::getProcessWindow(), GWLP_WNDPROC, (__int3264)(LONG_PTR)Hook::WndProc);
	if (Hook::oWndProc == NULL)
		Sys::exit(1340);
}

HRESULT __fastcall Hook::Present(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (pSwapChain == nullptr)
		return Hook::oPresent(pSwapChain, SyncInterval, Flags);

	ID3D12CommandQueue* commandQueue = (ID3D12CommandQueue*)(*(uintptr_t*)(Application::BaseAddress + (uintptr_t)Dynamic::D3D12::CommandQueue));
	if (commandQueue == 0)
		Sys::exit(1338);

	Graphics::start(pSwapChain, commandQueue);

	Graphics::begin();
	Application::tick();
	return Graphics::end(pSwapChain, SyncInterval, Flags);
}

__int64 __fastcall Hook::G_RunFrame(__int64 a1)
{
	__int64 result = Hook::oG_RunFrame(a1);

	// TODO
	
	return result;
}

__int64 __fastcall Hook::Cmd_ExecuteSingleCommand(__int64 text)
{
	__int64 result = Hook::oCmd_ExecuteSingleCommand(text);
	// std::cout << std::hex << result << " = Cmd_ExecuteSingleCommand(" << text << ");" << std::endl;
	return result;
}

void* __fastcall Hook::Dvar_FindMalleableVar(__int64 dvarName)
{
	void* result = Hook::oDvar_FindMalleableVar(dvarName);
	// std::cout << std::hex << result << " = Dvar_FindMalleableVar(" << dvarName << ");" << std::endl;
	return result;
}

void* __fastcall Hook::Dvar_FindVar(const char* dvarName)
{
	void* result = Hook::oDvar_FindVar(dvarName);
	// if (result != NULL)
		// std::cout << std::hex << result << " = Dvar_FindVar(" << dvarName << ");" << std::endl;
	return result;
}

NTSTATUS WINAPI Hook::NtQueryVirtualMemory(HANDLE ProcessHandle, PVOID BaseAddress, MEMORY_INFORMATION_CLASS MemoryInformationClass, PVOID MemoryInformation, SIZE_T MemoryInformationLength, PSIZE_T ReturnLength)
{
	// Ignore other processes and non-basic information
	if (ProcessHandle != GetCurrentProcess() || MemoryInformationClass != MEMORY_INFORMATION_CLASS::MemoryBasicInformation)
		return Hook::oNtQueryVirtualMemory(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);

	// Unhook
	Hook::unhookPresent();

	NTSTATUS response = Hook::oNtQueryVirtualMemory(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);

	// Hook
	Hook::hookPresent();

	return response;
}

SIZE_T Hook::VirtualQuery(LPCVOID lpAddress, PMEMORY_BASIC_INFORMATION lpBuffer, SIZE_T dwLength)
{
	// Unhook
	Hook::unhookPresent();

	SIZE_T response = Hook::oVirtualQuery(lpAddress, lpBuffer, dwLength);

	// Hook
	Hook::hookPresent();

	return response;
}

LRESULT APIENTRY Hook::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case 0x403:
		case WM_SIZE:
		{
			if (uMsg == WM_SIZE && wParam == SIZE_MINIMIZED)
				break;

			if (Graphics::started)
			{
				Graphics::clear();
				ImGui_ImplDX12_InvalidateDeviceObjects();
				Graphics::needsRestart = true;
			}
			break;
		}
	};

	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return true;
	return CallWindowProc(Hook::oWndProc, hwnd, uMsg, wParam, lParam);
}
