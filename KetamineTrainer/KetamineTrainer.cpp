// KetamineTrainer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include <Windows.h>

#include "help.h"


int main()
{
    bool fail_no = true;

    HANDLE proc = GetProcessByName(L"OH_MR_KRABS-Win64-Shipping.exe", PROCESS_ALL_ACCESS);

    if (proc == NULL) fail_no = false;

    DWORD pic = GetProcessId(proc);

    if (!pic) fail_no = false;

    if (!fail_no)
    {
        std::cout << "Start krabs first !!!" << std::endl;
        std::system("pause");
        exit(1);
    }

    std::cout << "Enjoy ketamine :-)" << std::endl;

    while (true)
    {
        uintptr_t base = GetModuleBaseAddress64(pic, L"OH_MR_KRABS-Win64-Shipping.exe");

        if (!base)
        {
            break;
        }

        // static address for mr krab's playerbase
        uintptr_t krab_base = base + 0x03ED0748;

        std::vector<uintptr_t> offsetList = { 0x128, 0x110, 0xAFC };

        uintptr_t ketamine_address = ResolveAddress(proc, krab_base, offsetList);
        int ketamines = 1337;

        WriteProcessMemory(proc, (LPVOID)ketamine_address, &ketamines, sizeof(ketamines), NULL);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

}
