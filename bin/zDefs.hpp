#ifndef GAME_DEFS_HPP
#define GAME_DEFS_HPP
#ifdef _WIN32
    #include <Windows.h>
#endif

#define IDI_MYICON 101

#include <iostream> // Include necessary standard library headers
#include <array>
#include <vector>
#include <string>
#include <sstream>

//make magic numbers for exit command 'Q'
#define USEREXIT 'Q'
#define USERGIT 'G'
#define USERHELP '?'

// make magic numbers for team identification
constexpr int PLAYERTEAM = 1;
constexpr int ENEMYTEAM = 2;

// define size of display limit
constexpr int CONSOLESIZE = 100;
constexpr int ZORBWIDTH = 7;
extern bool _LIGHTTHEME;
extern bool _DEBUGMODE;

namespace ansi {
    extern std::string RESET;
    constexpr std::string_view BLACK = "\x1B[30m";
    constexpr std::string_view RED = "\x1B[31m";
    constexpr std::string_view GREEN = "\x1B[32m";
    constexpr std::string_view YELLOW = "\x1B[33m";
    constexpr std::string_view BLUE = "\x1B[34m";
    constexpr std::string_view MAGENTA = "\x1B[35m";
    constexpr std::string_view CYAN = "\x1B[36m";
    constexpr std::string_view WHITE = "\x1B[37m";
    constexpr std::string_view GRAY = "\x1B[37;2m";

    constexpr std::string_view DLINE = "\033[1A\033[2K"; // ANSI escape code to delete the last line from the console
    constexpr std::string_view DLINE2 = "\033[2K"; // ANSI escape code to delete the current line from the console
    constexpr std::string_view UPLINE = "\033[1A"; // ANSI escape code to move the cursor up one line
    constexpr std::string_view ENDLINE = "\033[1F\033[999D"; // Ansi escape code to move the cursor the end of the last line
}

// Define which ANSI color code to use for the Zorb's color (color groups)
constexpr std::array<std::string_view, 2> CG_PLAYER = { ansi::GREEN, ansi::CYAN };
constexpr std::array<std::string_view, 2> CG_ENEMY = { ansi::MAGENTA, ansi::RED };

extern int turnCounter; //turn counter for the battle segment
extern int winCounter; //win counter for the game
extern int casualtyCounter; //casualties counter for the game
extern int battleCounter; //battle counter for the game

//force terminal to accept ansi color codes depending on operating system, make sure it does so for ALL escape
void ForceTerminalColor();

// function to change font size of terminal executeable depending on operating system
void ChangeFont(int spacing);

//set the background & text color of the console (F = black, T = white)
void ChangeConsoleTheme();

//function that changes the debug mode
void ChangeDebugMode();

//function that forces the terminal to have borders and a title
void ForceTerminalBorder();

void ForceTerminal();

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
    // Random enemy team names for the battle segment
    const std::array<std::string_view, 25> TEAMPREFIX = {
        "Local",
        "Ragged",
        "Raggedy",
        "Hairy",
        "Fuzzy",
        "Furry",
        "Furrious",
        "Vagrant",
        "Vagabond",
        "Zorg",
        "Zleepy",
        "Exhausted",
        "Eepy",
        "Deadly",
        "Wandering",
        "Aimless",
        "Lost",
        "Able",
        "Hunter",
        "Feral",
        "Wild",
        "Rabid",
        "Ravenous",
        "Gung-Ho",
        "Zorbian"
    };
    const std::array<std::string_view, 25> TEAMSUFFIX = {
        "Blob",
        "Crew",
        "Zorbos",
        "Zorbians",
        "Zorbites",
        "Gang",
        "Blorgal",
        "Fundamentalists",
        "Zorbianists",
        "Locals",
        "Zraban",
        "School",
        "Armed Militia",
        "Wranglers",
        "Bunch",
        "Platoon",
        "Squad",
        "Hit Squad",
        "Zorbs",
        "Zorbites",
        "Band",
        "Zealots",
        "Hunters",
        "Meowcenaries",
        "Guerilla"
    };

    const std::string N_DODGE = "DODGED";
    const std::string N_IMPLODE = "IMPLODE";
} // namespace zorb

extern std::string BARBERNAME;

namespace text {
    namespace battle {
        const std::initializer_list<std::string_view> encounter = {
            "You have encountered a Zorb!",
            "You are encountered by a group of Zorbs!",
            "You have been ambushed by a Zorb!",
            "You have been ambushed by a group of Zorbs!",
            "You spot some Zorb or 2 in the distance!",
            "You've been spotted by a group of enemies!",
            "You've been ambushed by a group of enemies!",
            "These Zorbs are looking for a fight!",
            "These Zorbs don't look friendly!",
            "These Zorbs look like they want to fight!"
        };

        const std::initializer_list<std::string_view> attackResult = {
            "ANNIHILATED",
            "DESTROYED",
            "slumped",
            "flattened",
            "obliterated",
            "CRUSHED",
            "smashed",
            "squashed",
            "crushed",
            "crunched",
            "crumpled",
            "pulverized",
            "demolished",
            "wrecked",
            "put into a deep rest",
            "put to sleep",
            "put to rest",
            "put to bed",
        };
        const std::initializer_list<std::string_view> implode = {
            "fought", 
            "battled", 
            "clashed", 
            "duked it out", 
            "went head to head",
            "went toe to toe",
            "went at it",
            "destroyed eachother",
            "EXPLODED",
            "imploded",
            "combusted",
            "self-destructed",
            "self-imploded",
            "self-combusted",
            "disintegrated"
        };
    }

    // stringstreams for the controls of the game
    namespace help {
        const std::string battle = 
            "During a battle, you have several options:\n\n"
            "1. Attack: Choose a Zorb from your party and an enemy Zorb to engage in a power comparison. \nEach Zorb has a power level and a position in the party order.\n\n"
            "2. Dodge: Select a Zorb to dodge, which temporarily increases its power and changes its position in the party order.\n\n"
            "3. Run Away: Attempt to disengage from the fight. This is a chance-based action and may not always succeed.\n\n"
            "You can return to the previous battle options by typing -1.\n\n";

        const std::string general = 
        "Each menu option has a corresponding command:\n\n"
        "In the input query, type the character of the option you want to select.\n";

        const std::string debug =
        "This menu allows you to toggle and view certain debug settings,\n"
        "As well as some display options for the game.\n\n"
        "- Battle Display Type: The way zorbs will appear in battle. All options in this column affect this value.\n"
        "- Toggle Theme: Toggles the game between light and dark mode.\n"
        "- Toggle Debug Mode: Toggles debug mode on and off. Explicitly for Development\n\n"
        "Most of these options are self-explanatory, but if you need help, type ? at any time.\n\n";

    }
}

#endif

