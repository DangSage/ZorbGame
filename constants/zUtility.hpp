#ifndef Z_UTILITY_HPP
#define Z_UTILITY_HPP

#include "gameDefs.hpp"
#include <utility>
#include <regex>
#include <random>
#include <stdexcept>

namespace z_debug {
    // Track the counts of Zorb and ZorbAppearance objects
    static int zorbCount;
    static int zorbAppearanceCount;

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

    inline std::string SpaceToPrint(size_t spaces) {
        return std::string(spaces, ' ');
    }
    
    std::string PrintError(const std::string& error) {
        std::cout << ansi::RED << error << ansi::RESET << std::endl;
        return ansi::RED + error + ansi::RESET;
    }

    // Function to get the length of a string without counting escape codes
    inline size_t GetLengthWithoutEscapeCodes(const std::string& input) {
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

    // Function to center-align a SINGLE-line string within a given width
    std::string CenterAlignString(const std::string& input, int width) {
        std::string output;
        std::vector<std::string> lines = SplitMultilineString(input);

        for (const std::string& line : lines) {
            int margin = (width - GetLengthWithoutEscapeCodes(line)) / 2;
            output += SpaceToPrint(margin) + line;
        }
        return output;
    }

     // Function to center-align a MULTI-line string within a given width
    std::string CenterAlignStrings(const std::string& input, int width = CONSOLESIZE) {
        std::string output;
        std::vector<std::string> lines = SplitMultilineString(input);

        for (const std::string& line : lines) {
            int margin = (width - GetLengthWithoutEscapeCodes(line)) / 2;
            output += SpaceToPrint(margin) + line + "\n";
        }
        return output;
    }

    // Function to print a string with color and reset color codes
    void PrintFormattedText(const std::string& text, const std::string& color = "") {
        if (!color.empty()) {
            std::cout << color;
        }
        std::cout << text << ansi::RESET;
    }

    // Function to return a string with colors and color reset codes
    std::string FormattedText(const std::string& text, const std::string& color = "") {
        if (!color.empty())
            return color + text + ansi::RESET;   // Wrap the text with the specified color codes
        else
        {
            OutputDebugString("Log: No color specified at call of FormattedText()\n\tYou may want to pass text element instead.\n");
            return text; // Return the text as is if no color is specified
        }
    }

    // Returns the static number of objects in memory
    void CountGameObjectsInMemory() {
        std::cout << ansi::YELLOW << std::endl
        << zorbCount << " Zorb Object(s) in Memory" << std::endl
        << zorbAppearanceCount << " ZorbAppearance Object(s) in Memory" << std::endl
        //reset color and end of zorb objects
        << ansi::RESET << std::endl;
    }

    // Clears the input buffer to prevent any UI errors and returns the first discarded character
    char clearInputBuffer() {
        std::cin.clear(); // clear any error flags
        char discardedChar = std::cin.get(); // read and return the first discarded character
        std::cin.ignore(1, '\n'); // read and discard remaining characters until newline
        return discardedChar;
    }

} // namespace z_debug

// Function template to validate user input
template <typename T>
T validatedInput(std::initializer_list<T> validInputs) {
    std::string validInputsStr;
    size_t numInputs = validInputs.size();
    // define maxInputLength as the length of the longest input
    size_t maxInputLength = 0;
    for (auto it = validInputs.begin(); it != validInputs.end(); ++it) {
        if (strlen(it) > maxInputLength) {
            maxInputLength = strlen(it);
        }
        validInputsStr += *it;
    }

    std::string input;
    std::cin >> input;
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    //validate input
    while (input.size() > maxInputLength || validInputsStr.find(input) == std::string::npos) {
        //remove the previous output from console output
        std::cout << "\033[F";
        std::cout << "\033[K";
        std::cout << ansi::RED << "Invalid input. Please try again: " << ansi::RESET;
        std::cin.clear();
        std::cin >> input;
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    }
    if (_DEBUGMODE) {
        std::cout << "DEBUG: validatedInput() - input: " << input << std::endl;
        z_debug::clearInputBuffer();
    }
    //return validated input
    return static_cast<T>(input.front());

    //example usage:
    //char validInputs[] = {'a', 'b', 'c'};
    //char input = validatedInput<char>(validInputs);

}

// GetRandomColor() returns a random ANSI color code that we defined already in the gameDefs.hpp header file
std::string ansi::GetRandomColor() {
    const std::string COLOR_CODES[] = {ansi::RED, ansi::GREEN, ansi::YELLOW, ansi::BLUE, ansi::MAGENTA, ansi::CYAN, ansi::WHITE};
    return COLOR_CODES[z_debug::RandomValue<int>(0, COLOR_CODES->size())];
}

#endif // Z_UTILITY_HPP
