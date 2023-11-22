#include "pch.hpp"
#include "zUI.hpp"
#include "Zorb.hpp"

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
void UI::screenIntro() const {
    int iteration = 0;
    const std::vector<std::string> introductionText = {
        "In the furthest reaches of the cosmos, in a galaxy far, far away, there exists a race of adorable yet feisty aliens known as Zorbs. These lovable creatures, resembling a delightful fusion of Earthly cats and fuzzy aliens, lived in a galaxy filled with cuddles, meows, and of course, intergalactic warfare...",
        "This is a rogue-like video game that introduces you to the whimsical world of these fluffy aliens. The game's first character, Neep Narp, is a Zorb with the heart of a true hero, and it's your mission to guide Neep Narp and their friends through the cosmic chaos...",
        "In Zorb Zenith, you'll control groups of Zorbs in epic battles that will test your tactical prowess.  But beware, Zorbs are not immortal - permadeath is a reality, and you'll need to recruit new Zorbs to bolster your ranks as you navigate the tumultuous galactic battlefield...",
        "The game is turn-based, and each turn you'll be able to move your Zorbs around the battlefield. You can move your Zorbs to attack enemy Zorbs, or you can move them to pick up power-ups that will increase their power. Prepare to face off against other Zorb groups, each with their own adorable names and appearances. Will you encounter a formidable foe named Glarp or a cunning adversary known as Quor Bleep? The galaxy is teeming with characters like Bleepy, Porg, and even Beep, each with their unique traits..."
    };
    
    _createStyledTextBox("Press any character to skip, other press enter to continue.");
    std::cout << '>';
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

void UI::screenBattleEncounter(std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team1, std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team2) {
    _clearScreen();
    std::cout << std::string(8, '\n') 
        << z_util::CenterAlignString(z_util::FormattedText("!! ENCOUNTER !!", ansi::RED)) << std::endl;
    
    // Create header text for the battle
    std::string headerText = std::string(team1.second.length() + team2.second.length() + 5, '-') + "\n";
    headerText += z_util::random::choice(text::battle::encounter);
    headerText += "\n\nIt's some group of " + team2.second + "!\n";
    headerText += std::string(team1.second.length() + team2.second.length() + 5, '-') + "\n";

    DisplayZorbs(SharedCast(team2.first), 'R');
    _createStyledTextBox(headerText);
    _createHorizontalLine('-');
    _pauseSystem();
}

void UI::screenBattleEncounterJump(std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team1, std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team2) {
    _clearScreen();
    std::cout << std::string(8, '\n') 
        << z_util::CenterAlignString(z_util::FormattedText("?!? HUH ?!?", ansi::RED)) << std::endl;
    _pauseSystem();
    _clearScreen();
    
    std::cout << std::string(8, '\n') 
        << z_util::CenterAlignString(z_util::FormattedText("!!! OH SNAP, YOU GOT JUMPED !!!", ansi::RED)) << std::endl;
    // Create header text for the battle
    std::string headerText = std::string(team1.second.length() + team2.second.length() + 5, '-') + "\n";
    headerText += "\n\nIt's " + team2.second + " again!!!\n";
    headerText += team2.first[0]->GetName() + " rallied his gang!\n";
    headerText += std::string(team1.second.length() + team2.second.length() + 5, '-') + "\n";

    DisplayZorbs(SharedCast(team2.first), 'R');
    _createStyledTextBox(headerText);
    _createHorizontalLine('-');
    _pauseSystem();
}


void UI::screenBattle(std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team1, std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string>& team2) {
    _clearScreen();
    // Create header text for the battle
    std::string headerText = "BATTLE TIME!\n" + team1.second + " vs. " + team2.second + "\n"
    + std::string(team1.second.length() + team2.second.length() + 5, '-') + "\n"
    + std::to_string(team1.first.size()) + " vs. " + std::to_string(team2.first.size());

    // Create control header text for the battle
    std::string controlHeaderText = "Turn: " + std::to_string(turnCounter) + " | " + "Casualties: " + std::to_string(casualtyCounter) + " | " + "Wins: " + std::to_string(winCounter) + "\n";

    _createStyledTextBox(headerText);
    // Display the Zorbs in the battle with the team names
    std::cout << std::endl << team1.second << std::endl << std::string(team1.second.length(), '-') << std::endl;
    DisplayZorbs(SharedCast(team1.first), 'L');

    std::cout << std::endl << std::right << std::setw(CONSOLESIZE) << team2.second << std::endl <<
    std::setw(CONSOLESIZE)  << std::string(team2.second.length(), '-') << std::endl;
    DisplayZorbs(SharedCast(team2.first), 'R');
    _createHorizontalLine('-');
    
    std::cout << z_util::CenterAlignString(z_util::FormattedText(controlHeaderText, ansi::YELLOW), CONSOLESIZE) << std::endl;
    std::cout << "1. Attack" << std::endl
    << "2. Dodge" << std::endl
    << "3. Runaway\n" << std::endl
    << "Q. Quit back to Title Screen" << std::endl;
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

    if(winZorb.GetTeamId() == -1) {
        if(winZorb.GetName() == zorb::N_DODGE)
                headerText += " \n"+lossZorb.GetName()+" missed their attack!";
        else if (winZorb.GetName() == zorb::N_IMPLODE) {
                headerText += " \n"+lossZorb.GetName()+" was ";
                headerText += "\nBoth zorbs ";
                headerText += z_util::random::choice(text::battle::implode);
        }

        _createStyledTextBox(headerText);
        std::cout << z_util::CenterAlignStrings(z_util::FormattedText(z_art::implode, ansi::RED)) << std::endl;
    } else {
        headerText += " \n"+lossZorb.GetName()+" was ";
        headerText += z_util::random::choice(text::battle::attackResult);
        headerText += " by "+winZorb.GetName()+"!";

        _createStyledTextBox(headerText+"\n \n"+winZorb.GetName()+" gained "+std::to_string(lossZorb.GetPower())+" power!");
        //set color of the losing zorb to the gray color
        lossZorb.SetColor((ansi::GRAY));
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
    std::string titleText = "You are approached by a peculiar rat who offers to change your Zorbs' appearances!\n \n";
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
