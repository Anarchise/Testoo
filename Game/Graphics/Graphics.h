#pragma once

#include "stdafx.h"
#include "vendors/imgui/imgui.h"
#include "vendors/imgui/imgui_impl_win32.h"
#include "vendors/imgui/imgui_impl_dx12.h"
#include <d3d12.h>
#include <dxgi1_4.h>

struct FrameContext
{
	ID3D12CommandAllocator* CommandAllocator = nullptr;
	UINT64                  FenceValue = 0;
};

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class Graphics
{
public:
	static void start(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue);
	static void stop();
	static void begin();
	static HRESULT end(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags);
	static void clear();
	static void clearVariables();
	static void beginScene();
	static void endScene();
	
	static void line(Vector2 from, Vector2 to, ImColor color = { 0, 0, 0, 255 }, float thickness = 1.0f);
	static void rectangle(Vector2 position, Vector2 size, ImColor color = { 0, 0, 0, 255 }, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All, float thickness = 1.0f);
	static void rectangleFilled(Vector2 position, Vector2 size, ImColor color = { 0, 0, 0, 255 }, float rounding = 0.0f, uint32_t roundingCornersFlags = ImDrawCornerFlags_All);
	static void text(Vector2 position, std::string message, ImColor color, float size = 0.0f);
	static Vector2 measureText(std::string message, float fontSize = 0.0f);
private:
	static void startD3D12(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue);
	static void restart(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue);

	static FrameContext* waitForNextFrameResources();
	static DXGI_SWAP_CHAIN_DESC1 getSwapchainDesc();
	static void createCbvDescriptorHeap();
	static void createRtvDescriptorHeap();
	static void createFence();
	static void createCommandAllocator();
	static void getDescriptors();
	static void createCommandList();
	static void createRenderTarget();
public:
	static bool started;
	static bool needsRestart;
	static IDXGISwapChain3* g_pSwapChain;
	static ID3D12CommandQueue* g_pCommandQueue;
private:
	static ImFont* font;
	static ID3D12Device* g_pDevice;
	static ID3D12DescriptorHeap* g_pRtvDescHeap;
	static ID3D12DescriptorHeap* g_pSrvDescHeap;
	static ID3D12Fence* g_pFence;
	static FrameContext* g_frameContext;
	static ID3D12Resource** g_resources;
	static D3D12_CPU_DESCRIPTOR_HANDLE* g_descriptors;
	static HANDLE g_waitableObject;
	static HANDLE g_FenceEvent;
	static UINT g_frameCount;
	static UINT g_frameIndex;
	static UINT64 g_fenceLastSignalValue;
	static ID3D12GraphicsCommandList* g_pCommandList;
};
