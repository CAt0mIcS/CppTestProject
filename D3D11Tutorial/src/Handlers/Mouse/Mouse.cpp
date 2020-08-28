#include "pch.h"
#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const
{
    return { m_X, m_Y };
}

int Mouse::GetPosX() const
{
    return m_Y;
}

int Mouse::GetPosY() const
{
    return m_X;
}

bool Mouse::IsInWindow() const
{
    return m_IsInWindow;
}

bool Mouse::IsLeftPressed() const
{
    return m_LeftIsPressed;
}

bool Mouse::IsRightPressed() const
{
    return m_RightIsPressed;
}

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

void Mouse::OnMouseMove(int x, int y)
{
    m_X = x;
    m_Y = y;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
    TrimBuffer();
}

void Mouse::OnMouseEnter()
{
    m_IsInWindow = true;
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void Mouse::OnMouseLeave()
{
    m_IsInWindow = false;
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y)
{
    m_LeftIsPressed = true;
    m_X = x;
    m_Y = y;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
}

void Mouse::OnLeftReleased(int x, int y)
{
    m_LeftIsPressed = false;
    m_X = x;
    m_Y = y;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
}

void Mouse::OnRightPressed(int x, int y)
{
    m_RightIsPressed = true;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
}

void Mouse::OnRightReleased(int x, int y)
{
    m_RightIsPressed = false;

    m_Buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y)
{
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y)
{
    m_Buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void Mouse::OnWheelDelta(int x, int y, int delta)
{
    m_WheelDeltaCarry += delta;
    //generate event for every 120
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

void Mouse::TrimBuffer()
{
    while (m_Buffer.size() > m_BufferSize)
    {
        m_Buffer.pop();
    }
}
