#pragma once

#include "Handlers/Hardware/Keyboard.h"
#include "Handlers/Hardware/Mouse.h"


class Window
{
private:
	//Singelton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const char* GetName();
		static HINSTANCE GetHINSTANCE();

	private:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;

	private:
		static constexpr const char* m_WndClassName = "Test Window";
		static WindowClass m_WndClass;
		HINSTANCE m_hInstance;
	};

public:
	Window(unsigned int width, unsigned int height, const char* name);
	~Window();

	LRESULT CALLBACK HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static std::optional<int> ProcessMessage();

private:
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int m_Width, m_Height;
	HWND m_hWnd;
	Mouse m_Mouse;
	Keyboard m_Keyboard;
};

