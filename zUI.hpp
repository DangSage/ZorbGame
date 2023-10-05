#ifndef Z_UI_HPP
#define Z_UI_HPP

#include "Zorb.hpp" // Include Zorb.hpp for Zorb class usage
#include "zUtility.hpp" // Include zUtility.hpp for z_debug namespace

enum DisplayFormat {
        TABLE,
        ASCII_ART,
        SIMPLE,
        COLOR
    };

class UI {
public:
    // Constructor to initialize the default display format
    UI(DisplayFormat defaultFormat = TABLE) : currentFormat(defaultFormat) {}

    // display driver for vector of zorbs
    void DisplayZorbs(const std::vector<Zorb>& zorbs);
    // display driver for zorb objects
    void DisplayZorb(const Zorb& zorb);
    // accessor for DisplayFormat (string)
    std::string GetDisplayFormatAsString() const;   

    // mutator for DisplayFormat
    void SetDisplayFormat(DisplayFormat format) { currentFormat = format; }

    // Constants
    static const int DISPLAYWIDTH = CONSOLESIZE;

    // title screen display
    void DisplayTitleScreen() const;
    // setting screen display
    void DisplayDebugScreen() const;

    // wipes output stream in the console
    friend void _clearScreen(); 
    // pauses output stream in console until user input
    friend void _pauseSystem();
    // stylized text box sent to output stream
    friend void _createStyledTextBox(const std::string& text);
    // puts out a line halfway across display to output stream
    friend void _createDivider(char borderChar);
private:
    DisplayFormat currentFormat; // Member variable to store the current display format

    // Functions for displaying Zorbs in different formats
    void DisplayZorbsAsTable(const std::vector<Zorb>& zorbs);
    void DisplayZorbsAsAscii(const std::vector<Zorb>& zorbs);
    void DisplayZorbsSimple(const std::vector<Zorb>& zorbs);
    void DisplayZorbsWithColor(const std::vector<Zorb>& zorbs);

    static void _createHorizontalLine(char borderChar);
    static int _countTextLines(const std::string& text);
    
    // Singleton destructors, only one UI object allowed to be declared
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;
};

void _clearScreen() {
    std::cout << std::right << std::setw(0);
    // Code for clearing the screen (platform-dependent)
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux and macOS
    #endif
}

void _pauseSystem() {
    // Code for pausing the system (platform-dependent)
    #ifdef _WIN32
    system("pause"); // Windows
    #else
        std::cout << "Press Enter to continue...";
        std::cin.get();
    #endif
}

int UI::_countTextLines(const std::string& text) {
    int lineCount = 0;
    std::istringstream iss(text);
    std::string line;
    while (std::getline(iss, line)) {
        lineCount++;
    }
    return lineCount;
}

void UI::_createHorizontalLine(char borderChar) {
    for (int i = 0; i < DISPLAYWIDTH; ++i) {
        if(i==0 || i==(DISPLAYWIDTH-1))
            std::cout << ANSI_RESET << '#';
        else
            std::cout << borderChar;
    }
    std::cout << "\n";
}

void _createStyledTextBox(const std::string& text) {
    int topMargin = 1,
        bottomMargin = 1;
    const int margin = 2; // Margin between text and box borders

    // Calculate the total height of the box
    int boxHeight = UI::_countTextLines(text) + topMargin + bottomMargin;

    // Ensure a minimum box height
    boxHeight = std::max(boxHeight, 3);

    // Calculate the remaining available DISPLAYWIDTH after considering margins
    const int availableWidth = UI::DISPLAYWIDTH - 2 * margin;

    // Top border
    UI::_createHorizontalLine('~');

    // Calculate the vertical padding
    int topPadding = (boxHeight - UI::_countTextLines(text) - topMargin - bottomMargin) / 2;
    int bottomPadding = boxHeight - UI::_countTextLines(text) - topMargin - bottomMargin - topPadding;

    // Print top margin
    for (int i = 0; i < topMargin + topPadding; ++i) {
        std::cout << "| " << std::setw(UI::DISPLAYWIDTH - 1) << " |\n";
    }

    std::istringstream iss(text);
    std::string line;

    while (std::getline(iss, line)) {
        // Split long lines into multiple lines to fit within the box DISPLAYWIDTH
        while (line.length() > availableWidth) {
            size_t lastSpace = line.rfind(' ', availableWidth);
            if (lastSpace == std::string::npos || lastSpace == 0) {
                // No space found within the available DISPLAYWIDTH, split at the exact character
                std::string part = line.substr(0, availableWidth);
                line = line.substr(availableWidth);
                int rightPadding = availableWidth - part.length();
                std::cout << "| " << std::string(rightPadding / 2, ' ') << part << std::string(rightPadding - rightPadding / 2, ' ') << std::setw(UI::DISPLAYWIDTH - 4 - availableWidth) << " |\n";
            } else {
                // Split at the last space within the available DISPLAYWIDTH
                std::string part = line.substr(0, lastSpace);
                line = line.substr(lastSpace + 1); // Skip the space
                int rightPadding = availableWidth - part.length();
                std::cout << "| " << std::string(rightPadding / 2, ' ') << part << std::string(rightPadding - rightPadding / 2, ' ') << std::setw(UI::DISPLAYWIDTH - 4 - availableWidth) << " |\n";
            }
        }

        // Calculate the remaining space on the right for the remaining part of the line
        int rightPadding = availableWidth - line.length();

        // Print the remaining part of the line or the short line, centered horizontally
        std::cout << "| " << std::string(rightPadding / 2, ' ') << line << std::string(rightPadding - rightPadding / 2, ' ') << std::setw(UI::DISPLAYWIDTH - 4 - availableWidth) << " |\n";
    }

    // Print bottom padding
    for (int i = 0; i < bottomMargin + bottomPadding; ++i) {
        std::cout << "| " << std::setw(UI::DISPLAYWIDTH - 1) << " |\n";
    }

    // Bottom border
    UI::_createHorizontalLine('~');
}

