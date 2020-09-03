#include "pch.h"
#include "MainWindow.h"


namespace NPE
{
	MainWindow::MainWindow(unsigned short width, unsigned short height, PCWSTR name)
	{
		
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = HandleMessageSetup;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = NPE_MAINWINDOW_CLASS_NAME;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		RegisterClass(&wc);

		m_hWnd = CreateWindow(NPE_MAINWINDOW_CLASS_NAME, L"WIN", WS_OVERLAPPEDWINDOW, 10, 10, 100, 100, NULL, NULL, NULL, this);

		SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
		
		ShowWindow(m_hWnd, SW_MAXIMIZE);
		Renderer2D.Init(m_hWnd);
		m_Buttons.emplace_back(Renderer2D, 10, 10, 100, 100, std::wstring(L"Ok"));
	}

	LRESULT MainWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{

		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;			
			HDC hDC = BeginPaint(hWnd, &ps);
			Paint(hDC, &ps.rcPaint, FALSE);
			EndPaint(hWnd, &ps);
			ReleaseDC(hWnd, hDC);
			
			m_Buttons[0].Paint();

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			if (m_Buttons[0].IsMouseOnButton(LOWORD(lParam), HIWORD(lParam)))
				m_Buttons[0].OnClick();

			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void MainWindow::Paint(HDC hDC, RECT* rcDirty, BOOL bErase)
	{
		FillRect(hDC, rcDirty, CreateSolidBrush(RGB(35, 38, 40)));
	}

	LRESULT MainWindow::HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			MainWindow* window = (MainWindow*)pCreate->lpCreateParams;

			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)window);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)HandleMessagePass);
			return window->HandleMessage(hWnd, uMsg, wParam, lParam);
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	LRESULT MainWindow::HandleMessagePass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MainWindow* window = (MainWindow*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
				return window->HandleMessage(hWnd, uMsg, wParam, lParam);
	}

}