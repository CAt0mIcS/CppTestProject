#ifndef RL_APPEVENT_H
#define RL_APPEVENT_H

#include "Reyal/RlBase.h"
#include "Reyal/Events/Event.h"

#include "Reyal/Core/Vec2.h"
#include <sstream>


namespace At0::Reyal
{
	class RL_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(const Size& oldSize, const Size& newSize)
			: m_OldSize(oldSize), m_NewSize(newSize)
		{

		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[WindowResizeEvent] New Size: " << m_NewSize;
			return oss.str();
		}

		const Size& GetSize() const { return m_NewSize; }
		const Size& GetOldSize() const { return m_OldSize; }

		RL_DECLARE_TYPE_FN(WindowResizeEvent)

	private:
		Size m_OldSize;
		Size m_NewSize;
	};

	class RL_API WindowMoveEvent : public Event
	{
	public:
		WindowMoveEvent(const Point2& oldPos, const Point2& newPos)
			: m_OldPos(oldPos), m_NewPos(newPos)
		{

		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[WindowMoveEvent] New Posititon: " << m_NewPos;
			return oss.str();
		}

		const Point2& GetPos() const { return m_NewPos; }
		const Point2& GetOldPos() const { return m_OldPos; }

		RL_DECLARE_TYPE_FN(WindowMoveEvent)

	private:
		Point2 m_OldPos;
		Point2 m_NewPos;
	};

	class RL_API SetCursorEvent : public Event
	{
	public:
		SetCursorEvent()
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[SetCursorEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(SetCursorEvent)
	};

	class RL_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent()
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[WindowCloseEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(WindowCloseEvent)
	};

	class RL_API PaintEvent : public Event
	{
	public:
		PaintEvent()
		{

		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[PaintEvent]";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(PaintEvent)
	private:
	};
}

#endif // RL_APPEVENT_H
