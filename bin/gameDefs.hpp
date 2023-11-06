#ifndef GAME_DEFS_HPP
#define GAME_DEFS_HPP
#ifdef _WIN32
    #include <Windows.h>
#endif

#define IDI_MYICON 101

#include <iostream> // Include necessary standard library headers
#include <iomanip>
#include <array>
#include <vector>
#include <string>
#include <cstdlib>
#include <initializer_list>

// define size of display limit
constexpr int CONSOLESIZE = 100;
constexpr int ZORBWIDTH = 7;
bool _LIGHTTHEME = false; //toggle for light theme
bool _DEBUGMODE = false; //toggle for debug mode

// Define ANSI color escape codes as macros
namespace ansi {
    static std::string RESET = "\x1B[0m";
    constexpr std::string_view BLACK = "\x1B[30m";
    constexpr std::string_view RED = "\x1B[31m";
    constexpr std::string_view GREEN = "\x1B[32m";
    constexpr std::string_view YELLOW = "\x1B[33m";
    constexpr std::string_view BLUE = "\x1B[34m";
    constexpr std::string_view MAGENTA = "\x1B[35m";
    constexpr std::string_view CYAN = "\x1B[36m";
    constexpr std::string_view WHITE = "\x1B[37m";

    // Randomly pick a color from the list of ANSI color codes. This function is defined in zUtility.hpp
    std::string_view GetRandomColor();
}

// Define which ANSI color code to use for the Zorb's color (color groups)
constexpr std::array<std::string_view, 2> CG_PLAYER = { ansi::GREEN, ansi::CYAN };
constexpr std::array<std::string_view, 2> CG_ENEMY = { ansi::MAGENTA, ansi::RED };

static int turnCounter = 1; //turn counter for the battle segment
static int winCounter = 0; //win counter for the game
static int casualtyCounter = 0; //casualties counter for the game

constexpr char DLINE[] = "\033[1A\033[2K"; // ANSI escape code to delete the last line from the console

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

//enum for game states to be used in the game driver
enum class GameState {
    MainMenu,
    OptionsMenu,
    OptionsMenuTheme,
    Game,
    InfoMenu,
    End = -1
};


//namespace for zorb utility functions, NOT to be confused with the Zorb class
namespace zorb {
    const std::array<std::string, 75> NAMES = {
        "Beepy",
        "Beeb",
        "Barg",
        "Bleeble",
        "Blarg",
        "Borp",
        "Breemy",
        "Bugg",
        "Byny",
        "Cthulhu",
        "Floob",
        "Flap",
        "Flubb",
        "Gegy",
        "Gleep",
        "Gleepy",
        "Gloop",
        "Glorb",
        "Glorp",
        "Googl",
        "Gnap",
        "Greep",
        "Groopy",
        "Lap",
        "Leep",
        "Lip",
        "Lorg",
        "Loop",
        "Map",
        "Mip",
        "Neep",
        "Nip",
        "Nepp",
        "Norb",
        "Peeg",
        "Plip",
        "Porg",
        "Qazy",
        "Qee-Qee",
        "Quip",
        "Quor",
        "Schloo",
        "Schrelb",
        "Skeep",
        "Snarp",
        "Snip",
        "Thlok",
        "Twip",
        "Vlup",
        "Vorp",
        "Waby",
        "Weep",
        "Wree",
        "Yabl",
        "Yip",
        "Zaby",
        "Zap",
        "Zep",
        "Zim",
        "Zleep",
        "Zlarp",
        "Zlorg",
        "Zorby",
        "Zop",
        "Zug",
        "Zulone",
        "Zorp",
        "Zip",
        "Zeegul"
    };
    // For the barber encounter, we want to randomly choose a name for the barber every runtime
    std::string BARBERNAME;
} // namespace zorb

#endif

