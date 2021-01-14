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
uint32_t wCounter = 0;
#pragma data_seg()

//
// instance specific data
//
HMODULE hInstance = 0;
Key prevKey = (Key)0;
uint64_t msOfLastPress = 0;


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hInstance = hModule;
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
		uint64_t upperThreshold = 100;
		MSG& msg = *(MSG*)lParam;
		//bool pressed = (HIWORD(msg.lParam) & KF_UP) ? false : true;
		if (msg.message == WM_KEYDOWN)
		{
			// Repeated keydown message
			if ((HIWORD(msg.lParam) & KF_REPEAT))
			{
				return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
			}

			int scancode = (HIWORD(msg.lParam) & (KF_EXTENDED | 0xff));
			Key key = GetKeycodeMap()[scancode];
			uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

			std::ostringstream oss;
			oss << "Now: " << now << "\nMsLastPress: " << msOfLastPress << "\nDiff: " << now - msOfLastPress << "\nTreshold: " << upperThreshold;
			strcpy(debugInfo, oss.str().c_str());

			if (prevKey == key && now - msOfLastPress < upperThreshold)
			{
				//std::cout << "---------- Skipping Key ----------\n";
				msg.message = WM_NULL;
				--wCounter;
				//return -1;
			}

			msOfLastPress = now;
			prevKey = key;
			++wCounter;
		}
	}
	return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
}


extern "C"
{
	__declspec(dllexport) void AttachHook()
	{
		hKeyboardHook = SetWindowsHookEx(WH_GETMESSAGE, KeyboardProc, hInstance, 0);
	}

	__declspec(dllexport) void DetachHook()
	{
		UnhookWindowsHookEx(hKeyboardHook);
		hKeyboardHook = 0;
	}

	__declspec(dllexport) uint32_t GetWCounter()
	{
		return wCounter;
	}

	__declspec(dllexport) void SetUpperThreshold(uint64_t threshold)
	{
		// upperThreshold = threshold;
	}

	__declspec(dllexport) char* DebugInfo()
	{
		return debugInfo;
	}
}