#pragma once


class Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum class Type
		{
			INVALID,
			LPress, LRelease,
			RPress, RRelease,
			WheelUp, WheelDown,
			Move,
			Enter, Leave,
		};

	public:
		Event()
			: m_Type(Type::INVALID), m_LeftIsPressed(false), m_RightIsPressed(false), m_X(0), m_Y(0) {}

		Event(const Type type, const Mouse& parent)
			: m_Type(type), m_LeftIsPressed(parent.m_LeftIsPressed), m_RightIsPressed(parent.m_RightIsPressed), m_X(parent.m_X), m_Y(parent.m_Y) {}

		~Event() = default;


		bool IsValid() const { return m_Type != Type::INVALID; }
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
	Mouse operator=(const Mouse&) = delete;

	std::pair<int, int> GetPos() const { return { m_X, m_Y }; }
	int GetPosX() const { return m_X; }
	int GetPosY() const { return m_Y; }
	bool IsLeftPressed() const { return m_LeftIsPressed; }
	bool IsRightPressed() const { return m_RightIsPressed; }
	Mouse::Event Read();
	void Flush();
	bool IsInWindow() const { return m_IsInWindow; }

	bool IsEmpty() const { return m_Buffer.empty(); }

private:
	void OnLMouseButtonPressed(int x, int y);
	void OnLMouseButtonReleased(int x, int y);
	void OnRMouseButtonPressed(int x, int y);
	void OnRMouseButtonReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseWheelDelta(int x, int y, int delta);
	void OnMouseMove(int x, int y);
	void OnMouseEnter(int x, int y);
	void OnMouseLeave(int x, int y);

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

