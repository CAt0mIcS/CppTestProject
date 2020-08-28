#pragma once

#include <queue>


class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress, LRelease,
			RPress, RRelease,
			WheelUp, WheelDown,
			Move,
			Enter, Leave,
			Invalid
		};

		Event()
			: m_Type(Type::Invalid), m_LeftIsPressed(false), m_RightIsPressed(false), m_X(0), m_Y(0) {}

		Event(Type type, const Mouse& parent)
			: m_Type(type), m_LeftIsPressed(parent.m_LeftIsPressed), m_RightIsPressed(parent.m_RightIsPressed), m_X(parent.m_X), m_Y(parent.m_Y) {}

		bool IsValid() const { return m_Type != Type::Invalid; }
		Type GetType() const { return m_Type; }
		std::pair<int, int> GetPos() const { return { m_X, m_Y }; }
		int GetPosX() const { return m_X; }
		int GetPosY() const { return m_Y; }
		bool IsLeftPressed() const { return m_LeftIsPressed; }
		bool IsRightPressed() const { return m_RightIsPressed; }

	private:
		Type m_Type;
		bool m_LeftIsPressed;
		bool m_RightIsPressed;
		int m_X;
		int m_Y;

	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;

	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool IsInWindow() const;
	bool IsLeftPressed() const;
	bool IsRightPressed() const;
	Mouse::Event Read();
	void Flush();

	bool IsEmpty() const
	{
		return m_Buffer.empty();
	}

private:
	void OnMouseMove(int x, int y);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnWheelDelta(int x, int y, int delta);

	void TrimBuffer();

private:
	static constexpr unsigned int m_BufferSize = 16u;
	int m_X;
	int m_Y;
	bool m_LeftIsPressed = false;
	bool m_RightIsPressed = false;
	bool m_IsInWindow = false;
	int m_WheelDeltaCarry = 0;
	std::queue<Event> m_Buffer;

};

