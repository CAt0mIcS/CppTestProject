#ifndef RL_EVENT_H
#define RL_EVENT_H

#include "RlBase.h"


#define RL_DECLARE_TYPE_FN(type)	static EventType GetStaticType() { return EventType::##type; }\
									virtual EventType GetType() const override { return GetStaticType(); }


namespace At0::Reyal
{
	enum class RL_API EventType
	{
		INVALID = 0,
		MouseButtonPressedEvent, MouseButtonReleasedEvent, MouseWheelUpEvent, MouseWheelDownEvent, MouseMoveEvent,
		HoverEnterEvent, HoverLeaveEvent,
		KeyPressedEvent, KeyReleasedEvent, CharEvent,
		WindowResizeEvent, WindowCloseEvent, WindowMoveEvent, PaintEvent, SetCursorEvent,
		FIRST = INVALID, LAST = SetCursorEvent
	};


	class RL_API Event
	{
	public:
		virtual EventType GetType() const = 0;
		virtual std::wstring ToString() const = 0;

	public:
		bool Handled = false;
	};

}


#endif // RL_EVENT_H
