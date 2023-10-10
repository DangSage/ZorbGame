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
#define CONSOLESIZE 85
#define ZORBWIDTH 7

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

//function that forces the terminal to have borders and a title
void ForceTerminalBorder() {    //make the text and console thinner like the terminal console in VSC
    #ifdef _WIN32
        HWND console = GetConsoleWindow();
        RECT r;
        GetWindowRect(console, &r); //stores the console's current dimensions
        MoveWindow(console, r.left, r.top, 800, 800, TRUE);
        SetConsoleTitle("Zorb Zenith"); //set the title of the console
        ChangeFont(15);
    #endif
}

void ForceTerminal() {
    ForceTerminalBorder();
    ForceTerminalColor();
}


//region INGAME DEFINITIONS
enum class GameState {
    MainMenu,
    OptionsMenu,
    Game,
    GameOver,
    InfoMenu,
    End = -1
};

//make a list of names that we can access later
const std::vector<std::string> zorbNames = {
    "Barg",
    "Blarg",
    "Borp",
    "Bugg",
    "Gleep",
    "Gleepy",
    "Glip",
    "Glarp",
    "Greep",
    "Groopy",
    "Lap",
    "Leep",
    "Lip",
    "Map",
    "Mip",
    "Neep",
    "Nip",
    "Nipp",
    "Norp",
    "Peeg",
    "Porg",
    "Plip",
    "Waby",
    "Weep",
    "Wree",
    "Zaby",
    "Zap",
    "Zip",
    "Zep",
    "Zug",
    "Zorp",
    "Flap",
    "Quip",
    "Snarp",
    "Snip",
    "Twip",
    "Vorp",
    "Whap",
    "Yip",
    "Beepy",
    "Quor",
    "Qazy",
    "Qee-Qee",
    "Beeb",
    "Gegy",
    "Glorb",
    "Norb",
    "Zleep",
    "Zeegul",
    "Googl",
    "Yabl",
    "Cthulhu",
    "Breemy",
    "Byny",
    "Glorp",
    "Lorg",
    "Loop",
    "Beerny",
    "Zlorg",
    "Zlarp",
    "Zim",
    "Zorby"
};

//endregion

//region ASCII ART DEFINITIONS
namespace z_art {
    std::string const introSpace = R"(
        .              +   .                .   . .     .  .
                   .                    .       .     *
  .       *                        . . . .  .   .  + .
            "You Are Here"            .   .  +  . . .
.                 |             .  .   .    .    . .
                  |           .     .     . +.    +  .
                 \|/            .       .   . .
        . .       V          .    * . . .  .  +   .
           +      .           .   .      +
                            .       . +  .+. .
  .                      .     . + .  . .     .      .
           .      .    .     . .   . . .        ! /
      *             .    . .  +    .  .       - O -
          .     .    .  +   . .  *  .       . / |
               . + .  .  .  .. +  .
.      .  .  .  *   .  *  . +..  .            *
 .      .   . .   .   .   . .  +   .    .            +)";

    std::string const optionsScreen = R"(
  ` : | | | |:  ||  :     `  :  |  |+|: | : : :|   .        `              .
      ` : | :|  ||  |:  :    `  |  | :| : | : |:   |  .                    :
         .' ':  ||  |:  |  '       ` || | : | |: : |   .  `           .   :.
                `'  ||  |  ' |   *    ` : | | :| |*|  :   :               :|
        *    *       `  |  : :  |  .      ` ' :| | :| . : :         *   :.||
             .`            | |  |  : .:|       ` | || | : |: |          | ||
      '          .         + `  |  :  .: .         '| | : :| :    .   |:| ||
         .                 .    ` *|  || :       `    | | :| | :      |:| |
 .                .          .        || |.: *          | || : :     :|||
        .            .   . *    .   .  ` |||.  +        + '| |||  .  ||`
     .             *              .     +:`|!             . ||||  :.||`
 +                      .                ..!|*          . | :`||+ |||`
     .                         +      : |||`        .| :| | | |.| ||`     .
       *     +   '               +  :|| |`     :.+. || || | |:`|| `
                            .      .||` .    ..|| | |: '` `| | |`  +
  .       +++                      ||        !|!: `       :| |
              +         .      .    | .      `|||.:      .||    .      .    `
          '         ___   pick your    `|.   .  `:|||   + ||'     `
  __    +      *  _[|=|]_  < settings!    `'       `'|.    `:
"'  `---"""----...( ^.^ ).._--`^``----.,.___           `.    `.  .    ____,.,-
    ___,--'""`---"o> ^ <n  ^        ^       """'---,..___ __,..---""'
--"'                        ^                         ``--..,__  ^`
    )";

    std::string const gameTitle = R"(
_/\____/\_             ___.      __________  O            .__   __   .__     
\____    / ____ _______\_ |__    \____    / _|__    ____  |__|_/  |_ |  |__  
  /     / /  _ \\_  __ \| __ \     /     /_/ __ \  /    \ |  |\   __\|  |  \ 
 /     /_(  <_> )|  | \/| \_\ \   /     /_\  ___/ |   |  \|  | |  |  |   Y  \
/_______ \\____/ |__|   |___  /  /_______ \\___  >|___|  /|__| |__|  |___|  /
        \/                  \/            \/    \/      \/                 \/ 
)";
}
//endregion
#endif

