//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Libs and Includes
#pragma once
#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxguid.lib")
#include <d3d11_4.h>


// Namespace Case_Engine
namespace Case_Engine
{
	class Window;
	class GfxCommandContext;

	class GfxDevice
	{
	public:
		explicit GfxDevice(Window* window);
		GfxDevice(GfxDevice const&) = delete;
		GfxDevice(GfxDevice&&) = default;
		GfxDevice& operator=(GfxDevice const&) = delete;
		GfxDevice& operator=(GfxDevice&&) = default;
		~GfxDevice();

		void ResizeBackbuffer(uint32_t w, uint32_t h);
		void ClearBackbuffer();
		void SwapBuffers(bool vsync);
		void SetBackbuffer();

		ID3D11Device3* GetDevice() const;
		ID3D11DeviceContext4* GetContext() const;
		GfxCommandContext* GetCommandContext() const { return command_context.get(); }
		Window* GetWindow() const { return window; }

	public:
		ArcPtr<ID3D11Device3> device = nullptr;

	private:
		Window* window;
		uint32_t width, height;
		ArcPtr<IDXGISwapChain> swapchain = nullptr;
		std::unique_ptr<GfxCommandContext> command_context;
		ArcPtr<ID3D11Texture2D> backbuffer = nullptr;
		ArcPtr<ID3D11RenderTargetView> backbuffer_rtv = nullptr;

	private:

		void WaitForGPU();
		void CreateBackBufferResources(uint32_t w, uint32_t h);
	};
}