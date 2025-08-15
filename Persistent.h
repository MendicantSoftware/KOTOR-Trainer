#include "Windows.h"

#include <atomic>
#include <thread>
#include <functional>

#include "ReadWrite.h"
#include "Offsets.h"

class PersistenceManager {

    private: 

        std::thread sMainThread;

        std::atomic<bool> bRunThread;

        HANDLE hProcess;

    public:

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

    void EvaluateMemoryWrites () {

        
        while (bRunThread.load()) {

            if (bReplenishForce.load()) WriteAtAddress(this->hProcess, CalculateExperienceDerivedAddress(hProcess, ptrForceOffset), DWORD(999));
            if (bReplenishHealth.load()) WriteAtAddress(this->hProcess, FindDynamicAddress(hProcess, ptrBaseAddress, vptrHealthOffsets), DWORD(999));


            Sleep (50);
        }

    }

};