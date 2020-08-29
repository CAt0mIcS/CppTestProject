#pragma once

#include "pch.h"
#include "Exceptions/Exception.h"

#include "Handlers/Keyboard/Keyboard.h"
#include "Handlers/Mouse/Mouse.h"

#include "Rendering/Graphics.h"

#include <optional>


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
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessage();
	Graphics& GetGraphics() const;

public:
	Keyboard kbd;
	Mouse mouse;

private:
	static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK HandleMessageThunk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
private:
	int m_Width, m_Height;
	HWND m_hWnd;
	std::unique_ptr<Graphics> m_pGraphics;

};

