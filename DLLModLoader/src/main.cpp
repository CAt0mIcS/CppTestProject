#include "pch.h"

#include "ToolBase.h"

#include <filesystem>


typedef ToolBase* (* CreateFunc)();

int main(int argc, char** argv)
{

	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const auto& dirEntry : recursive_directory_iterator("D:\\dev\\Cpp\\Projects\\DLLModLoader\\DLLs"))
	{
		HMODULE lib = LoadLibrary(L"D:\\dev\\Cpp\\Projects\\DLLModLoader\\bin\\Debug-Win32\\CustomMod.dll");
		if (!lib || lib == INVALID_HANDLE_VALUE)
		{
			std::cout << "[LoadLibrary] Error occured: " << GetLastError() << '\n';
			return -1;
		}

		CreateFunc func = (CreateFunc)GetProcAddress(lib, "CreateTool");
		if (!func || func == INVALID_HANDLE_VALUE)
		{
			std::cout << "[GetProcAddress] Error occured: " << GetLastError() << '\n';
			return -1;
		}

		ToolBase* tb = func();
		tb->ToolDo();
		while (!tb->ShouldClose())
		{
			tb->OnUpdate();
		}
		tb->OnClose();
	}
}

