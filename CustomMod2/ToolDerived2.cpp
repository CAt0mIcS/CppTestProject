#include "ToolDerived2.h"

#include <iostream>


void ToolDerived2::ToolDo()
{
	std::cout << "ToolDerived2 initialized\n";
}

void ToolDerived2::OnUpdate()
{
	std::cout << "ToolDerived2 OnUpdate\n";
}

void ToolDerived2::OnClose()
{
	std::cout << "ToolDerived2 OnClose\n";
}
