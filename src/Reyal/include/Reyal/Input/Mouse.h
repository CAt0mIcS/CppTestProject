#ifndef RL_MOUSE_H
#define RL_MOUSE_H

#include "Reyal/RlBase.h"

#include "Reyal/Core/Vec2.h"


namespace At0::Reyal
{
	class Widget;

	class RL_API Mouse
	{
	public:
		/// <summary>
		/// Mouse Constructor. Initializes everything to false
		/// </summary>
		Mouse();

		/// <summary>
		/// Checks if the left mouse button is pressed
		/// </summary>
		/// <returns>True if it's pressed, false otherwise</returns>
		bool IsLeftPressed() const;
		
		/// <summary>
		/// Checks if the middle mouse button is pressed
		/// </summary>
		/// <returns>True if it's pressed, false otherwise</returns>
		bool IsMiddlePressed() const;

		/// <summary>
		/// Checks if the right mouse button is pressed
		/// </summary>
		/// <returns>True if it's pressed, false otherwise</returns>
		bool IsRightPressed() const;

		/// <summary>
		/// Checks if the mouse is on a certain widget, handles widget transforms
		/// </summary>
		/// <param name="widget">Is the widget to check</param>
		/// <returns>True if the mouse is on the widget, false otherwise</returns>
		bool IsOnWidget(const Widget* widget) const;
		
		/// <summary>
		/// Checks if the mouse is on a certain widget, handles widget transforms
		/// </summary>
		/// <param name="widget">Is the widget to check</param>
		/// <returns>True if the mouse is on the widget, false otherwise</returns>
		bool IsOnWidget(const Scope<Widget>& widget) const;

		/// <summary>
		/// Getter for the current mouse position
		/// </summary>
		/// <returns>The current mouse position</returns>
		const Point2& GetPos() const;

		/// <summary>
		/// Getter for the previous mouse position (last MouseMoveEvent)
		/// </summary>
		/// <returns>The old mouse position</returns>
		const Point2& GetOldPos() const;

		/// <summary>
		/// Sets the position of the mouse, doesn't actually move the mouse
		/// </summary>
		/// <param name="mousePos">Is the new mouse position</param>
		void SetPos(const Point2& mousePos);

	private:
		/// <summary>
		/// True if the left mouse button is pressed
		/// </summary>
		bool m_IsLeftPressed;
		
		/// <summary>
		/// True if the middle mouse button is pressed
		/// </summary>
		bool m_IsMiddlePressed;
		
		/// <summary>
		/// True if the right mouse button is pressed
		/// </summary>
		bool m_IsRightPressed;

		/// <summary>
		/// Current mouse position, set in SetPos()
		/// </summary>
		Point2 m_MousePos;

		/// <summary>
		/// Old mouse position, set in SetPos()
		/// </summary>
		Point2 m_OldMousePos;
	};
}



#endif // RL_MOUSE_H
