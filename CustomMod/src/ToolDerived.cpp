#include "ToolDerived.h"

#include <iostream>
#include <Windows.h>

ToolDerived::ToolDerived()
	:ToolBase()
{
}

void ToolDerived::ToolDo()
{
	std::cout << "ToolDerived initialized\n";
}

void ToolDerived::OnUpdate()
{
	std::cout << "ToolDerived OnUpdate\n";
}

void ToolDerived::OnClose()
{
	std::cout << "ToolDerived OnClose\n";
}


ToolBase* CreateTool()
{
	ToolDerived* pDerived = new ToolDerived();
	return pDerived;
}


//ToolBar* __stdcall DoStuff()
//{
//	std::cout << "Function called\n";
//	ToolBar* bar = new ToolBar();
//	bar->i = 32;
//	return bar;
//}

