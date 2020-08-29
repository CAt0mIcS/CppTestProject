#pragma once

#include "pch.h"
#include "Exceptions/Exception.h"

#include <d3d11.h>


class Graphics
{
public:
	Graphics(HWND hWnd);

	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;

	/**
	* Switches back and front buffer
	*/
	void EndFrame();

	/*
	* Clears the back buffer
	*/
	void ClearBuffer(float red, float green, float blue);

	~Graphics();

private:
	ID3D11Device* m_pDevice;
	IDXGISwapChain* m_pSwapChain;
	ID3D11DeviceContext* m_pContext;
	ID3D11RenderTargetView* m_pTarget;
};

