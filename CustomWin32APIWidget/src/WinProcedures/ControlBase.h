#pragma once
#include "pch.h"


inline BOOL ShouldUseUxThemeDll()
{
	DWORD dwMajorVersion = 0;
	HMODULE hDll = LoadLibrary(L"ComCtl32.dll");

	if (hDll)
	{
		DLLGETVERSIONPROC vProcFn;
		DLLVERSIONINFO vInfo;

		vProcFn = (DLLGETVERSIONPROC)GetProcAddress(hDll, "DllGetVersion");
		if (vProcFn)
		{
			vInfo.cbSize = sizeof(DLLVERSIONINFO);
			vProcFn(&vInfo);
			dwMajorVersion = vInfo.dwMajorVersion;
		}
		FreeLibrary(hDll);
	}
	return dwMajorVersion >= 6;

}
