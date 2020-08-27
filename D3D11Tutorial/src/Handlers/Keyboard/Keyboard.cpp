#include "pch.h"
#include "Keyboard.h"

bool Keyboard::IsKeyPressed(unsigned char keycode) const
{
    return m_KeyStates[keycode];
}

Keyboard::Event Keyboard::ReadKey()
{
    if (m_KeyBuffer.size() > 0)
    {
        Keyboard::Event e = m_KeyBuffer.front();
        m_KeyBuffer.pop();
        return e;
    }
    
    return Keyboard::Event();
}

bool Keyboard::IsKeyEmpty() const
{
    return m_KeyBuffer.empty();
}

void Keyboard::FlushKey()
{
    m_KeyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar()
{
    if (m_CharBuffer.size() > 0)
    {
        unsigned char e = m_CharBuffer.front();
        m_CharBuffer.pop();
        return e;
    }
    
    return 0;
}

bool Keyboard::IsCharEmpty() const
{
    return m_CharBuffer.empty();
}

void Keyboard::FlushChar()
{
    m_CharBuffer = std::queue<char>();
}

void Keyboard::Flush()
{
    FlushKey();
    FlushChar();
}

void Keyboard::EnableAutorepeat()
{
    m_AutorepeatEnabled = true;
}

void Keyboard::DisableAutorepeat()
{
    m_AutorepeatEnabled = false;
}

bool Keyboard::IsAutorepeatEnabled()
{
    return m_AutorepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char keycode)
{
    m_KeyStates[keycode] = true;
    m_KeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
    TrimBuffer(m_KeyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char keycode)
{
    m_KeyStates[keycode] = false;
    m_KeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
    TrimBuffer(m_KeyBuffer);
}

void Keyboard::OnChar(unsigned char character)
{
    m_CharBuffer.push(character);
    TrimBuffer(m_CharBuffer);
}

void Keyboard::ClearState()
{
    m_KeyStates.reset();
}

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer)
{
    while (buffer.size() > m_BufferSize)
    {
        buffer.pop();
    }
}

