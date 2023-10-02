#ifndef Z_UTILITY_HPP
#define Z_UTILITY_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>
#include <map>
#include <unordered_map>
#include <random>
#include <windows.h> // Include Windows.h for Windows-specific functions

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

namespace ANSIConsole {
    void EnableANSIEscapeSupport() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) {
            return; // Failed to get console handle
        }

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) {
            return; // Failed to get console mode
        }

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        if (!SetConsoleMode(hOut, dwMode)) {
            return; // Failed to set console mode
        }
    }
}

// Function template to generate a random value within a specified range
template <typename T>
T RandomValue(const T min, const T max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> distribution(min, max);
        return distribution(gen);
    } else if constexpr (std::is_floating_point_v<T>) {
        std::uniform_real_distribution<T> distribution(min, max);
        return distribution(gen);
    } else {
        // Handle other types (e.g., custom types) here or throw an error
        throw std::invalid_argument("Unsupported data type");
    }
}


#endif // Z_UTILITY_HPP
