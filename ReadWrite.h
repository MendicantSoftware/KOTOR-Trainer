#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#include <string>
#include <iostream>
#include <vector>

#include "WolfgangDebug.h"

#include "AddressScanning.h"
#include "Offsets.h"

#include "KeyManager.h"


#pragma once

DWORD_PTR CalculateExperienceDerivedAddress(HANDLE hProcess, DWORD_PTR iOffset) {

    return FindDynamicAddress(hProcess, ptrBaseAddress, vptrExperienceOffsets) + iOffset;

}

//Make Templated
DWORD ReadDWORDAtAddress(HANDLE hProcess, DWORD_PTR ptrAddress) {
    DWORD iValue = 0;

    if (ReadProcessMemory(hProcess, (LPCVOID)ptrAddress, &iValue, sizeof(DWORD), nullptr) == 0) {
        return 0;
    } else {
    }

    return iValue;
}

template <typename DataType>
bool WriteAtAddress(HANDLE hProcess, DWORD_PTR ptrAddress, DataType tValue) {
    size_t iBytesWritten = 0;

    if (WriteProcessMemory(hProcess, (LPVOID)ptrAddress, &tValue, sizeof(tValue), &iBytesWritten) == 0) {
        return false;
    } else {
        return true;
    }
}

//Make Templated
DWORD ReadDWORDAtAddress(HANDLE hProcess, DWORD_PTR ptrAddress, bool bDebug) {
    DWORD iValue = 0;
    
    WG::printvarhex("Reading Value at Address", ptrAddress);

    if (ReadProcessMemory(hProcess, (LPCVOID)ptrAddress, &iValue, sizeof(DWORD), nullptr) == 0) {
        if (bDebug) WG::print("Read Failed!");
        return 0;
    } else {
        if (bDebug) WG::printvarhex("Read Value", iValue, true);
    }
    return iValue;
}

//Make Templated
bool WriteDWORDAtAddress(HANDLE hProcess, DWORD_PTR ptrTargetPointer, DWORD iValue) {

    size_t iBytesWritten = 0;

    WG::printvarhex("Writing value", iValue, true);
    WG::printvarhex("Address", ptrTargetPointer);

    if (WriteProcessMemory(hProcess, (LPVOID)ptrTargetPointer, &iValue, sizeof(iValue), &iBytesWritten) == 0) {
        WG::print("Write Failed");
        return false;
    } else {
        WG::print("Write Successful");
        return true;
    }
}

//Make templated?
bool WriteUserByteAtAddress(HANDLE hProcess, DWORD_PTR ptrTargetPointer, int iEntryKey) {

    size_t iBytesWritten = 0;

    KeyManager kmManager(iEntryKey);

    std::cout << "Enter desired value:\n";

    int iValue = kmManager.CaptureInt();


    if (iValue == BAD_INT_CAPTURE) {
        clearScreen();
        return false;
    }

    if (iValue < 0 || iValue > 255) {
        clearScreen();
        return false;
    }

    BYTE bValue = static_cast<BYTE>(iValue);

    if (WriteProcessMemory(hProcess, (LPVOID)ptrTargetPointer, &bValue, sizeof(bValue), &iBytesWritten) == 0) {
        clearScreen();
        std::cout << "Failure";
        Sleep(5000);
        return false;
    } else {
        clearScreen();
        return true;
    }

}

bool WriteUserDWORDAtAddress(HANDLE hProcess, DWORD_PTR ptrTargetPointer) {

    size_t iBytesWritten = 0;
    DWORD iValue = 0;

    std::cout << "Enter desired value:\n";

    std::cin >> iValue;

    if (std::cin.fail()) {
        WG::print("ERROR -> INVALID ENTRY");
        return false;
    }

    WG::printvarhex("Writing value", iValue, true);
    WG::printvarhex("Address", ptrTargetPointer);

    if (WriteProcessMemory(hProcess, (LPVOID)ptrTargetPointer, &iValue, sizeof(iValue), &iBytesWritten) == 0) {
        WG::print("Write Failed");
        return false;
    } else {
        WG::print("Write Successful");
        return true;
    }
}

void WriteByteAtAddress(HANDLE hProcess, DWORD_PTR ptrDynamicAddress, BYTE bValue) {

    size_t iBytesWritten = 0;

    if (WriteProcessMemory(hProcess, (LPVOID)ptrDynamicAddress, &bValue, sizeof(bValue), &iBytesWritten) == 0) {
        std::cout << "Failed to write byte\n";
    } else {
        std::cout << "Successfully injected value: " << std::to_string(bValue) << " at address: " << std::hex << ptrDynamicAddress << '\n';
    }

}


void WriteFloatAtAddress(HANDLE hProcess, DWORD_PTR ptrDynamicAddress, float fValue) {

    size_t iBytesWritten = 0;

    if (WriteProcessMemory(hProcess, (LPVOID)ptrDynamicAddress, &fValue, sizeof(fValue), &iBytesWritten) == 0) {
        std::cout << "Failed to write byte\n";
    } else {
        std::cout << "Successfully injected value: " << std::to_string(fValue) << " at address: " << std::hex << ptrDynamicAddress << '\n';
    }

}

void WriteOverScannedAddresses(HANDLE hProcess) {

    int iSearchValue = 0;
    int iIntendedValue = 0;

    std::cout << "WARNING!\n";
    std::cout << "This method of memory manipulation brute forces changes to your process. This can result in memory corruption, crashes and data loss!\n";
    std::cout << "It is recommended this method is used ONCE.\n";
    std::cout << "If you do not wish to continue, type an invalid value into the following prompt.\n\n\n";

    std::cout << "Enter the target address' CURRENT VALUE:\n";
    std::cin >> iSearchValue;
    if (std::cin.fail()) {
        std::cout << "Invalid Value at Search: Cancelling Search!\n";
        return;
    }
    std::cout << "\nEnter the target address' INTENDED VALUE:\n";
    std::cin >> iIntendedValue;
    if (std::cin.fail()) {
        std::cout << "Invalid Value at Intended: Cancelling Search!\n";
        return;
    }

    std::vector<DWORD_PTR> vptrFoundAddresses = ScanProcessForValue(hProcess, iSearchValue);

    if (!vptrFoundAddresses.empty()) {
        std::cout << "\nFound " << std::dec << vptrFoundAddresses.size() << " possible addresses for credits:\n";
        for (const auto& ptrAddress : vptrFoundAddresses) {
            std::cout << "  - 0x" << std::hex << ptrAddress << '\n';
            int iWriteValue = iIntendedValue;
            size_t bytesWritten = 0;
            WriteProcessMemory(hProcess, (LPVOID)ptrAddress, &iWriteValue, sizeof(iWriteValue), &bytesWritten);
        }
    } else {
        std::cerr << "Credits value not found in any memory region.\n";
    }

}

void InvertWriteBool(HANDLE hProcess, bool& bValue, DWORD_PTR ptrAddress) {

    bValue = !bValue;
    WriteByteAtAddress(hProcess, ptrAddress, bValue);

}