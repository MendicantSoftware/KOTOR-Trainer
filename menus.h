#include "console.h"
#include "KeyManager.h"
#include "ReadWrite.h"

#include "Persistent.h"

bool AttributesMenu(HANDLE hProcess, PersistenceManager& pmPersistent) {

    clearScreen();

    bool bMenu = true;

    KeyManager kmManager(VK_NUMPAD1);
    kmManager.SetControlSwitch(clearScreen);

    int iSelection = 0;

    bool bControl = false;

    while (bMenu) {

        if (pmPersistent.bInputLock.load()) {
            Sleep(100);
            continue;
        } 

        ResetCursor();

        std::cout << "Attributes:\n";

        if (!bControl) {
            std::cout << "1. Set Strength\n";
            std::cout << "2. Set Strength Modifier\n";
            std::cout << "3. Set Dexterity\n";
            std::cout << "4. Set Dexterity Modifier\n";
            std::cout << "5. Set Constitution\n";
            std::cout << "6. Set Constitution Modifier\n";
            std::cout << "7. Set Intelligence\n";
            std::cout << "8. Set Intelligence Modifier\n";
            std::cout << "9. Set Wisdom\n";
            std::cout << "Hold ctrl for more options\n";
        } else {
            std::cout << "1. Set Wisdom Modifier\n";
            std::cout << "2. Set Charisma\n";
            std::cout << "3. Set Charisma Modifier\n";
            std::cout << "4. Set Experience\n"; // Levels are held in a list
        }

        std::cout << "*. Return to Root Menu\n";

        kmManager.GetKeyPresses();

        bControl = kmManager.GetControlState();

        if (bControl) {
            for (const auto iKeyPressed : kmManager.viLastKeyPresses) {
                switch(iKeyPressed) {
                    case VK_NUMPAD1:
                        std::cout << "Wisdom Modifier\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrWisdomModifierOffset), VK_NUMPAD1);
                        break;
                    case VK_NUMPAD2:
                        std::cout << "Charisma\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrCharismaOffset), VK_NUMPAD2);
                        break;
                    case VK_NUMPAD3:
                        std::cout << "Charisma Modifier\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrCharismaModifierOffset), VK_NUMPAD3);
                        break;
                    case VK_NUMPAD4:
                        std::cout << "Experience\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, 0x0), VK_NUMPAD3);
                        break;
                }
            }     
        } else {
            for (const auto iKeyPressed : kmManager.viLastKeyPresses) {
                switch(iKeyPressed) {
                    case VK_NUMPAD1:
                        std::cout << "Strength\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrStrengthOffset), VK_NUMPAD1);
                        break;
                    case VK_NUMPAD2:
                        std::cout << "Strength Modifier\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrStrengthModifierOffset), VK_NUMPAD2);
                        break;
                    case VK_NUMPAD3:
                        std::cout << "Dexterity\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrDexterityOffset), VK_NUMPAD3);
                        break;
                    case VK_NUMPAD4:
                        std::cout << "Dexterity Modifier\n";   
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrDexterityModifierOffset), VK_NUMPAD4);
                        break;
                    case VK_NUMPAD5:
                        std::cout << "Constitution\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrConstitutionOffset), VK_NUMPAD5);
                        break;
                    case VK_NUMPAD6:
                        std::cout << "Constitution Modifier\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrConstitutionModifierOffset), VK_NUMPAD6);
                        break;
                    case VK_NUMPAD7:
                        std::cout << "Intelligence\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrIntelligenceOffset), VK_NUMPAD7);
                        break;
                    case VK_NUMPAD8:
                        std::cout << "Intelligence Modifier\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrIntelligenceModifierOffset), VK_NUMPAD8);
                        break;
                    case VK_NUMPAD9:
                        std::cout << "Wisdom\n";
                        WriteUserByteAtAddress(hProcess, CalculateExperienceDerivedAddress(hProcess, ptrWisdomOffset), VK_NUMPAD9);
                        break;
                    case VK_MULTIPLY:
                        bMenu = false;
                        break;
                }
            }            
        }
    }

    clearScreen();
    return false;
}

