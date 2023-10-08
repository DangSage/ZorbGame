#ifndef GAME_DEFS_HPP
#define GAME_DEFS_HPP

#ifdef _WIN32
    #include <Windows.h>
#endif

// Define ANSI color escape codes as macros
#define ANSI_RESET "\x1B[0m"
#define ANSI_BLACK "\x1B[30m"
#define ANSI_RED "\x1B[31m"
#define ANSI_GREEN "\x1B[32m"
#define ANSI_YELLOW "\x1B[33m"
#define ANSI_BLUE "\x1B[34m"
#define ANSI_MAGENTA "\x1B[35m"
#define ANSI_CYAN "\x1B[36m"
#define ANSI_WHITE "\x1B[37m"

#include <iostream> // Include necessary standard library headers
#include <iomanip>
#include <vector>
#include <string>

// define size of display limit
#define CONSOLESIZE 70
#define ZORBWIDTH 7

// define function to force terminal to accept ansi color codes depending on operating system, make sure it does so for ALL escape
void ForceTerminalColor() {
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE)
            return;
        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode))
            return;
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, dwMode))
            return;
    #endif
}

// define function to change font size of terminal executeable depending on operating system
void ChangeFont(int spacing) {
    #ifdef _WIN32
        CONSOLE_FONT_INFOEX info = {0};
        info.cbSize = sizeof(info);
        info.dwFontSize.Y = spacing;
        info.FontWeight = FW_NORMAL;
        wcscpy(info.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &info);
    #endif
}

//define function that forces the terminal to have borders and a title
void ForceTerminalBorder() {    //make the text and console thinner like the terminal console in VSC
    #ifdef _WIN32
        HWND console = GetConsoleWindow();
        RECT r;
        GetWindowRect(console, &r); //stores the console's current dimensions
        MoveWindow(console, r.left, r.top, 700, 700, TRUE); // 700 width, 100 height
        SetConsoleTitle("Zorb Zenith"); //set the title of the console
        ChangeFont(14);
    #endif
}

void ForceTerminal() {
    ForceTerminalBorder();
    ForceTerminalColor();
}


#endif

