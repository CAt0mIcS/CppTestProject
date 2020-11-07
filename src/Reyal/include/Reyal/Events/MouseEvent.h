#ifndef RL_MOUSEEVENT_H
#define RL_MOUSEEVENT_H

#include "Reyal/RlBase.h"
#include "Reyal/Events/Event.h"

#include "Reyal/Core/Vec2.h"
#include "Reyal/Widgets/ZWidget.h"


namespace At0::Reyal
{

	enum class RL_API MouseButton
	{
		INVALID = 0,
		Left, Middle, Right
	};

	class RL_API MouseMoveEvent : public Event
	{
	public:
		MouseMoveEvent(const Point2& oldPos, const Point2& newPos)
			: m_OldPos(oldPos), m_NewPos(newPos)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseMoveEvent] New Position: " << m_NewPos;
			return oss.str();
		}

		const Point2& GetPos() const { return m_NewPos; }
		const Point2& GetOldPos() const { return m_OldPos; }

		RL_DECLARE_TYPE_FN(MouseMoveEvent)

	private:
		Point2 m_OldPos;
		Point2 m_NewPos;
	};

	class RL_API MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(const MouseButton btn)
			: m_Button(btn)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseButtonPressedEvent] Button: " << (int)m_Button;
			return oss.str();
		}

		MouseButton GetButton() const { return m_Button; }

		RL_DECLARE_TYPE_FN(MouseButtonPressedEvent)

	private:
		MouseButton m_Button;
	};

	class RL_API MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(const MouseButton btn)
			: m_Button(btn)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseButtonReleasedEvent] Button: " << (int)m_Button;
			return oss.str();
		}

		MouseButton GetButton() const { return m_Button; }

		RL_DECLARE_TYPE_FN(MouseButtonReleasedEvent)

	private:
		MouseButton m_Button;
	};

	class RL_API MouseWheelEvent : public Event
	{
	public:
		int GetDelta() const { return m_Delta; }

	protected:
		MouseWheelEvent(int delta)
			: m_Delta(delta) {}

	private:
		int m_Delta;
	};

	class RL_API MouseWheelUpEvent : public MouseWheelEvent
	{
	public:
		MouseWheelUpEvent(int delta)
			: MouseWheelEvent(delta)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseWheelUpEvent] Delta: " << GetDelta();
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(MouseWheelUpEvent)
	};

	class RL_API MouseWheelDownEvent : public MouseWheelEvent
	{
	public:
		MouseWheelDownEvent(int delta)
			: MouseWheelEvent(delta)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[MouseWheelDownEvent] Delta: " << GetDelta();
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(MouseWheelDownEvent)
	};

	class RL_API HoverEnterEvent : public Event
	{
	public:
		HoverEnterEvent(_In_ Widget* hoverEntered)
			: m_HoverEntered(hoverEntered) {}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[HoverEnterEvent] Widget with name " << m_HoverEntered->GetName() << " gained hover focus";
			return oss.str();
		}

		Widget* GetHoverWidget() const { return m_HoverEntered; }

		RL_DECLARE_TYPE_FN(HoverEnterEvent);

	private:
		Widget* m_HoverEntered;
	};

	class RL_API HoverLeaveEvent : public Event
	{
	public:
		HoverLeaveEvent(_In_ Widget* hoverLeft)
			: m_HoverLeft(hoverLeft) {}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[HoverLeaveEvent] The Widget with name " << m_HoverLeft->GetName() << " lost hover focus";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(HoverLeaveEvent);

	private:
		Widget* m_HoverLeft;
	};
}

#endif // RL_MOUSEEVENT_H