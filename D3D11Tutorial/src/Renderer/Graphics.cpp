#include "pch.h"
#include "Graphics.h"

namespace wrl = Microsoft::WRL;


Graphics::Graphics(HWND hWnd)
{
	CoInitialize(NULL);


	DXGI_SWAP_CHAIN_DESC sd{};

	//Window has the width and height
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;

	//color format (BGR)
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;

	//use default values
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;

	//use default valuse
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//disable anti-aliasing
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	//set buffer to render target output
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;


	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		nullptr,
		&m_pContext
	);

	//gain access to texture subresource in swap chain
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

	m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget);
}

void Graphics::EndFrame()
{
	m_pSwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[]{ red, green, blue };
	m_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
}

