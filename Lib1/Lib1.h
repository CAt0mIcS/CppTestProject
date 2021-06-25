#pragma once

#ifdef LIB1_BUILD
	#define LIB1_API __declspec(dllexport)
#else
	#define LIB1_API __declspec(dllimport)
#endif

#include <string>

LIB1_API void LibraryFunction(std::string text);