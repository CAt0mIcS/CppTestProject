#include "pch.h"


int main()
{
	constexpr bool lib1 = true;

	HMODULE lib;
	if constexpr (lib1)
		lib = LoadLibraryA("C:/dev/Cpp/Projects/TestProject/build/bin/Debug/Lib1.dll");
	else
		lib = LoadLibraryA("C:/dev/Cpp/Projects/TestProject/build/bin/Debug/Lib2.dll");


	void (*LibraryFunction)(std::string) = (void (*)(std::string))GetProcAddress(lib,
		"?LibraryFunction@@YAXV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z");

	LibraryFunction("Hello World");

	FreeLibrary(lib);
}
