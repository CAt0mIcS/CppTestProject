#include "pch.h"

#include "NPE/Window/MainWindow.h"
using namespace NPE;


void OnFrame(MainWindow& win);

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	MainWindow win(600, 600, L"HA");
	return MainWindow::ProcessMessage([&]() { OnFrame(win); });
}


void OnFrame(MainWindow& win)
{

}

