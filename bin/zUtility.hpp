#ifndef Z_UTILITY_HPP
#define Z_UTILITY_HPP

#include "gameDefs.hpp"
#include <utility>
#include <regex>
#include <random>
#include <stdexcept>

void _pauseSystem() {
    // Code for pausing the system (platform-dependent)
    #ifdef _WIN32
    system("pause"); // Windows
    #else
        std::cout << "Press Enter to continue...";
        std::cin.get();
    #endif
}

namespace z_debug {
    // Function template to generate a random value within a specified range
    template <typename T>
    T RandomValue(const T min, const T max) {
        // Check if the type T is a numeric type
        if (!std::is_arithmetic<T>::value) {
            throw std::invalid_argument("RandomValue() requires a numeric type");
        }
        // Check if the min value is greater than the max value
        if (min > max) {
            throw std::invalid_argument("RandomValue() requires min <= max");
        }
        // Create a random device
        std::random_device rd;
        // Create a Mersenne Twister pseudo-random generator
        std::mt19937 gen(rd());
        // Create a uniform distribution of values within the specified range
        std::uniform_int_distribution<T> dist(min, max);
        // Return a random value within the specified range
        return dist(gen);
    }

    inline std::string SpaceToPrint(size_t spaces) {
        return std::string(spaces, ' ');
    }
    
    std::string PrintError(const std::string& error) {
        std::cout << ansi::RED << error << ansi::RESET << std::endl;
        return ansi::RED + error + ansi::RESET;
    }

    // Helper function to get the length of a string with or without counting escape codes, don't count if the escape code is ansi::RESET
    inline size_t GetLength(const std::string& input, bool countEscapeCodes) {
        size_t length = 0;
        bool inEscape = false;

        // if countEscapeCodes, use regex to find all escape codes, then count the number of matches, if the escape code = ansi::RESET, don't count it
        // else, count the number of characters in the string without counting escape codes
        if (countEscapeCodes) {
            std::regex escapeCodeRegex("\x1B\\[[0-9;]*m");
            std::sregex_iterator it(input.begin(), input.end(), escapeCodeRegex);
            std::sregex_iterator end;
            while (it != end) {
                std::smatch match = *it;
                    //add the length of the escape code to the total length
                    length += match.str().length();
                    //if the escape code is ansi::RESET, subtract the length of the escape code from the total length
                ++it;
            }
        } else {
            for (char c : input) {
                if (c == '\x1B') {
                    inEscape = true;
                } else if (inEscape && c == 'm') {
                    inEscape = false;
                } else if (!inEscape) {
                    length++;
                }
            }
        }
        return length;
    }

    // Function to get the length of a string without counting escape codes
    inline size_t GetLengthWithoutEscapeCodes(const std::string& input) {
        return GetLength(input, false);
    }

    // Function to get the length of a string's escape codes only
    inline size_t GetLengthOfEscapeCodes(const std::string& input) {
        return GetLength(input, true);
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

// Function template to choose one random element defined in a list of parameters in this function
// parameters should be able to take (int i, int j, int k, int l, int m) or (std::string a, std::string b, std::string c, std::string d, std::string e)
template <typename T>
T randomChoice(std::initializer_list<T> choices) {
    //use previously defined function to get random value
    int randomIndex = z_debug::RandomValue<int>(0, choices.size()-1);
    //define iterator to the random element
    auto it = choices.begin();
    //advance iterator to the random element
    std::advance(it, randomIndex);

    //return random element
    return *it;
}

// GetRandomColor() returns a random ANSI color code that we defined already in the gameDefs.hpp header file
std::string ansi::GetRandomColor() {
    const std::string COLOR_CODES[] = {ansi::RED, ansi::GREEN, ansi::YELLOW, ansi::BLUE, ansi::MAGENTA, ansi::CYAN, ansi::WHITE};
    return COLOR_CODES[z_debug::RandomValue<int>(0, COLOR_CODES->size())];
}


#endif // Z_UTILITY_HPP