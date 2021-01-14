#pragma once

#include <stdint.h>
#include <string>


enum class MouseButton : uint16_t
{
	Button0 = 0,
	Button1 = 1,
	Button2 = 2,
	Button3 = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,

	Left = Button0,
	Right = Button1,
	Middle = Button2,

	FIRST = Button0, LAST = Button7
};


enum class Key : uint16_t
{
	// From glfw3.h
	Space = 32,
	Apostrophe = 39, /* ' */
	Comma = 44, /* , */
	Minus = 45, /* - */
	Period = 46, /* . */
	Slash = 47, /* / */

	D0 = 48, /* 0 */
	D1 = 49, /* 1 */
	D2 = 50, /* 2 */
	D3 = 51, /* 3 */
	D4 = 52, /* 4 */
	D5 = 53, /* 5 */
	D6 = 54, /* 6 */
	D7 = 55, /* 7 */
	D8 = 56, /* 8 */
	D9 = 57, /* 9 */

	Semicolon = 59, /* ; */
	Equal = 61, /* = */

	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,

	LeftBracket = 91,  /* [ */
	Backslash = 92,  /* \ */
	RightBracket = 93,  /* ] */
	GraveAccent = 96,  /* ` */

	World1 = 161, /* non-US #1 */
	World2 = 162, /* non-US #2 */

	/* Function keys */
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	PageUp = 266,
	PageDown = 267,
	Home = 268,
	End = 269,
	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
	PrintScreen = 283,
	Pause = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,

	/* Keypad */
	NumPad0 = 320,
	NumPad1 = 321,
	NumPad2 = 322,
	NumPad3 = 323,
	NumPad4 = 324,
	NumPad5 = 325,
	NumPad6 = 326,
	NumPad7 = 327,
	NumPad8 = 328,
	NumPad9 = 329,
	NumPadDecimal = 330,
	NumPadDivide = 331,
	NumPadMultiply = 332,
	NumPadSubtract = 333,
	NumPadAdd = 334,
	NumPadEnter = 335,
	NumPadEqual = 336,

	LeftShift = 340,
	LeftControl = 341,
	LeftAlt = 342,
	LeftSuper = 343,
	RightShift = 344,
	RightControl = 345,
	RightAlt = 346,
	RightSuper = 347,
	Menu = 348
};

