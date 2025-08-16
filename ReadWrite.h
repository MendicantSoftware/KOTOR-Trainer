#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#include <string>
#include <iostream>
#include <vector>


#include "AddressScanning.h"
#include "Offsets.h"

#include "KeyManager.h"


#pragma once

DWORD_PTR CalculateExperienceDerivedAddress(HANDLE hProcess, DWORD_PTR ptrOffset) {
    return FindDynamicAddress(hProcess, ptrBaseAddress, vptrExperienceOffsets) + ptrOffset;
}

/**
 * @brief Applies an offset to a pointer chain to determine a DWORD_PTR address. `FindDynamicAddress()` does this without applying the custom offset.
 *
 * @param hProcess A handle to the target process.
 * @param vptrPointerChain A vector of offsets that produce an address the custom offset is relative to.
 * @param ptrOffset The final offset to be applied to the pointer chain to calculate the final address, negative offsets are permitted.
 * @return Returns the address of the pointer chain, with the final offset applied.
 */
DWORD_PTR CalculateOffsetFromPointerChain(HANDLE hProcess, std::vector<DWORD_PTR> vptrPointerChain, DWORD_PTR ptrOffset) {
    return FindDynamicAddress(hProcess, ptrBaseAddress, vptrPointerChain) + ptrOffset;
}

template <typename DataType>
bool ReadAtAddress(HANDLE hProcess, DWORD_PTR ptrAddress, DataType& dtValue) {

    if (ReadProcessMemory(hProcess, (LPCVOID)ptrAddress, &dtValue, sizeof(DataType), nullptr) == 0) return false;

    return true;
}

template <typename DataType>
DataType ReadAtAddress(HANDLE hProcess, DWORD_PTR ptrAddress) {
    DataType dtValue{};

    if (ReadProcessMemory(hProcess, (LPCVOID)ptrAddress, &dtValue, sizeof(DataType), nullptr) == 0) return -1;
    return dtValue;

}

/**a
 * @brief Writes a given value to the provided memory address within the target process.
 *
 * @param hProcess A handle to the target process.
 * @param ptrAddress The address to be written to. See `CalculateOffsetFromPointerChain()` or `FindDynamicAddress()` to assist with this.
 * @param tValue The value and type to be written to the target address.
 * @return True if the write is successful, otherwise false. Note that successful writes to the wrong address still return true.
 */
template <typename DataType>
bool WriteAtAddress(HANDLE hProcess, DWORD_PTR ptrAddress, DataType tValue) {
    size_t iBytesWritten = 0;

    if (WriteProcessMemory(hProcess, (LPVOID)ptrAddress, &tValue, sizeof(tValue), &iBytesWritten) == 0) {
        return false;
    } else {
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

bool WriteUserDWORDAtAddress(HANDLE hProcess, DWORD_PTR ptrTargetPointer, int iEntryKey) {

    size_t iBytesWritten = 0;

    KeyManager kmManager(iEntryKey);

    std::cout << "Enter desired value:\n";

    int iValue = kmManager.CaptureInt();


    if (iValue == BAD_INT_CAPTURE) {
        clearScreen();
        return false;
    }

    DWORD dwValue = static_cast<DWORD>(iValue);

    if (WriteProcessMemory(hProcess, (LPVOID)ptrTargetPointer, &dwValue, sizeof(dwValue), &iBytesWritten) == 0) {
        clearScreen();
        std::cout << "Failure";
        Sleep(5000);
        return false;
    } else {
        clearScreen();
        return true;
    }

}

bool RescanAddresses(HANDLE hProcess, std::vector<DWORD_PTR>& vptrPreviousResults, KeyManager& kmManager) {

    kmManager.UpdateEntryKey(VK_RETURN);

    clearScreen();
    std::cout << "A re-scan requires the target value to be changed PRIOR to the scan.\n";
    std::cout << "Enter the target address' NEW VALUE:\n";
    int iValue = kmManager.CaptureInt();

    kmManager.UpdateEntryKey(VK_RETURN);

    if (iValue == BAD_INT_CAPTURE) {
        return false;
    } else {
        ScanProcessForValue(hProcess, vptrPreviousResults, iValue);
        return true;
    }
   
}

void WriteOverScannedAddresses(HANDLE hProcess) {

    KeyManager kmManager(VK_NUMPAD4);

    //So fucking deprecated it isn't funny but I'm lazy :(

    int iSearchValue = 0;
    int iIntendedValue = 0;

    std::cout << "WARNING!\n";
    std::cout << "This method of memory manipulation brute forces changes to your process. This can result in memory corruption, crashes and data loss!\n";
    std::cout << "It is recommended this method is used ONCE.\n";
    std::cout << "If you do not wish to continue, type an invalid value into the following prompt.\n\n\n";

    std::cout << "Enter the target address' CURRENT VALUE:\n";
    iSearchValue = kmManager.CaptureInt();

    if (iSearchValue == BAD_INT_CAPTURE) {
        return;
    }


    std::cout << "\nEnter the target address' INTENDED VALUE:\n";
    iIntendedValue = kmManager.CaptureInt();

    if (iIntendedValue == BAD_INT_CAPTURE) {
        return;
    }

    std::vector<DWORD_PTR> vptrFoundAddresses = ScanProcessForValue(hProcess, iSearchValue);
    std::cout << '\n';

    if (!vptrFoundAddresses.empty()) {

        bool bScanning = true;

        while (bScanning) {
            clearScreen();
            std::cout << "Found " << std::dec << vptrFoundAddresses.size() << " possible addresses:\n";
            std::cout << "A lower number indicates a higher chance of success\n";
            std::cout << "Would you like to rescan? Enter a valid number for YES\n";
            if (kmManager.CaptureInt() == BAD_INT_CAPTURE) {
                bScanning = false;
                for (const auto& ptrAddress : vptrFoundAddresses) {
                    std::cout << "  - 0x" << std::hex << ptrAddress << '\n';
                    int iWriteValue = iIntendedValue;
                    size_t bytesWritten = 0;
                    WriteProcessMemory(hProcess, (LPVOID)ptrAddress, &iWriteValue, sizeof(iWriteValue), &bytesWritten);
                }
            } else {
                RescanAddresses(hProcess, vptrFoundAddresses, kmManager);
            }
        }
    } else {
        std::cerr << "Value not found in any memory region.\n";
    }

    std::cout << "Press enter to continue\n";
    kmManager.CaptureInt();

}

void InvertWriteBool(HANDLE hProcess, bool& bValue, DWORD_PTR ptrAddress) {

    bValue = !bValue;
    WriteAtAddress(hProcess, ptrAddress, bValue);

}