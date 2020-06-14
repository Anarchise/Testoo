//Graphics.cpp
#include "Graphics.h"
#include "Hook\Hook.h"
#include "Utilities\Process\Process.h"
#include "Application.h"

bool Graphics::started = false;
bool Graphics::needsRestart = false;
ImFont* Graphics::font = nullptr;
IDXGISwapChain3* Graphics::g_pSwapChain = nullptr;
ID3D12CommandQueue* Graphics::g_pCommandQueue = nullptr;
ID3D12Device* Graphics::g_pDevice = nullptr;
ID3D12DescriptorHeap* Graphics::g_pRtvDescHeap = nullptr;
ID3D12DescriptorHeap* Graphics::g_pSrvDescHeap = nullptr;
ID3D12Fence* Graphics::g_pFence = nullptr;
FrameContext* Graphics::g_frameContext = nullptr;
ID3D12Resource** Graphics::g_resources = nullptr;
D3D12_CPU_DESCRIPTOR_HANDLE* Graphics::g_descriptors = nullptr;
HANDLE Graphics::g_waitableObject = NULL;
HANDLE Graphics::g_FenceEvent = NULL;
UINT Graphics::g_frameCount = 0;
UINT Graphics::g_frameIndex = 0;
UINT64 Graphics::g_fenceLastSignalValue = 0;
ID3D12GraphicsCommandList* Graphics::g_pCommandList = nullptr;

void Graphics::start(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue)
{
	if (Graphics::started)
	{
		if (Graphics::needsRestart)
		{
			Graphics::restart(pSwapChain, pCommandQueue);
			Graphics::needsRestart = false;
		}
		return;
	}

	Graphics::startD3D12(pSwapChain, pCommandQueue);

	// Setup imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup dark style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(Process::getProcessWindow());
	ImGui_ImplDX12_Init(Graphics::g_pDevice, Graphics::g_frameCount,
		DXGI_FORMAT_R8G8B8A8_UNORM, Graphics::g_pSrvDescHeap,
		Graphics::g_pSrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
		Graphics::g_pSrvDescHeap->GetGPUDescriptorHandleForHeapStart());

	// Add font
	std::string fontPath = Process::getSystemWindowDirectory() + "Fonts\\msyh.ttc";
	static char szFontPath[MAX_PATH] = { 0 };
	memcpy(szFontPath, fontPath.c_str(), fontPath.length());
	Graphics::font = io.Fonts->AddFontFromFileTTF(szFontPath, 20.0f, NULL, io.Fonts->GetGlyphRangesDefault());

	if (Graphics::font == nullptr)
		Sys::exit(9);

	// Hook wnd proc
	Hook::hookWndProc();

	Graphics::started = true;
}

void Graphics::startD3D12(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue)
{
	Graphics::g_pSwapChain = pSwapChain;
	Graphics::g_pCommandQueue = pCommandQueue;

	// Get the device
	if (FAILED(Graphics::g_pSwapChain->GetDevice(__uuidof(ID3D12Device), (void**)&Graphics::g_pDevice)))
		Sys::exit(1);

	// Get the swap chain description
	DXGI_SWAP_CHAIN_DESC1 sdesc = Graphics::getSwapchainDesc();

	Graphics::g_frameCount = sdesc.BufferCount;

	// Create the RTV descriptor heap
	Graphics::createRtvDescriptorHeap();

	// Create the CBV descriptor heap
	Graphics::createCbvDescriptorHeap();

	// Create fence
	Graphics::createFence();

	// ALlocate frame context
	Graphics::g_frameContext = new FrameContext[Graphics::g_frameCount];
	Graphics::g_resources = new ID3D12Resource*[Graphics::g_frameCount];
	Graphics::g_descriptors = new D3D12_CPU_DESCRIPTOR_HANDLE[Graphics::g_frameCount];

	// Create the command allocator
	Graphics::createCommandAllocator();

	// Get descriptors
	Graphics::getDescriptors();

	// Create the command list
	Graphics::createCommandList();

	// Waitable Object
	Graphics::g_waitableObject = Graphics::g_pSwapChain->GetFrameLatencyWaitableObject();

	// Create event
	Graphics::g_FenceEvent = CreateEvent(nullptr, false, false, nullptr);
	if (Graphics::g_FenceEvent == NULL)
		Sys::exit(7);

	// Create render target
	Graphics::createRenderTarget();
}

void Graphics::restart(IDXGISwapChain3* pSwapChain, ID3D12CommandQueue* pCommandQueue)
{
	Graphics::startD3D12(pSwapChain, pCommandQueue);
	ImGui_ImplDX12_CreateDeviceObjects();
}

