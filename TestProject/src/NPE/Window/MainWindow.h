#pragma once

#include "pch.h"
#include "BaseWindow.h"

#include "NPE/Renderer/Renderer2D.h"
#include "NPE/Controls/Button.h"

namespace NPE
{
	class MainWindow
	{
	public:
		MainWindow(unsigned short width, unsigned short height, PCWSTR name);

		template<typename F>
		static int ProcessMessage(F&& func);

		LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		HWND GetNativeWindow() const { return m_hWnd; }

	public:
		Renderer2D Renderer2D;

	private:
		void Paint(HDC hDC, RECT* rcDirty, BOOL bErase);

	private:
		static LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK HandleMessagePass(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		HWND m_hWnd;
		std::vector<Button> m_Buttons;
	};


	template<typename F>
	inline int MainWindow::ProcessMessage(F&& func)
	{
		MSG msg{};
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			func();

		}
		return (int)msg.wParam;
	}

}



