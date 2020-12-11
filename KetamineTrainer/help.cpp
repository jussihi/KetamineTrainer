#include "help.h"


HANDLE GetProcessByName(const TCHAR* szProcessName, const DWORD dwDesiredAccess)
{
    if (szProcessName == NULL) return NULL;
    CString strProcessName = szProcessName;

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return NULL;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);

    // Print the name and process identifier for each process.
    for (unsigned int i = 0; i < cProcesses; i++)
    {
        DWORD dwProcessID = aProcesses[i];
        // Get a handle to the process.
        HANDLE hProcess = OpenProcess(dwDesiredAccess, FALSE, dwProcessID);

        // Get the process name.
        TCHAR szEachProcessName[MAX_PATH];
        if (NULL != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                GetModuleBaseName(hProcess, hMod, szEachProcessName, sizeof(szEachProcessName) / sizeof(TCHAR));
            }
        }

        if (strProcessName.CompareNoCase(szEachProcessName) == 0)
            return hProcess;

        CloseHandle(hProcess);
    }

    return NULL;
}

uintptr_t GetModuleBaseAddress64(DWORD dwProcId, const TCHAR* szModName)
{
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, dwProcId);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry))
        {
            do
            {
                if (!_wcsicmp(modEntry.szModule, szModName))
                {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}


uintptr_t ResolveAddress(HANDLE hProc, uintptr_t pPtr, std::vector<uintptr_t> vPtrs)
{
    uintptr_t curr_addr = pPtr;

    for (DWORD i = 0; i < vPtrs.size(); i++)
    {
        ReadProcessMemory(hProc, (LPCVOID)curr_addr, (LPVOID)&curr_addr, sizeof(curr_addr), 0);
        curr_addr += vPtrs[i];
    }
    return curr_addr;
}