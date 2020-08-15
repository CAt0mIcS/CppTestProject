#pragma once

#include "pch.h"

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	//case WM_NCPAINT:
	//{
	//	
	//}
	case WM_ERASEBKGND:
		/**
		* Removing WM_ERASEBKGND from being handled by DefWindowProc to avoid The Flickering Problem
		*/
		break;
	break;
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
