#pragma once

#include "Handlers/Window.h"


class Application
{
public:
	Application();
	int Run();

	void OnFrame();
private:
	Window m_Window;
};

