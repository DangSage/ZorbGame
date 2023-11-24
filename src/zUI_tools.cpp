#include "pch.hpp"
#include "zUI.hpp"
#include "Zorb.hpp"

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
    for (int i = 0; i < CONSOLESIZE; ++i) {
        if(i==0 || i==(CONSOLESIZE-1))
            std::cout << ansi::RESET << '#';
        else
            std::cout << borderChar;
    }
    std::cout << "\n";
}
void UI::SetDisplayFormat(DisplayFormat format) {
    if (currentFormat == format) {
        std::cout << z_util::FormattedText("Display format is already set to " + GetDisplayFormatAsString(), ansi::YELLOW) << std::endl;
        z_util::clearInputBuffer();
        return; // no need to change the format if it's already set
    }

    currentFormat = format;
    _clearScreen();
    _createStyledTextBox("Display format changed to " + z_util::FormattedText(GetDisplayFormatAsString(), ansi::YELLOW));

    // initialize a vector of 2 zorbs that are initialized as Zorb 1 and Zorb 2 with DEFAULT appearance
    std::vector<Zorb> sample = {
        Zorb(ZorbAppearance(), 0, 1, "Zorb 1"),
        Zorb(ZorbAppearance(), 0, 2, "Zorb 2")
    };

    DisplayZorbs(sample);
    std::cout << std::endl;
    _createHorizontalLine('-');
    _pauseSystem();

    //clear the vector sample with the deconstructor
    sample.clear();
}

