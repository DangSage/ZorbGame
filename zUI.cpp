#include "zUI.hpp"

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
        std::string powerText = z_util::CenterAlignString((z_util::FormattedText(std::to_string(&zorb - &zorbs[0])) + ", " + std::to_string(zorb.GetPower()) + 'p'), ZORBWIDTH);

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
        for (const std::string& charLine : rowBuffer) {
            std::cout << charLine << std::endl;
        }
    }
}

void UI::screenMainMenu() const {
    _clearScreen();
    _createStyledTextBox("This is a game made by Dang. Report any bugs to the repo:\n[0] https://github.com/DangSage/ZorbGame");
    // Define the title text
    std::string titleText = z_art::gameTitle;
    titleText = z_util::CenterAlignStrings(titleText);

    // Define menu options
    std::string menuText =
        "<3 Dedicated to Thomas Worrall <3\n\n"
        "1. Start Game\n"
        "2. Instructions\n"
        "3. Debug Settings\n\n"
        "Q. Quit to Desktop\n";

    // Display the title text
    _createHorizontalLine('-');
    std::cout << titleText << std::endl;
    z_debug::PrintZorbAppearances(4, false, ansi::CYAN);
    _createHorizontalLine('-');
    // Display menu options
    std::cout << menuText;
    _createHorizontalLine('-');
    std::cout << "Enter your choice: ";
}
void UI::screenDebugOptions() const {
    _clearScreen();
    _createStyledTextBox("DEBUG MENU: Change game settings here!");

    int columnWidth = CONSOLESIZE / 2;
    auto columnDisplay = [&](const std::string& option, const std::string& option2) {
        int optionLength = z_util::GetLengthWithoutEscapeCodes(option);
        int option2Length = z_util::GetLengthWithoutEscapeCodes(option2);
        std::cout << std::left << std::setw(columnWidth) << option
                  << std::setw(columnWidth + (option2Length - optionLength)) << option2 << std::endl;
    };

    std::cout << z_art::optionsScreen << std::endl;
    _createHorizontalLine('-');
    columnDisplay(("Battle Display Type: " + z_util::FormattedText(GetDisplayFormatAsString(), ansi::YELLOW) + '\t'), 
        "Other Options: ");
    std::cout << std::endl;
    columnDisplay(" 1. Display Zorbs (Simple Text)", " A. Show all colors");
    columnDisplay(" 2. Display Zorbs (ASCII Art)", " B. Show all Zorbs");
    columnDisplay(" 3. Display Zorbs (Table Format)", " C. Toggle Theme: " + z_util::FormattedText((_LIGHTTHEME == false ? "Light" : "Dark"), ansi::BLUE));
    columnDisplay(" 4. Display Zorbs (Compact Art)", " D. Toggle Debug Mode: " + z_util::FormattedText((_DEBUGMODE == true ? "On" : "Off"), ansi::RED));

    std::cout << std::right << std::setw(0) << std::endl
              << "Q. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
    std::cout << "Enter your choice: ";
}
void UI::screenDebugColors() const {
    _clearScreen();
    _createStyledTextBox("DEBUG MENU: These are all the possible colors for the console defined in this game!");
    z_debug::DisplayDebugColors();
    _createHorizontalLine('-');
    _pauseSystem();
}
void UI::screenDebugZorbs() const {
    _clearScreen();
    _createStyledTextBox("DEBUG MENU: These are all the possible Zorb appearances defined in this game!");
    z_debug::PrintZorbAppearances(appearanceMap.size(), true);
    _createHorizontalLine('-');
    _pauseSystem();
}
void UI::screenInfo() const {
    int iteration = 0;
    const std::vector<std::string> introductionText = {
        "In the furthest reaches of the cosmos, in a galaxy far, far away, there exists a race of adorable yet feisty aliens known as Zorbs. These lovable creatures, resembling a delightful fusion of Earthly cats and fuzzy aliens, lived in a galaxy filled with cuddles, meows, and of course, intergalactic warfare...",
        "This is a rogue-like video game that introduces you to the whimsical world of these fluffy aliens. The game's first character, Neep Narp, is a Zorb with the heart of a true hero, and it's your mission to guide Neep Narp and their friends through the cosmic chaos...",
        "In Zorb Zenith, you'll control groups of Zorbs in epic battles that will test your tactical prowess.  But beware, Zorbs are not immortal - permadeath is a reality, and you'll need to recruit new Zorbs to bolster your ranks as you navigate the tumultuous galactic battlefield...",
        "The game is turn-based, and each turn you'll be able to move your Zorbs around the battlefield. You can move your Zorbs to attack enemy Zorbs, or you can move them to pick up power-ups that will increase their power. Prepare to face off against other Zorb groups, each with their own adorable names and appearances. Will you encounter a formidable foe named Glarp or a cunning adversary known as Quor Bleep? The galaxy is teeming with characters like Bleepy, Porg, and even Beep, each with their unique traits..."
    };
    
    _createStyledTextBox("Press any character to skip, other press enter to continue.");
    if (z_util::clearInputBuffer() != '\n')
        return;
    
    for (const auto& text : introductionText) {
        _clearScreen();
        if (iteration==0)
            std::cout << std::endl << z_util::CenterAlignStrings(z_art::introSpace) << std::endl;
        else if (iteration==1){
            Zorb neepnarp(ZorbAppearance(static_cast<appearanceEnum>(0),ansi::GREEN), 0, 1, "Neep Narp");
            std::cout << neepnarp << std::endl;
        }
        else if (iteration==2) {
            std::cout << std::endl << std::endl;
            z_debug::PrintZorbAppearances(8, false, ansi::GREEN);
        }
        else if (iteration==3){
            std::cout << std::endl << z_util::CenterAlignStrings(z_art::planetZorb);
            std::vector<Zorb> sZorbs, eZorbs;
            sZorbs.emplace_back(ZorbAppearance(GetRandomAppearance(), ansi::GREEN), 1, 0, zorb::RandomName());
            eZorbs.emplace_back(ZorbAppearance(GetRandomAppearance(), ansi::MAGENTA), 1, 0, zorb::RandomName());       

            // Display the Zorbs in the battle with the team names
            // make sure the first team is displayed from the left and the 2nd team from the right
            std::cout << std::endl << "Player Team" << std::endl << "---------" << std::endl;
            DisplayZorbs(sZorbs);

            std::cout << std::endl << std::right << std::setw(CONSOLESIZE) << "Enemy Team" << std::endl <<
            std::setw(CONSOLESIZE)  << "---------" << std::endl;
            DisplayZorbs(eZorbs, 'R');
        }

        iteration++;
        _createStyledTextBox(text);
        _createHorizontalLine('-');
        _pauseSystem();
    }
    _clearScreen();
    std::cout << z_util::CenterAlignStrings(z_art::introSpace) << std::endl;
    _createStyledTextBox("The galaxy is waiting for you, commander. Are you ready to lead the Zorbs to victory?");
    _createHorizontalLine('-');
    std::cout << "CONTINUE TO THE MAIN MENU: ";
    _pauseSystem();
}

