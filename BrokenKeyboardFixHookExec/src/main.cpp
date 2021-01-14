#include <Windows.h>
#include <iostream>
#include <chrono>

#include "../../BrokenKeyboardFixHook/src/KeyCodes.h"


void(*HookAttachFn)();
void(*HookDetachFn)();

Key prevKey = (Key)0;
uint64_t msOfLastPress = 0;
int wWritten = 0;
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_KEYDOWN:
	{
		//int scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
		//bool pressed = (HIWORD(lParam) & KF_UP) ? false : true;
		//Key key = GetKeycodeMap()[scancode];
		//uint64_t now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		//static constexpr uint32_t threshold = 89285500;  //ns

		//std::cout << now - nsOfLastPress << '\n';
		//if (pressed && key == prevKey && now - nsOfLastPress < threshold)
		//{
		//	return 0;
		//}
		//else if (now - nsOfLastPress > threshold)
		//{
		//	nsOfLastPress = now;
		//}

		//prevKey = key;
		//std::cout << KeyToString(key) << '\n';
		++wWritten;

		HDC hDC = GetDC(hWnd);
		RECT rc;
		GetClientRect(hWnd, &rc);
		SetTextColor(hDC, 0x00000000);
		DrawTextA(hDC, std::to_string(wWritten).c_str(), -1, &rc, DT_CENTER);
		ReleaseDC(hWnd, hDC);

		break;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//int main()
//{
//	HMODULE hLib = LoadLibrary(L"BrokenKeyboardFixHook.dll");
//	HookAttachFn = (void(*)())GetProcAddress(hLib, "AttachHook");
//	HookDetachFn = (void(*)())GetProcAddress(hLib, "DetachHook");
//
//
//	WNDCLASS wc{};
//	wc.lpfnWndProc = WndProc;
//	wc.hInstance = GetModuleHandle(NULL);
//	wc.lpszClassName = L"CLASSNAME";
//
//	RegisterClass(&wc);
//	HWND hWnd = CreateWindow(L"CLASSNAME", L"W", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, 0);
//	ShowWindow(hWnd, SW_SHOWDEFAULT);
//
//	HookAttachFn();
//
//	MSG msg{};
//	while (GetMessage(&msg, 0, 0, 0))
//	{
//		TranslateMessage(&msg);
//		DispatchMessageW(&msg);
//	}
//
//	HookDetachFn();
//}


#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>

#include <CommCtrl.h>
#pragma comment(lib, "ComCtl32.lib")

#define ID_TRACKBAR 1
#define SHELLICON_CALLBACK_ID WM_USER + 0x33

HWND WINAPI CreateTrackbar(
	HWND hwndDlg,  // handle of dialog box (parent window) 
	UINT iMin,     // minimum value in trackbar range 
	UINT iMax,     // maximum value in trackbar range 
	UINT iSelMin,  // minimum value in trackbar selection 
	UINT iSelMax)  // maximum value in trackbar selection 
{

	InitCommonControls(); // loads common control's DLL 

	HWND hwndTrack = CreateWindowEx(
		0,                               // no extended styles 
		TRACKBAR_CLASS,                  // class name 
		L"Change upper threshold",              // title (caption) 
		WS_CHILD |
		WS_VISIBLE |
		TBS_AUTOTICKS |
		TBS_HORZ,
		10, 10,                          // position 
		200, 30,                         // size 
		hwndDlg,                         // parent window 
		(HMENU)ID_TRACKBAR,                     // control identifier 
		GetModuleHandle(NULL),                         // instance 
		NULL                             // no WM_CREATE parameter 
	);

	SendMessage(hwndTrack, TBM_SETRANGE,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)MAKELONG(iMin, iMax));  // min. & max. positions

	SendMessage(hwndTrack, TBM_SETPAGESIZE,
		0, (LPARAM)4);                  // new page size 

	SendMessage(hwndTrack, TBM_SETPOS,
		(WPARAM)TRUE,                   // redraw flag 
		(LPARAM)iMin);

	SetFocus(hwndTrack);

	return hwndTrack;
}






HHOOK hHook{ NULL };
uint32_t upperThreshold = 100;
//uint32_t lowerThreshold = 60;

LRESULT CALLBACK KeyboardHook(int code, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT* pHook = (KBDLLHOOKSTRUCT*)lParam;
		Key key = GetKeycodeMap()[pHook->scanCode];
		uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		// Trying to detect repeated keys
		std::cout << now - msOfLastPress << '\n';
		//if (now - msOfLastPress < lowerThreshold)
		//	return CallNextHookEx(hHook, code, wParam, lParam);

		if (prevKey == key && now - msOfLastPress < upperThreshold)
		{
			//std::cout << "---------- Skipping Key ----------\n";
			return -1;
		}

		if (prevKey == key)
			msOfLastPress = now;
		prevKey = key;
	}

	return CallNextHookEx(hHook, code, wParam, lParam);
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		UnhookWindowsHookEx(hHook);
		PostQuitMessage(0);
		return 0;
	}
	case WM_HSCROLL:
	{
		if (LOWORD(wParam) == SB_THUMBPOSITION || LOWORD(wParam) == SB_THUMBTRACK)
		{
			upperThreshold = HIWORD(wParam);
			InvalidateRect(hWnd, nullptr, TRUE);
			UpdateWindow(hWnd);
		}
		return 0;
	}
	case WM_PAINT:
	{
		RECT rc;
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rc);

		FillRect(hDC, &ps.rcPaint, CreateSolidBrush(RGB(255, 255, 255)));

		DrawTextA(hDC, std::to_string(upperThreshold).c_str(), -1, &rc, DT_BOTTOM | DT_SINGLELINE);

		EndPaint(hWnd, &ps);
		return 0;
	}
	case SHELLICON_CALLBACK_ID:
	{
		if (lParam == WM_LBUTTONDOWN)
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			SetFocus(hWnd);
		}
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
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
	//AllocConsole();
	//freopen("CONOUT$", "w", stdout);

	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardHook, NULL, 0);
	if (!hHook)
	{
		MessageBox(NULL, L"Failed to set hook", L"Error", MB_OK | MB_ICONERROR);
		exit(-1);
	}

	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"MAINWINDOWCLASS";

	RegisterClass(&wc);
	HWND hWnd = CreateWindow(L"MAINWINDOWCLASS", L"Broken Keyboard Fix Hook", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 350, 120, 0, 0, hInstance, 0);

	HWND trackbar = CreateTrackbar(hWnd, 0, 200, 0, 200);
	SendMessage(trackbar, TBM_SETPOS, TRUE, upperThreshold);

	NOTIFYICONDATA dt{};
	dt.cbSize = sizeof(dt);
	dt.hWnd = hWnd;
	dt.uFlags = NIF_MESSAGE | NIF_TIP;
	dt.uCallbackMessage = SHELLICON_CALLBACK_ID;
	wcscpy(dt.szTip, L"Broken Keyboard Fix Hook");
	dt.uID = 0x3f;

	Shell_NotifyIcon(NIM_ADD, &dt);

	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}