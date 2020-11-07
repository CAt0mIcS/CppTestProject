#ifndef RL_KBDEVENT_H
#define RL_KBDEVENT_H

#include "RlBase.h"
#include "Events/Event.h"



namespace At0::Reyal
{
	class RL_API KeyEvent : public Event
	{
	protected:
		KeyEvent(const unsigned char keycode)
			: m_KeyCode(keycode)
		{
		}

	public:
		unsigned char GetKeyCode() const { return m_KeyCode; }

	private:
		unsigned char m_KeyCode;
	};

	class RL_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(const unsigned char keycode, uint32_t repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount)
		{
		}

		uint32_t GetRepeatCount() const { return m_RepeatCount; }

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[KeyPressedEvent] Key " << (uint16_t)GetKeyCode() << " pressed";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(KeyPressedEvent)

	private:
		uint32_t m_RepeatCount;
	};

	class RL_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(const unsigned char keycode)
			: KeyEvent(keycode)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[KeyReleasedEvent] Key " << (uint16_t)GetKeyCode() << " released";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(KeyReleasedEvent)
	};

	class RL_API CharEvent : public KeyEvent
	{
	public:
		CharEvent(const unsigned char keycode)
			: KeyEvent(keycode)
		{
		}

		virtual std::wstring ToString() const override
		{
			std::wostringstream oss;
			oss << L"[CharEvent] Character '" << (char)GetKeyCode() << "' written";
			return oss.str();
		}

		RL_DECLARE_TYPE_FN(CharEvent)
	};
}


#endif // RL_KBDEVENT_H
