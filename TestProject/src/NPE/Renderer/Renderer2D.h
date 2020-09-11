#pragma once

#include "pch.h"
#include <d2d1_1.h>
#include <wincodec.h>

inline float g_DPIScaleX = 1.0f;
inline float g_DPIScaleY = 1.0f;

inline void InitializeDPIScale(ID2D1Factory* pFactory)
{
	FLOAT dpiX = 96.0f, dpiY = 96.0f;

	//pFactory->GetDesktopDpi(&dpiX, &dpiY);

	g_DPIScaleX = dpiX / 96.0f;
	g_DPIScaleY = dpiY / 96.0f;
}

template <typename T>
inline float PixelsToDipsX(T x)
{
	return static_cast<float>(x) / g_DPIScaleX;
}

template <typename T>
inline float PixelsToDipsY(T y)
{
	return static_cast<float>(y) / g_DPIScaleY;
}



class Renderer2D
{
public:
	Renderer2D() = default;
	void Init(HWND hWnd);

	void DrawTestButton(float x, float y, float width, float height);

	void DrawString(const float x, const float y, const float width, const float height, const int fontSize, const wchar_t* str);

	void RenderBackgroundFromImage(const std::wstring& filepath);

private:
	void InitGraphicsResources();


public:
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
	Microsoft::WRL::ComPtr<ID2D1Factory> pFactory;
	Microsoft::WRL::ComPtr<IDWriteFactory> pWriteFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> pTextFormat;
	Microsoft::WRL::ComPtr<ID2D1DeviceContext> pContext;

	IWICImagingFactory* m_pIWICFactory;

	ID2D1Bitmap* m_pD2DBitmap;
	IWICFormatConverter* m_pConvertedSourceBitmap;

	HWND m_hWnd;
};

