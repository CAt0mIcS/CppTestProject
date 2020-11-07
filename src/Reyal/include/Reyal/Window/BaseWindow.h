#ifndef RL_BASEWINDOW_H
#define RL_BASEWINDOW_H

#include "Reyal/RlBase.h"
#include "Reyal/RlWin.h"

#include <stdint.h>
#include <string>

#pragma warning(disable : 4251)


namespace At0::Reyal
{
	template<typename DERIVED_TYPE>
	class RL_API BaseWindow
	{
	public:
		/// <summary>
		/// BaseWindow Constructor
		/// </summary>
		BaseWindow()
			: m_hWnd(0) {}

		/// <summary>
		/// Virtual BaseWindow Deconstructor, unregisters the window class
		/// </summary>
		virtual ~BaseWindow()
		{
			//QUESTION: Global constant for this and the amount of characters generated? (Window.cpp)
			//wchar_t className[5];
			//GetClassName(m_hWnd, className, std::size(className));
			//UnregisterClass(className, GetModuleHandle(NULL));
			UnregisterClass(m_ClassName.c_str(), GetModuleHandle(NULL));
		}

		/// <summary>
		/// Creates the native window
		/// </summary>
		/// <param name="windowName">Is the title of the window</param>
		/// <param name="windowClassName">Is the window class name</param>
		/// <param name="style">Are window styles</param>
		/// <param name="exStyle">Are extended window styles</param>
		/// <param name="x">Is the x-position of the window</param>
		/// <param name="y">Is the y-position of the window</param>
		/// <param name="width">Is the width of the window</param>
		/// <param name="height">Is the height of the window</param>
		/// <param name="hWndParent">Is a handle to the parent window</param>
		/// <param name="hMenu">Is a window Id</param>
		/// <returns>TRUE(1) if the window was created successfully, FALSE(0) otherwise</returns>
		uint8_t CreateNativeWindow(
			_In_ const wchar_t* windowName,
			_In_ const wchar_t* windowClassName,
			_In_ DWORD style,
			_In_opt_ _Maybenull_ DWORD exStyle = 0,
			_In_opt_ uint32_t x = CW_USEDEFAULT,
			_In_opt_ uint32_t y = CW_USEDEFAULT,
			_In_opt_ uint32_t width = CW_USEDEFAULT,
			_In_opt_ uint32_t height = CW_USEDEFAULT,
			_In_opt_ _Maybenull_ HWND hWndParent = 0,
			_In_opt_ _Maybenull_ HMENU hMenu = 0
		)
		{
			m_ClassName = windowClassName;

			WNDCLASS wc{};
			wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpszClassName = m_ClassName.c_str();
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);

			RegisterClass(&wc);

			m_hWnd = CreateWindowEx(
				exStyle, windowClassName, windowName, style, x, y, width, height,
				hWndParent, hMenu, wc.hInstance, this
			);

			return m_hWnd ? TRUE : FALSE;
		}

	private:
		/// <summary>
		/// Window procedure which calls the DERIVED_TYPE::HandleMessage function
		/// </summary>
		/// <param name="hwnd">Is the window handle to the window which should receive the event</param>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		static LRESULT CALLBACK WindowProc(_In_ HWND hWnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
		{
			DERIVED_TYPE* pDerived = nullptr;
			if (uMsg == WM_NCCREATE)
			{
				CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
				pDerived = (DERIVED_TYPE*)pCreate->lpCreateParams;
				SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pDerived);

				pDerived->m_hWnd = hWnd;
			}
			else
			{
				pDerived = (DERIVED_TYPE*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			}

			if (pDerived)
				return pDerived->HandleMessage(uMsg, wParam, lParam);
			else
				return DefWindowProc(hWnd, uMsg, wParam, lParam);

		}

	protected:
		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		virtual LRESULT HandleMessage(_In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) = 0;

		/// <summary>
		/// Getter for the window class name of this window
		/// </summary>
		/// <returns>The window class name</returns>
		const std::wstring_view GetWindowClassName() const { return m_ClassName; }

	protected:
		/// <summary>
		/// Window handle
		/// </summary>
		HWND m_hWnd;

	private:
		/// <summary>
		/// Stored to unregister class in Deconstructor
		/// <TODO>Try using GetClassName instead of storing it</TODO>
		/// </summary>
		std::wstring m_ClassName;
	};
}


#endif // RL_BASEWINDOW_H
