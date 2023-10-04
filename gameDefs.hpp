#ifndef GAME_DEFS_HPP
#define GAME_DEFS_HPP

#define STR(x) #x
#define STRINGIZE(x) STR(x)

// Define ANSI escape codes as macros
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

#endif

