#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#include <string>
#include <vector>

#include "WolfgangDebug.h"

#pragma once

std::vector<DWORD_PTR> FindValueInMemory(HANDLE hProcess, int valueToFind, DWORD_PTR baseAddress, size_t regionSize) {
    std::vector<BYTE> buffer(regionSize);
    ReadProcessMemory(hProcess, (LPCVOID)baseAddress, buffer.data(), regionSize, nullptr);

    std::vector<DWORD_PTR> vptrAddresses = {}; 

    for (size_t i = 0; i <= regionSize - sizeof(int); ++i) {
        if (*(int*)(buffer.data() + i) == valueToFind) {
            vptrAddresses.push_back(baseAddress + i);
        }
    }
    return vptrAddresses;
}


DWORD_PTR FindDynamicAddress(HANDLE hProcess, DWORD_PTR ptrBaseAddress, const std::vector<DWORD_PTR>& vptrOffsets) {

    DWORD_PTR ptrCurrentAddress = ptrBaseAddress + vptrOffsets[0];
    DWORD_PTR ptrNextAddressValue = 0;
    ReadProcessMemory(hProcess, (LPCVOID)ptrCurrentAddress, &ptrNextAddressValue, sizeof(DWORD), nullptr);

    for (size_t i = 1; i < vptrOffsets.size() - 1; ++i) {
        ptrCurrentAddress = ptrNextAddressValue + vptrOffsets[i];
        ReadProcessMemory(hProcess, (LPCVOID)ptrCurrentAddress, &ptrNextAddressValue, sizeof(DWORD), nullptr);
    }

        return ptrNextAddressValue + vptrOffsets.back();

}

std::vector<DWORD_PTR> ScanProcessForValue(HANDLE hProcess, int iValueToFind) {
    if (hProcess == NULL) {
        std::cerr << "Invalid process handle passed to ScanProcessForValue.\n";
        return {};
    }

    MEMORY_BASIC_INFORMATION memInfo;
    DWORD_PTR ptrAddr = 0;
    std::vector<DWORD_PTR> vptrFoundAddresses;

    while (VirtualQueryEx(hProcess, (LPCVOID)ptrAddr, &memInfo, sizeof(memInfo))) {
        if (memInfo.State == MEM_COMMIT && (memInfo.Protect == PAGE_READWRITE || memInfo.Protect == PAGE_READONLY)) {
            std::vector<DWORD_PTR> vptrAddresses = FindValueInMemory(hProcess, iValueToFind, (DWORD_PTR)memInfo.BaseAddress, memInfo.RegionSize);
            vptrFoundAddresses.insert(vptrFoundAddresses.end(), vptrAddresses.begin(), vptrAddresses.end());
        }
        ptrAddr += memInfo.RegionSize;
        if (ptrAddr == 0) break;
    }
    return vptrFoundAddresses;
}
