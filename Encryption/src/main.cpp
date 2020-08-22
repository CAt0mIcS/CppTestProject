#include <iostream>
#include <Windows.h>
#include "Encriptor.h"

int main()
{
	Encriptor enc;
	auto estr = enc.Encript("Hello, this is a password!#");
	std::cout << "Encrypted string: " << estr << '\n';
	
	auto dstr = enc.Decript(estr);
	std::cout << "\nDecrypted string: " << dstr << '\n';
	while (true)
	{
		std::cout << estr << '\n';
		Sleep(10000);
	}
}