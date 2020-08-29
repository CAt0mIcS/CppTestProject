#include "pch.h"
#include "Graphics.h"
#include "Exceptions/dxerr.h"

#pragma comment(lib, "d3d11.lib")


Graphics::Graphics(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd{};
	
	//D3D11 has the HWND so it should figure it out by itself
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;

	//color format (BGR) 
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	//let the system choose the refresh rate
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;

	//not neccessary for now
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//disable anti-aliasing like this
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	//set buffer usage to be render target output
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	//we want one buffer
	sd.BufferCount = 1;

	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;

	//best performance in most cases
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	//flags
	sd.Flags = 0;

	//for checking results of d3d11 functions
	HRESULT hr;
	
	//create device and front/back buffers, swap chain and rendering context
	GFX_THROW_FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pContext
	));

	//gain access to texture subresource in swap chain (0 = back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&pBackBuffer));
	
	//create render target view
	GFX_THROW_FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pTarget));
	
	//release handle to back buffer
	pBackBuffer->Release();
}

void Graphics::EndFrame()
{
	//for checking results of d3d11 functions
	HRESULT hr;

	//SyncInterval one means 60 fps, two means 30fps...
	if (FAILED(hr = m_pSwapChain->Present(1u, 0u)))
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw GFX_DEVICE_REMOVED_EXCEPT(m_pDevice->GetDeviceRemovedReason());
		}
		else
		{
			GFX_THROW_FAILED(hr);
		}
	}
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	m_pContext->ClearRenderTargetView(m_pTarget, color);
}

Graphics::~Graphics()
{
	//free resources
	if (m_pTarget)
		m_pTarget->Release();
	if (m_pContext)
		m_pContext->Release();
	if (m_pSwapChain)
		m_pSwapChain->Release();
	if (m_pDevice)
		m_pDevice->Release();
}




