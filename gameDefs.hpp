#ifndef GAME_DEFS_HPP
#define GAME_DEFS_HPP

#ifdef _WIN32
    #include <Windows.h>
#endif

#include <iostream> // Include necessary standard library headers
#include <iomanip>
#include <array>
#include <vector>
#include <string>
#include <cstdlib>

// define size of display limit
constexpr int CONSOLESIZE = 85;
constexpr int ZORBWIDTH = 7;
bool _LIGHTTHEME = false; //toggle for light theme
bool _DEBUGMODE = false; //toggle for debug mode

// Define ANSI color escape codes as macros
namespace ansi {
    static std::string RESET = "\x1B[0m";
    constexpr char BLACK[] = "\x1B[30m";
    constexpr char RED[] = "\x1B[31m";
    constexpr char GREEN[] = "\x1B[32m";
    constexpr char YELLOW[] = "\x1B[33m";
    constexpr char BLUE[] = "\x1B[34m";
    constexpr char MAGENTA[] = "\x1B[35m";
    constexpr char CYAN[] = "\x1B[36m";
    constexpr char WHITE[] = "\x1B[37m";

    // Randomly pick a color from the list of ANSI color codes. This function is defined in zUtility.hpp
    std::string GetRandomColor();
}

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

//set the background & text color of the console (F = black, T = white)
void ChangeConsoleTheme() {
    if (!_LIGHTTHEME) {
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
        HWND console = GetConsoleWindow();
        RECT r;
        GetWindowRect(console, &r); //stores the console's current dimensions
        MoveWindow(console, r.left, r.top, CONSOLESIZE*15, CONSOLESIZE*15, TRUE);
        SetConsoleTitle("Zorb Zenith"); //set the title of the console
        ChangeFont(14);
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
    GameOver,
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

    std::string RandomName() {
        std::string name = NAMES[rand() % NAMES.size()] + " " + NAMES[rand() % NAMES.size()];
        return name;
    }

} // namespace zorb

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
 .      .   . .   .   .   . .  +   .    .            +   .  )";

    std::string const planetZorb = R"(
.         _  .          .          .    +     .          .          .      .
        .(_)          .            .            .            .       :      
        .   .      .    .     .     .    .      .   .      . .  .  -+-      
          .           .   .        .           .          /         :  .    
    . .        .  .      /.   .      .    .     .     .  / .      . ' .     
        .  +       .    /     .          .          .   /      .            
       .            .  /         .            .        *   .         .     .
      .   .      .    *     .     .    .      .   .       .  .              
          .           .           .           .           .         +  .    
  . .        .  .       .   .      .    .     .     .    .      .   .       
                                                                            
 .   +      .         ,___/\_._/~~\_...__/\__.._._/~\-,      .         .   .
 ,_    ',\_._/~~\_.-/'                                 `--./\_    .   .     
/~~\_.-/'                                                     `--._/~\/\-.__)";

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
  /     / /  _ \\_  __ \| __ \     /     /_/ __ \  /    \ |  |\   __\|  |  \.,
 /     /_(  <_> )|  | \/| \_\ \   /     /_\  ___/ |   |  \|  | |  |  |   Y  \.
/_______ \\____/ |__|   |___  /  /_______ \\___  >|___|  /|__| |__|  |___|  / 
        \/                  \/           \/     \/     \/                 \/  
)";

    std::string const endCard = R"(
/\/\      o      /\/\
\  /   ./\|/\.   \  /
 \/    ( ^.^ )    \/ 
       b> ^ <d       
)";
}
//endregion
#endif