void UI::DisplayZorbs(const std::vector<Zorb>& zorbs, char displaySide) const{
    if(zorbs.size() < 1) {
        std::cout << "No Zorbs in memory" << std::endl;
        return;
    }
    else if(zorbs.size() == 1) {
        DisplayZorb(zorbs.at(0), displaySide);
        return;
    }
    
    switch (currentFormat) {
        case TABLE:
            ZorbDisplayTable(zorbs, displaySide);
            break;
        case ASCII_ART:
            ZorbDisplayAscii(zorbs, displaySide);
            break;
        case SIMPLE:
            ZorbDisplaySimple(zorbs, displaySide);
            break;
        case COMPACT:
            ZorbDisplayCompact(zorbs, displaySide);
            break;
        default:
            std::cout << "Invalid display format\n";
    }
    std::cout << std::endl;
}
void UI::DisplayZorb(const Zorb& zorb, char displaySide) const{
    switch (currentFormat) {
        case TABLE:
            ZorbDisplayTable({zorb}, displaySide);
            break;
        case ASCII_ART:
            ZorbDisplayAscii({zorb}, displaySide);
            break;
        case SIMPLE:
            ZorbDisplaySimple({zorb}, displaySide);
            break;
        case COMPACT:
            ZorbDisplayCompact({zorb}, displaySide);
            break;
        default:
            std::cout << "Invalid display format\n";
    }
    std::cout << std::endl;
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

void UI::ZorbDisplayTable(const std::vector<Zorb>& zorbs, char displaySide) const{
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
void UI::ZorbDisplayAscii(const std::vector<Zorb>& zorbs, char displaySide) const{
    // Display the Zorbs as ASCII art as they do in the same way as the ASCII art in the z_debug function PrintAllZorbAppearances()
    std::vector<std::string> charLines;
    std::vector<std::vector<std::string>> rowBuffers;
    
    // Calculate margin
    size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

    for (auto it = zorbs.begin(); it != zorbs.end(); ++it) {
        const Zorb& zorb = *it;
        std::string appearanceText = zorb.GetAppearance();
        std::vector<std::string> appearanceLines = z_util::SplitMultilineString(appearanceText);
        
        std::string nameText = zorb.GetName();
        std::string powerText;
        
        zorb.GetPower() > 0 ? powerText = z_util::FormattedText(std::to_string((&zorb - &zorbs[0])+1) + ", " + std::to_string(zorb.GetPower()) + 'p') 
        : powerText = " [X,X] ";

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
        if ((charLines.back().length() + z_util::GetLengthWithoutEscapeCodes(appearanceLines.back()) + ZORBWIDTH) >= CONSOLESIZE) {
            rowBuffers.push_back(charLines);
            charLines.clear();
            //go back by one iteration so that the current zorb is added to the new rowBuffer
            --it;
        } else    // If not, add the current charLines vector to the rowBuffer & clear the charLines vector
        {
            for (size_t i = 0; i < appearanceLines.size(); ++i) {
                charLines[i] += z_util::SpaceToPrint(margin);
                charLines[i] += appearanceLines[i];
            }
            charLines[appearanceLines.size()] += z_util::SpaceToPrint(margin) + nameText;
            charLines[appearanceLines.size()+1] += z_util::SpaceToPrint(margin) + powerText;
        }
    }

    rowBuffers.push_back(charLines);
    for (const auto& rowBuffer : rowBuffers) {
        for (const std::string& displayLine : rowBuffer) {
            if(displaySide == 'R') {
                int escapeCodeLength = z_util::GetLengthOfEscapeCodes(displayLine);
                if(escapeCodeLength == 0)
                    std::cout << std::right << std::setw(CONSOLESIZE-margin) << displayLine << std::endl;
                else
                    std::cout << std::right << std::setw(CONSOLESIZE+escapeCodeLength-margin) << displayLine << std::endl;
            }
            else
                std::cout << std::left << std::setw(0) << displayLine << std::endl;
        }
    }
}
void UI::ZorbDisplaySimple(const std::vector<Zorb>& zorbs, char displaySide) const{
    // Display the zorbs as a simple with the index of the zorb, name, and power on each line
    for (const Zorb& zorb : zorbs) {
        std::ostringstream oss;
        if(displaySide == 'R') {
            oss << zorb.GetName() << ": " << (zorb.GetPower() > 0 ? std::to_string(zorb.GetPower()) + " power" : "X")
                << std::setw(3) << std::right << (zorb.GetPower() > 0 ? " [" + std::to_string((&zorb - &zorbs[0])+1) + "]" : " [X]");

            int escapeCodeLength = z_util::GetLengthOfEscapeCodes(oss.str());
            if(escapeCodeLength == 0)
                std::cout << std::right << std::setw(CONSOLESIZE) << oss.str() << std::endl;
            else
                std::cout << std::right << std::setw(CONSOLESIZE+escapeCodeLength) << oss.str() << std::endl;
        }
        else {
            oss << std::setw(3) << std::right << (zorb.GetPower() > 0 ? '[' + std::to_string((&zorb - &zorbs[0])+1) + "] " : "[X] ")
                << zorb.GetName() << ": " << (zorb.GetPower() > 0 ? std::to_string(zorb.GetPower()) + " power" : "X");
            std::cout << std::left << std::setw(0) << oss.str() << std::endl;
        }
    }
}
void UI::ZorbDisplayCompact(const std::vector<Zorb>& zorbs, char displaySide) const{
    // Display the Zorbs as ASCII art as they do in the same way as the ASCII art in the z_debug function PrintAllZorbAppearances()
    std::vector<std::string> charLines;
    std::vector<std::vector<std::string>> rowBuffers;
    
    // Calculate margin
    size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

    for (const Zorb& zorb : zorbs) {
        std::string appearanceText = zorb.GetAppearance();
        std::vector<std::string> appearanceLines = z_util::SplitMultilineString(appearanceText);
        //resize the appearanceLines vector so that it is the 3rd and 4th lines of the zorb
        appearanceLines.erase(appearanceLines.begin(), appearanceLines.begin()+2);

        std::string nameText = zorb.GetName();
        std::string powerText = z_util::CenterAlignString((z_util::FormattedText(std::to_string((&zorb - &zorbs[0])+1)) + ":" + std::to_string(zorb.GetPower()) + 'p'), ZORBWIDTH);

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
        if ((charLines.back().length() + z_util::GetLengthWithoutEscapeCodes(appearanceLines[0]) + ZORBWIDTH) >= CONSOLESIZE) {
            rowBuffers.push_back(charLines);
            charLines.clear();
        } else    // If not, add the current charLines vector to the rowBuffer & clear the charLines vector
        {
            charLines.at(0) += z_util::SpaceToPrint(margin) + nameText;
            for (size_t i = 1; i < appearanceLines.size(); ++i) {
                charLines[i] += z_util::SpaceToPrint(margin);
                charLines[i] += appearanceLines[i];
            }
            charLines.at(appearanceLines.size()) += z_util::SpaceToPrint(margin) + powerText;
        }
    }

    rowBuffers.push_back(charLines);
    for (const auto& rowBuffer : rowBuffers) {
        for (const std::string& displayLine : rowBuffer) {
            if(displaySide == 'R') {
                int escapeCodeLength = z_util::GetLengthOfEscapeCodes(displayLine);
                if(escapeCodeLength == 0)
                    std::cout << std::right << std::setw(CONSOLESIZE-margin) << displayLine << std::endl;
                else
                    std::cout << std::right << std::setw(CONSOLESIZE+escapeCodeLength-margin) << displayLine << std::endl;
            }
            else
                std::cout << std::left << std::setw(0) << displayLine << std::endl;
        }
    }
}

// Friend function definitions
void _clearScreen() {
    std::cout << std::right << std::setw(0);
    // Code for clearing the screen (platform-dependent)
    #ifdef _WIN32
        system("cls"); // Windows
    #else
        system("clear"); // Linux and macOS
    #endif
    }
void _createStyledTextBox(const std::string& text) {
    int topMargin = 1,
        bottomMargin = 1;
    const int margin = 2; // Margin between text and box borders

    // Calculate the total height of the box
    int boxHeight = UI::_countTextLines(text) + topMargin + bottomMargin;

    // Ensure a minimum box height
    boxHeight = std::max(boxHeight, 3);

    // Calculate the remaining available CONSOLESIZE after considering margins
    const int availableWidth = CONSOLESIZE - (2 * margin);

    // Top border
    UI::_createHorizontalLine('~');

    // Calculate the vertical padding
    int topPadding = (boxHeight - UI::_countTextLines(text) - topMargin - bottomMargin) / 2;
    int bottomPadding = boxHeight - UI::_countTextLines(text) - topMargin - bottomMargin - topPadding;
    int hPadding;

    // Print top margin
    for (int i = 0; i < topMargin + topPadding; ++i) {
        std::cout << "| " << std::setw(CONSOLESIZE - 1) << " |\n";
    }

    std::istringstream iss(text);
    std::string line;
    std::string part;

    while (std::getline(iss, line)) {     
        // Split the line into parts that are less than or equal the available width
        std::vector<std::string> parts;
        std::istringstream iss_line(line);
        std::string part_line;

        // account for newline characters in the text and split the line into parts
        while (std::getline(iss_line, part_line, '\n')) {
            std::istringstream iss_part(part_line);
            std::string part_part;
            while (std::getline(iss_part, part_part, ' ')) {
                if (z_util::GetLengthWithoutEscapeCodes(part + part_part) >= availableWidth) {
                    parts.push_back(part);
                    part = "";
                }
                part += part_part + ' ';
            }
        }

        // Push the remaining part into the parts vector
        if (!part.empty()) {
            parts.push_back(part);
        }

        // Print each part on a new line with right border and padding, centered horizontally without breaking words
        for (const std::string& part : parts) {
            // Calculate the remaining space on the right for the remaining part of the line
            hPadding = availableWidth - z_util::GetLengthWithoutEscapeCodes(part);

            // Print the part with padding and centered horizontally
            std::cout << "| " << std::string(hPadding / 2, ' ') << part << std::string(hPadding - hPadding / 2, ' ') << std::setw(CONSOLESIZE - availableWidth - 1) << " |\n";
        }
        part = "";
    }

    // Print bottom padding
    for (int i = 0; i < bottomMargin + bottomPadding; ++i) {
        std::cout << "| " << std::setw(CONSOLESIZE - 1) << " |\n";
    }
    // Bottom border
    UI::_createHorizontalLine('~');
}
void _createDivider(char borderChar) {
    for (int i = 0; i < (CONSOLESIZE/2); ++i)
            std::cout << borderChar;
    std::cout << std::endl;
}

namespace z_debug {
    void DisplayDebugColors() {
        int columnWidth = CONSOLESIZE / 2;
        auto columnDisplay = [&](const std::string& option, const std::string& option2) {
            int optionLength = z_util::GetLengthWithoutEscapeCodes(option);
            int option2Length = z_util::GetLengthWithoutEscapeCodes(option2);
            std::cout << std::left << std::setw(columnWidth) << option
                    << std::setw(columnWidth + (option2Length - optionLength)) << option2 << std::endl;
        };
        std::cout << "Debug ANSI Colors:\n";
        columnDisplay(z_util::FormattedText(" White: Text", ansi::WHITE), z_util::FormattedText("Black: Text", ansi::BLACK));
        columnDisplay(z_util::FormattedText(" Red: Text", ansi::RED), z_util::FormattedText("Green: Text", ansi::GREEN));
        columnDisplay(z_util::FormattedText(" Yellow: Text", ansi::YELLOW), z_util::FormattedText("Blue: Text", ansi::BLUE));
        columnDisplay(z_util::FormattedText(" Magenta: Text", ansi::MAGENTA), z_util::FormattedText("Cyan: Text", ansi::CYAN));
    }
}
