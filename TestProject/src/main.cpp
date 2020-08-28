#include "pch.h"


int main()
{
	CoInitialize(NULL);
	IActiveDesktop* pDesktop = nullptr;
	wchar_t wszWallpaper[MAX_PATH];

	CoCreateInstance(CLSID_ActiveDesktop, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IActiveDesktop), (void**)&pDesktop);

	pDesktop->GetWallpaper(wszWallpaper, MAX_PATH, 0);
	pDesktop->Release();

	std::wcout << wszWallpaper << '\n';

	IShellLink* pLink = nullptr;
	CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IShellLink), (void**)&pLink);
	
	char path[MAX_PATH];
	wcstombs(path, wszWallpaper, MAX_PATH);
	pLink->SetPath(path);
	
	IPersistFile* pPersist = nullptr;
	pLink->QueryInterface(__uuidof(IPersistFile), (void**)&pPersist);

	pPersist->Save(L"D:\\dev\\ProgramFiles\\wallpaper.lnk", FALSE);
	pPersist->Release();
	pLink->Release();

	CoUninitialize();
	return 0;
}
