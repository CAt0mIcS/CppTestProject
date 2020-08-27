#pragma once

#include <queue>
#include <bitset>


class Keyboard
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press, Release, Invalid
		};

		Event()
			: m_Type(Type::Invalid), m_Code(0u) {}

		Event(Type type, unsigned char code)
			: m_Type(type), m_Code(code) {}

		bool IsPress() const { return m_Type == Type::Press; }
		bool IsRelease() const { return m_Type == Type::Release; }
		bool IsValid() const { return m_Type != Type::Invalid; }
		bool GetCode() const { return m_Code; }

	private:
		Type m_Type;
		unsigned char m_Code;

	};

public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	//Key event
	bool IsKeyPressed(unsigned char keycode) const;
	Event ReadKey();
	bool IsKeyEmpty() const;
	void FlushKey();

	//Char event
	char ReadChar();
	bool IsCharEmpty() const;
	void FlushChar();
	void Flush();

	//Autorepeat control
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool IsAutorepeatEnabled();

private:
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(unsigned char character);
	void ClearState();

	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer);

private:
	static constexpr unsigned int m_NKeys = 256u;
	static constexpr unsigned int m_BufferSize = 16u;
	bool m_AutorepeatEnabled;
	std::bitset<m_NKeys> m_KeyStates;
	std::queue<Event> m_KeyBuffer;
	std::queue<char> m_CharBuffer;

};

