#include "pch.h"

#include "NPE/Window/MainWindow.h"
using namespace NPE;


void OnFrame(MainWindow& win);

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	MainWindow win(600, 600, L"HA");
	return MainWindow::ProcessMessage([&]() { OnFrame(win); });
}


void DrawCaret(MainWindow& win, float x, float y, float width, float height, const wchar_t* str)
{
    win.Renderer2D.pRenderTarget->BeginDraw();
    IDWriteTextLayout* pTextLayout;
    win.Renderer2D.pWriteFactory->CreateTextLayout(str, wcslen(str), win.Renderer2D.pTextFormat.Get(), width, height, &pTextLayout);

    DWRITE_HIT_TEST_METRICS hitTestMetrics;
    float caretX, caretY;
    bool isTrailingHit = false; // Use the leading character edge for simplicity here.

    // Map text position index to caret coordinate and hit-test rectangle.
    pTextLayout->HitTestTextPosition(
        x,
        isTrailingHit,
        OUT & caretX,
        OUT & caretY,
        OUT & hitTestMetrics
    );

    // Respect user settings.
    DWORD caretWidth = 1;
    SystemParametersInfo(SPI_GETCARETWIDTH, 0, OUT & caretWidth, 0);
    DWORD halfCaretWidth = caretWidth / 2u;

    // Draw a thin rectangle.
    D2D1_RECT_F caretRect = {
        x + caretX - halfCaretWidth,
        y + hitTestMetrics.top,
        x + caretX + (caretWidth - halfCaretWidth),
        y + hitTestMetrics.top + hitTestMetrics.height
    };
    win.Renderer2D.pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::AliceBlue));
    win.Renderer2D.pRenderTarget->FillRectangle(&caretRect, win.Renderer2D.pBrush.Get());
    win.Renderer2D.pRenderTarget->EndDraw();
}


void OnFrame(MainWindow& win)
{
    float x = 250, y = 250, width = 100, height = 100;
    const wchar_t* str = L"Text";

	win.Renderer2D.DrawString(x, y, width, height, 50, str);
    DrawCaret(win, x, y, width, height, str);
}