bool DebugMenu(HANDLE hProcess, PersistenceManager& pmPersistent) {

    clearScreen();

    bool bMenu = true;

    static bool bTriggerRendering = false;
    static bool bBoundingBoxes = false;
    static bool bWireFrame = false;
    static bool bPersonalSpace = false;
    static bool bLighting = true;

    KeyManager kmManager(VK_NUMPAD3);

    while (bMenu) {

        if (pmPersistent.bInputLock.load()) {
            Sleep(100);
            continue;
        } 

        ResetCursor();

        std::cout << "Debug:\n";

        PrintBoolColouredText("1. Toggle Trigger Rendering\n", bTriggerRendering);
        PrintBoolColouredText("2. Toggle Bounding Boxes(AABB)\n", bBoundingBoxes);
        PrintBoolColouredText("3. Toggle Wireframe\n", bWireFrame);
        PrintBoolColouredText("4. Toggle Personal Space Rendering\n", bPersonalSpace);
        PrintBoolColouredText("5. Toggle Lighting\n", bLighting);

        std::cout << "*. Return to Root Menu\n";

        kmManager.GetKeyPresses();

        for (const auto iKeyPress : kmManager.viLastKeyPresses) {

            switch (iKeyPress) {
                case VK_NUMPAD1:
                    InvertWriteBool(hProcess, bTriggerRendering, ptrTriggerRendering);
                    break;
                case VK_NUMPAD2:
                    InvertWriteBool(hProcess, bBoundingBoxes, ptrRenderBoundingBoxes);
                    break;
                case VK_NUMPAD3:
                    InvertWriteBool(hProcess, bWireFrame, ptrWireFrame);
                    break;
                case VK_NUMPAD4:
                    InvertWriteBool(hProcess, bPersonalSpace, ptrPersonalSpace);
                    break;
                case VK_NUMPAD5:
                    InvertWriteBool(hProcess, bLighting, ptrLighting);
                    break;
                case VK_MULTIPLY:
                    bMenu = false;
                    break;
            }

        }

    }

    clearScreen();
    return false;
}

bool CombatMenu(HANDLE hProcess, PersistenceManager& pmPersistent) {
    
    clearScreen();

    bool bMenu = true;

    static bool bInfiniteForce = false;
    static bool bInfiniteHealth = false;

    KeyManager kmManager(VK_NUMPAD2);

    while (bMenu) {

        if (pmPersistent.bInputLock.load()) {
            Sleep(100);
            continue;
        } 

        ResetCursor();

        std::cout << "Combat:\n";

        std::cout << "1. Damage Multiplier\n";
        PrintBoolColouredText("2. Toggle Infinite Force\n", bInfiniteForce);
        PrintBoolColouredText("3. Toggle Infinite Health\n", bInfiniteHealth);
        std::cout << "*. Return to Root Menu\n";

        kmManager.GetKeyPresses();

        for (const auto iKeyPress : kmManager.viLastKeyPresses) {

            switch (iKeyPress) {
                case VK_NUMPAD1:
                    WriteUserDWORDAtAddress(hProcess, ptrDamageMultiplier, VK_NUMPAD1);
                    break;
                case VK_NUMPAD2:
                    bInfiniteForce = !bInfiniteForce;
                    pmPersistent.bReplenishForce.store(bInfiniteForce);
                    break;
                case VK_NUMPAD3:
                    bInfiniteHealth = !bInfiniteHealth;
                    pmPersistent.bReplenishHealth.store(bInfiniteHealth);
                    break;
                case VK_MULTIPLY:
                    bMenu = false;
                    break;
            }

        }

    }

    clearScreen();
    return false;

}