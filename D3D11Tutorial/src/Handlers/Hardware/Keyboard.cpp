#include "pch.h"
#include "Keyboard.h"

void Keyboard::OnKeyPressed(int keycode)
{
	m_KeyStates[keycode] = true;
	m_KeyBuffer.push(Event(Event::Type::Press, keycode));
	TrimBuffer();
}

void Keyboard::OnKeyReleased(int keycode)
{
	m_KeyStates[keycode] = false;
	m_KeyBuffer.push(Event(Event::Type::Release, keycode));
	TrimBuffer();
}

Keyboard::Event Keyboard::ReadKey()
{
	if (m_KeyBuffer.size() > 0u)
	{
		const auto e = m_KeyBuffer.front();
		m_KeyBuffer.pop();
		return e;
	}
	return Event();
}

void Keyboard::TrimBuffer()
{
	while (m_KeyBuffer.size() > m_BufferSize)
	{
		m_KeyBuffer.pop();
	}
}
