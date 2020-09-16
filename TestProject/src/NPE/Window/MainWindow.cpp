#include "pch.h"
#include "MainWindow.h"

void CALLBACK OnTimer(HWND hWnd, UINT uMsg, UINT_PTR id, DWORD passedMs)
{
	std::cout << id << '\n';
	KillTimer(hWnd, id);
}


class Timer
{
public:
	Timer()
		: id(0)
	{

	}

	void Run(HWND hWnd, unsigned int secs)
	{
		id = s_id;
		++s_id;

		SetTimer(hWnd, id, secs, &OnTimer);

	}

private:
	static unsigned int s_id;
	unsigned int id;
};

inline unsigned int Timer::s_id = 0;


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
			
			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			Timer timer;
			timer.Run(m_hWnd, 1000);

			return 0;
		}
		}

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void MainWindow::Paint(HDC hDC, RECT* rcDirty, BOOL bErase)
	{
		//FillRect(hDC, rcDirty, CreateSolidBrush(RGB(35, 38, 40)));
		Renderer2D.RenderBackgroundFromImage(L"image.bmp");
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