static const char* KeyToString(Key k)
{
	switch (k)
	{
	case Key::Space:			return "Space";
	case Key::Apostrophe:		return "Apostrophe";
	case Key::Comma:			return "Comma";
	case Key::Minus:			return "Minus";
	case Key::Period:			return "Period";
	case Key::Slash:			return "Slash";
	case Key::D0:				return "D0";
	case Key::D1:				return "D1";
	case Key::D2:				return "D2";
	case Key::D3:				return "D3";
	case Key::D4:				return "D4";
	case Key::D5:				return "D5";
	case Key::D6:				return "D6";
	case Key::D7:				return "D7";
	case Key::D8:				return "D8";
	case Key::D9:				return "D9";
	case Key::Semicolon:		return "Semicolon";
	case Key::Equal:			return "Equal";
	case Key::A:				return "A";
	case Key::B:				return "B";
	case Key::C:				return "c";
	case Key::D:				return "D";
	case Key::E:				return "E";
	case Key::F:				return "F";
	case Key::G:				return "G";
	case Key::H:				return "H";
	case Key::I:				return "I";
	case Key::J:				return "J";
	case Key::K:				return "K";
	case Key::L:				return "L";
	case Key::M:				return "M";
	case Key::N:				return "N";
	case Key::O:				return "O";
	case Key::P:				return "P";
	case Key::Q:				return "Q";
	case Key::R:				return "R";
	case Key::S:				return "S";
	case Key::T:				return "T";
	case Key::U:				return "U";
	case Key::V:				return "V";
	case Key::W:				return "W";
	case Key::X:				return "X";
	case Key::Y:				return "Y";
	case Key::Z:				return "Z";
	case Key::LeftBracket:		return "LeftBracket";
	case Key::Backslash:		return "Backslash";
	case Key::RightBracket:		return "RightBracket";
	case Key::GraveAccent:		return "GraveAccent";
	case Key::World1:			return "World1";
	case Key::World2:			return "World2";
	case Key::Escape:			return "Escape";
	case Key::Enter:			return "Enter";
	case Key::Tab:				return "Tab";
	case Key::Backspace:		return "Backspace";
	case Key::Insert:			return "Insert";
	case Key::Delete:			return "Delete";
	case Key::Right:			return "Right";
	case Key::Left:				return "Left";
	case Key::Down:				return "Down";
	case Key::Up:				return "Up";
	case Key::PageUp:			return "PageUp";
	case Key::PageDown:			return "PageDown";
	case Key::Home:				return "Home";
	case Key::End:				return "End";
	case Key::CapsLock:			return "CapsLock";
	case Key::ScrollLock:		return "ScrollLock";
	case Key::NumLock:			return "NumLock";
	case Key::PrintScreen:		return "PrintScreen";
	case Key::Pause:			return "Pause";
	case Key::F1:				return "F1";
	case Key::F2:				return "F2";
	case Key::F3:				return "F3";
	case Key::F4:				return "F4";
	case Key::F5:				return "F5";
	case Key::F6:				return "F6";
	case Key::F7:				return "F7";
	case Key::F8:				return "F8";
	case Key::F9:				return "F9";
	case Key::F10:				return "F10";
	case Key::F11:				return "F11";
	case Key::F12:				return "F12";
	case Key::F13:				return "F13";
	case Key::F14:				return "F14";
	case Key::F15:				return "F15";
	case Key::F16:				return "F16";
	case Key::F17:				return "F17";
	case Key::F18:				return "F18";
	case Key::F19:				return "F19";
	case Key::F20:				return "F2";
	case Key::F21:				return "F21";
	case Key::F22:				return "F22";
	case Key::F23:				return "F23";
	case Key::F24:				return "F24";
	case Key::F25:				return "F25";
	case Key::NumPad0:			return "NumPad0";
	case Key::NumPad1:			return "NumPad1";
	case Key::NumPad2:			return "NumPad2";
	case Key::NumPad3:			return "NumPad3";
	case Key::NumPad4:			return "NumPad4";
	case Key::NumPad5:			return "NumPad5";
	case Key::NumPad6:			return "NumPad6";
	case Key::NumPad7:			return "NumPad7";
	case Key::NumPad8:			return "NumPad8";
	case Key::NumPad9:			return "NumPad9";
	case Key::NumPadDecimal:	return "NumPadDecimal";
	case Key::NumPadDivide:		return "NumPadDivide";
	case Key::NumPadMultiply:	return "NumPadMultiply";
	case Key::NumPadSubtract:	return "NumPadSubtract";
	case Key::NumPadAdd:		return "NumPadAdd";
	case Key::NumPadEnter:		return "NumPadEnter";
	case Key::NumPadEqual:		return "NumPadEqual";
	case Key::LeftShift:		return "LeftShift";
	case Key::LeftControl:		return "LeftControl";
	case Key::LeftAlt:			return "LeftAlt";
	case Key::LeftSuper:		return "LeftSuper";
	case Key::RightShift:		return "RightShift";
	case Key::RightControl:		return "RightControl";
	case Key::RightAlt:			return "RightAlt";
	case Key::RightSuper:		return "RightSuper";
	case Key::Menu:				return "Menu";
	}

	return "";
}

