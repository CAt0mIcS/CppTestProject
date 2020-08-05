// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "ModClass.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }

    std::ofstream writer("C:\\dev\\ProgramFiles\\test.txt");
    writer << "Hello1\n";
    writer << "World!1\n";

    return TRUE;
}

extern "C" __declspec(dllexport) int funci()
{
    std::ofstream writer("C:\\dev\\ProgramFiles\\test.txt");
    writer << "Hello2\n";
    writer << "World!2\n";
    writer.close();
    return 0;
}


