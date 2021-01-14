#include "pch.h"
#include <chrono>
#include <iostream>
#include "KeyCodes.h"

//
// some data will be shared across all
// instances of the DLL
//
#pragma comment(linker, "/SECTION:.SHARED,RWS")
#pragma data_seg(".SHARED")
HHOOK hKeyboardHook = 0;
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

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if ((uint32_t)(lParam & 0xff) == 1)
		return CallNextHookEx(hKeyboardHook, code, wParam, lParam);

	int scancode = (HIWORD(lParam) & (KF_EXTENDED | 0xff));
	Key key = GetKeycodeMap()[scancode];
	uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	uint64_t threshold = 80;  //ms

	if (key == prevKey && now - msOfLastPress < threshold)
	{
		return -1;
	}

	if (key == prevKey)
	{
		msOfLastPress = now;
	}

	prevKey = key;
	return CallNextHookEx(hKeyboardHook, code, wParam, lParam);
}


extern "C"
{
	__declspec(dllexport) void AttachHook()
	{
		hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hInstance, 0);
	}

	__declspec(dllexport) void DetachHook()
	{
		UnhookWindowsHookEx(hKeyboardHook);
		hKeyboardHook = 0;
	}
}