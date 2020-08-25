#include "pch.h"

#include "Errors/TError.h"



#define MAIN_WINDOW_CLASS "MAINWINDOWCLASS" 


HWND hWnd;


LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR nCmdLine, _In_ int nCmdShow)
{
	try
	{

		WNDCLASS wc{ 0 };
		wc.lpfnWndProc = MainWindowProc;
		wc.lpszClassName = MAIN_WINDOW_CLASS;
		wc.hInstance = hInstance;

		RegisterClass(&wc);
		hWnd = CreateWindowEx(0, MAIN_WINDOW_CLASS, "D3D11", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
		
		if (!hWnd)
			throw T_ERROR_LAST();

		ShowWindow(hWnd, nCmdShow);

		MSG msg{};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		return msg.wParam;
	}
	catch (TError& e)
	{
		MessageBox(NULL, e.what(), e.GetType(), MB_OK);
	}
	catch (std::exception& e)
	{
		MessageBox(NULL, e.what(), "Standard Exception", MB_OK);
	}
	catch (...)
	{
		MessageBox(NULL, "An unidentified exception occured", "Error", MB_OK);
	}
	return -1;
}

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
