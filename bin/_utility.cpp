#include "zUtility.hpp"

// Create a random device
std::random_device rd;
// Create a Mersenne Twister pseudo-random generator
std::mt19937 gen(rd());

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
    void _tag(const std::string& a) {
        std::cout << std::endl << "\t[" << a << "] ";
        _pauseSystem();
    }
    
    // Function to print an error message and throw a zException with the error message
    void PrintError(zException& error) {
        std::cout << std::endl;
        error.message();
    }

} // namespace z_debug

namespace z_util {
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
    std::string CenterAlignStrings(const std::string& input, int width) {
        std::string output;
        std::vector<std::string> lines = SplitMultilineString(input);

        for (const std::string& line : lines) {
            int margin = (width - GetLengthWithoutEscapeCodes(line)) / 2;
            output += SpaceToPrint(margin) + line + "\n";
        }
        return output;
    }

    // Function to print a string with color and reset color codes
    void PrintFormattedText(const std::string& text, const std::string_view& color) {
        if (!color.empty()) {
            std::cout << color;
        }
        std::cout << text << ansi::RESET;
    }

    // Function to return a string with colors and color reset codes
    std::string FormattedText(const std::string& text, const std::string_view& color) {
        if (!color.empty())
            return std::string(color) + text + ansi::RESET;   // Wrap the text with the specified color codes
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // discard remaining characters until newline
        return discardedChar;
    }
    
    namespace random {
        int value() {
            std::uniform_int_distribution<int> dist(1, 2);
            return dist(gen);
        }

        // pick a random color from the color codes
        std::string_view getColor() {
            constexpr std::string_view COLOR_CODES[] = {ansi::RED, ansi::GREEN, ansi::YELLOW, ansi::BLUE, ansi::MAGENTA, ansi::CYAN, ansi::WHITE};
            return COLOR_CODES[z_util::random::value<int>(0, COLOR_CODES->size())];
        }
    }
}

namespace zorb {
    std::string RandomName() {
        std::string name1 = NAMES[z_util::random::value(static_cast<size_t>(0), NAMES.size()-1)];
        std::string name2 = NAMES[z_util::random::value(static_cast<size_t>(0), NAMES.size()-1)];

        //return name1 + " " + name2 if name 2 exists, otherwise just return name1 without the space
        return name2.size() > 0 ? name1 + " " + name2 : name1;
    }
} // namespace zorb