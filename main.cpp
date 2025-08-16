#define NOMINMAX

#include <Windows.h>
#include <TlHelp32.h>
#include <tchar.h>

#include <string>
#include <iostream>
#include <vector>

#include <limits>


#include "Offsets.h"
#include "ModuleData.h"
#include "AddressScanning.h"
#include "ReadWrite.h"

#include "Freecam.h"

#include "console.h"
#include "KeyManager.h"
#include "menus.h"



int main(int argc, char* argv[]) {

    HideCursor();
    TitleScreen();

    bool bProcess = false;
    
    const wchar_t* czProcess = L"swkotor.exe";
    int pID = 0;

    while (!bProcess) {
        pID = GetProcessID(czProcess);
        if (pID != 0) {
            clearScreen();
            bProcess = true;
        }
        Sleep (25);
    }
    
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (hProcess == NULL) {
        std::cout << "Could not open process. Error: " << GetLastError() << '\n';
        return 1;
    }

    ptrBaseAddress = GetModuleBaseAddress(pID, L"swkotor.exe");
    if (ptrBaseAddress == 0) {
        std::cout << "Failure to attain base address.\n";
        CloseHandle(hProcess);
        return 1;
    }

    bool bMenu = true;

    KeyManager kmManager(-1);

    PersistenceManager pmPersistent(hProcess);

    clearScreen();

    while (bMenu) {

        if (pmPersistent.bInputLock.load()) {
            Sleep(100);
            continue;
        } 

        ResetCursor();

        std::cout << "MAIN MENU:\n";

        std::cout << "1. Attributes & Levelling\n";
        std::cout << "2. Combat\n";
        std::cout << "3. Debugging & Developer Tools\n";
        std::cout << "4. Find & Write Value\n";
        std::cout << "5. Freecam\n";
        std::cout << "CTRL + *. Exit\n";

        kmManager.GetKeyPresses();

        if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {

            for (auto iKey : kmManager.viLastKeyPresses) {

                switch (iKey) {

                    case VK_MULTIPLY:
                        clearScreen();
                        CloseHandle(hProcess);
                        pmPersistent.JoinThread();
                        return 0;
                        break;

                    default:
                        break;

                }
            }

        } else {

            for (auto iKey : kmManager.viLastKeyPresses) {

                switch (iKey) {

                    case VK_NUMPAD1:
                        AttributesMenu(hProcess, pmPersistent);
                        break;

                    case VK_NUMPAD2:
                        CombatMenu(hProcess, pmPersistent);
                        break;

                    case VK_NUMPAD3:
                        DebugMenu(hProcess, pmPersistent);
                        break;

                    case VK_NUMPAD4:
                        WriteOverScannedAddresses(hProcess);
                        clearScreen();
                        break;

                    case VK_NUMPAD5:
                        Freecam(hProcess, pmPersistent);
                        break;

                    default:
                        break;

                }

            }

        }

    }


    return 0;
}
