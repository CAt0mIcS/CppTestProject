#include "pch.h"
#include <Windows.h>
#include <stdio.h>

// TODO: create a mutex so this can only be loaded once
HMODULE thisModule;
HHOOK hook;
LRESULT CALLBACK LaunchListener(int nCode, WPARAM wParam, LPARAM lParam);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    thisModule = hModule;

    // Very restricted set of things that can be done in DllMain, refer to documentation
    // before adding anything here.

    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif
//window message loop is necessary for hooks to work? (didn't work with console app)
//provide function pointer to execute when notepad is launched.
    __declspec(dllexport) void AttachHook(DWORD threadID) {
        hook = SetWindowsHookEx(WH_CALLWNDPROC, LaunchListener, thisModule, threadID);
    }
#ifdef __cplusplus
}
#endif

LRESULT CALLBACK LaunchListener(int nCode, WPARAM wParam, LPARAM lParam) {
    // process event here
    if (nCode >= 0) 
    {
        CWPSTRUCT* pCwp = (CWPSTRUCT*)lParam;
        switch (pCwp->message)
        {
        case WM_MOVE:
        {
            std::cout << "Window moved\n";
            break; 
        }
        case WM_SIZE:
        {
            std::cout << "Window resized\n";
            break;
        }
        }

    }

    return CallNextHookEx(NULL, nCode, wParam, lParam);
}