#pragma once

#ifdef LIB2_BUILD
	#define LIB2_API __declspec(dllexport)
#else
	#define LIB2_API __declspec(dllimport)
#endif

#include <string>

LIB2_API void LibraryFunction(std::string text);