#include "pch.h"
#include "Graphics.h"
#include "Exceptions/dxerr.h"

#include <d3dcompiler.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;

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
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	GFX_THROW_FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer));
	
	//create render target view
	GFX_THROW_FAILED(m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget));
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
	m_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
}

void Graphics::DrawTestTriangle()
{
	HRESULT hr;
	struct Vertex
	{
		float x, y;
	};
	
	//create vertex buffer (1 2d triangle at center of screen)
	const Vertex vertices[] =
	{
		{  0.0f,  0.5f },
		{  0.5f, -0.5f },
		{ -0.5f, -0.5f }
	};

	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;

	D3D11_BUFFER_DESC bd{};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	GFX_THROW_FAILED(m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer));

	//bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;

	m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//create pixel shader
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	wrl::ComPtr<ID3DBlob> pBlob;
	GFX_THROW_FAILED(D3DReadFileToBlob(L"../bin/Debug-Win32/PixelShader.cso", &pBlob));
	GFX_THROW_FAILED(m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));

	// bind pixel shader
	m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	// create vertex shader
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	GFX_THROW_FAILED(D3DReadFileToBlob(L"../bin/Debug-Win32/VertexShader.cso", &pBlob));
	GFX_THROW_FAILED(m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));

	// bind vertex shader
	m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//input (vertex) layout (2d position only)
	wrl::ComPtr<ID3D11InputLayout> pInputLayout;
	// every element in Vertex is goint to have one element in descriptor array (x and y in Vertex are summarized into one element)
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	m_pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	
	//bind vertex layout
	m_pContext->IASetInputLayout(pInputLayout.Get());

	//bind render target
	m_pContext->OMSetRenderTargets(1, m_pTarget.GetAddressOf(), nullptr);

	//set primitive topology to triangle list (groups of 3 vertices)
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//configure viewport
	D3D11_VIEWPORT vp{};
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1u, &vp);


	m_pContext->Draw((UINT)std::size(vertices), 0u);
}




