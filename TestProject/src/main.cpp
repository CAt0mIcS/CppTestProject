#include "pch.h"

#include "../../TPTools/src/IGun.h"


class Game : public IGun
{
public:
	virtual void OnGameInit() override
	{
		typedef void(__stdcall* dllFunction)();

		HMODULE mod = LoadLibrary("C:\\dev\\Cpp\\Projects\\TestProject\\bin\\Debug-Win32\\TestDLL\\TestDLL.dll");
		dllFunction dllFunc = (dllFunction)GetProcAddress(mod, "OnGameInit");
		if (dllFunc)
		{
			std::cout << "Executing function\n";
			dllFunc();
		}
		else
		{
			std::cout << "Could not find function\n";
			return;
		}
	}
};



/**
* Function loops over all paths where it expects dll mods to be, loads them and calls their Init() function
*/
void LoadLibraries()
{
	Game game;
	game.OnGameInit();

}

int main()
{
	LoadLibraries();

}


