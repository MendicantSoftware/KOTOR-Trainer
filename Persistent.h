#include "Windows.h"

#include <atomic>
#include <thread>
#include <functional>

#include "ReadWrite.h"
#include "Offsets.h"

class PersistenceManager {

    private: 

        bool bDivision = false;

        std::thread sMainThread;

        std::atomic<bool> bRunThread;

        HANDLE hProcess;

    public:

        std::atomic<bool> bInputLock;

        std::atomic<bool> bReplenishForce;
        std::atomic<bool> bReplenishHealth;
        

        PersistenceManager(HANDLE hProcess) {

            this->hProcess = hProcess;

            bRunThread.store(true);

            bReplenishForce.store(false);
            bReplenishHealth.store(false);
            
            sMainThread = std::thread(&PersistenceManager::EvaluateMemoryWrites, this); 
            
        }

        void JoinThread() {

            bRunThread.store(false);
            sMainThread.join();

        }

        bool SwitchInputLock() {

            bool bLock = bInputLock.load();
            bLock = !bLock;
            bInputLock.store(bLock);

            if (bLock) {
                SetConsoleTitleA("INPUT LOCK ON");
            } else {
                SetConsoleTitleA("INPUT LOCK OFF");
            }

            return bLock;

        }

        void EvaluateMemoryWrites () {

            
            while (bRunThread.load()) {

                if (GetAsyncKeyState(VK_DIVIDE) & 0x8000) {
                    if (!bDivision) {
                        SwitchInputLock();
                        bDivision = true;
                    } 
                } else {
                    bDivision = false;
                }
                if (bReplenishForce.load()) WriteAtAddress(this->hProcess, CalculateExperienceDerivedAddress(hProcess, ptrForceOffset), DWORD(999));
                if (bReplenishHealth.load()) WriteAtAddress(this->hProcess, FindDynamicAddress(hProcess, ptrBaseAddress, vptrHealthOffsets), DWORD(999));


                Sleep (50);
            }

        }

};