#include "pch.h"
#include "App.h"

#include <iomanip>

App::App()
    : m_Window(800, 600, "D3D11 Test Window")
{
    //create console and hook up std::cout for debug purposes
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
}

App::~App()
{
    FreeConsole();
}

int App::Run()
{
    while (true)
    {
        //process all pending messages
        if (const auto ecode = Window::ProcessMessage())
        {
            return *ecode;
        }
        OnFrame();
    }
}

void App::OnFrame()
{
    const float c = (float)(std::sin(m_Timer.Peek()) / 2.0 + 0.5);
    m_Window.GetGraphics().ClearBuffer(c, c, 1.0f);
    m_Window.GetGraphics().EndFrame();
}
