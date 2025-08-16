#include <Windows.h>

#include <string>
#include <vector>
#include <functional>
#include <atomic>

#include "console.h"

#define BAD_INT_CAPTURE -999999

#pragma once

class KeyManager {

    private:

        bool bOneKey = false;
        bool bTwoKey = false;
        bool bThreeKey = false;
        bool bFourKey = false;
        bool bFiveKey = false;
        bool bSixKey = false;
        bool bSevenKey = false;
        bool bEightKey = false;
        bool bNineKey = false;
        bool bZeroKey = false;

        bool bAstrixKey = false;

        bool bControlKey = false;
        std::function<void(void)> fControlKeySwitchFunction = nullptr;

        bool bBackSpaceKey = false; 
        bool bEnterKey = false;

    public:

        KeyManager(int iEntryKey) {

            UpdateEntryKey(iEntryKey);

        }

        bool UpdateEntryKey(int iEntryKey) {

            bool bUpdated = false;

            switch (iEntryKey) {

                case VK_NUMPAD1:
                    bOneKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD2:
                    bTwoKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD3:    
                    bThreeKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD4:
                    bFourKey = true;
                    bUpdated = true;
                    break;  
                case VK_NUMPAD5:
                    bFiveKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD6:
                    bSixKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD7:
                    bSevenKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD8:
                    bEightKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD9:
                    bNineKey = true;
                    bUpdated = true;
                    break;
                case VK_NUMPAD0:
                    bZeroKey = true;
                    bUpdated = true;
                    break;
                case VK_MULTIPLY:
                    bAstrixKey = true;
                    bUpdated = true;
                    break;
                case VK_CONTROL:
                    bControlKey = true;
                    bUpdated = true;
                    break;
                case VK_BACK:
                    bBackSpaceKey = true; 
                    bUpdated = true;
                    break;
                case VK_RETURN:
                    bEnterKey = true;
                    bUpdated = true;
                    break;
                default:
                    break;

            }

            return bUpdated;

        }
        
        std::vector<int> viLastKeyPresses = {};

        size_t GetKeyPresses() {

            viLastKeyPresses.clear();

            size_t iKeyPresses = 0;

            if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
                if (!bOneKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD1);
                    iKeyPresses++;
                    bOneKey = true;
                } 
            } else {
                bOneKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000) {
                if (!bTwoKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD2);
                    iKeyPresses++;
                    bTwoKey = true;
                } 
            } else {
                bTwoKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
                if (!bThreeKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD3);
                    iKeyPresses++;
                    bThreeKey = true;
                } 
            } else {
                bThreeKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000) {
                if (!bFourKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD4);
                    iKeyPresses++;
                    bFourKey = true;
                } 
            } else {
                bFourKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000) {
                if (!bFiveKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD5);
                    iKeyPresses++;
                    bFiveKey = true;
                } 
            } else {
                bFiveKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000) {
                if (!bSixKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD6);
                    iKeyPresses++;
                    bSixKey = true;
                } 
            } else {
                bSixKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) {
                if (!bSevenKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD7);
                    iKeyPresses++;
                    bSevenKey = true;
                } 
            } else {
                bSevenKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000) {
                if (!bEightKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD8);
                    iKeyPresses++;
                    bEightKey = true;
                } 
            } else {
                bEightKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) {
                if (!bNineKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD9);
                    iKeyPresses++;
                    bNineKey = true;
                } 
            } else {
                bNineKey = false;
            }

            if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000) {
                if (!bZeroKey) {
                    viLastKeyPresses.push_back(VK_NUMPAD0);
                    iKeyPresses++;
                    bZeroKey = true;
                } 
            } else {
                bZeroKey = false;
            }

            if (GetAsyncKeyState(VK_MULTIPLY) & 0x8000) {
                if (!bAstrixKey) {
                    viLastKeyPresses.push_back(VK_MULTIPLY);
                    iKeyPresses++;
                    bAstrixKey = true;
                } 
            } else {
                bAstrixKey = false;
            }

            if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                if (!bControlKey) {
                    if (fControlKeySwitchFunction != nullptr) fControlKeySwitchFunction();
                    viLastKeyPresses.push_back(VK_CONTROL);
                    iKeyPresses++;
                    bControlKey = true;
                }
            } else {
                if (bControlKey && fControlKeySwitchFunction != nullptr) fControlKeySwitchFunction();
                bControlKey = false;
            }

            if (GetAsyncKeyState(VK_BACK) & 0x8000) {
                if (!bBackSpaceKey) {
                    viLastKeyPresses.push_back(VK_BACK);
                    iKeyPresses++;
                    bBackSpaceKey = true;
                }
            } else {
                bBackSpaceKey = false;
            }

            if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                if (!bEnterKey) {
                    viLastKeyPresses.push_back(VK_RETURN);
                    iKeyPresses++;
                    bEnterKey = true;
                }
            } else {
                bEnterKey = false;
            }

            return iKeyPresses;

        }

        void Reset() {

            bOneKey = false;
            bTwoKey = false;
            bThreeKey = false;
            bFourKey = false;
            bFiveKey = false;
            bSixKey = false;
            bSevenKey = false;
            bEightKey = false;
            bNineKey = false;
            bAstrixKey = false;

            viLastKeyPresses.clear();

        }

        bool GetControlState() {
            return bControlKey;
        }

        void SetControlSwitch(std::function<void(void)> fFunction) {
            this->fControlKeySwitchFunction = fFunction;
        }

        //Make Templated
        int CaptureInt() {

            //User needs to be able to read... Not cool.
            clearCurrentLine();

            std::string sValue = "";

            bool bInput = true;

            while (bInput) {

                this->GetKeyPresses();

                for (const auto iKeyPresses : this->viLastKeyPresses) {

                    switch (iKeyPresses) {

                        case VK_NUMPAD1:
                            sValue += '1';
                            std::cout << '1';
                            break;
                        case VK_NUMPAD2:
                            sValue += '2';
                            std::cout << '2';
                            break;
                        case VK_NUMPAD3:
                            sValue += '3';
                            std::cout << '3';
                            break;
                        case VK_NUMPAD4:
                            sValue += '4';
                            std::cout << '4';
                            break;
                        case VK_NUMPAD5:
                            sValue += '5';
                            std::cout << '5';
                            break;
                        case VK_NUMPAD6:
                            sValue += '6';
                            std::cout << '6';
                            break;
                        case VK_NUMPAD7:
                            sValue += '7';
                            std::cout << '7';
                            break;
                        case VK_NUMPAD8:
                            sValue += '8';
                            std::cout << '8';
                            break;
                        case VK_NUMPAD9:
                            sValue += '9';
                            std::cout << '9';
                            break;
                        case VK_NUMPAD0:
                            sValue += '0';
                            std::cout << '0';
                            break;
                        case VK_BACK:
                            if (sValue.size() > 0) {
                                clearCurrentLine();
                                sValue.pop_back();
                                std::cout << sValue;
                            } 
                            break;
                        case VK_RETURN:
                            bInput = false;
                            break;
                    }
                }
            }

            int iValue = 0;

            try {
                iValue = std::stoi(sValue);
            } catch (...) {
                iValue = BAD_INT_CAPTURE;
            }

            return iValue;

        }

};