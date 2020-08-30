#pragma once

#include "pch.h"


class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			INVALID, Press, Release
		};

		Event()
			: m_Type(Type::INVALID), m_Code(0u) {}

		Event(const Type type, unsigned char keycode)
			: m_Type(type), m_Code(keycode) {}

		bool IsKeyPressed() const { return m_Type == Type::Press; }
		bool IsKeyReleased() const { return m_Type == Type::Release; }
		bool IsKeyValid() const { return m_Type != Type::INVALID; }
		unsigned char GetCode() const { return m_Code; }

	private:
		Type m_Type;
		unsigned char m_Code;
	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	void ClearKeys() { m_KeyStates.reset(); }

	bool IsKeyPressed(unsigned char keycode) const { return m_KeyStates[keycode]; }
	Event ReadKey();
	bool IsKeyEmpty() const { return m_KeyBuffer.empty(); }
	void FlushKey() { m_KeyBuffer = std::queue<Event>(); }

	void EnableAutorepeat() { m_AutorepeatEnabled = true; }
	void DisableAutorepeat() { m_AutorepeatEnabled = false; }
	bool IsAutorepeatEnabled() const { return m_AutorepeatEnabled; }

private:
	void OnKeyPressed(int keycode);
	void OnKeyReleased(int keycode);

	void TrimBuffer();

private:
	static constexpr unsigned int m_nKeys = 256u;
	static constexpr unsigned int m_BufferSize = 16u;
	bool m_AutorepeatEnabled = false;
	std::bitset<m_nKeys> m_KeyStates;
	std::queue<Event> m_KeyBuffer;
};