static const char* MouseButtonToString(MouseButton btn)
{
	switch (btn)
	{
	case MouseButton::Button0:	return "LeftMouseButton";
	case MouseButton::Button1:	return "RightMouseButton";
	case MouseButton::Button2:	return "MiddleMouseButton";
	case MouseButton::Button3:	return "Button3";
	case MouseButton::Button4:	return "Button4";
	case MouseButton::Button5:	return "Button5";
	case MouseButton::Button6:	return "Button6";
	case MouseButton::Button7:	return "Button7";
	}

	return "";
}

inline Key* GetKeycodeMap()
{
	static Key s_KeycodeMap[512];
	s_KeycodeMap[0x00B] = Key::D0;
	s_KeycodeMap[0x002] = Key::D1;
	s_KeycodeMap[0x003] = Key::D2;
	s_KeycodeMap[0x004] = Key::D3;
	s_KeycodeMap[0x005] = Key::D4;
	s_KeycodeMap[0x006] = Key::D5;
	s_KeycodeMap[0x007] = Key::D6;
	s_KeycodeMap[0x008] = Key::D7;
	s_KeycodeMap[0x009] = Key::D8;
	s_KeycodeMap[0x00A] = Key::D9;
	s_KeycodeMap[0x01E] = Key::A;
	s_KeycodeMap[0x030] = Key::B;
	s_KeycodeMap[0x02E] = Key::C;
	s_KeycodeMap[0x020] = Key::D;
	s_KeycodeMap[0x012] = Key::E;
	s_KeycodeMap[0x021] = Key::F;
	s_KeycodeMap[0x022] = Key::G;
	s_KeycodeMap[0x023] = Key::H;
	s_KeycodeMap[0x017] = Key::I;
	s_KeycodeMap[0x024] = Key::J;
	s_KeycodeMap[0x025] = Key::K;
	s_KeycodeMap[0x026] = Key::L;
	s_KeycodeMap[0x032] = Key::M;
	s_KeycodeMap[0x031] = Key::N;
	s_KeycodeMap[0x018] = Key::O;
	s_KeycodeMap[0x019] = Key::P;
	s_KeycodeMap[0x010] = Key::Q;
	s_KeycodeMap[0x013] = Key::R;
	s_KeycodeMap[0x01F] = Key::S;
	s_KeycodeMap[0x014] = Key::T;
	s_KeycodeMap[0x016] = Key::U;
	s_KeycodeMap[0x02F] = Key::V;
	s_KeycodeMap[0x011] = Key::W;
	s_KeycodeMap[0x02D] = Key::X;
	s_KeycodeMap[0x015] = Key::Y;
	s_KeycodeMap[0x02C] = Key::Z;

	s_KeycodeMap[0x028] = Key::Apostrophe;
	s_KeycodeMap[0x02B] = Key::Backslash;
	s_KeycodeMap[0x033] = Key::Comma;
	s_KeycodeMap[0x00D] = Key::Equal;
	s_KeycodeMap[0x029] = Key::GraveAccent;
	s_KeycodeMap[0x01A] = Key::LeftBracket;
	s_KeycodeMap[0x00C] = Key::Minus;
	s_KeycodeMap[0x034] = Key::Period;
	s_KeycodeMap[0x01B] = Key::RightBracket;
	s_KeycodeMap[0x027] = Key::Semicolon;
	s_KeycodeMap[0x035] = Key::Slash;
	s_KeycodeMap[0x056] = Key::World2;

	s_KeycodeMap[0x00E] = Key::Backspace;
	s_KeycodeMap[0x153] = Key::Delete;
	s_KeycodeMap[0x14F] = Key::End;
	s_KeycodeMap[0x01C] = Key::Enter;
	s_KeycodeMap[0x001] = Key::Escape;
	s_KeycodeMap[0x147] = Key::Home;
	s_KeycodeMap[0x152] = Key::Insert;
	s_KeycodeMap[0x15D] = Key::Menu;
	s_KeycodeMap[0x151] = Key::PageDown;
	s_KeycodeMap[0x149] = Key::PageUp;
	s_KeycodeMap[0x045] = Key::Pause;
	s_KeycodeMap[0x146] = Key::Pause;
	s_KeycodeMap[0x039] = Key::Space;
	s_KeycodeMap[0x00F] = Key::Tab;
	s_KeycodeMap[0x03A] = Key::CapsLock;
	s_KeycodeMap[0x145] = Key::NumLock;
	s_KeycodeMap[0x046] = Key::ScrollLock;
	s_KeycodeMap[0x03B] = Key::F1;
	s_KeycodeMap[0x03C] = Key::F2;
	s_KeycodeMap[0x03D] = Key::F3;
	s_KeycodeMap[0x03E] = Key::F4;
	s_KeycodeMap[0x03F] = Key::F5;
	s_KeycodeMap[0x040] = Key::F6;
	s_KeycodeMap[0x041] = Key::F7;
	s_KeycodeMap[0x042] = Key::F8;
	s_KeycodeMap[0x043] = Key::F9;
	s_KeycodeMap[0x044] = Key::F10;
	s_KeycodeMap[0x057] = Key::F11;
	s_KeycodeMap[0x058] = Key::F12;
	s_KeycodeMap[0x064] = Key::F13;
	s_KeycodeMap[0x065] = Key::F14;
	s_KeycodeMap[0x066] = Key::F15;
	s_KeycodeMap[0x067] = Key::F16;
	s_KeycodeMap[0x068] = Key::F17;
	s_KeycodeMap[0x069] = Key::F18;
	s_KeycodeMap[0x06A] = Key::F19;
	s_KeycodeMap[0x06B] = Key::F20;
	s_KeycodeMap[0x06C] = Key::F21;
	s_KeycodeMap[0x06D] = Key::F22;
	s_KeycodeMap[0x06E] = Key::F23;
	s_KeycodeMap[0x076] = Key::F24;
	s_KeycodeMap[0x038] = Key::LeftAlt;
	s_KeycodeMap[0x01D] = Key::LeftControl;
	s_KeycodeMap[0x02A] = Key::LeftShift;
	s_KeycodeMap[0x15B] = Key::LeftSuper;
	s_KeycodeMap[0x137] = Key::PrintScreen;
	s_KeycodeMap[0x138] = Key::RightAlt;
	s_KeycodeMap[0x11D] = Key::RightControl;
	s_KeycodeMap[0x036] = Key::RightShift;
	s_KeycodeMap[0x15C] = Key::RightSuper;
	s_KeycodeMap[0x150] = Key::Down;
	s_KeycodeMap[0x14B] = Key::Left;
	s_KeycodeMap[0x14D] = Key::Right;
	s_KeycodeMap[0x148] = Key::Up;

	s_KeycodeMap[0x052] = Key::NumPad0;
	s_KeycodeMap[0x04F] = Key::NumPad1;
	s_KeycodeMap[0x050] = Key::NumPad2;
	s_KeycodeMap[0x051] = Key::NumPad3;
	s_KeycodeMap[0x04B] = Key::NumPad4;
	s_KeycodeMap[0x04C] = Key::NumPad5;
	s_KeycodeMap[0x04D] = Key::NumPad6;
	s_KeycodeMap[0x047] = Key::NumPad7;
	s_KeycodeMap[0x048] = Key::NumPad8;
	s_KeycodeMap[0x049] = Key::NumPad9;
	s_KeycodeMap[0x04E] = Key::NumPadAdd;
	s_KeycodeMap[0x053] = Key::NumPadDecimal;
	s_KeycodeMap[0x135] = Key::NumPadDivide;
	s_KeycodeMap[0x11C] = Key::NumPadEnter;
	s_KeycodeMap[0x059] = Key::NumPadEqual;
	s_KeycodeMap[0x037] = Key::NumPadMultiply;
	s_KeycodeMap[0x04A] = Key::NumPadSubtract;

	return s_KeycodeMap;
}
