#include "pch.h"
#include "Application.h"

Application::Application()
	: m_Window(1280, 720, "D3D11 TestWindow")
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

float rotation = 0.0f;
void Application::OnFrame()
{
	rotation += 0.01f;
	auto& gfx = m_Window.GetGraphics();
	gfx.ClearBuffer(1.0f, 1.0f, 1.0f);
	
	gfx.DrawTestTriangle(
		rotation, 
		m_Window.GetMouse().GetPosX() / 640.0f - 1.0f, 
		-m_Window.GetMouse().GetPosY() / 360.0f + 1.0f
	);

	gfx.EndFrame();
}
