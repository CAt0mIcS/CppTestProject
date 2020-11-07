#include "Application.h"

#include <iostream>
#include <Windows.h>


void Application::Run()
{
	for (uint64_t i = 0; ; ++i)
	{
		if(i % 10 == 0)
			std::cout << i << '\n';

		Sleep(1);
	}
}
