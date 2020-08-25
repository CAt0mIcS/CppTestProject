#include "pch.h"

typedef struct
{
	unsigned int t : 1;
} st_t;


int main()
{
	st_t tStruct{};
	tStruct.t = 5;
	//tStruct.v = 2;
	std::cout << "Size: " << sizeof(tStruct) << '\n';
	std::cout << "t: " << tStruct.t << '\n';
	//std::cout << "v: " << tStruct.v << '\n';

}
