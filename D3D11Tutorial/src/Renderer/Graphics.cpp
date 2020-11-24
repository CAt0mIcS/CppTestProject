#include "pch.h"
#include "Graphics.h"

namespace WRL = Microsoft::WRL;
namespace DX = DirectX;

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
	WRL::ComPtr<ID3D11Resource> pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);

	m_pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pTarget);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Z-Buffer ///////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	m_pDevice->CreateDepthStencilState(&dsDesc, &pDSState);
	m_pContext->OMSetDepthStencilState(pDSState.Get(), 1u);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Depth Stencil Texture //////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = 1280u;
	descDepth.Height = 720u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	m_pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Create view of depth stencil texture ///////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	m_pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_pDSV);

	// Bind depth stencil view
	m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), m_pDSV.Get());
}

void Graphics::EndFrame()
{
	m_pSwapChain->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[]{ red, green, blue };
	m_pContext->ClearRenderTargetView(m_pTarget.Get(), color);
	m_pContext->ClearDepthStencilView(m_pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTestTriangle(float angle, float x, float z)
{
	struct Vertex
	{
		struct
		{
			float x, y, z;
		} pos;
	};

	const Vertex vertices[]
	{
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
							  
		{ -1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f }
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Vertex Buffer //////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(vertices);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertices;

	m_pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	UINT strides = sizeof(Vertex);
	UINT offset = 0;
	m_pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &strides, &offset);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Index Buffer ///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const unsigned short indices[]
	{
		0, 2, 1, 2, 3, 1,
		1, 3, 5, 3, 7, 5,
		2, 6, 3, 3, 6, 7,
		4, 5, 7, 4, 7, 6,
		0, 4, 2, 2, 4, 6,
		0, 1, 4, 1, 5, 4
	};

	WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	bd.ByteWidth = sizeof(indices);
	bd.StructureByteStride = sizeof(unsigned short);
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	sd.pSysMem = indices;
	m_pDevice->CreateBuffer(&bd, &sd, &pIndexBuffer);
	m_pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Translation Constant Buffer ////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct ConstantBuffer
	{
		DX::XMMATRIX translation;
	};
	const ConstantBuffer cb =
	{
		DX::XMMatrixTranspose(
			DX::XMMatrixRotationZ(angle) *
			DX::XMMatrixRotationX(angle) *
			DX::XMMatrixTranslation(x, 0.0f, z + 4.0f) * 
			DX::XMMatrixPerspectiveLH(1.0f, 16.0f / 9.0f, 0.5f, 10.0f)
		)
	};

	WRL::ComPtr<ID3D11Buffer> pConstBuffer;
	bd.StructureByteStride = 0u;
	bd.ByteWidth = sizeof(cb);
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0u;

	sd.pSysMem = &cb;
	m_pDevice->CreateBuffer(&bd, &sd, &pConstBuffer);
	m_pContext->VSSetConstantBuffers(0u, 1u, pConstBuffer.GetAddressOf());


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Color Aray Constant Buffer /////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct ConstantBuffer2
	{
		struct
		{
			float r, g, b, a;
		} faceColors[6];
	};
	const ConstantBuffer2 cb2
	{
		{
			{ 1.0f, 0.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			{ 1.0f, 1.0f, 0.0f },
			{ 0.0f, 1.0f, 1.0f }
		}
	};
	WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2{};
	csd2.pSysMem = &cb2;
	m_pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);
	m_pContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Pixel Shader ///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"Shaders/PixelShader.cso", &pBlob);

	m_pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	m_pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Vertex Shader //////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"Shaders/VertexShader.cso", &pBlob);
	m_pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	m_pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Input Layout ///////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_pDevice->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);
	m_pContext->IASetInputLayout(pInputLayout.Get());

	
	// !!! Already bound using Z-Buffer int Constructor
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Render Target //////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//m_pContext->OMSetRenderTargets(1u, m_pTarget.GetAddressOf(), nullptr);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Primitive Topology /////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Viewport ///////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3D11_VIEWPORT vp;
	vp.Width = 1280;
	vp.Height = 720;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pContext->RSSetViewports(1u, &vp);


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////// Drawcall ///////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	m_pContext->DrawIndexed((UINT)std::size(indices), 0u, 0);
}

