#include "zDefs.hpp"

bool _LIGHTTHEME = false; //toggle for light theme
bool _DEBUGMODE = false; //toggle for debug mode

namespace ansi{
    std::string RESET = "\x1B[0m";
}

int turnCounter = 1; //turn counter for the battle segment
int winCounter = 0; //win counter for the game
int casualtyCounter = 0; //casualties counter for the game

//force terminal to accept ansi color codes depending on operating system, make sure it does so for ALL escape
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

    //define for linux
    #ifdef __linux__
        std::cout << "\033[1;1H";
    #endif
}

// function to change font size of terminal executeable depending on operating system
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

//set the background & text color of the console (F = black, T = white)
void ChangeConsoleTheme() {
    if (_LIGHTTHEME) {
        system("color F0");
        ansi::RESET = "\x1B[30m";    //set ansi::RESET to black on white
    } else {
        system("color 07");
        ansi::RESET = "\x1B[37m";    //set ansi::RESET to white on black
    }
    _LIGHTTHEME = !_LIGHTTHEME; //toggle the theme
}

//function that changes the debug mode
void ChangeDebugMode() {
    _DEBUGMODE = !_DEBUGMODE;
}

//function that forces the terminal to have borders and a title
void ForceTerminalBorder() {    //make the text and console thinner like the terminal console in VSC
    #ifdef _WIN32
        HWND hwndTerminal = GetConsoleWindow();
        RECT rect;
        GetWindowRect(hwndTerminal, &rect);

        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        int borderSize = 10; // set the size of the border
        int consoleWidth = screenWidth - borderSize * 2;
        int consoleHeight = screenHeight - borderSize * 2;

        // calculate the new width and height to maintain 1:1 ratio
        if (consoleWidth > consoleHeight) {
            consoleWidth = consoleHeight;
        } else {
            consoleHeight = consoleWidth;
        }

        int x = (screenWidth - consoleWidth) / 2;
        int y = (screenHeight - consoleHeight) / 2;

        MoveWindow(hwndTerminal, x, y, consoleWidth, consoleHeight, TRUE);
        SetConsoleTitle("Zorb Zenith");
        ChangeFont(13);
    #endif
}

void ForceTerminal() {
    ForceTerminalBorder();
    ForceTerminalColor();
    ChangeConsoleTheme();
}
