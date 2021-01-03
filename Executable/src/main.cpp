#include "pch.h"

#pragma comment(lib, "Gdiplus.lib")

using namespace Gdiplus;
Gdiplus::Graphics* gfx;



LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static bool cursorShown = true;
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam == 'C')
		{
			if (cursorShown)
			{
				ShowCursor(FALSE);
			}
			else
			{
				ShowCursor(TRUE);
			}

			cursorShown = !cursorShown;
		}

		return 0;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


bool EndsWith(const std::wstring& str, const std::wstring& end)
{
	auto endSub = str.substr(str.size() - end.size(), str.size());

	return endSub == end;
}


int main()
{
	WNDCLASS wc{};
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"WINDOWCLASSNAME";
	RegisterClass(&wc);

	HWND hWnd = CreateWindowExW(0, L"WINDOWCLASSNAME", L"Picture Slideshow", 0, 0, 0, 0, 0, 0, 0, GetModuleHandle(NULL), 0);
	SetWindowLong(hWnd, GWL_STYLE, 0);
	ShowWindow(hWnd, SW_MAXIMIZE);


	GdiplusStartupInput startupInput;
	ULONG_PTR token;
	GdiplusStartup(&token, &startupInput, nullptr);

	RECT rc;
	GetClientRect(hWnd, &rc);
	Rect rect;
	rect.X = rc.left;
	rect.Y = rc.top;
	rect.Width = rc.right - rc.left;
	rect.Height = rc.bottom - rc.top;

	gfx = Gdiplus::Graphics::FromHWND(hWnd);

	MSG msg{};
	std::wstring filepath;
	std::wstring path = L"";
	auto dirEntry = std::filesystem::recursive_directory_iterator(path);
	auto tLast = std::chrono::high_resolution_clock::now();
	auto tNow = std::chrono::high_resolution_clock::now();

	while (true)
	{
		tNow = std::chrono::high_resolution_clock::now();

		if (std::chrono::duration_cast<std::chrono::seconds>(tNow - tLast).count() >= 5)
		{
			while (!EndsWith((*dirEntry).path(), L".JPG"))
			{
				++dirEntry;
			}
			filepath = (*dirEntry).path();
			++dirEntry;

			Image* image = new Image(filepath.c_str());
			gfx->DrawImage(image, rect);
			delete image;

			tLast = tNow;
		}


		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}