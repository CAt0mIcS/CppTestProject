#include "pch.h"
#include <Windows.h>
#include <stdio.h>


// https://stackoverflow.com/questions/29091028/windows-api-write-to-screen-as-on-screen-display
// https://www.daniweb.com/programming/software-development/threads/213358/intercept-windows-mouse-events

#include <Windows.h>
#include <iostream>

LRESULT CALLBACK LaunchListener(int nCode, WPARAM wParam, LPARAM lParam);
static RECT g_Rc{ 5, 5, 1000, 500 };
HMODULE g_hInstance;
HHOOK g_Hook;


// ---------------------------------------------------------
// WINDOW SHOULD RESIZE TO RECT
bool MouseIsInRect()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	if (PtInRect(&g_Rc, mousePos))
	{
		return true;
	}

	return false;
}

// ---------------------------------------------------------
// RESIZING LOGIC
void ResizeMovingWindow(HWND hWnd)
{
	int32_t newWidth = g_Rc.right - g_Rc.left;
	int32_t newHeight = g_Rc.bottom - g_Rc.top;

	SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(newWidth, newHeight));
}

// ---------------------------------------------------------
// DRAWING LOGIC
void DrawRectangle(int r, int g, int b)
{
	HDC hDC = GetDC(NULL);
	FrameRect(hDC, &g_Rc, CreateSolidBrush(RGB(r, g, b)));
	ReleaseDC(NULL, hDC);
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	g_hInstance = hModule;

	// Very restricted set of things that can be done in DllMain, refer to documentation
	// before adding anything here.

	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		g_Hook = SetWindowsHookEx(WH_CALLWNDPROC, LaunchListener, g_hInstance, 0);
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		UnhookWindowsHookEx(g_Hook);
		break;
	}
	return TRUE;
}

#ifdef __cplusplus
extern "C" 
{
#endif
	__declspec(dllexport) void AttachHook(DWORD threadID) 
	{
		g_Hook = SetWindowsHookEx(WH_CALLWNDPROC, LaunchListener, g_hInstance, threadID);
	}

	__declspec(dllexport) void DetachHook()
	{
		UnhookWindowsHookEx(g_Hook);
	}
#ifdef __cplusplus
}
#endif

bool g_MouseUp = true;
bool g_CurrentlyMoving = false;
LRESULT CALLBACK LaunchListener(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) 
	{
		CWPSTRUCT* pCwp = (CWPSTRUCT*)lParam;
		switch (pCwp->message)
		{
		case WM_MOVE:
		{
			g_CurrentlyMoving = true;
			std::cout << "Window moved\n";

			if (MouseIsInRect() && g_MouseUp)
			{
				ResizeMovingWindow(pCwp->hwnd);
				DrawRectangle(255, 0, 0);
			}
			else
			{
				DrawRectangle(0, 255, 0);
			}
			break;
		}
		case WM_LBUTTONUP:
		{
			std::cout << "LMB Up\n";
			if (g_CurrentlyMoving)
			{
				g_MouseUp = true;
				g_CurrentlyMoving = false;
			}
			break;
		}
		case WM_LBUTTONDOWN:
		{
			std::cout << "LMB Down\n";
			g_MouseUp = false;
			g_CurrentlyMoving = true;
			break;
		}
		case WM_SIZE:
		{
			std::cout << "Window resized\n";
			break;
		}
		}

	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}