#include "pch.h"
#include "Renderer2D.h"

#include <wincodec.h>


template <typename T>
inline void SafeRelease(T*& p)
{
    if (nullptr != p)
    {
        p->Release();
        p = nullptr;
    }
}



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

void Renderer2D::DrawString(const float x, const float y, const float width, const float height, const int fontSize, const wchar_t* str)
{
    if (FAILED(pWriteFactory->CreateTextFormat(L"Verdana", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSize, L"", &pTextFormat))) throw std::exception("");
    
    pRenderTarget->BeginDraw();

    pBrush->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f));
    pRenderTarget->DrawTextW(str, wcslen(str), pTextFormat.Get(), D2D1::RectF(x, y, width, height), pBrush.Get()); 

    if (FAILED(pRenderTarget->EndDraw())) throw std::exception();
}

void Renderer2D::RenderBackgroundFromImage(const std::wstring& filepath)
{
    HRESULT hr = S_OK;
    
    // Create WIC factory
    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&m_pIWICFactory)
    );
    
    IWICBitmapDecoder* pDecoder = nullptr;
    
    hr = m_pIWICFactory->CreateDecoderFromFilename(
        filepath.c_str(),                // Image to be decoded
        nullptr,                         // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );
    
    // Retrieve the first frame of the image from the decoder
    IWICBitmapFrameDecode* pFrame = nullptr;
    
    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }
    
    //Step 3: Format convert the frame to 32bppPBGRA
    if (SUCCEEDED(hr))
    {
        SafeRelease(m_pConvertedSourceBitmap);
        hr = m_pIWICFactory->CreateFormatConverter(&m_pConvertedSourceBitmap);
    }
    
    if (SUCCEEDED(hr))
    {
        hr = m_pConvertedSourceBitmap->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            nullptr,                         // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }
    
    if (SUCCEEDED(hr))
    {
        // Need to release the previous D2DBitmap if there is one
        SafeRelease(m_pD2DBitmap);
        hr = pRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, nullptr, &m_pD2DBitmap);
    }
    
    pRenderTarget->BeginDraw();
    
    pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    
    auto rtSize = pRenderTarget->GetSize();
    
    // Create a rectangle with size of current window
    auto rectangle = D2D1::RectF(0.0f, 0.0f, rtSize.width, rtSize.height);
    
    // D2DBitmap may have been released due to device loss. 
    // If so, re-create it from the source bitmap
    if (m_pConvertedSourceBitmap && !m_pD2DBitmap)
    {
        pRenderTarget->CreateBitmapFromWicBitmap(m_pConvertedSourceBitmap, nullptr, &m_pD2DBitmap);
    }
    
    // Draws an image and scales it to the current window size
    if (m_pD2DBitmap)
    {
        pRenderTarget->DrawBitmap(m_pD2DBitmap, rectangle);
    }
    
    hr = pRenderTarget->EndDraw();


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

    //pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    //pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    const D2D1_COLOR_F color = D2D1::ColorF(35.0f / 255.0f, 38.0f / 255.0f, 40.0f / 255.0f);
    if (FAILED(pRenderTarget->CreateSolidColorBrush(color, &pBrush))) throw std::exception("");

}
