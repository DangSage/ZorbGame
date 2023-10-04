#ifndef Z_UTILITY_HPP
#define Z_UTILITY_HPP

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <utility>
#include <regex>
#include <map>
#include <unordered_map>
#include <random>
#include <stdexcept>
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

// define size of display limit
#define CONSOLESIZE 75
#define ZORBWIDTH 7

namespace z_debug {
    // Track the counts of Zorb and ZorbAppearance objects
    static int zorbCount = 0;
    static int zorbAppearanceCount = 0;

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

    // Function to split a multi-line string into individual lines
    std::vector<std::string> SplitMultilineString(const std::string& multilineString) {
        std::vector<std::string> lines;
        std::istringstream iss(multilineString);
        std::string line;

        while (std::getline(iss, line)) {
            lines.push_back(line);
        }

        return lines;
    }
    // Function to center-align a string within a given width
    std::string CenterAlignString(const std::string& input, int width) {
        std::string result;
        int inputLength = static_cast<int>(input.length());

        if (inputLength >= width) {
            // If the input string is longer than the width, return it as is
            return input;
        }

        int padding = (width - inputLength) / 2;
        result.append(padding, ' ');
        result.append(input);
        result.append(padding, ' ');

        // Adjust for odd width
        if (inputLength % 2 != 0) {
            result.push_back(' ');
        }

        return result;
    }

    void PrintFormattedText(const std::string& text, const std::string& color = "") {
        if (!color.empty()) {
            std::cout << color;
        }
        std::cout << text << ANSI_RESET;
    }

    std::string FormattedText(const std::string& text, const std::string& color = "") {
        if (!color.empty())
            return color + text + ANSI_RESET;   // Wrap the text with the specified color codes
        else
            return text; // Return the text as is if no color is specified
    }

    void CountGameObjectsInMemory() {
        std::cout << zorbCount << " Zorb Object(s) in Memory" << std::endl;
        std::cout << zorbAppearanceCount << " ZorbAppearance Object(s) in Memory" << std::endl;
    }

    std::string SpaceToPrint(size_t spaces) {
        return std::string(spaces, ' ');
    }

    size_t GetLengthWithoutEscapeCodes(const std::string& input) {
        size_t length = 0;
        bool inEscape = false;

        for (char c : input) {
            if (c == '\033') { // Check for escape code
                inEscape = true;
                continue;
            }
            
            if (inEscape) {
                if (c >= 'A' && c <= 'Z') {
                    inEscape = false;
                }
                continue;
            }
            length++;
        }
        return length;
    }
}

#endif // Z_UTILITY_HPP
