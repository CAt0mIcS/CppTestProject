#include "pch.h"
#include "App.h"

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
    MSG msg{};
    BOOL gResult;
    while ((gResult = GetMessage(&msg, NULL, 0, 0)) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        
        OnFrame();
    }

    //check if GetMessage call itself failed
    if (gResult == -1)
    {
        throw THWND_LAST_EXCEPT();
    }

    //wParam here is the value passed to PostQuitMessage
    return (int)msg.wParam;
}

void App::OnFrame()
{
    const auto e = m_Window.mouse.Read();
    if (e.GetType() == Mouse::Event::Type::Move)
    {
        std::ostringstream oss;
        oss << "Position: (" << e.GetPosX() << ", " << e.GetPosY() << ')';
        m_Window.SetTitle(oss.str());
    }
}
