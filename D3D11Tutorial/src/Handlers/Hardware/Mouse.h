#pragma once


class Mouse
{
	friend class Window;
private:
	void OnLMouseButtonPressed(int x, int y);
	void OnLMouseButtonReleased(int x, int y);
	void OnRMouseButtonPressed(int x, int y);
	void OnRMouseButtonReleased(int x, int y);
	void OnMouseWheelDelta(int x, int y, int delta);
	void OnMouseMove(int x, int y);
	void OnMouseEnter(int x, int y);
	void OnMouseLeave(int x, int y);
};

