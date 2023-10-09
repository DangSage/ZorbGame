#ifndef Z_UTILITY_HPP
#define Z_UTILITY_HPP

#include "gameDefs.hpp"
#include <utility>
#include <regex>
#include <random>
#include <stdexcept>

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

    std::string SpaceToPrint(size_t spaces) {
        return std::string(spaces, ' ');
    }
    
    // Function to get the length of a string without counting escape codes
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
        std::string output;
        std::vector<std::string> lines = SplitMultilineString(input);

        for (const std::string& line : lines) {
            int margin = (width - GetLengthWithoutEscapeCodes(line)) / 2;
            output += SpaceToPrint(margin) + line + '\n';
        }
        return output;
    }

    // Function to print a string with color and reset color codes
    void PrintFormattedText(const std::string& text, const std::string& color = "") {
        if (!color.empty()) {
            std::cout << color;
        }
        std::cout << text << ANSI_RESET;
    }

    // Function to return a string with colors and color reset codes
    std::string FormattedText(const std::string& text, const std::string& color = "") {
        if (!color.empty())
            return color + text + ANSI_RESET;   // Wrap the text with the specified color codes
        else
        {
            OutputDebugString("Log: No color specified at call of FormattedText()\n\tYou may want to pass text element instead.\n");
            return text; // Return the text as is if no color is specified
        }
    }

    // Returns the static number of objects in memory
    void CountGameObjectsInMemory() {
        std::cout << ANSI_YELLOW << std::endl
        << zorbCount << " Zorb Object(s) in Memory" << std::endl
        << zorbAppearanceCount << " ZorbAppearance Object(s) in Memory" << std::endl
        //reset color and end of zorb objects
        << ANSI_RESET << std::endl;
    }

    // Clears the input buffer to prevent any UI errors and returns the first discarded character
    char clearInputBuffer() {
        std::cin.clear(); // clear any error flags
        char discardedChar = std::cin.get(); // read and return the first discarded character
        std::cin.ignore(1, '\n'); // read and discard remaining characters until newline
        return discardedChar;
    }

    // GetRandomColor() returns a random ANSI color code that we defined already in the gameDefs.hpp header file
    std::string GetRandomColor() {
        const std::string ANSI_COLOR_CODES[] = {ANSI_RED, ANSI_GREEN, ANSI_YELLOW, ANSI_BLUE, ANSI_MAGENTA, ANSI_CYAN, ANSI_WHITE};
        return ANSI_COLOR_CODES[RandomValue<int>(0, ANSI_COLOR_CODES->size())];
    }
} // namespace z_debug


#endif // Z_UTILITY_HPP