void _createDivider(char borderChar) {
    for (int i = 0; i < (UI::DISPLAYWIDTH/2); ++i)
            std::cout << borderChar;
    std::cout << std::endl;
}

void UI::DisplayTitleScreen() const {
    _clearScreen();
    _createStyledTextBox("This is a game made by Dang. Dedicated to Thomas Worrall...");

    // Define the title text
    std::string titleText =
        "  __________         ___.      __________            .__  __  .__     \n"
        "  \\____    /__________\\ |__    \\____    /____   ____ |__|/  |_|  |__  \n"
        "    /     //  _ \\_  __ \\ __ \\    /     // __ \\ /    \\|  \\   __\\  |  \\ \n"
        "   /     /(  <_> )  | \\/ \\_\\ \\  /     /\\  ___/|   |  \\  ||  | |   Y  \\\n"
        "  /_______ \\____/|__|  |___  / /_______ \\___  >___|  /__||__| |___|  /\n"
        "          \\/               \\/          \\/   \\/     \\/              \\/ ";
    
    titleText = z_debug::CenterAlignString(titleText, DISPLAYWIDTH);

    // Define menu options
    std::string menuText =
        "\n"
        "1. Start Game\n"
        "2. Instructions\n"
        "3. Debug Settings\n"
        "Q. Quit\n"
        "Enter your choice: ";

    // Display the title text
    _createHorizontalLine('-');
    std::cout << titleText << std::endl;
    _createHorizontalLine('-');
    // Display menu options
    std::cout << menuText;
}

void UI::DisplayDebugScreen() const {
    _clearScreen();
    _createStyledTextBox("DEBUG MENU: Change game settings here!");

    int columnWidth = DISPLAYWIDTH / 2;
    auto columnDisplay = [&](const std::string& option, const std::string& option2) {
        std::cout << std::left << std::setw(columnWidth) << option
                  << std::setw(columnWidth) << option2 << std::endl;
    };

    columnDisplay("Battle Display Type:", "Other Options:");
    columnDisplay(" 1. Display Zorbs (Simple Text)", " A. Show all colors");
    columnDisplay(" 2. Display Zorbs (ASCII Art)", " B. Show all Zorbs");
    columnDisplay(" 3. Display Zorbs (Table Format)", " C. option");
    columnDisplay(" 4. Display Zorbs (Color Coding)", " D. option");

    std::cout << std::right << std::setw(0) << std::endl
    << "Q. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
    std::cout << "your choice: ";
}

void UI::DisplayZorbs(const std::vector<Zorb>& zorbs) {
    switch (currentFormat) {
        case TABLE:
            DisplayZorbsAsTable(zorbs);
            break;
        case ASCII_ART:
            DisplayZorbsAsAscii(zorbs);
            break;
        case SIMPLE:
            DisplayZorbsSimple(zorbs);
            break;
        case COLOR:
            DisplayZorbsWithColor(zorbs);
            break;
        default:
            std::cout << "Invalid display format\n";
    }
}

void UI::DisplayZorb(const Zorb& zorb) {
    switch (currentFormat) {
        case TABLE:
            DisplayZorbsAsTable({zorb});
            break;
        case ASCII_ART:
            DisplayZorbsAsAscii({zorb});
            break;
        case SIMPLE:
            DisplayZorbsSimple({zorb});
            break;
        case COLOR:
            DisplayZorbsWithColor({zorb});
            break;
        default:
            std::cout << "Invalid display format\n";
    }
}

std::string UI::GetDisplayFormatAsString() const {
    switch (currentFormat) {
        case TABLE:
            return "TABLE";
        case ASCII_ART:
            return "ASCII_ART";
        case SIMPLE:
            return "SIMPLE";
        case COLOR:
            return "COLOR";
        default:
            return "Invalid display format";
    }
}

