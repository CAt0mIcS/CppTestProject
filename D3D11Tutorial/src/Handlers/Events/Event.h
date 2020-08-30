#pragma once


enum class MouseEventType
{
	NONE,
	LeftButtonPressed, LeftButtonReleased,
	RightButtonPressed, RightButtonReleased,
	Enter, Leave,
	MouseWheelUp, MouseWheelDown,
	MouseMoved
};

enum class KeyboardEventType
{
	NONE,
	KeyPressed, KeyReleased
};


