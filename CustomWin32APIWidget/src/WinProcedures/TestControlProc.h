#pragma once

#include "pch.h"

#define TEST_CTRL_CLASS_NAME		L"TESTCONTROLCLASS"
#define ID_TEST_CTRL				0x1

#define XXM_TESTMESSAGE				(WM_USER + 1)

#define XXS_BEMOREFANCY             0x0001
#define XXS_HIGHESTCUSTOMSTYLE      0x8000

#define XXN_NOTIFICATIONCODE		0x1


struct TestControlData
{
	const char* text;
	unsigned int pressCount;
};

void CustomPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


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
	/**
	* Should be implemented because some snippet tools might use it
	*/
	//case WM_PRINTCLIENT:
	//{
	//	RECT rect;
	//	GetClientRect(hWnd, &rect);
	//	CustomPaint(hWnd, (UINT)wParam, *(WPARAM*)&rect, TRUE);
	//}
	break;
	case XXM_TESTMESSAGE:
	{
		MessageBox(NULL, L"Testmessage received", L"", NULL);
	}
	break;
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
		CustomPaint(hWnd, uMsg, wParam, lParam);
	}
	break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}


void CustomPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	//RECT rect;
	//GetClientRect(hWnd, &rect);

	HDC hdc = BeginPaint(hWnd, &ps);
	//FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(255, 255, 255)));
	//SetTextColor(hdc, RGB(0, 0, 0));
	//SetBkMode(hdc, TRANSPARENT);
	//DrawText(hdc, _T("Hello World!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	#pragma region RoundButton

	/**
	* These values work with a window size of 100 by 50
	*/

	//auto left = ps.rcPaint.left;
	//auto top = ps.rcPaint.top;
	//auto bottom = ps.rcPaint.bottom + 50;	/* - Moves it away from the right side */
	//auto right = ps.rcPaint.right - 50;		/* - Moves it away from the bottom */
	//auto width = right - left;
	//auto height = bottom - top;
	//BOOL ret = RoundRect(hdc, left, top, bottom, right, width, height);
	//if (!ret)
	//	MessageBox(NULL, L"NOPAINT", L"NOPAINT", NULL);

	#pragma endregion

	EndPaint(hWnd, &ps);

}

