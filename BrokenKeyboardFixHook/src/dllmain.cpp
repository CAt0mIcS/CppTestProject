#include "pch.h"
#include <chrono>
#include <iostream>
#include <sstream>
#include "KeyCodes.h"

//
// some data will be shared across all
// instances of the DLL
//
#pragma comment(linker, "/SECTION:.SHARED,RWS")
#pragma data_seg(".SHARED")
HHOOK hKeyboardHook = 0;
//uint32_t wCounter = 0;
#pragma data_seg()


// DEBUG DATA
uint32_t numApps = 0;
char apps[2048][MAX_PATH + 1]{ 0 };


//
// instance specific data
//
HMODULE hInstance = 0;
Key prevKey = (Key)0;
uint64_t msOfLastPress = 0;
uint64_t upperThreshold = 15000;
//uint64_t upperThreshold = 150;


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInstance = hModule;
		char filename[MAX_PATH + 1];
		GetModuleFileNameA(NULL, filename, MAX_PATH);
		strcpy(apps[numApps], filename);
		++numApps;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

char debugInfo[256];
LRESULT __declspec(dllexport) CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code >= HC_ACTION)
	{
		bool pressed = (HIWORD(lParam) & KF_UP) ? false : true;
		if (pressed)
		{
			// Repeated keydown message
			if ((HIWORD(lParam) & KF_REPEAT))
			{
				return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
			}

			int scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
			Key key = GetKeycodeMap()[scancode];
			uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

			std::ostringstream oss;
			oss << "Now: " << now << "\nMsLastPress: " << msOfLastPress << "\nDiff: " << now - msOfLastPress << "\nTreshold: " << upperThreshold;
			strcpy(debugInfo, oss.str().c_str());

			if (prevKey == key && now - msOfLastPress < upperThreshold)
			{
				std::ostringstream oss2;
				oss2 << "Skipping key " << KeyToString(key);
				strcpy(debugInfo, oss2.str().c_str());
				return 0;
			}

			msOfLastPress = now;
			prevKey = key;
		}
	}
	return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
}


extern "C"
{
	__declspec(dllexport) void AttachHook()
	{
		strcpy(debugInfo, "Attaching Hook");
		hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hInstance, 0);
	}

	__declspec(dllexport) void DetachHook()
	{
		strcpy(debugInfo, "Detaching Hook");
		UnhookWindowsHookEx(hKeyboardHook);
		hKeyboardHook = 0;
	}

	__declspec(dllexport) uint32_t GetWCounter()
	{
		//return wCounter;
		return 0;
	}

	__declspec(dllexport) void SetUpperThreshold(uint64_t threshold)
	{
		upperThreshold = threshold;
	}

	__declspec(dllexport) char* DebugInfo()
	{
		return debugInfo;
	}

	__declspec(dllexport) char** GetApplications(uint32_t* numaps)
	{
		*numaps = numApps;
		return (char**)apps;
	}
}