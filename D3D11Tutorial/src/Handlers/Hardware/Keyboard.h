#pragma once



class Keyboard
{
	friend class Window;
private:
	void OnKeyPressed(int keycode);
	void OnKeyReleased(int keycode);

};

