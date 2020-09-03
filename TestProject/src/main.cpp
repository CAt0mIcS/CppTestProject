#include "pch.h"

#include "NPE/Window/MainWindow.h"
using namespace NPE;


void OnFrame(MainWindow& win);

int APIENTRY wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	MainWindow win(600, 600, L"HA");
	return MainWindow::ProcessMessage([&]() { OnFrame(win); });
}


void OnFrame(MainWindow& win)
{
	win.Renderer2D.DrawTestButton(10, 10, 100, 100);
}

