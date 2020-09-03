#include "pch.h"
#include "Renderer2D.h"


void Renderer2D::Init(HWND hWnd)
{
    m_hWnd = hWnd;
    InitGraphicsResources();
}

void Renderer2D::DrawTestButton(float x, float y, float width, float height)
{
    pRenderTarget->BeginDraw();

    D2D1_RECT_F rc;
    rc.left = x;
    rc.top = y;
    rc.right = (x + width);
    rc.bottom = (y + height);
    pBrush->SetColor(D2D1::ColorF(15.0f / 255.0f, 17.0f / 255.0f, 19.0f / 255.0f));
    pRenderTarget->FillRectangle(&rc, pBrush.Get());

    if (FAILED(pRenderTarget->EndDraw())) throw std::exception("");
}

void Renderer2D::DrawString(const float x, const float y, const int fontSize, const wchar_t* str)
{
    pRenderTarget->BeginDraw();

    D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
    pRenderTarget->DrawTextW(str, wcslen(str), pTextFormat.Get(), D2D1::RectF(x, y, renderTargetSize.width, renderTargetSize.height), pBrush.Get());
    if (FAILED(pRenderTarget->EndDraw())) throw std::exception();
}

void Renderer2D::InitGraphicsResources()
{
	if(FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pFactory.GetAddressOf()))) throw std::exception("");

    RECT rc;
    GetClientRect(m_hWnd, &rc);
    D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

    if (FAILED(pFactory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(m_hWnd, size),
        &pRenderTarget))) throw std::exception("");

    if(FAILED(DWriteCreateFactory(
        DWRITE_FACTORY_TYPE_SHARED,
        __uuidof(pWriteFactory), &pWriteFactory
    ))) throw std::exception();

    if (FAILED(pWriteFactory->CreateTextFormat(L"Verdana", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 50, L"", &pTextFormat))) throw std::exception("");
    pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    const D2D1_COLOR_F color = D2D1::ColorF(35.0f / 255.0f, 38.0f / 255.0f, 40.0f / 255.0f);
    if (FAILED(pRenderTarget->CreateSolidColorBrush(color, &pBrush))) throw std::exception("");

}
