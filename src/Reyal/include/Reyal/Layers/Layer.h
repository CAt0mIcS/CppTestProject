#ifndef RL_LAYER_H
#define RL_LAYER_H

#include "Reyal/RlBase.h"

#include "Reyal/Widgets/ZWidget.h"


namespace At0::Reyal
{
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class MouseWheelUpEvent;
	class MouseWheelDownEvent;
	class MouseMoveEvent;
	class HoverEnterEvent;
	class HoverLeaveEvent;
	class KeyPressedEvent;
	class KeyReleasedEvent;
	class CharEvent;
	class WindowResizeEvent;
	class WindowMoveEvent;
	class PaintEvent;
	class WindowCloseEvent;

	class RL_API Layer
	{
	public:
		/// <summary>
		/// Layer Constructor
		/// </summary>
		/// <param name="name">Is the unique name of this Layer</param>
		Layer(const std::wstring_view name);

		/// <summary>
		/// Function which is called every frame
		/// </summary>
		virtual void OnUpdate() { };

		/// <summary>
		/// Receives all MouseButtonPressed Events. The button which was pressed is stored in "e"
		/// </summary>
		/// <param name="receiver">Is the Widget which was clicked</param>
		/// <param name="e">Is the generated event containing information about the click</param>
		virtual void OnMousePress(_In_ Widget* receiver, MouseButtonPressedEvent& e) {}

		/// <summary>
		/// Receives all MouseButtonReleaased Events. The button which was releaased is stored in "r"
		/// </summary>
		/// <param name="receiver">Is the Widget where the mouse was on when the button was released</param>
		/// <param name="e">Is the generated event containing information about the click</param>
		virtual void OnMouseRelease(_In_ Widget* receiver, MouseButtonReleasedEvent& e) {}

		/// <summary>
		/// Receives all MouseWheelUp Events. The delta is stored in "e"
		/// </summary>
		/// <param name="receiver">Is the Widget where the mouse was on when the scroll wheel was used</param>
		/// <param name="e">Is the generated event containing wheel delta</param>
		virtual void OnMouseWheelUp(_In_ Widget* receiver, MouseWheelUpEvent& e) {}
		
		/// <summary>
		/// Receives all MouseWheelDown Events. The delta is stored in "e"
		/// </summary>
		/// <param name="receiver">Is the Widget where the mouse was on when the scroll wheel was used</param>
		/// <param name="e">Is the generated event containing wheel delta</param>
		virtual void OnMouseWheelDown(_In_ Widget* receiver, MouseWheelDownEvent& e) {}

		/// <summary>
		/// Receives all MouseMove Events. The new and old position are stored in "e"
		/// </summary>
		/// <param name="receiver">Is the Widget where the mouse is currently on</param>
		/// <param name="e">Is the generated event containing new and old mouse position</param>
		virtual void OnMouseMove(_In_ Widget* receiver, MouseMoveEvent& e) {}

		/// <summary>
		/// Receives all HoverEnter Events. The function is called whenever the mouse passes
		/// into the bounding rect of any Widget
		/// </summary>
		/// <param name="receiver">Is the Widget which was entered</param>
		/// <param name="e">Is the generated event</param>
		virtual void OnHoverEnter(_In_ Widget* receiver, HoverEnterEvent& e) {}
		
		/// <summary>
		/// Receives all HoverLeave Events. The function is called whenever the mouse leaves
		/// the bounding rect of any Widget
		/// </summary>
		/// <param name="receiver">Is the Widget which was the mouse left</param>
		/// <param name="e">Is the generated event</param>
		virtual void OnHoverLeave(_In_ Widget* receiver, HoverLeaveEvent& e) {}

		/// <summary>
		/// Receives all KeyPressed Events.
		/// </summary>
		/// <param name="receiver">Is the Widget which had keyboard focus</param>
		/// <param name="e">Is the event containing the keycode of the pressed key and the repeat count</param>
		virtual void OnKeyPress(_In_ Widget* receiver, KeyPressedEvent& e) {}
		
		/// <summary>
		/// Receives all KeyReleased Events.
		/// </summary>
		/// <param name="receiver">Is the Widget which had keyboard focus</param>
		/// <param name="e">Is the event containing the keycode of the released key</param>
		virtual void OnKeyRelease(_In_ Widget* receiver, KeyReleasedEvent& e) {}

		/// <summary>
		/// Receives all Char Events. This function is only called for printable characters (including tab, space, return; excluding e.g. f10)
		/// </summary>
		/// <param name="receiver">Is the Widget which had keyboard focus</param>
		/// <param name="e">Is the event containing the keycode of the pressed key</param>
		virtual void OnChar(_In_ Widget* receiver, CharEvent& e) {}

		/// <summary>
		/// Receives all Resize Events.
		/// </summary>
		/// <param name="receiver">Is the Widget which was resized</param>
		/// <param name="e">Is the generated event containing new and old size</param>
		virtual void OnResize(_In_ Widget* receiver, WindowResizeEvent& e) {}

		/// <summary>
		/// Receives all WindowMove Events.
		/// </summary>
		/// <param name="receiver">Is the Widget which was moved</param>
		/// <param name="e">Is the generated event containing old and new position</param>
		virtual void OnWindowMove(_In_ Widget* receiver, WindowMoveEvent& e) {}

		/// <summary>
		/// Receives all Paint Events.
		/// </summary>
		/// <param name="receiver">Is the Widget which requested a repaint</param>
		/// <param name="e">Is the generated event</param>
		virtual void OnPaint(_In_ Widget* receiver, PaintEvent& e) {}
	
		/// <summary>
		/// Receives all WindowClose Events.
		/// </summary>
		/// <param name="receiver">Is the window which was closed</param>
		/// <param name="e">Is the generated event</param>
		/// <returns>True if the WindowProc should return 0, false if it should break</returns>
		virtual bool OnWindowClose(_In_ Widget* receiver, WindowCloseEvent& e) { return true; }

	private:
		/// <summary>
		/// Is the name of the current Layer, this name should be unique between Layers
		/// </summary>
		std::wstring m_Name;
	};
	
}


#endif // RL_LAYER_H

