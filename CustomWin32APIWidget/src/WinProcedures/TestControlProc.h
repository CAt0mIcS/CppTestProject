#pragma once

#include "pch.h"

#define TEST_CTRL_CLASS_NAME L"TESTCONTROLCLASS"
#define ID_TEST_CTRL 0x1

#define XXM_TESTMESSAGE				(WM_USER + 1)

#define XXS_BEMOREFANCY             0x0001
#define XXS_HIGHESTCUSTOMSTYLE      0x8000

#define XXN_NOTIFICATIONCODE                   0x1


struct TestControlData
{
	const char* text;
	unsigned int pressCount;
};


LRESULT CALLBACK TestControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	TestControlData* pData = (TestControlData*)GetWindowLongPtr(hWnd, 0);

	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		pData = (TestControlData*)malloc(sizeof(TestControlData));
		if (!pData)
			return FALSE;

		pData->text = "Hello World";
		pData->pressCount = 0;

		SetWindowLongPtr(hWnd, 0, (LONG_PTR)pData);
	}
	break;
	case XXM_TESTMESSAGE:
	{
		MessageBox(NULL, L"Testmessage received", L"", NULL);
	}
	case WM_NCDESTROY:
	{
		if (!pData)
			break;
		free(pData);
	}
	break;
	case WM_LBUTTONDOWN:
	{

		//if(click on specific element (location))
		//	SendMessage(hWnd, CWM_TESTMESSAGE, 0, 0);

		//++pData->pressCount;
		//wchar_t buff[156];
		//wsprintfW(buff, L"%d", pData->pressCount);
		//MessageBox(NULL, buff, buff, NULL);

		NMHDR hdr;
		hdr.hwndFrom = hWnd;
		hdr.idFrom = GetWindowLongPtr(hWnd, GWL_ID);
		hdr.code = XXN_NOTIFICATIONCODE;
		SendMessage(GetParent(hWnd), WM_NOTIFY, hdr.idFrom, (LPARAM)&hdr);

	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(255, 255, 255)));
		EndPaint(hWnd, &ps);
	}
	break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

