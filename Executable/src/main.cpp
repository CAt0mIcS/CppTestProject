#include "pch.h"
#include "Components.h"

#include "ECS\Registry.h"


#include <Windows.h>
uint32_t Combine(uint16_t x, uint16_t y)
{
	LPARAM result = 0;
	result = (x << 16) | (y);
	return result;
}


void PrintSplit(uint32_t lParam)
{
	std::bitset<32> bits(lParam);
	std::cout << "Bits: " << bits << '\n';
}


int main()
{
	uint32_t lParam = Combine(11, 22);
	PrintSplit(lParam);
}