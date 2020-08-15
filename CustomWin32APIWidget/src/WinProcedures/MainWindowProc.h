#pragma once

#include "pch.h"

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(35, 38, 40)));
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_NOTIFY:
	{
		wchar_t buff[256];
		wsprintfW(buff, L"%d", wParam);
		MessageBox(NULL, buff, buff, NULL);
	}
	break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}
