#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <tchar.h>
#include <stdio.h>

#include <iostream>
#include <vector>

#pragma comment(lib, "psapi.lib")


DWORD GetPID(const char* procName)
{
	DWORD pId = -1;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (!hSnap || hSnap == INVALID_HANDLE_VALUE)
		return -1;
	
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);

	while (Process32Next(hSnap, &procEntry))
	{
		std::cout << procEntry.szExeFile << '\n';
		if (!_stricmp(procName, procEntry.szExeFile))
		{
            std::cout << "\n\nFound process " << procName << " with PID " << procEntry.th32ProcessID << "\n\n";
			CloseHandle(hSnap);
			return procEntry.th32ProcessID;
		}
	}
    std::cout << "\n\n";
	CloseHandle(hSnap);
	return -1;
}

// https://stackoverflow.com/questions/28231054/how-can-i-search-for-a-string-in-the-memory-of-another-process/28232548
char* GetAddressOfData(DWORD pid, const char* data, size_t len)
{
    HANDLE process = OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (process)
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);

        MEMORY_BASIC_INFORMATION info;
        std::vector<char> chunk;
        char* p = 0;
        while (p < si.lpMaximumApplicationAddress)
        {
            if (VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info))
            {
                p = (char*)info.BaseAddress;
                chunk.resize(info.RegionSize);
                SIZE_T bytesRead;
                if (ReadProcessMemory(process, p, &chunk[0], info.RegionSize, &bytesRead))
                {
                    for (size_t i = 0; i < (bytesRead - len); ++i)
                    {
                        if (memcmp(data, &chunk[i], len) == 0)
                        {
                            return (char*)p + i;
                        }
                    }
                }
                p += info.RegionSize;
            }
        }
    }
    return 0;
}

template<typename DATA_TYPE>
DATA_TYPE rpm(HANDLE* hProc, DWORD memAddr)
{
    DATA_TYPE dt = NULL;
    ReadProcessMemory(*hProc, (PVOID)memAddr, &dt, sizeof(dt), 0);
    return dt;
}


int main(int argc, char** argv)
{
	const char* procName = "Encryption.exe";
    const char* data = "Khoor/#wklv#lv#d#sdvvzrug$&";
	DWORD pId = GetPID(procName);
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pId);

    if (!hProc || hProc == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to open process " << procName << '\n';
        std::cin.get();
        return 0;
    }

	char* procRegion = GetAddressOfData(pId, data, strlen(data));

    if (procRegion)
        std::cout << "Found process base address " << (void*)procRegion << '\n';
    else
        std::cout << "Not found!\n";

    int procRegionAddr = (int)procRegion;

    // Start address of encrypted string: 0xBC4888 + 32
    std::string encStr = "";
    while (true)
    {
        std::cout << "Memory address read: 0x" << std::uppercase << std::hex << procRegionAddr << '\n';
        int buffer = rpm<int>(&hProc, procRegionAddr);

        if (buffer)
        {
            if ((char)buffer == data[0] && (char)rpm<int>(&hProc, procRegionAddr + 1) == data[1])
            {
                std::cout << "Encrypted string found at location: 0x" << std::uppercase << std::hex << procRegionAddr << '\n';

                while ((char)rpm<int>(&hProc, procRegionAddr) != '\0')
                {
                    encStr += (char)rpm<int>(&hProc, procRegionAddr);
                    ++procRegionAddr;
                }
                break;
            }
        }
        ++procRegionAddr;
    }

    std::cout << "Encrypted string: " << encStr << '\n';

}