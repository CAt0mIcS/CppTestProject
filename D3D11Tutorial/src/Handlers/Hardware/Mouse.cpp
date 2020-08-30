#include "pch.h"
#include "Mouse.h"


Mouse::Event Mouse::Read()
{
	if (m_Buffer.size() > 0u)
	{
		Mouse::Event e = m_Buffer.front();
		m_Buffer.pop();
		return e;
	}
	return Mouse::Event();
}

void Mouse::Flush()
{
	m_Buffer = std::queue<Mouse::Event>();
}


void Mouse::OnLMouseButtonPressed(int x, int y)
{
	m_LeftIsPressed = true;
	m_Buffer.push(Event(Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLMouseButtonReleased(int x, int y)
{
	m_LeftIsPressed = false;
	m_Buffer.push(Event(Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRMouseButtonPressed(int x, int y)
{
	m_RightIsPressed = true;
	m_Buffer.push(Event(Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRMouseButtonReleased(int x, int y)
{
	m_RightIsPressed = false;
	m_Buffer.push(Event(Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
	m_Buffer.push(Event(Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
	m_Buffer.push(Event(Event::Type::WheelDown, *this));
}

void Mouse::OnMouseWheelDelta(int x, int y, int delta)
{
	m_WheelDeltaCarry += delta;
	while (m_WheelDeltaCarry >= WHEEL_DELTA)
	{
		m_WheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (m_WheelDeltaCarry <= -WHEEL_DELTA)
	{
		m_WheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}

void Mouse::OnMouseMove(int x, int y)
{
	m_X = x;
	m_Y = y;

	m_Buffer.push(Event(Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter(int x, int y)
{
	m_IsInWindow = true;

	m_Buffer.push(Event(Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave(int x, int y)
{
	m_IsInWindow = false;
	
	m_Buffer.push(Event(Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer()
{
	while (m_Buffer.size() > m_BufferSize)
	{
		m_Buffer.pop();
	}
}
