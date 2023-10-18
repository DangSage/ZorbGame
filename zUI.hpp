#ifndef Z_UI_HPP
#define Z_UI_HPP

#include "Zorb.hpp" // Include Zorb.hpp for Zorb class usage
#include "pch.hpp" // Include pch.hpp for precompiled header usage

namespace z_debug {
    void DisplayDebugColors() {
        int columnWidth = CONSOLESIZE / 2;
        auto columnDisplay = [&](const std::string& option, const std::string& option2) {
            int optionLength = z_debug::GetLengthWithoutEscapeCodes(option);
            int option2Length = z_debug::GetLengthWithoutEscapeCodes(option2);
            std::cout << std::left << std::setw(columnWidth) << option
                    << std::setw(columnWidth + (option2Length - optionLength)) << option2 << std::endl;
        };
        std::cout << "Debug ANSI Colors:\n";
        columnDisplay(FormattedText(" White: Text", ansi::WHITE), FormattedText("Black: Text", ansi::BLACK));
        columnDisplay(FormattedText(" Red: Text", ansi::RED), FormattedText("Green: Text", ansi::GREEN));
        columnDisplay(FormattedText(" Yellow: Text", ansi::YELLOW), FormattedText("Blue: Text", ansi::BLUE));
        columnDisplay(FormattedText(" Magenta: Text", ansi::MAGENTA), FormattedText("Cyan: Text", ansi::CYAN));
    }
}

enum DisplayFormat {
        TABLE,
        ASCII_ART,
        SIMPLE,
        COMPACT
    };

class UI {
public:
    // Constructor to initialize the default display format
    UI(DisplayFormat defaultFormat = ASCII_ART) : currentFormat(defaultFormat) {}

    // display driver for vector of zorbs
    void DisplayZorbs(const std::vector<Zorb>& zorbs) const;
    // display driver for zorb objects
    void DisplayZorb(const Zorb& zorb) const;
    // accessor for DisplayFormat (string)
    std::string GetDisplayFormatAsString() const;   

    // mutator for DisplayFormat
    void SetDisplayFormat(DisplayFormat format);

    // Constants
    static const int DISPLAYWIDTH = CONSOLESIZE;

    void screenMainMenu() const;                            // title screen display
    void screenDebugOptions() const;                        // setting screen display
    void screenInfo() const;                                // info screen display
    void screenGameOver() const;                            // gameover screen display
    void screenDebugColors() const;                         // debug color screen display
    void screenDebugZorbs() const;                          // debug zorb screen display

    void screenRecruitment(const Zorb& zorb) const; // recruitment screen display
    //screen for when a zorb is defeated
    void screenDefeated(const Zorb& zorb, const std::string& causeOfDeath, const std::string& killerName, const std::string& killerAppearance);
    //screen for when a zorb is victorious
    void screenVictorious(const Zorb& zorb, const std::string& causeOfDeath, const std::string& victimName, const std::string& victimAppearance);
    //main battle screen that takes 2 arrays of zorbs as parameters
    void screenBattle(const std::vector<Zorb>& team1, const std::vector<Zorb>& team2, const std::string& team1Name, const std::string& team2Name);

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
    void ZorbDisplayTable(const std::vector<Zorb>& zorbs) const;
    void ZorbDisplayAscii(const std::vector<Zorb>& zorbs) const;
    void ZorbDisplaySimple(const std::vector<Zorb>& zorbs) const;
    void ZorbDisplayCompact(const std::vector<Zorb>& zorbs) const;

    static void _createHorizontalLine(char borderChar);
    static int _countTextLines(const std::string& text);
    
    // Singleton destructors, only one UI object allowed to be declared
    UI(const UI&) = delete;
    UI& operator=(const UI&) = delete;
};