void UI::screenGameOver(std::vector<std::string>& names) const {
    _clearScreen();
    _createStyledTextBox("GAME OVER! R.I.P. To your last Zorb :(");

    //display a zorb appearance into the middle of the screen
    std::string deadZorb = R"(
       o   
    ./\|/\.
    ( x.x )
     > n < )";

    std::cout << z_util::CenterAlignStrings(deadZorb) << std::endl;
    // Create control header text that contains stats about your current team
    // including the number of wins, and average power
    std::string controlHeaderText;

    // display the number of wins and the amount of zorbs that died
    controlHeaderText += "Wins: " + std::to_string(winCounter) + " | ";
    controlHeaderText += "Zorbs met: " + std::to_string(names.size()) + "\n";

    std::cout << z_util::CenterAlignString(z_util::FormattedText(controlHeaderText, ansi::YELLOW)) << std::endl;

    // Display the names of the Zorbs that were recruited
    std::cout << z_util::CenterAlignString("Zorbs recruited on this Journey:") << std::endl;
    for (const auto& name : names) {
        std::string formattedName = z_util::FormattedText(name, ansi::GREEN);
        std::cout << z_util::CenterAlignString(formattedName) << std::endl;
    }
    std::cout << "\nQ. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
    std::cout << "Enter your choice: ";
}
void UI::screenBattle(const std::vector<Zorb>& team1, const std::vector<Zorb>& team2, const std::string& team1Name, const std::string& team2Name) {
    _clearScreen();
    // Create header text for the battle
    std::string headerText = "BATTLE TIME!\n" + team1Name + " vs. " + team2Name + "\n"
    + std::string(team1Name.length() + team2Name.length() + 5, '-') + "\n"
    + std::to_string(team1.size()) + " vs. " + std::to_string(team2.size());

    // Create control header text for the battle
    std::string controlHeaderText = "Turn: " + std::to_string(turnCounter) + " | " + "Casualties: " + std::to_string(casualtyCounter) + " | " + "Wins: " + std::to_string(winCounter) + "\n";

    _createStyledTextBox(headerText);
    // Display the Zorbs in the battle with the team names
    std::cout << std::endl << team1Name << std::endl << std::string(team1Name.length(), '-') << std::endl;
    DisplayZorbs(team1, 'L');

    std::cout << std::endl << std::right << std::setw(CONSOLESIZE) << team2Name << std::endl <<
    std::setw(CONSOLESIZE)  << std::string(team2Name.length(), '-') << std::endl;
    DisplayZorbs(team2, 'R');
    _createHorizontalLine('-');
    
    std::cout << z_util::CenterAlignString(z_util::FormattedText(controlHeaderText, ansi::YELLOW), CONSOLESIZE) << std::endl;
    std::cout << "1. Attack" << std::endl
    << "2. Dodge" << std::endl
    << "3. Runaway\n" << std::endl
    << "Q. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
    std::cout << "Enter your choice: ";
}
void UI::screenRecruitment(const Zorb& zorb, const std::vector<Zorb>& playerZorbs) const {
    _clearScreen();
    _createStyledTextBox("A Zorb approaches you and asks to join your team!");
    std::cout << std::endl << zorb << std::endl << std::endl;

    //display the players zorbs
    std::cout << "Your Zorbs:" << std::endl;
    DisplayZorbs(playerZorbs, 'L');
    _createHorizontalLine('-');
    // Create control header text that contains stats about your current team
    // including the number of Zorbs, the number of wins, and average power
    std::string controlHeaderText;
    double medianPower;
    // Get median power of the player's Zorbs altogether
    if (playerZorbs.size() % 2 == 0) {
        medianPower = (playerZorbs[playerZorbs.size() / 2.0].GetPower() + playerZorbs[playerZorbs.size() / 2.0 - 1].GetPower()) / 2.0;
    } else {
        medianPower = playerZorbs[playerZorbs.size() / 2].GetPower();
    }

    controlHeaderText += "Zorbs: " + std::to_string(playerZorbs.size()) + " | ";
    controlHeaderText += "Wins: " + std::to_string(winCounter) + " | ";
    //fix the precision of the median power to 2 decimal places
    std::stringstream stream;
    stream << std::fixed << std::setprecision(2) << medianPower;
    controlHeaderText += "Team Power: " + stream.str() + "\n";

    std::cout << z_util::CenterAlignString(z_util::FormattedText(controlHeaderText, ansi::YELLOW), CONSOLESIZE) << std::endl;
    std::cout << "Y. Recruit" << std::endl
    << "N. Reject\n" << std::endl
    << "Q. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
}
void UI::screenFightOutcome(Zorb& winZorb, Zorb& lossZorb, const std::string& attack) const {
    _clearScreen();
    std::string headerText = attack;

    if(winZorb.GetName() == "Imploded Zorb") {
        headerText += "\nBoth zorbs ";
        headerText += z_util::random::choice({"fought", "battled", "clashed", "duked it out", "went head to head"});
        headerText +=" and ";
        headerText += z_util::random::choice({"imploded!", "exploded!", "self-destructed!", "self-destructed!", "self-destructed!"});

        _createStyledTextBox(headerText);
        std::cout << z_util::CenterAlignStrings(z_util::FormattedText(z_art::implode, ansi::RED)) << std::endl;
    } else {
        headerText += " \n"
        + lossZorb.GetName()+" was "+z_util::random::choice({"SLAIN", "put into a deep rest", "destroyed", "put to sleep", "ANNIHILATED"})
        + " by the attack! (sweet dreams!)";

        _createStyledTextBox(headerText);
        lossZorb.SetAppearance(static_cast<appearanceEnum>(0), ansi::RED);
        std::cout << winZorb << std::endl << lossZorb << std::endl;
    }

    std::cout << std::endl << std::endl;

    _createHorizontalLine('-');
    std::string controlHeaderText = "Turn: " + std::to_string(turnCounter) + " | " + "Casualties: " + std::to_string(casualtyCounter) + "\n";
    std::cout << z_util::CenterAlignString(z_util::FormattedText(controlHeaderText, ansi::YELLOW), CONSOLESIZE) << std::endl;

    _pauseSystem();
}
void UI::screenBarber(const std::vector<Zorb>& pZorbs) const {
    _clearScreen();
    std::string titleText = "You are approached by a pecular rat who offers to change your Zorbs' appearances!\n \n";
    titleText+= BARBERNAME + ": \"You seem to need a haircut!!\"\n";
    titleText+= "You: \"What?\"\n";

    // Create control header text for zorb selection
    std::string controlHeaderText = "Pick a zorb [" + std::to_string(1) + "-" + std::to_string(pZorbs.size()) + "]";

    _createStyledTextBox(titleText);
    std::cout << z_util::CenterAlignStrings(z_art::barber);
    std::cout << "Your Zorbs:" << std::endl;
    DisplayZorbs(pZorbs, 'L');

    _createHorizontalLine('-');
    std::cout << z_util::CenterAlignString(z_util::FormattedText(controlHeaderText, ansi::YELLOW), CONSOLESIZE) << std::endl;
    std::cout << "Y. 'You're right'" << std::endl
    << "N. 'No thanks..'\n" << std::endl
    << "Q. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
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
