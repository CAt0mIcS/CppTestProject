#include "pch.h"
#include "App.h"

#include <iomanip>

App::App()
    : m_Window(800, 600, "D3D11 Test Window")
{
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
    //const float t = m_Timer.Peek();
    //std::ostringstream oss;
    //oss << "Time elapsed: " << std::setprecision(1) << std::fixed << t << 's';
    //m_Window.SetTitle(oss.str());

    const auto e = m_Window.mouse.Read();
    if (e.GetType() == Mouse::Event::Type::Move)
    {
        std::ostringstream oss;
        oss << "Position: (" << e.GetPosX() << ", " << e.GetPosY() << ")";
        m_Window.SetTitle(oss.str());
    }

}