//region UI Utility Functions
void _clearScreen() {
    std::cout << std::right << std::setw(0);
    // Code for clearing the screen (platform-dependent)
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux and macOS
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
            std::cout << ansi::RESET << '#';
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
    int rightPadding;

    // Print top margin
    for (int i = 0; i < topMargin + topPadding; ++i) {
        std::cout << "| " << std::setw(UI::DISPLAYWIDTH - 1) << " |\n";
    }
    
    if(_DEBUGMODE) {
        // Print debug text at the top of the box, make sure to do formatting regarding GetLengthWithoutEscapeCodes
        const std::string debugText = (z_debug::FormattedText("[ DEBUG MODE ]", ansi::RED));
        std::cout << "| " << std::string((UI::DISPLAYWIDTH - 6 - z_debug::GetLengthWithoutEscapeCodes(debugText)) / 2, ' ') 
        << debugText << std::string((UI::DISPLAYWIDTH - 6 - z_debug::GetLengthWithoutEscapeCodes(debugText)) / 2, ' ') << " |\n";
    }

    std::istringstream iss(text);
    std::string line;
    std::string part;

    while (std::getline(iss, line)) {
        // Split long lines into multiple lines to fit within the box DISPLAYWIDTH
        while (z_debug::GetLengthWithoutEscapeCodes(line) > availableWidth) {
            size_t lastSpace = line.rfind(' ', availableWidth);
            if (lastSpace == std::string::npos || lastSpace == 0) {
                // No space found within the available DISPLAYWIDTH, split at the exact character
                part = line.substr(0, availableWidth);
                line = line.substr(availableWidth);
            } else {
                // Split at the last space within the available DISPLAYWIDTH
                part = line.substr(0, lastSpace);
                line = line.substr(lastSpace + 1); // Skip the space
            }
            rightPadding = availableWidth - z_debug::GetLengthWithoutEscapeCodes(part);
            std::cout << "| " << std::string(rightPadding / 2, ' ');
            std::cout << part;
            std::cout << std::string(rightPadding - rightPadding / 2, ' ') << std::setw(UI::DISPLAYWIDTH - 4 - availableWidth) << " |\n";
        }

        // Calculate the remaining space on the right for the remaining part of the line
        rightPadding = availableWidth - z_debug::GetLengthWithoutEscapeCodes(line);

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
//end region

#include "zUI_display.hpp" // Include zUI_display.hpp for UI class usage

//region Zorb display functions
void UI::SetDisplayFormat(DisplayFormat format) {
    if (currentFormat == format) {
        std::cout << z_debug::FormattedText("Display format is already set to " + GetDisplayFormatAsString(), ansi::YELLOW) << std::endl;
        z_debug::clearInputBuffer();
        return; // no need to change the format if it's already set
    }

    currentFormat = format;
    _clearScreen();
    _createStyledTextBox("Display format changed to " + z_debug::FormattedText(GetDisplayFormatAsString(), ansi::YELLOW));

    // initialize a vector of 2 zorbs that are initialized as Zorb 1 and Zorb 2 with DEFAULT appearance
    std::vector<Zorb> sample = {
        Zorb(0, 1, "Zorb 1", ZorbAppearance()),
        Zorb(0, 2, "Zorb 2", ZorbAppearance())
    };

    DisplayZorbs(sample);
    std::cout << std::endl;
    _createHorizontalLine('-');
    _pauseSystem();

    //clear the vector sample with the deconstructor
    sample.clear();
}
void UI::DisplayZorbs(const std::vector<Zorb>& zorbs) const{
    switch (currentFormat) {
        case TABLE:
            ZorbDisplayTable(zorbs);
            break;
        case ASCII_ART:
            ZorbDisplayAscii(zorbs);
            break;
        case SIMPLE:
            ZorbDisplaySimple(zorbs);
            break;
        case COMPACT:
            ZorbDisplayCompact(zorbs);
            break;
        default:
            std::cout << "Invalid display format\n";
    }
}
void UI::DisplayZorb(const Zorb& zorb) const{
    switch (currentFormat) {
        case TABLE:
            ZorbDisplayTable({zorb});
            break;
        case ASCII_ART:
            ZorbDisplayAscii({zorb});
            break;
        case SIMPLE:
            ZorbDisplaySimple({zorb});
            break;
        case COMPACT:
            ZorbDisplayCompact({zorb});
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
        case COMPACT:
            return "COMPACT";
        default:
            return "Invalid display format";
    }
}

void UI::ZorbDisplayTable(const std::vector<Zorb>& zorbs) const{
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
void UI::ZorbDisplayAscii(const std::vector<Zorb>& zorbs) const{
    // Display the Zorbs as ASCII art as they do in the same way as the ASCII art in the z_debug function PrintAllZorbAppearances()
    std::vector<std::string> charLines;
    std::vector<std::vector<std::string>> rowBuffers;
    
    // Calculate margin
    size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

    for (auto it = zorbs.begin(); it != zorbs.end(); ++it) {
        const Zorb& zorb = *it;
        std::string appearanceText = zorb.GetAppearance();
        std::vector<std::string> appearanceLines = z_debug::SplitMultilineString(appearanceText);
        
        std::string nameText = zorb.GetName();
        std::string powerText = z_debug::FormattedText(std::to_string(&zorb - &zorbs[0])) + ", " + std::to_string(zorb.GetPower()) + 'p';

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
        if ((charLines.back().length() + z_debug::GetLengthWithoutEscapeCodes(appearanceLines.back()) + ZORBWIDTH) >= CONSOLESIZE) {
            rowBuffers.push_back(charLines);
            charLines.clear();
            //go back by one iteration so that the current zorb is added to the new rowBuffer
            --it;
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
void UI::ZorbDisplaySimple(const std::vector<Zorb>& zorbs) const{
    for (const Zorb& zorb : zorbs) {
        std::cout << zorb.GetName() << ": T" << zorb.GetTeamId() << ", " << zorb.GetPower() << " Power\n";
    }
}
void UI::ZorbDisplayCompact(const std::vector<Zorb>& zorbs) const{
    // Display the Zorbs as ASCII art as they do in the same way as the ASCII art in the z_debug function PrintAllZorbAppearances()
    std::vector<std::string> charLines;
    std::vector<std::vector<std::string>> rowBuffers;
    
    // Calculate margin
    size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

    for (const Zorb& zorb : zorbs) {
        std::string appearanceText = zorb.GetAppearance();
        std::vector<std::string> appearanceLines = z_debug::SplitMultilineString(appearanceText);
        //resize the appearanceLines vector so that it is the 3rd and 4th lines of the zorb
        appearanceLines.erase(appearanceLines.begin(), appearanceLines.begin()+2);

        std::string nameText = zorb.GetName();
        std::string powerText = z_debug::CenterAlignString((z_debug::FormattedText(std::to_string(&zorb - &zorbs[0])) + ", " + std::to_string(zorb.GetPower()) + 'p'), ZORBWIDTH);

        if (charLines.size() < appearanceLines.size()) {
            charLines.resize(appearanceLines.size()+1);
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
            charLines.at(0) += z_debug::SpaceToPrint(margin) + nameText;
            for (size_t i = 1; i < appearanceLines.size(); ++i) {
                charLines[i] += z_debug::SpaceToPrint(margin);
                charLines[i] += appearanceLines[i];
            }
            charLines.at(appearanceLines.size()) += z_debug::SpaceToPrint(margin) + powerText;
        }
    }

    rowBuffers.push_back(charLines);
    for (const auto& rowBuffer : rowBuffers) {
        for (const std::string& charLine : rowBuffer) {
            std::cout << charLine << std::endl;
        }
    }
}
//end region
#endif // Z_UI_HPP
