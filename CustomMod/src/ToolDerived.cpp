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


ToolBase* CreateTool()
{
	ToolDerived* pDerived = new ToolDerived();
	pDerived->i = 32;
	return pDerived;
}


//ToolBar* __stdcall DoStuff()
//{
//	std::cout << "Function called\n";
//	ToolBar* bar = new ToolBar();
//	bar->i = 32;
//	return bar;
//}

