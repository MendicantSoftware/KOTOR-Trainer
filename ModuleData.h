#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#include "WolfgangDebug.h"

int GetProcessID(const wchar_t* czProcessName) {

    DWORD pID = 0;
    HANDLE hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Process Snapshot ERROR: Invalid Handle Value\n";
        return 0;
    }

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    //Attempt to access information about the first process, if this values, an error has occured
    if (!Process32First(hProcessSnapshot, &pe32)) {
        std::cout << "Process32 Access ERROR: Could not access first process. Indicates snapshot creation error in GetProcessID\n";
        CloseHandle(hProcessSnapshot);
        return 0;
    }

    //Iterate through each process in an attempt to find the target process.
    do {

        if (wcscmp(pe32.szExeFile, czProcessName) == 0) {
            CloseHandle(hProcessSnapshot);
            pID = pe32.th32ProcessID;
            break;
        }

    } while (Process32Next(hProcessSnapshot, &pe32));

    CloseHandle(hProcessSnapshot);
    return pID;

}

bool GetModuleInfo(DWORD dwProcessId, const wchar_t* moduleName, MODULEENTRY32& modEntry) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
    if (hSnap == INVALID_HANDLE_VALUE) {
        return false;
    }
    
    modEntry.dwSize = sizeof(MODULEENTRY32);
    if (Module32First(hSnap, &modEntry)) {
        do {
            if (_wcsicmp(modEntry.szModule, moduleName) == 0) {
                CloseHandle(hSnap);
                return true;
            }
        } while (Module32Next(hSnap, &modEntry));
    }
    CloseHandle(hSnap);
    return false;
}

DWORD_PTR GetModuleBaseAddress(DWORD pID, const wchar_t* czModuleName) {

    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pID);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        std::cout << "Module snapshot produced an invalid handle value\n";
        return 0;
    }

    do {

        if (wcscmp(me32.szModule, czModuleName) == 0) {
            CloseHandle(hSnapshot);
            return (DWORD_PTR)me32.modBaseAddr;
        } 
    } while (Module32Next(hSnapshot, &me32));

    CloseHandle(hSnapshot);
    return 0;

}