void Graphics::stop()
{
	delete Graphics::g_frameContext;
}

void Graphics::begin()
{
	// Start the frame
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

HRESULT Graphics::end(IDXGISwapChain3* pSwapChain, UINT SyncInterval, UINT Flags)
{
	// Rendering
	FrameContext* frameCtxt = Graphics::waitForNextFrameResources();

	UINT backBufferIdx = Graphics::g_pSwapChain->GetCurrentBackBufferIndex();

	frameCtxt->CommandAllocator->Reset();

	{
		static D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.pResource = Graphics::g_resources[backBufferIdx];
		Graphics::g_pCommandList->Reset(frameCtxt->CommandAllocator, nullptr);
		Graphics::g_pCommandList->ResourceBarrier(1, &barrier);
		Graphics::g_pCommandList->OMSetRenderTargets(1, &Graphics::g_descriptors[backBufferIdx], FALSE, nullptr);
		Graphics::g_pCommandList->SetDescriptorHeaps(1, &Graphics::g_pSrvDescHeap);
	}

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), Graphics::g_pCommandList);

	static D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.pResource = Graphics::g_resources[backBufferIdx];

	Graphics::g_pCommandList->ResourceBarrier(1, &barrier);
	Graphics::g_pCommandList->Close();

	Graphics::g_pCommandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&Graphics::g_pCommandList);

	HRESULT result = Hook::oPresent(pSwapChain, SyncInterval, Flags);

	UINT64 fenceValue = Graphics::g_fenceLastSignalValue + 1;
	Graphics::g_pCommandQueue->Signal(Graphics::g_pFence, fenceValue);
	Graphics::g_fenceLastSignalValue = fenceValue;
	frameCtxt->FenceValue = fenceValue;

	return result;
}

void Graphics::clear()
{
	if (Graphics::g_frameContext == nullptr)
		return;

	FrameContext* frameCtxt = &Graphics::g_frameContext[Graphics::g_frameIndex % Graphics::g_frameCount];

	UINT64 fenceValue = frameCtxt->FenceValue;

	// No fence was signaled
	if (fenceValue == 0)
		return;

	frameCtxt->FenceValue = 0;

	if (Graphics::g_pFence->GetCompletedValue() < fenceValue)
	{
		Graphics::g_pFence->SetEventOnCompletion(fenceValue, Graphics::g_FenceEvent);
		WaitForSingleObject(Graphics::g_FenceEvent, INFINITE);
	}

	Graphics::clearVariables();
}

void Graphics::clearVariables()
{
	Graphics::g_pSwapChain = nullptr;
	Graphics::g_pDevice = nullptr;
	Graphics::g_pCommandQueue = nullptr;

	if (Graphics::g_pFence != nullptr)
	{
		Graphics::g_pFence->Release();
		Graphics::g_pFence = nullptr;
	}

	if (Graphics::g_pSrvDescHeap != nullptr)
	{
		Graphics::g_pSrvDescHeap->Release();
		Graphics::g_pSrvDescHeap = nullptr;
	}

	if (Graphics::g_pRtvDescHeap != nullptr)
	{
		Graphics::g_pRtvDescHeap->Release();
		Graphics::g_pRtvDescHeap = nullptr;
	}

	if (Graphics::g_pCommandList != nullptr)
	{
		Graphics::g_pCommandList->Release();
		Graphics::g_pCommandList = nullptr;
	}

	if (Graphics::g_frameContext != nullptr)
	{
		for (UINT i = 0; i < Graphics::g_frameCount; ++i)
		{
			if (Graphics::g_frameContext[i].CommandAllocator != nullptr)
			{
				Graphics::g_frameContext[i].CommandAllocator->Release();
				Graphics::g_frameContext[i].CommandAllocator = nullptr;
			}
		}

		delete[] Graphics::g_frameContext;
		Graphics::g_frameContext = nullptr;
	}

	if (Graphics::g_resources != nullptr)
	{
		for (UINT i = 0; i < Graphics::g_frameCount; ++i)
		{
			if (Graphics::g_resources[i] != nullptr)
			{
				Graphics::g_resources[i]->Release();
				Graphics::g_resources[i] = nullptr;
			}
		}

		delete[] Graphics::g_resources;
		Graphics::g_resources = nullptr;
	}

	if (Graphics::g_descriptors != nullptr)
	{
		delete[] Graphics::g_descriptors;
		Graphics::g_descriptors = nullptr;
	}

	Graphics::g_waitableObject = nullptr;

	if (Graphics::g_FenceEvent)
	{
		CloseHandle(Graphics::g_FenceEvent);
		Graphics::g_FenceEvent = nullptr;
	}

	Graphics::g_frameCount = 0;
	Graphics::g_frameIndex = 0;
}

