#include <Windows.h>

#include <vector>

#pragma once

DWORD_PTR ptrBaseAddress;

std::vector<DWORD_PTR> vptrWeirdOffsetEitherKilledPlayerOrExceededLevelCap = {0x003A39FC, 0x4, 0x4, 0x3C0, 0xF8, 0xA74, 0x86};

std::vector<DWORD_PTR> vptrExperienceOffsets = {0x003A39FC, 0x4, 0x4, 0x3C0, 0xF8, 0xA74, 0x68};

DWORD_PTR ptrExperienceDynamicAddress = 0;

DWORD_PTR ptrForceOffset = 0xBC;

DWORD_PTR ptrStrengthOffset = 0x81;
DWORD_PTR ptrStrengthModifierOffset = 0x82;

DWORD_PTR ptrDexterityOffset = 0x83;
DWORD_PTR ptrDexterityModifierOffset = 0x84;

DWORD_PTR ptrConstitutionOffset = 0x85;
DWORD_PTR ptrConstitutionModifierOffset = 0x86;

DWORD_PTR ptrIntelligenceOffset = 0x87;
DWORD_PTR ptrIntelligenceModifierOffset = 0x88;

DWORD_PTR ptrWisdomOffset = 0x89;
DWORD_PTR ptrWisdomModifierOffset = 0x8A;

DWORD_PTR ptrCharismaOffset = 0x8B;
DWORD_PTR ptrCharismaModifierOffset = 0x8C;

DWORD_PTR ptrExperienceRequiredOffset = -0xCE31748;

//Static Pointers
//Note - These pointers were found by Lane, the kotor speedrunner, long before I looked for them

DWORD_PTR ptrTriggerRendering = 0x007b92e4;
DWORD_PTR ptrRenderBoundingBoxes = 0x007fbf5c;
DWORD_PTR ptrWireFrame = 0x007bb4f0;
DWORD_PTR ptrPersonalSpace = 0x007b9314;
DWORD_PTR ptrLighting = 0x0078e61c;

//Camera Pointers

DWORD_PTR ptrCameraLock = 0x007bb4f4;

DWORD_PTR ptrCameraXPosition = 0x007bb504;
DWORD_PTR ptrCameraZPosition = 0x007bb508;
DWORD_PTR ptrCameraYPosition = 0x007bb50c;

DWORD_PTR ptrCameraZRotation = 0x0078e444;
DWORD_PTR ptrCameraXRotation = 0x0078e448;
DWORD_PTR ptrCameraYRotation = 0x0078e44C;

//GetBasePointer

