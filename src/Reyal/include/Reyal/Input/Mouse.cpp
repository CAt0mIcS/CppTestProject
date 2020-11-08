#include "rlpch.h"
#include "Reyal/Input/Mouse.h"

#include "Reyal/Widgets/ZWidget.h"


namespace At0::Reyal
{
	Mouse::Mouse()
		: m_IsLeftPressed(false), m_IsMiddlePressed(false), m_IsRightPressed(false), m_MousePos{}
	{
	}

	bool Mouse::IsLeftPressed() const
	{
		return m_IsLeftPressed;
	}
	
	bool Mouse::IsMiddlePressed() const
	{
		return m_IsMiddlePressed;
	}
	
	bool Mouse::IsRightPressed() const
	{
		return m_IsRightPressed;
	}

	bool Mouse::IsOnWidget(const Widget* widget) const
	{
		return false;
	}

	bool Mouse::IsOnWidget(const Scope<Widget>& widget) const
	{
		return IsOnWidget(widget.get());
	}

	const Point2& Mouse::GetPos() const
	{
		return m_MousePos;
	}

	const Point2& Mouse::GetOldPos() const
	{
		return m_OldMousePos;
	}

	void Mouse::SetPos(const Point2& mousePos)
	{
		m_OldMousePos = m_MousePos;
		m_MousePos = mousePos;
	}
}


