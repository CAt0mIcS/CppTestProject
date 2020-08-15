#include "pch.h"
#include "Base.h"
#include "WinProcedures/MainWindowProc.h"
#include "WinProcedures/TestControlProc.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR nCmdLine, _In_ int nCmdShow)
{
	CREATE_MAIN_WINDOW();

	WNDCLASS wcCustom{};
	wcCustom.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wcCustom.lpfnWndProc = TestControlProc;
	wcCustom.lpszClassName = TEST_CTRL_CLASS_NAME;
	wcCustom.cbWndExtra = sizeof(TestControlData*);
	RegisterClass(&wcCustom);
	
	HWND hWndCustom = CreateWindow(TEST_CTRL_CLASS_NAME, L"NULL", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 100, 100, hWnd, (HMENU)ID_TEST_CTRL, 0, 0);
	ShowWindow(hWndCustom, nCmdShow);

	ENTER_MAIN_MESSAGE_LOOP();
}

