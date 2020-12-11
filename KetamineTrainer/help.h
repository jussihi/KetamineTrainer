#pragma once

#include <vector>

#include <Windows.h>
#include <psapi.h>
#include <atlstr.h>
#include <processthreadsapi.h>
#include <TlHelp32.h>


HANDLE GetProcessByName(const TCHAR* szProcessName, const DWORD dwDesiredAccess);

uintptr_t GetModuleBaseAddress64(DWORD procId, const TCHAR* modName);

uintptr_t ResolveAddress(HANDLE hProc, uintptr_t pPtr, std::vector<uintptr_t> vPtrs);