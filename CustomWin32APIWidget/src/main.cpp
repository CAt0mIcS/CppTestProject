#include "pch.h"

#define IDC_OWNERDRAWBUTTON 1
#define CUSTOM_CONTROL_ID 100

HWND g_hWndLabel;
LRESULT CALLBACK ButtonSubProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


struct PrevWindowData
{
	int x, y, width, height;
};

struct PrevCustomControlData
{
	int x, y, width, height;
};

PrevWindowData prevWinData{ CW_USEDEFAULT, CW_USEDEFAULT, 400, 400 };
PrevCustomControlData prevCustomCtrlData{ 10, 10, 200, 200 };


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR nCmdLine, int nCmdShow)
{

	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);
	HWND hWnd = CreateWindowEx(0, CLASS_NAME, L"MainWindow", WS_OVERLAPPEDWINDOW, prevWinData.x, prevWinData.y, prevWinData.width, prevWinData.height, NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
		return 0;

	//g_hWndLabel = CreateWindowEx(0, L"STATIC", L"Some static text", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 25, 125, 150, 20, hWnd, 0, 0, 0);
	//HWND hWndButton = CreateWindowEx(0, L"BUTTON", L"Button Text", WS_CHILD | WS_VISIBLE, 200, 200, 150, 20, hWnd, (HMENU)IDC_OWNERDRAWBUTTON, 0, 0);
	//ShowWindow(g_hWndLabel, nCmdShow);
	//ShowWindow(hWndButton, nCmdShow);

	// Custom Control class
	const wchar_t CUSTOM_CLASS_NAME[] = L"CUSTOMSQUARECLASS";

	WNDCLASS sqWc = { 0 };
	sqWc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	sqWc.lpfnWndProc = CtrlProc;
	sqWc.hInstance = hInstance;
	sqWc.lpszClassName = CUSTOM_CLASS_NAME;
	RegisterClass(&sqWc);

	HWND hWndCustom = CreateWindow(CUSTOM_CLASS_NAME, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, prevCustomCtrlData.x, prevCustomCtrlData.y, prevCustomCtrlData.width, prevCustomCtrlData.height, hWnd, (HMENU)CUSTOM_CONTROL_ID, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	ShowWindow(hWndCustom, nCmdShow);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (IsDialogMessage(hWnd, &msg))
			continue;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;

}

LRESULT CALLBACK ButtonSubProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		// Missing documentation for WM_PAINT! (Microsoft)
	}
	break;
	default:
		return DefSubclassProc(hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

LRESULT CALLBACK CtrlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		//MessageBox(0, L"Hello", L"Hello2", 0);
		break;
	case WM_SIZE:
	{
		if (wParam % 10 == 0 && wParam != 0 && wParam != 1 && wParam != 2 && wParam != 3 && wParam != 4)
		{
			UINT winWidth = LOWORD(lParam);
			UINT winHeight = HIWORD(lParam);

			UINT customWidth = winWidth - prevWinData.width + prevCustomCtrlData.width;
			UINT customHeight = winHeight - prevWinData.height + prevCustomCtrlData.height;

			MoveWindow(hWnd, 10, 10, customWidth, customHeight, TRUE);

			prevWinData.width = winWidth;
			prevWinData.height = winHeight;

			prevCustomCtrlData.width = customWidth;
			prevCustomCtrlData.height = customHeight;
		}
	}
	break;
	case WM_PAINT:
	{
		//PAINTSTRUCT ps;
		//HDC hdc;
		//RECT rect;

		//GetClientRect(hWnd, &rect);

		//hdc = BeginPaint(hWnd, &ps);
		//SetTextColor(hdc, RGB(0, 0, 0));
		//SetBkMode(hdc, TRANSPARENT);
		//DrawText(hdc, _T("Hello World!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		//EndPaint(hWnd, &ps);

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

int msgSendCount = 1;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
	{
		HWND hwndCustom = GetDlgItem(hWnd, CUSTOM_CONTROL_ID);
		if (!hwndCustom)
			break;

		SendMessage(hwndCustom, WM_SIZE, msgSendCount, lParam);
		++msgSendCount;
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, CreateSolidBrush(RGB(35, 38, 40)));
		EndPaint(hWnd, &ps);
	}
	break;
	//case WM_INITDIALOG:
	//{
	//	HWND button = GetDlgItem(hWnd, IDC_OWNERDRAWBUTTON);
	//	SetWindowSubclass(button, ButtonSubProc, 0, 0);
	//}
	//break;
	//case WM_DRAWITEM:
	//{
	//	LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
	//	if (!lParam)
	//		break;
	//
	//	if (pDIS->hwndItem == g_hWndLabel)
	//	{
	//		SetTextColor(pDIS->hDC, RGB(100, 0, 100));
	//		WCHAR staticText[99];
	//		int len = SendMessage(g_hWndLabel, WM_GETTEXT, ARRAYSIZE(staticText), (LPARAM)staticText);
	//		TextOut(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, staticText, len);
	//	}
	//}
	//break;
	case WM_COMMAND:
	{
	}
	break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}