void UI::DisplayZorbsAsTable(const std::vector<Zorb>& zorbs) {
    _createDivider('-');
    std::cout << std::left << std::setw(20) << "Name" 
    << std::setw(10) << "Team" 
    << std::setw(10) << "Power" << "\n" << std::setw(0);
    _createDivider('-');

    for (const Zorb& zorb : zorbs) {
        std::cout << std::left << std::setw(20) << zorb.GetName() 
        << std::setw(10) << "T" + std::to_string(zorb.GetTeamId()) 
        << std::setw(10) << zorb.GetPower() << "\n";
    }

    _createDivider('-');
    std::cout << std::right << std::setw(0);
}

void UI::DisplayZorbsAsAscii(const std::vector<Zorb>& zorbs) {
    // Display the Zorbs as ASCII art as they do in the same way as the ASCII art in the z_debug function PrintAllZorbAppearances()
    std::vector<std::string> charLines;
    std::vector<std::vector<std::string>> rowBuffers;
    
    // Calculate margin
    size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

    for (const Zorb& zorb : zorbs) {
        std::string appearanceText = zorb.GetAppearance();
        std::vector<std::string> appearanceLines = z_debug::SplitMultilineString(appearanceText);
        
        std::string nameText = zorb.GetName();
        std::string powerText = std::to_string(&zorb - &zorbs[0]) + ", " + std::to_string(zorb.GetPower()) + 'p';

        if (charLines.size() < appearanceLines.size()+1) {
            charLines.resize(appearanceLines.size()+2);
        }
        //resize nameText to fit the zorb width, if it doesn't, remove the difference between the margin and the zorb width, then replace the last 2 letters with ..
        if(nameText.size() > ZORBWIDTH)
        {
            nameText.resize(ZORBWIDTH);
            nameText.replace(ZORBWIDTH-2, 2, "..");
        }
        else if(nameText.size() < ZORBWIDTH)
        {
            nameText.resize(ZORBWIDTH, ' ');
        }
        //format the powerText to be centered
        powerText.resize(ZORBWIDTH, ' ');
        
        
        // check if Zorb will fit on the current line in the console window
        if ((charLines.back().length() + z_debug::GetLengthWithoutEscapeCodes(appearanceLines[0]) + ZORBWIDTH) >= CONSOLESIZE) {
            rowBuffers.push_back(charLines);
            charLines.clear();
        } else    // If not, add the current charLines vector to the rowBuffer & clear the charLines vector
        {
            for (size_t i = 0; i < appearanceLines.size(); ++i) {
                charLines[i] += z_debug::SpaceToPrint(margin);
                charLines[i] += appearanceLines[i];
            }
            charLines[appearanceLines.size()] += z_debug::SpaceToPrint(margin) + nameText;
            charLines[appearanceLines.size()+1] += z_debug::SpaceToPrint(margin) + powerText;
        }
    }

    rowBuffers.push_back(charLines);
    for (const auto& rowBuffer : rowBuffers) {
        for (const std::string& charLine : rowBuffer) {
            std::cout << charLine << std::endl;
        }
    }

}

void UI::DisplayZorbsSimple(const std::vector<Zorb>& zorbs) {
    for (const Zorb& zorb : zorbs) {
        std::cout << "Zorb Name: " << zorb.GetName() << "\n";
        std::cout << "Team: T" << zorb.GetTeamId() << "\n";
        std::cout << "Power: " << zorb.GetPower() << "\n";
        std::cout << "\n";
    }
}

void UI::DisplayZorbsWithColor(const std::vector<Zorb>& zorbs) {
    std::cout << "DISPLAY WITH COLOR:\n";

    for (const Zorb& zorb : zorbs) {
        // Assuming ANSI escape codes for text color (may not work on all systems)
        std::string textColor = "\x1B[33m"; // Yellow text color
        std::string resetColor = "\x1B[0m"; // Reset color to default

        std::cout << textColor << "Zorb Name: " << zorb.GetName() << resetColor << "\n";
        std::cout << textColor << "Team: T" << zorb.GetTeamId() << resetColor << "\n";
        std::cout << textColor << "Power: " << zorb.GetPower() << resetColor << "\n";
        std::cout << "\n";
    }
}


namespace z_debug {
    void DisplayDebugColors() {
        int columnWidth = UI::DISPLAYWIDTH / 2;
        auto columnDisplay = [&](const std::string& option, const std::string& option2) {
            std::cout << std::left << std::setw(columnWidth) << option
                    << std::setw(columnWidth) << option2 << std::endl;
        };
        std::cout << "Debug ANSI Colors:\n";
        columnDisplay(FormattedText(" White: Text", ANSI_WHITE), FormattedText("Black: Text", ANSI_BLACK));
        columnDisplay(FormattedText(" Red: Text", ANSI_RED), FormattedText("Green: Text", ANSI_GREEN));
        columnDisplay(FormattedText(" Yellow: Text", ANSI_YELLOW), FormattedText("Blue: Text", ANSI_BLUE));
        columnDisplay(FormattedText(" Magenta: Text", ANSI_MAGENTA), FormattedText("Cyan: Text", ANSI_CYAN));
    }
}
#endif // Z_UI_HPP
