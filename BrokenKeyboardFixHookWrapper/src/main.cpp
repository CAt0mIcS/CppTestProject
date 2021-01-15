#include <Windows.h>
#include <iostream>
#include <chrono>
#include <sstream>

#include "../../BrokenKeyboardFixHook/src/KeyCodes.h"



#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>

#include <CommCtrl.h>
#pragma comment(lib, "ComCtl32.lib")

#define ID_TRACKBAR 1
#define ID_CHECKBOX 2
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


PROCESS_INFORMATION pi;
void StartProcess(const wchar_t* path)
{
	STARTUPINFO si;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CreateProcess(
		path,
		nullptr,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);

	//CloseHandle(pi.hProcess);
	//CloseHandle(pi.hThread);
}


uint32_t upperThreshold = 150;
HWND hWndWin32;
HWND hWndx64;
const UINT SetUpperThreshldMessageId = RegisterWindowMessage(L"SetUpperThreshold");

#define CWM_SETUPPERTHRESHOLD WM_USER + 1

#define CWM_ACTIVATE WM_USER + 2


LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostMessage(hWndWin32, WM_CLOSE, 0, -1);
		//PostMessage(hWndx64, WM_CLOSE, 0, -1);

		PostQuitMessage(0);
		return 0;
	}
	case WM_HSCROLL:
	{
		if (LOWORD(wParam) == SB_THUMBPOSITION || LOWORD(wParam) == SB_THUMBTRACK)
		{
			upperThreshold = HIWORD(wParam);

			// repurposing WM_CLOSE 'cause it works best
			PostMessage(hWndWin32, WM_CLOSE, CWM_SETUPPERTHRESHOLD, upperThreshold);
			//PostMessage(hWndx64, WM_CLOSE, CWM_SETUPPERTHRESHOLD, upperThreshold);

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

		DrawTextA(hDC, std::to_string(upperThreshold).c_str(), -1, &rc, DT_BOTTOM | DT_SINGLELINE | DT_RIGHT);

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
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_CHECKBOX:
		{
			switch (HIWORD(wParam))
			{
			case BN_CLICKED:
				if (SendDlgItemMessage(hWnd, ID_CHECKBOX, BM_GETCHECK, 0, 0))
				{
					CheckDlgButton(hWnd, ID_CHECKBOX, BST_UNCHECKED);
					SendMessage(hWndWin32, WM_CLOSE, CWM_ACTIVATE, false);
					//SendMessage(hWndx64, WM_CLOSE, CWM_ACTIVATE, false);
				}
				else
				{
					CheckDlgButton(hWnd, ID_CHECKBOX, BST_CHECKED);
					SendMessage(hWndWin32, WM_CLOSE, CWM_ACTIVATE, true);
					//SendMessage(hWndx64, WM_CLOSE, CWM_ACTIVATE, true);
				}
				break;
			}
		}
		break;
		}
		break;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"FixBrokenKeyboardUsingHookAdjustmentWindow";

	RegisterClass(&wc);
	HWND hWnd = CreateWindow(L"FixBrokenKeyboardUsingHookAdjustmentWindow", L"Broken Keyboard Fix Hook", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, 350, 120, 0, 0, hInstance, 0);

	HWND trackbar = CreateTrackbar(hWnd, 0, 250, 0, 200);
	SendMessage(trackbar, TBM_SETPOS, TRUE, upperThreshold);

	CreateWindow(TEXT("button"), TEXT("Active"),
		WS_VISIBLE | WS_CHILD | BS_CHECKBOX,
		0, 50, 185, 35,
		hWnd, (HMENU)ID_CHECKBOX, hInstance, NULL);
	CheckDlgButton(hWnd, ID_CHECKBOX, BST_CHECKED);

	NOTIFYICONDATA dt{};
	dt.cbSize = sizeof(dt);
	dt.hWnd = hWnd;
	dt.uFlags = NIF_MESSAGE | NIF_TIP | NIF_ICON;
	dt.uCallbackMessage = SHELLICON_CALLBACK_ID;
	dt.hIcon = LoadIcon(NULL, IDI_SHIELD);
	wcscpy(dt.szTip, L"Broken Keyboard Fix Hook");
	dt.uID = 0x3f;

	Shell_NotifyIcon(NIM_ADD, &dt);

	StartProcess(L"BrokenKeyboardFixHookExec-Win32.exe");
	//StartProcess(L"BrokenKeyboardFixHookExec-x64.exe");

	while (!hWndWin32)
	{
		hWndWin32 = FindWindow(NULL, L"Broken Keyboard Fix Hook (Win32)");
		Sleep(10);
	}

	//while (!hWndx64)
	//{
	//	hWndx64 = FindWindow(NULL, L"Broken Keyboard Fix Hook (x64)");
	//	Sleep(10);
	//}

	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}