#ifndef RL_WINDOW_H
#define RL_WINDOW_H

#include "BaseWindow.h"

#include "Reyal/Input/Keyboard.h"
#include "Reyal/Input/Mouse.h"

#include "Reyal/Widgets/ZWidget.h"
#include "Reyal/Core/Queue.h"

#include <functional>



namespace At0::Reyal
{
	class Event;
	class Renderer3D;

	/// <summary>
	/// Receives all events
	/// <param name="receiver">Is the Widget that received the event</param>
	/// <param name="e">Is the received Event</param>
	/// <returns>True if the event was handled by the client, false if the event should be handled by the Window</returns>
	/// </summary>
	using EventCallbackFn = std::function<bool(Widget* receiver, Event& e)>;

	/// <summary>
	/// Struct pushed into the event queue
	/// </summary>
	struct EventMessage
	{
		Widget* receiver;
		Scope<Event> e;
	};

	class RL_API Window : public BaseWindow<Window>, public Widget
	{
	private:
		static constexpr uint8_t s_MaxMessagesInQueue = 16u;

	public:
		/// <summary>
		/// From https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-showwindow
		/// Controls how the window is to be shown
		/// </summary>
		enum class ShowCommand : uint8_t
		{
			/// <summary>
			/// Hides the window and activates another window.
			/// </summary>
			Hide		= SW_HIDE,

			/// <summary>
			/// Activates and displays a window. If the window is minimized or maximized,
			///  the system restores it to its original size and position. 
			/// An application should specify this flag when displaying the window for the first time.
			/// </summary>
			Normal		= SW_SHOWNORMAL,

			/// <summary>
			/// Activates the window and displays it as a minimized window.
			/// </summary>
			Minimized	= SW_SHOWMINIMIZED,

			/// <summary>
			/// Activates the window and displays it as a maximized window.
			/// </summary>
			Maximized	= SW_SHOWMAXIMIZED,

			/// <summary>
			/// Maximizes the specified window.
			/// </summary>
			Maximize	= SW_MAXIMIZE,

			/// <summary>
			/// Activates the window and displays it in its current size and position.
			/// </summary>
			Show		= SW_SHOW,

			/// <summary>
			/// Minimizes the specified window and activates the next top-level window in the Z order.
			/// </summary>
			Minimize	= SW_MINIMIZE,

			/// <summary>
			/// Activates and displays the window. If the window is minimized or maximized, the
			///  system restores it to its original size and position. 
			/// An application should specify this flag when restoring a minimized window.
			/// </summary>
			Restore		= SW_RESTORE,

			/// <summary>
			/// Sets the show state based on the SW_ value specified in the STARTUPINFO structure
			/// passed to the CreateProcess function by the program that started the application.
			/// </summary>
			Default		= SW_SHOWDEFAULT,

			FIRST = Hide,
			LAST = Default
		};
	public:
		/// <summary>
		/// Window Constructor
		/// </summary>
		/// <param name="name">Is the unique name of the window</param>
		/// <param name="parent">Is the parent of this window</param>
		/// <param name="isMainWindow">Specifies whether this window is the main one which will exit the application when closed</param>
		Window(const std::wstring_view name, _In_opt_ Widget* parent = nullptr, bool isMainWindow = false);

		/// <summary>
		/// Window Deconstructor
		/// </summary>
		~Window();

		/// <summary>
		/// Handles all window messages
		/// </summary>
		/// <param name="uMsg">Is the message code</param>
		/// <param name="wParam">Is an additional parameter</param>
		/// <param name="lParam">Is an additional parameter</param>
		/// <returns>LRESULT code</returns>
		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		/// <summary>
		/// Checks if this window is the main Window
		/// </summary>
		/// <returns>True if it's the main Window, false otherwise</returns>
		bool IsMainWindow() const { return m_IsMainWindow; }

		/// <summary>
		/// Gets the title of this Window
		/// </summary>
		/// <returns>The Window title</returns>
		std::wstring GetTitle() const;

		/// <summary>
		/// Getter for the Renderer of this Window
		/// </summary>
		/// <returns>The Window Renderer</returns>
		virtual Renderer3D* GetRenderer3D() const override;

