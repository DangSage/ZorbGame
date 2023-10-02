#ifndef UI_HPP
#define UI_HPP

#include "Zorb.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

class UI {
public:
    enum DisplayFormat {
        TABLE,
        ASCII_ART,
        SIMPLE,
        COLOR
    };
    // Constructor to initialize the default display format
    UI(DisplayFormat defaultFormat = TABLE) : currentFormat(defaultFormat) {}


    // Main Zorb display driver
    void DisplayZorbs(const std::vector<Zorb>& zorbs);
    void DisplayZorb(const Zorb& zorb);
    void SetDisplayFormat(DisplayFormat format) { currentFormat = format; }
    std::string GetDisplayFormatAsString() const;

    void DisplayTitleScreen() const;
    void DisplayDebugScreen() const ;

    // Friend function to create a styled text box
    friend void _createStyledTextBox(const std::string& text);
    friend void _createDivider(char borderChar);
    friend void _clearScreen();
    friend void _pauseSystem();
    // Other utility functions...

private:
    DisplayFormat currentFormat; // Member variable to store the current display format

    // Functions for displaying Zorbs in different formats
    void DisplayZorbsAsTable(const std::vector<Zorb>& zorbs);
    void DisplayZorbsAsAscii(const std::vector<Zorb>& zorbs);
    void DisplayZorbsSimple(const std::vector<Zorb>& zorbs);
    void DisplayZorbsWithColor(const std::vector<Zorb>& zorbs);

    // Function to create a horizontal line
    static void _createHorizontalLine(char borderChar);
    static int _countTextLines(const std::string& text);

    // Constants
    static const int DISPLAYWIDTH = 75;
};

// IMPLEMENTATIONS AFTER THIS POINT //

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
        cout << "Press Enter to continue...";
        cin.get();
    #endif
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

void _createDivider(char borderChar) {
    for (int i = 0; i < (UI::DISPLAYWIDTH/2); ++i)
            std::cout << borderChar;
    std::cout << std::endl;
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

// Title screen display
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

    // Define menu options
    std::string menuText =
        "\n"
        "1. Start Game\n"
        "2. Instructions\n"
        "Q. Quit\n"
        "Enter your choice: ";

    // Display the title text
    _createHorizontalLine('-');
    std::cout << titleText << std::endl;
    _createHorizontalLine('-');
    // Display menu options
    std::cout << menuText;
}

/*void UI::DisplayDebugScreen() const {
    std::vector<Zorb> sample_zorbs;
    sample_zorbs.push_back(Zorb(20, 1, "Zorb 1"));
    sample_zorbs.push_back(Zorb(20, 2, "Zorb 2"));
    while (true) {
        _clearScreen();
        char input;
        std::cout << "Battle Display Type:" << std::endl;
        std::cout << "1. Display Zorbs (Simple Text)" << std::endl;
        std::cout << "2. Display Zorbs (ASCII Art)" << std::endl;
        std::cout << "3. Display Zorbs (Table Format)" << std::endl;
        std::cout << "4. Display Zorbs (Color Coding)" << std::endl;
        std::cout << "5. Display Zorbs (Status Indicators)" << std::endl;
        std::cout << "Q. Quit back to Title Screen" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> input;

        switch (input) {
            case '1':
                this->SetDisplayFormat(SIMPLE);
                break;
            case '2':
                _battleManager.setCurrentDisplayType(ASCII_ART);
                break;
            case '3':
                _battleManager.setCurrentDisplayType(TABLE);
                break;
            case '4':
                _battleManager.setCurrentDisplayType(COLOR);
                break;
            case '5':
                _battleManager.setCurrentDisplayType(STATUS);
                break;
            case 'Q':
            case 'q':
                return; // Return to the previous menu or exit the function
            default:
                break;
        }
        
        // Optionally, you can display the Zorbs here or perform any other action.
        UserInterface::displayZorbsArray(_battleManager, sample_zorbs);
        // You should define this behavior based on your application's requirements.
        // For now, this code simply pauses the system.
        pauseSystem();
    }
    sample_zorbs.clear();
}
*/
// Main Zorb display driver
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

// Display Zorbs as a table
void UI::DisplayZorbsAsTable(const std::vector<Zorb>& zorbs) {
    _createDivider('-');
    std::cout << std::left << std::setw(20) << "Name" << std::setw(10) << "Team" << std::setw(10) << "Power" << "\n" << std::setw(0);
    _createDivider('-');

    for (const Zorb& zorb : zorbs) {
        std::cout << std::left << std::setw(20) << zorb.GetName() << std::setw(10) << "T" + std::to_string(zorb.GetTeamId()) << std::setw(10) << zorb.GetPower() << "\n";
    }

    _createDivider('-');
    std::cout << std::right << std::setw(0);
}

void DisplayZorbsAsAscii(const std::vector<Zorb>& zorbs) {
        int row = 1;  // Starting row
        int col = 1;  // Starting column

        for (const Zorb& zorb : zorbs) {
            // Move the cursor to the specified row and column
            std::cout << "\x1B[" << row << ";" << col << "H";

            // Display Zorb information next to ASCII art
            std::cout << zorb.GetAppearance() << "  " << zorb.GetTeamId() << ':' << zorb.GetPower() << ' ' << zorb.GetName() << std::endl;

            // Adjust the row for the next Zorb
            row += 4;  // Assuming the ASCII art has 4 rows
        }
    }

// Display Zorbs in a simple format
void UI::DisplayZorbsSimple(const std::vector<Zorb>& zorbs) {
    for (const Zorb& zorb : zorbs) {
        std::cout << "Zorb Name: " << zorb.GetName() << "\n";
        std::cout << "Team: T" << zorb.GetTeamId() << "\n";
        std::cout << "Power: " << zorb.GetPower() << "\n";
        std::cout << "\n";
    }
}

// Display Zorbs with color (simplified example, may not work on all systems)
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
#endif // UI_HPP
