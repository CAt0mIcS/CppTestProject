#include "ToolDerived.h"

#include <iostream>


ToolDerived::ToolDerived()
	:ToolBase()
{
}

void ToolDerived::ToolDo()
{
	std::cout << "Hello World\n";
}

void ToolDerived::OnUpdate()
{
	std::cout << "Updated\n";
	m_ShouldClose = true;
}

void ToolDerived::OnClose()
{
	std::cout << "Closed\n";
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

