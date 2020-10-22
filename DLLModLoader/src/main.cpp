#include "pch.h"

#include "ToolBase.h"


typedef ToolBase* (* CreateFunc)();

int main(int argc, char** argv)
{
	HMODULE lib = LoadLibrary(L"D:\\dev\\Cpp\\Projects\\DLLModLoader\\bin\\Debug-Win32\\CustomMod.dll");
	if (!lib || lib == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		__debugbreak();
	}

	CreateFunc func = (CreateFunc)GetProcAddress(lib, "CreateTool");
	if (!func || func == INVALID_HANDLE_VALUE)
	{
		DWORD err = GetLastError();
		__debugbreak();
	}

	ToolBase* tb = func();
	tb->ToolDo();
	std::cout << tb->i << '\n';
}

