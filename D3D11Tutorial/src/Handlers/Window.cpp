#include "pch.h"
#include "Window.h"


Window::WindowClass Window::WindowClass::m_WndClass;

Window::WindowClass::WindowClass()
	: m_hInstance(GetModuleHandle(NULL))
{
	WNDCLASSEX wc{ 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMessageSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetHINSTANCE();
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GetName();
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetName(), GetHINSTANCE());
}

const char* Window::WindowClass::GetName()
{
	return m_WndClassName;
}

HINSTANCE Window::WindowClass::GetHINSTANCE()
{
	return m_WndClass.m_hInstance;
}


//Window
Window::Window(unsigned int width, unsigned int height, const char* name)
	: m_Width(width), m_Height(height)
{
	//Calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = m_Width + wr.left;
	wr.top = 100;
	wr.bottom = m_Height + wr.top;

	//AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = CreateWindow(WindowClass::GetName(), name, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, WindowClass::GetHINSTANCE(), this);

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
}

std::optional<int> Window::ProcessMessage()
{
	MSG msg;
	//while the message queue has messages, remove and dispatch them
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		//check for quit because PeekMessage does not signal this via return
		if (msg.message == WM_QUIT)
		{
			//return optional wrapping int
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(10);

	//return empty optional when not quitting app
	return {};

}

LRESULT Window::HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//use create parameter passed in from CreateWindow() to store window class pointer
	if (uMsg == WM_NCCREATE)
	{
		//extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = (CREATESTRUCTW*)lParam;
		Window* const pWnd = (Window*)pCreate->lpCreateParams;

		//Set Win-API managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWnd);

		//Set WindowProc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)&Window::HandleMessageThunk);

		//Forward messages to window class handler
		return pWnd->HandleMessage(hWnd, uMsg, wParam, lParam);
	}

	// If we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Window* pWnd = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	return pWnd->HandleMessage(hWnd, uMsg, wParam, lParam);
}

LRESULT Window::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnMouseMove(pt.x, pt.y);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnLMouseButtonPressed(pt.x, pt.y);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnLMouseButtonReleased(pt.x, pt.y);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRMouseButtonPressed(pt.x, pt.y);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		POINTS pt = MAKEPOINTS(lParam);
		m_Mouse.OnRMouseButtonReleased(pt.x, pt.y);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		return 0;
	}
	case WM_KEYDOWN:
	{
		m_Keyboard.OnKeyPressed((int)wParam);
		return 0;
	}
	case WM_KEYUP:
	{
		m_Keyboard.OnKeyReleased((int)wParam);
		return 0;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}