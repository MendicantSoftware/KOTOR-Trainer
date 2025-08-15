#include <Windows.h>

#include "Offsets.h"
#include "ReadWrite.h"

#include "console.h"
#include "WolfgangDebug.h"

void CappedFloat(float& fFloat, float fSpeed, const int iDirection) {

    fSpeed = fSpeed * iDirection;

    fFloat += fSpeed;

    if (iDirection < 0) if (fFloat < -1) fFloat = -1; 

    if (iDirection > 0) if (fFloat > 1) fFloat = 1;


}

void Freecam(HANDLE hProcess) {

        bool bFreeCam = true;

        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        WriteAtAddress(hProcess, ptrCameraLock, BYTE(1));

        float fSpeed = 0.5;
        float fRotationSpeed = 0.01f;

        float fXPosition = 0;
        float fYPosition = 0;
        float fZPosition = 0;

        float fXRotation = 0;
        float fZRotation = 0;
        float fYRotation = 0;

        clearScreen();

        std::cout << "Camera X: " << fXPosition << '\n';
        std::cout << "Camera Y: " << fYPosition << '\n'; 
        std::cout << "Camera Z: " << fZPosition << '\n';

        std::cout << "Numpad controls X & Z movement/rotation\n";
        std::cout << "Plus and Minus controls Y movement/rotation\n";
        std::cout << "Shift controls speed\n";
        std::cout << "Ctrl swaps between rotation and movement\n";
        std::cout << "Astrix to close menu and return camera control to default\n";

        while(bFreeCam) {

            float fSpeed = 0.5;
            float fRotationSpeed = 0.01f;

            ResetCursor();

            std::cout << "Camera X: " << fXPosition << '\n';
            std::cout << "Camera Y: " << fYPosition << '\n'; 
            std::cout << "Camera Z: " << fZPosition << '\n';

            if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                fSpeed = fSpeed / 2;
                fRotationSpeed = fRotationSpeed / 2;
            }

            if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {

                if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
                    CappedFloat(fXRotation, fRotationSpeed, 1);
                    WriteAtAddress(hProcess, ptrCameraXRotation, fXRotation);
                }

                if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
                    CappedFloat(fXRotation, fRotationSpeed, -1);
                    WriteAtAddress(hProcess, ptrCameraXRotation, fXRotation);
                }

                if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) {
                    CappedFloat(fZRotation, fRotationSpeed, 1);
                    WriteAtAddress(hProcess, ptrCameraZRotation, fZRotation);
                }

                if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) {
                    CappedFloat(fZRotation, fRotationSpeed, -1);
                    WriteAtAddress(hProcess, ptrCameraZRotation, fZRotation);
                }

                if (GetAsyncKeyState(VK_ADD) & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    CappedFloat(fYRotation, fRotationSpeed, 1);
                    WriteAtAddress(hProcess, ptrCameraYRotation, fYRotation);
                }

                if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000 && GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                    CappedFloat(fYRotation, fRotationSpeed, -1);
                    WriteAtAddress(hProcess, ptrCameraYRotation, fYRotation);
                }

            } else {

                if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
                    fZPosition += fSpeed;
                    WriteAtAddress(hProcess, ptrCameraZPosition, fZPosition);
                }

                if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
                    fZPosition -= fSpeed;
                    WriteAtAddress(hProcess, ptrCameraZPosition, fZPosition);
                }

                if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) {
                    fXPosition += fSpeed;
                    WriteAtAddress(hProcess, ptrCameraXPosition, fXPosition);
                }

                if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) {
                    fXPosition -= fSpeed;
                    WriteAtAddress(hProcess, ptrCameraXPosition, fXPosition);
                }

                if (GetAsyncKeyState(VK_ADD) & 0x8000) {
                    fYPosition += fSpeed;
                    WriteAtAddress(hProcess, ptrCameraYPosition, fYPosition);
                }

                if (GetAsyncKeyState(VK_SUBTRACT) & 0x8000) {
                    fYPosition -= fSpeed;
                    WriteAtAddress(hProcess, ptrCameraYPosition, fYPosition);
                }


                if (GetAsyncKeyState(VK_MULTIPLY) & 0x8000) {
                    bFreeCam = false;
                    WriteAtAddress(hProcess, ptrCameraLock, BYTE(0));
                    return;
                }

            }

            Sleep(10);

        }

}