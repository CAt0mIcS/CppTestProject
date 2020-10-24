#pragma once

#include "pch.h"

#include <wrl.h>


class Graphics
{
public:
	Graphics(HWND hWnd);

	void EndFrame();

	void ClearBuffer(float red, float green, float blue);
	
	void DrawTestTriangle(float angle, float x, float y);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pTarget;
};

