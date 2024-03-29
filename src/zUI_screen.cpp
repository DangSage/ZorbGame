#include "pch.hpp"
#include "zUI.hpp"
#include "Zorb.hpp"


const std::string CONTROLTEXT = "Q. Quit back to Title Screen" + std::string(CONSOLESIZE-36, ' ') + "Help [" + USERHELP + "]\n";

void UI::screenMainMenu() const {
    _clearScreen();
    std::stringstream ss;
    ss << "This is a game made by Dang. Report any bugs to the repo:\n" 
        << '[' << USERGIT << "] " << "https://github.com/DangSage/ZorbGame";
    _createStyledTextBox(ss.str());
    // Define the title text
    std::string titleText = z_art::gameTitle;
    titleText = z_util::CenterAlignString(titleText);

    // Define menu options
    std::string menuText =
        "<3 Dedicated to Thomas Worrall <3\n\n"
        " 1. Start Game\n"
        " 2. Introduction\n"
        " 3. Debug Settings\n\n";
    menuText += CONTROLTEXT;

    // Display the title text
    _createHorizontalLine('-');
    std::cout << std::endl << titleText << std::string(4, '\n') << std::endl;
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

    std::cout << z_util::CenterAlignString(z_art::optionsScreen) << std::endl;
    _createHorizontalLine('-');
    columnDisplay(("Battle Display Type: " + z_util::FormattedText(GetDisplayFormatAsString(), ansi::YELLOW) + '\t'), 
        "Other Options: ");
    std::cout << std::endl;
    columnDisplay(" 1. Display Zorbs (Simple Text)", " A. Show all colors");
    columnDisplay(" 2. Display Zorbs (ASCII Art)", " B. Show all Zorbs");
    columnDisplay(" 3. Display Zorbs (Table Format)", " C. Toggle Theme: " + z_util::FormattedText((_LIGHTTHEME == false ? "Light" : "Dark"), ansi::BLUE));
    columnDisplay(" 4. Display Zorbs (Compact Art)", " D. Toggle Debug Mode: " + z_util::FormattedText((_DEBUGMODE == true ? "On" : "Off"), ansi::RED));

    std::cout << std::right << std::setw(0) << std::endl
              << CONTROLTEXT;
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
void UI::screenIntro() const {
    int iteration = 0;
    const std::vector<std::string> introductionText = {
        "In the furthest reaches of the cosmos, in a galaxy far, far away, there exists a race of adorable yet feisty aliens known as Zorbs. These lovable creatures, resembling a delightful fusion of Earthly cats and fuzzy aliens, lived in a galaxy filled with cuddles, meows, and of course, intergalactic warfare...",
        "This is a rogue-like video game that introduces you to the whimsical world of these fluffy aliens. The game's first character, Neep Narp, is a Zorb with the heart of a true hero, and it's your mission to guide Neep Narp and their friends through the cosmic chaos...",
        "In Zorb Zenith, you'll control groups of Zorbs in epic battles that will test your tactical prowess.  But beware, Zorbs are not immortal - permadeath is a reality, and you'll need to recruit new Zorbs to bolster your ranks as you navigate the tumultuous galactic battlefield...",
        "The game is turn-based, and each turn you'll be able to move your Zorbs around the battlefield.\nYou can move your Zorbs to attack enemy Zorbs. Prepare to face off against other Zorb groups, each with their own adorable names and appearances.\n\nWill you encounter a formidable foe named Glarp or a cunning adversary known as Quor Bleep? The galaxy is teeming with characters like Bleepy, Porg, and even Beep, each with their unique traits..."
    };
    
    _createStyledTextBox("Press any character to skip, other press enter to continue.");
    std::cout << '>';
    if (z_util::clearInputBuffer() != '\n')
        return;
    
    for (const auto& text : introductionText) {
        _clearScreen();
        if (iteration==0)
            std::cout << std::endl << z_util::CenterAlignString(z_art::introSpace) << std::endl;
        else if (iteration==1){
            Zorb neepnarp(ZorbAppearance(static_cast<appearanceEnum>(0),ansi::GREEN), 0, 1, "Neep Narp");
            std::cout << std::string(8, '\n') << neepnarp << std::string(7, '\n') << std::endl;
        }
        else if (iteration==2) {
            std::cout << std::string(8, '\n');
            z_debug::PrintZorbAppearances(8, false, ansi::GREEN);
            std::cout << std::string(8, '\n');
        }
        else if (iteration==3){
            std::cout << std::endl << z_util::CenterAlignString(z_art::planetZorb);
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
    std::cout << z_util::CenterAlignString(z_art::introSpace) << std::endl;
    _createStyledTextBox("The galaxy is waiting for you, commander. Are you ready to lead the Zorbs to victory?\n\nEND OF INTRODUCTION");
    _createHorizontalLine('-');
    _pauseSystem();
}

void UI::screenStart() const {
    _clearScreen();
    for(int i=0; i<7; i++)
        std::cout << std::endl;
    std::cout << Zorb(ZorbAppearance(appearanceEnum::A_DEFAULT, ansi::GREEN), 1, 1, "Neep Narp") << "\n\n\n\n" << std::endl;
    std::cout << z_util::CenterAlignString(z_util::FormattedText("What's the name of this journey you will embark on with Neep Narp?", ansi::BLUE)) << std::endl;

    for(int i=0; i<8; i++)
        std::cout << std::endl;
    _createHorizontalLine('-');
    for(int i=0; i<8; i++)
        std::cout << ansi::UPLINE;
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

    std::cout << z_util::CenterAlignString(deadZorb) << std::endl;
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

void UI::screenBattleEncounter(std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team1, std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team2) {
    _clearScreen();
    std::cout << std::string(8, '\n') 
        << z_util::CenterAlignString(z_util::FormattedText("!! ENCOUNTER !!", ansi::RED)) << std::endl;
    
    // header text for the encounter
    std::stringstream ss;
    ss << std::string(team1.second.length() + team2.second.length() + 5, '-') << "\n"
        << z_util::random::choice(text::battle::encounter)
        << "\n\nIt's some group of " << team2.second << "!\n"
        << std::string(team1.second.length() + team2.second.length() + 5, '-') << std::endl;

    DisplayZorbs(SharedCast(team2.first), 'R');
    _createStyledTextBox(ss.str());
    _createHorizontalLine('-');
    _pauseSystem();
}

void UI::screenBattleEncounterJump(std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team1, std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team2) {
    _clearScreen();
    std::stringstream ss;

    std::cout << std::string(10, '\n') 
        << z_util::CenterAlignString(z_util::FormattedText("?!? HUH ?!?", ansi::RED)) << std::string(8, '\n');
    _pauseSystem();
    _clearScreen();

    std::cout << std::string(10, '\n') 
        << z_util::CenterAlignString(z_util::FormattedText("!!! OH SNAP, YOU GOT JUMPED !!!", ansi::RED)) << std::endl;

    // header text for the surprise encounter
    ss << std::string(team1.second.length() + team2.second.length() + 5, '-') << std::endl
        << "\n\nIt's " << team2.second << " again!!!\n"
        << team2.first[0]->GetName() << " rallied his gang!\n"
        << std::string(team1.second.length() + team2.second.length() + 5, '-') << std::endl;

    DisplayZorbs(SharedCast(team2.first), 'R');
    _createStyledTextBox(ss.str());
    _createHorizontalLine('-');
    _pauseSystem();
}

void UI::screenBattle(std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team1, std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team2) {
    _clearScreen();
    std::stringstream ss;

    ss << "BATTLE #" << battleCounter << ":\n" 
        << team1.second << " vs. " << team2.second << "\n"
        << std::string(team1.second.length() + team2.second.length() + 5, '-') << "\n"
        << team1.first.size() << " vs. " << team2.first.size();

    _createStyledTextBox(ss.str());
    ss.str(std::string());
    ss.clear();

    // Display the Zorbs in the battle with the team names
    std::cout << std::endl << team1.second << std::endl << std::string(team1.second.length(), '-') << std::endl;
    DisplayZorbs(SharedCast(team1.first), 'L');

    std::cout << std::endl << std::right << std::setw(CONSOLESIZE) << team2.second << std::endl <<
    std::setw(CONSOLESIZE)  << std::string(team2.second.length(), '-') << std::endl;
    DisplayZorbs(SharedCast(team2.first), 'R');
    _createHorizontalLine('-');

    ss << "Turn: " << turnCounter << " | " 
        << "Casualties: " << casualtyCounter 
        << " | " << "Wins: " << winCounter;

    std::string controlHeaderText = z_util::CenterAlignString(z_util::FormattedText(ss.str(), ansi::YELLOW), CONSOLESIZE);
    std::cout << controlHeaderText << std::right << std::setw(CONSOLESIZE-controlHeaderText.size()+8) 
        << "GitHub [" << USERGIT << "]" << std::endl;
    
    std::cout << " 1. Attack" << std::endl
        << " 2. Dodge" << std::endl
        << " 3. Runaway\n" << std::endl
        << CONTROLTEXT;
    _createHorizontalLine('-');
}
void UI::screenRecruitment(const Zorb& zorb, const std::vector<Zorb>& playerZorbs) const {
    _clearScreen();
    _createStyledTextBox("A Zorb approaches you and asks to join your team!");
    std::cout << std::endl << zorb << std::endl << std::endl;

    //display the players zorbs
    std::cout << "Your Zorbs:" << std::endl;
    DisplayZorbs(playerZorbs, 'L');
    _createHorizontalLine('-');

    // including the number of Zorbs, the number of wins, and average power
    double medianPower;
    // Get median power of the player's Zorbs altogether
    if (playerZorbs.size() % 2 == 0) {
        medianPower = (playerZorbs[playerZorbs.size() / 2.0].GetPower() + playerZorbs[playerZorbs.size() / 2.0 - 1].GetPower()) / 2.0;
    } else {
        medianPower = playerZorbs[playerZorbs.size() / 2].GetPower();
    }

    std::stringstream controlHeaderText;
    controlHeaderText << "Zorbs: " << playerZorbs.size() << " | "
        << "Wins: " << winCounter << " | "
        << std::fixed << std::setprecision(2) << "Team Power: " << medianPower << "\n";

    std::cout << z_util::CenterAlignString(z_util::FormattedText(controlHeaderText.str(), ansi::YELLOW), CONSOLESIZE) << std::endl;
    std::cout << " Y. Recruit" << std::endl
    << " N. Reject\n" << std::endl
    << CONTROLTEXT;
    _createHorizontalLine('-');
}
void UI::screenFightOutcome(Zorb& winZorb, Zorb& lossZorb, const std::string& attack) const {
    std::stringstream ss;
    _clearScreen();
    ss << attack;

    if(winZorb.GetTeamId() == -1) {
        if(winZorb.GetName() == zorb::N_DODGE)
            ss << " \n" << lossZorb.GetName() << " missed their attack!";
        else if (winZorb.GetName() == zorb::N_IMPLODE) {
            ss << "\nBoth zorbs were equal in power, creating a fission in time and space!\n The zorbs "
            << z_util::random::choice(text::battle::implode)
            << " simultaneously!";
        }
        _createStyledTextBox(ss.str());
        std::cout << z_util::CenterAlignString(z_util::FormattedText(z_art::implode, ansi::RED)) << std::endl;
    } else {
        ss << std::endl << lossZorb.GetName() << " was " 
            << z_util::random::choice(text::battle::attackResult) << " by " << winZorb.GetName() << "!\n\n"
            << winZorb.GetName() << " gained " << std::to_string(lossZorb.GetPower()) << " power!" << std::endl;
        lossZorb.SetColor((ansi::GRAY)); //set color of the losing zorb to the gray color
        _createStyledTextBox(ss.str());
        std::cout << winZorb << std::endl << lossZorb << std::endl;
    }

    ss.str(std::string());
    ss.clear();
    std::cout << std::endl << std::endl;

    _createHorizontalLine('-');
    ss << "Turn: " << turnCounter << " | " << "Casualties: " << casualtyCounter << " | " << std::endl;
    std::string controlHeaderText = z_util::CenterAlignString(z_util::FormattedText(ss.str(), ansi::YELLOW), CONSOLESIZE);
    std::cout << controlHeaderText << std::endl;

    _pauseSystem();
}
void UI::screenBarber(const std::vector<Zorb>& pZorbs) const {
    _clearScreen();
    
    std::stringstream titleText;
    titleText << "You are approached by a peculiar rat who offers to change your Zorbs' appearances!\n \n"
              << BARBERNAME << ": \"You seem to need a haircut!!\"\n"
              << "You: \"What?\"\n";

    // Create control header text for zorb selection
    std::stringstream controlHeaderText;
    controlHeaderText << "Pick a zorb [" << 1 << "-" << pZorbs.size() << "]";

    _createStyledTextBox(titleText.str());
    std::cout << z_util::CenterAlignString(z_art::barber);
    std::cout << "Your Zorbs:" << std::endl;
    DisplayZorbs(pZorbs, 'L');

    _createHorizontalLine('-');
    std::string centeredControlHeader = z_util::CenterAlignString(z_util::FormattedText(controlHeaderText.str(), ansi::YELLOW), CONSOLESIZE);
    std::cout << centeredControlHeader << std::right << std::setw(CONSOLESIZE-centeredControlHeader.size()+9) 
        << "GitHub [" << USERGIT << "]" << std::endl;
    std::cout << " Y. 'You're right'" << std::endl
    << " N. 'No thanks..'\n" << std::endl
    << CONTROLTEXT;
    _createHorizontalLine('-');
}