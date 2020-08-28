#pragma once

#include "Handlers/Window/Window.h"
#include "Util/Timer.h"


class App
{
public:
	App();
	~App();

	int Run();

private:
	void OnFrame();

private:
	Window m_Window;
	Timer m_Timer;
};