		/// <summary>
		/// Getter for the native window
		/// </summary>
		/// <returns>The native window handle</returns>
		virtual HWND GetNativeWindow() const override;

		/// <summary>
		/// Sets the Window title
		/// </summary>
		/// <param name="title">Is the new title of this Window</param>
		void SetTitle(const std::wstring_view title);

		/// <summary>
		/// Shows the window
		/// </summary>
		/// <param name="cmdShow">Is a flag to set how the window should be shown</param>
		void Show(ShowCommand cmdShow = ShowCommand::Default) const;

		/// <summary>
		/// Hides the Window
		/// </summary>
		void Hide() const;
		
		/// <summary>
		/// Sends a WM_CLOSE message to the window, thus closing it
		/// </summary>
		void Close() const;

		/// <summary>
		/// Checks if a window is open using the Window Class name
		/// </summary>
		/// <returns>True if the Window is open, false otherwise</returns>
		bool IsOpen() const;

		/// <summary>
		/// Will return false as long as the window hasn't been closed
		/// </summary>
		/// <param name="exitCode">Will be filled with the exit code when the application quits, specify nullptr to ignore</param>
		/// <returns>True if the window was closed, false otherwise</returns>
		bool ShouldClose(_In_opt_ _Maybenull_ int* exitCode = nullptr);

		/// <summary>
		/// Getter for the current event queue
		/// </summary>
		/// <returns>Returns the event queue for this window</returns>
		Queue<EventMessage, s_MaxMessagesInQueue>& GetEventQueue() { return m_EventQueue; }

		/// <summary>
		/// Sets the function which will be called when any event is received which needs to be handled immediately
		/// </summary>
		/// <typeparam name="F">Is any callable, std::function castable type</typeparam>
		/// <param name="func">Is the function to call</param>
		template<typename F, typename = std::enable_if_t<std::is_convertible_v<F, std::function<bool(_In_ Widget*, Event&)>>>>
		void SetImmediateEventHandler(F&& func) { m_ImmediateEvent = func; }

		/// <summary>
		/// Initializes the 2D Renderer, checks if it has already been initialized
		/// </summary>
		void InitRenderer2D();

		/// <summary>
		/// Initializes the 3D Renderer, checks if it has already been initialized
		/// </summary>
		void InitRenderer3D();

	private:
		/// <summary>
		/// Loops over all children and figures out if we need to send a HoverEnter/HoverLeave Event
		/// </summary>
		void SetHoveringWidget();

	public:
		/// <summary>
		/// The keyboard holding information about which key is pressed in this window
		/// </summary>
		Keyboard Keyboard;
		
		/// <summary>
		/// The mouse holding information about which mouse button is pressed in this window
		/// </summary>
		Mouse Mouse;

	private:
		/// <summary>
		/// Is true, then if this window closes, the application will exit
		/// </summary>
		bool m_IsMainWindow;

		/// <summary>
		/// Specifies the old position of this window
		/// </summary>
		Point2 m_OldWindowPos;

		/// <summary>
		/// Specifies the old size of this window
		/// </summary>
		Size m_OldSize;

		// TODO: Both renderers in here or only one renderer

		///// <summary>
		///// Specifies the 2D renderer
		///// </summary>
		//Scope<Renderer2D> m_Renderer2D;
		
		/// <summary>
		/// Specifies the 3D renderer
		/// </summary>
		Scope<Renderer3D> m_Renderer3D;

		/// <summary>
		/// Is the Queue of messages to process, they will be popped in Reyal::Application and dispatched to the layers
		/// </summary>
		Queue<EventMessage, s_MaxMessagesInQueue> m_EventQueue;

		/// <summary>
		/// Specifies the control where the mouse is currently on
		/// </summary>
		Widget* m_CurrentlyHovering;

		/// <summary>
		/// Called when any event occurs which needs to be handled immediately (WindowCloseEvent)
		/// <param name="receiver">Is the window which received the immediate event</param>
		/// <param name="e">Is the immediate event</param>
		/// <returns>True if the WindowProc should return 0, false if the WindowProc should break</returns>
		/// </summary>
		std::function<bool(_In_ Widget* receiver, Event& e)> m_ImmediateEvent;
	};
}

#endif // RL_WINDOW_H