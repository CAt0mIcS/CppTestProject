#include "pch.h"
#include "Application.h"

Application::Application()
	: m_Window(800, 600, "D3D11 TestWindow")
{

}

int Application::Run()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessage())
		{
			return *ecode;
		}
		OnFrame();
	}
}

void Application::OnFrame()
{
	if (m_Window.GetKeyboard().IsKeyPressed(VK_SPACE))
	{
		MessageBox(NULL, "Hello WOrld", "d", NULL);
	}
}
