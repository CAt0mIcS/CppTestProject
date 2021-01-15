#include <Windows.h>
#include <iostream>
#include <chrono>
#include <sstream>

#include "../../BrokenKeyboardFixHook/src/KeyCodes.h"


void(*HookAttachFn)();
void(*HookDetachFn)();
uint32_t(*GetWCounter)();
char* (*DebugInfo)();
void(*SetUpperThreshold)(uint64_t);
char** (*GetApplications)(uint32_t* numaps);

#define CWM_SETUPPERTHRESHOLD WM_USER + 1
#define CWM_ACTIVATE WM_USER + 2
char* prevDebugInfo{ 0 };

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifndef NDEBUG
	if (DebugInfo() != prevDebugInfo)
	{
		prevDebugInfo = DebugInfo();
		std::cout << prevDebugInfo << '\n';
	}
#endif

	switch (uMsg)
	{
	case WM_DESTROY:
	{
		HookDetachFn();
		PostQuitMessage(0);
		return 0;
	}
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
		{
			long style = GetWindowLong(hWnd, GWL_STYLE);
			style &= ~(WS_VISIBLE);    // this works - window become invisible 

			style |= WS_EX_TOOLWINDOW;   // flags don't work - windows remains in taskbar
			style &= ~(WS_EX_APPWINDOW);

			ShowWindow(hWnd, SW_HIDE); // hide the window
			SetWindowLong(hWnd, GWL_STYLE, style); // set the style
			ShowWindow(hWnd, SW_SHOW); // show the window for the new style to come into effect
			ShowWindow(hWnd, SW_HIDE); // hide the window so we can't see it
			return 0;
		}
		break;
	}
	case WM_COPYDATA:
	{
		//COPYDATASTRUCT* copyData = (COPYDATASTRUCT*)lParam;

		//std::cout << (uint32_t)copyData->lpData << '\n';
		//SetUpperThreshold((uint32_t)copyData->lpData);
		//return 32;
		break;
	}
	case WM_CLOSE:
	{
		switch (wParam)
		{
		case CWM_ACTIVATE:
		{
			std::cout << "CWM_ACTIVATE: " << (uint32_t)lParam << "\n";
			if (lParam == 0)
			{
				HookDetachFn();
				std::cout << "Detaching Hook\n";
			}
			else
			{
				HookAttachFn();
				std::cout << "Attaching Hook\n";
			}

			return 0;
		}
		case CWM_SETUPPERTHRESHOLD:
		{
			std::cout << "CWM_SETUPPERTHRESHOLD: " << (uint32_t)lParam << "\n";
			SetUpperThreshold((uint32_t)lParam);
			return 0;
		}
		}
		break;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#ifdef _WIN64
#define WINTOKEN "x64"
#else
#define WINTOKEN "Win32"
#endif


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
#ifndef NDEBUG
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif

	HMODULE hLib = LoadLibrary(L"BrokenKeyboardFixHook-" WINTOKEN ".dll");
	HookAttachFn = (void(*)())GetProcAddress(hLib, "AttachHook");
	HookDetachFn = (void(*)())GetProcAddress(hLib, "DetachHook");
	GetWCounter = (uint32_t(*)())GetProcAddress(hLib, "GetWCounter");
	DebugInfo = (char* (*)())GetProcAddress(hLib, "DebugInfo");
	SetUpperThreshold = (void(*)(uint64_t))GetProcAddress(hLib, "SetUpperThreshold");
	GetApplications = (char** (*)(uint32_t*))GetProcAddress(hLib, "GetApplications");

	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"FixBrokenKeyboardUsingHookAdjustmentWindow(" WINTOKEN ")";

	RegisterClass(&wc);
	HWND hWnd = CreateWindow(L"FixBrokenKeyboardUsingHookAdjustmentWindow(" WINTOKEN ")", L"Broken Keyboard Fix Hook (" WINTOKEN ")", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 350, 120, HWND_MESSAGE, 0, hInstance, 0);

	HookAttachFn();

	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}