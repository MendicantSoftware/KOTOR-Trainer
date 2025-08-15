#include <Windows.h>

#include <iostream>

#pragma once

void HideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(hConsole, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(hConsole, &CursorInfo);
}

void ResetCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cPosition = {0, 0};
    SetConsoleCursorPosition(hConsole, cPosition);
}

void clearScreen() {
    HANDLE hOut;
    COORD position;
    DWORD charsWritten;
    DWORD conSize;

    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    conSize = 0;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hOut, &csbi);
    conSize = csbi.dwSize.X * csbi.dwSize.Y;

    position.X = 0;
    position.Y = 0;
    FillConsoleOutputCharacter(hOut, ' ', conSize, position, &charsWritten);

    SetConsoleCursorPosition(hOut, position);
}

void clearCurrentLine() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD iCharsWritten = 0;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    COORD cInitalPosition{0, csbi.dwCursorPosition.Y};

    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X, cInitalPosition, &iCharsWritten);

    SetConsoleCursorPosition(hConsole, cInitalPosition);

}

void clearLine(SHORT iCursorYPos) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD iCharsWritten = 0;
    COORD cInitalPosition{0, iCursorYPos};

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    FillConsoleOutputCharacter(hConsole, ' ', csbi.dwSize.X, cInitalPosition, &iCharsWritten);

    SetConsoleCursorPosition(hConsole, cInitalPosition);

}

void PrintBoolColouredText(const char* czMessage, const bool bBool) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    if (bBool) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
    if (!bBool) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);

    std::cout << czMessage;

    SetConsoleTextAttribute(hConsole, csbi.wAttributes);

}

void TitleScreen() {
    std::cout << R"( _  _____ _____ ___  ____    _____ ____      _    ___ _   _ _____ ____  )" << "\n";
    std::cout << R"(| |/ / _ \_   _/ _ \|  _ \  |_   _|  _ \    / \  |_ _| \ | | ____|  _ \ )" << "\n";
    std::cout << R"(| ' / | | || || | | | |_) |   | | | |_) |  / _ \  | ||  \| |  _| | |_) |)" << "\n";
    std::cout << R"(| . \ |_| || || |_| |  _ <    | | |  _ <  / ___ \ | || |\  | |___|  _ < )" << "\n";
    std::cout << R"(|_|\_\___/ |_| \___/|_| \_\   |_| |_| \_\/_/   \_\___|_| \_|_____|_| \_\)" << "\n";

    std::cout << "Waiting until a KOTOR Process is detected!\n";
}

