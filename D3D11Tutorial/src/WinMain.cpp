#include "pch.h"
#include "Handlers/Window/Window.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR nCmdLine, _In_ int nCmdShow)
{
	try
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);

		Window win(400, 400, "Name");

		MSG msg{ 0 };
		while (GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			while (!win.mouse.IsEmpty())
			{
				const auto e = win.mouse.Read();
				if (e.GetType() == Mouse::Event::Type::Move)
				{
					std::ostringstream oss;
					oss << "Mouse Position (" << e.GetPosX() << ", " << e.GetPosY() << ')';
					win.SetTitle(oss.str());
				}
			}

		}

		FreeConsole();
		return (int)msg.wParam;
	}
	catch (const Except::Exception& e)
	{
		MessageBox(NULL, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBox(NULL, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBox(NULL, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	FreeConsole();
	return -1;
}

