#include "pch.h"
#include "Window.h"

#include "resource.h"


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
	wc.hIcon = (HICON)LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0);
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = GetName();
	wc.hIconSm = (HICON)LoadImage(m_hInstance, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0);

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
	if (!AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE))
		throw THWND_LAST_EXCEPT();

	m_hWnd = CreateWindow(WindowClass::GetName(), name, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top, NULL, NULL, WindowClass::GetHINSTANCE(), this);

	if (!m_hWnd)
		throw THWND_LAST_EXCEPT();

	ShowWindow(m_hWnd, SW_SHOWDEFAULT);
}

Window::~Window()
{
	DestroyWindow(m_hWnd);
}

void Window::SetTitle(const std::string& title)
{
	if(!SetWindowText(m_hWnd, title.c_str()))
	{
		throw THWND_LAST_EXCEPT();
	}
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
			return msg.wParam;
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
	case WM_KILLFOCUS:
	{
		kbd.ClearState();
		break;
	}
	case WM_KEYDOWN:
	//Syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
	{
		if(!(lParam & 0x40000000) || kbd.IsAutorepeatEnabled())
			kbd.OnKeyPressed((unsigned char)wParam);
		break;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		kbd.OnKeyReleased((unsigned char)wParam);
		break;
	}
	case WM_CHAR:
	{
		kbd.OnChar((unsigned char)wParam);
		break;
	}
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);

		//In client region -> log move and enter + capture mouse
		if (pt.x >= 0 && pt.x < m_Width && pt.y >= 0 && pt.y < m_Height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		//not in client -> log move and maintain capture if button down
		else
		{
			if (mouse.IsLeftPressed() || mouse.IsRightPressed())
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			//button up
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//Exception
Window::Exception::Exception(int line, const char* file, HRESULT hr)
	: Except::Exception(line, file), m_Hr(hr)
{
}

const char* Window::Exception::what() const
{
	std::ostringstream oss;
	oss << GetType() << '\n'
		<< "[Error Code] " << GetErrorCode() << '\n'
		<< "[Description] " << GetErrorString() << '\n'
		<< GetOriginString();

	m_WhatBuffer = oss.str();
	return m_WhatBuffer.c_str();
}

const char* Window::Exception::GetType() const
{
	return "Tutorial Window Exception";
}

std::string Window::Exception::TranslateErrorCode(HRESULT hr)
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&pMsgBuf, 0, nullptr);

	if (nMsgLen == 0)
		return "Unidentified error code";

	std::string errorString = pMsgBuf;
	LocalFree(pMsgBuf);
	return errorString;
}

HRESULT Window::Exception::GetErrorCode() const
{
	return m_Hr;
}

std::string Window::Exception::GetErrorString() const
{
	return TranslateErrorCode(m_Hr);
}
