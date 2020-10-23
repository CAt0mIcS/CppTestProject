#include "pch.h"

#include "ToolBase.h"

#include <filesystem>


typedef ToolBase* (* CreateFunc)();

std::vector<ToolBase*> tools{};

int main(int argc, char** argv)
{

	using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
	for (const auto& dirEntry : recursive_directory_iterator("D:\\dev\\Cpp\\Projects\\DLLModLoader\\DLLs"))
	{
		HMODULE lib = LoadLibrary(dirEntry.path().c_str());
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
		tools.push_back(tb);
	}

	bool shouldClose = false;
	while (!shouldClose)
	{
		if (GetKeyState(VK_CONTROL))
		{
			shouldClose = true;
		}

		for (ToolBase* tb : tools)
		{
			tb->OnUpdate();
		}
	}

	for (ToolBase* tb : tools)
	{
		tb->OnClose();
	}

}