FrameContext* Graphics::waitForNextFrameResources()
{
	Graphics::g_frameIndex++;

	HANDLE waitableObjects[] = { Graphics::g_waitableObject, NULL };
	constexpr DWORD numWaitableObjects = 1;

	FrameContext* frameCtxt = &Graphics::g_frameContext[Graphics::g_frameIndex % Graphics::g_frameCount];
	WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);
	return frameCtxt;
}

DXGI_SWAP_CHAIN_DESC1 Graphics::getSwapchainDesc()
{
	DXGI_SWAP_CHAIN_DESC1 sdesc;
	Graphics::g_pSwapChain->GetDesc1(&sdesc);
	return sdesc;
}

void Graphics::createRtvDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC descriptorBackBuffers;
	descriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	descriptorBackBuffers.NumDescriptors = Graphics::g_frameCount;
	descriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	descriptorBackBuffers.NodeMask = 1;

	if (FAILED(Graphics::g_pDevice->CreateDescriptorHeap(&descriptorBackBuffers, IID_PPV_ARGS(&g_pRtvDescHeap))))
		Sys::exit(2);
}

void Graphics::createCbvDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;

	if (FAILED(Graphics::g_pDevice->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&g_pSrvDescHeap))))
		Sys::exit(3);
}

void Graphics::createFence()
{
	if (FAILED(Graphics::g_pDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_pFence))))
		Sys::exit(4);
}

void Graphics::createCommandAllocator()
{
	for (size_t i = 0; i < Graphics::g_frameCount; i++)
	{
		if (FAILED(Graphics::g_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Graphics::g_frameContext[i].CommandAllocator))))
			Sys::exit(5);
	}
}

void Graphics::getDescriptors()
{
	SIZE_T rtvDescriptorSize = Graphics::g_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = g_pRtvDescHeap->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < Graphics::g_frameCount; i++)
	{
		Graphics::g_descriptors[i] = rtvHandle;
		rtvHandle.ptr += rtvDescriptorSize;
	}
}

void Graphics::createCommandList()
{
	if (FAILED(Graphics::g_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Graphics::g_frameContext[0].CommandAllocator, NULL, IID_PPV_ARGS(&g_pCommandList)))
		|| FAILED(g_pCommandList->Close()))
		Sys::exit(6);
}

void Graphics::createRenderTarget()
{
	ID3D12Resource* pBackBuffer = nullptr;
	for (UINT i = 0; i < Graphics::g_frameCount; i++)
	{
		if (FAILED(Graphics::g_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer))))
			Sys::exit(8);
		Graphics::g_pDevice->CreateRenderTargetView(pBackBuffer, NULL, Graphics::g_descriptors[i]);
		Graphics::g_resources[i] = pBackBuffer;
	}
}

void Graphics::beginScene()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });
	ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs);

	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::SetWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y), ImGuiCond_Always);
}

void Graphics::endScene()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGui::GetWindowDrawList()->PushClipRectFullScreen();
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
}

void Graphics::rectangle(Vector2 position, Vector2 size, ImColor color, float rounding, uint32_t roundingCornersFlags, float thickness)
{
	ImGui::GetWindowDrawList()->AddRect(
		ImVec2(position.x, position.y),
		ImVec2(position.x + size.x, position.y + size.y),
		color,
		rounding,
		roundingCornersFlags,
		thickness
	);
}

void Graphics::rectangleFilled(Vector2 position, Vector2 size, ImColor color, float rounding, uint32_t roundingCornersFlags)
{
	ImGui::GetWindowDrawList()->AddRectFilled(
		ImVec2(position.x, position.y),
		ImVec2(position.x + size.x, position.y + size.y),
		color,
		rounding,
		roundingCornersFlags
	);
}

void Graphics::line(Vector2 from, Vector2 to, ImColor color, float thickness)
{
	ImGui::GetWindowDrawList()->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), color, thickness);
}

void Graphics::text(Vector2 position, std::string message, ImColor color, float size)
{
	ImGui::GetWindowDrawList()->AddText(NULL, size, ImVec2(position.x, position.y), color, message.c_str());
}

Vector2 Graphics::measureText(std::string message, float fontSize)
{
	ImVec2 size = ImGui::CalcTextSize(message.c_str(), NULL, false, -1.0f, fontSize);
	return Vector2(size.x, size.y);
}
