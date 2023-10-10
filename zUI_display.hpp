#ifndef ZUI_DISPLAY_HPP
#define ZUI_DISPLAY_HPP

#include <string>
#include <vector>

#include "zUI.hpp"

//region screen Display Functions
void UI::screenMainMenu() const {
    _clearScreen();
    _createStyledTextBox("This is a game made by Dang. Report any bugs to the repo:\nhttps://github.com/DangSage/ZorbGame");
    // Define the title text
    std::string titleText = z_art::gameTitle;
    titleText = z_debug::CenterAlignStrings(titleText);

    // Define menu options
    std::string menuText =
        "<3 Dedicated to Thomas Worrall <3\n\n"
        "1. Start Game\n"
        "2. Instructions\n"
        "3. Debug Settings\n"
        "Q. Quit\n";

    // Display the title text
    _createHorizontalLine('-');
    std::cout << titleText << std::endl;
    z_debug::PrintZorbAppearances(4, false, ANSI_CYAN);
    _createHorizontalLine('-');
    // Display menu options
    std::cout << menuText;
    _createHorizontalLine('-');
    std::cout << "Enter your choice: ";
}
void UI::screenDebugOptions() const {
    _clearScreen();
    _createStyledTextBox("DEBUG MENU: Change game settings here!");

    int columnWidth = DISPLAYWIDTH / 2;
    auto columnDisplay = [&](const std::string& option, const std::string& option2) {
        int optionLength = z_debug::GetLengthWithoutEscapeCodes(option);
        int option2Length = z_debug::GetLengthWithoutEscapeCodes(option2);
        std::cout << std::left << std::setw(columnWidth) << option
                  << std::setw(columnWidth + (option2Length - optionLength)) << option2 << std::endl;
    };

    std::cout << z_art::optionsScreen << std::endl;
    _createHorizontalLine('-');
    columnDisplay(("Battle Display Type: " + z_debug::FormattedText(GetDisplayFormatAsString(), ANSI_YELLOW) + '\t'), 
        "Other Options: ");
    std::cout << std::endl;
    columnDisplay(" 1. Display Zorbs (Simple Text)", " A. Show all colors");
    columnDisplay(" 2. Display Zorbs (ASCII Art)", " B. Show all Zorbs");
    columnDisplay(" 3. Display Zorbs (Table Format)", " C. Toggle Theme: " + z_debug::FormattedText((_LIGHTTHEME == true ? "Light" : "Dark"), ANSI_BLUE));
    columnDisplay(" 4. Display Zorbs (Compact Art)", " D. option");

    std::cout << std::right << std::setw(0) << std::endl
              << "Q. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
    std::cout << "Enter your choice: ";
}
void UI::screenInfo(const std::vector<Zorb>& zorbs) const {
    const std::vector<std::string> introductionText = {
        "In the furthest reaches of the cosmos, in a galaxy far, far away, there exists a race of adorable yet feisty aliens known as Zorbs. These lovable creatures, resembling a delightful fusion of Earthly cats and fuzzy aliens, lived in a galaxy filled with cuddles, meows, and of course, intergalactic warfare...",
        "This is a rogue-like video game that introduces you to the whimsical world of these fluffy aliens. The game's first character, Neep Narp, is a Zorb with the heart of a true hero, and it's your mission to guide Neep Narp and their friends through the cosmic chaos...",
        "In Zorb Zenith, you'll control groups of Zorbs in epic battles that will test your tactical prowess.  But beware, Zorbs are not immortal - permadeath is a reality, and you'll need to recruit new Zorbs to bolster your ranks as you navigate the tumultuous galactic battlefield...",
        "The game is turn-based, and each turn you'll be able to move your Zorbs around the battlefield. You can move your Zorbs to attack enemy Zorbs, or you can move them to pick up power-ups that will increase their power. Prepare to face off against other Zorb groups, each with their own adorable names and appearances. Will you encounter a formidable foe named Glarp or a cunning adversary known as Quor Bleep? The galaxy is teeming with characters like Bleepy, Porg, and even Beep, each with their unique traits..."
    };
    
    _createStyledTextBox("Press any character to skip, other press enter to continue.");
    if (z_debug::clearInputBuffer() != '\n') {
        return;
    }
    
    bool firstIteration = true;
    for (const auto& text : introductionText) {
        _clearScreen();
        if (firstIteration) {
            std::cout << z_debug::CenterAlignStrings(z_art::introSpace) << std::endl;
            firstIteration = false;
        }
        else 
            std::cout << zorbs.at(0) << std::endl;
        _createStyledTextBox(text);
        _createHorizontalLine('-');
        _pauseSystem();
    }
    _clearScreen();
    std::cout << z_debug::CenterAlignStrings(z_art::introSpace) << std::endl;
    _createStyledTextBox("The galaxy is waiting for you, commander. Are you ready to lead the Zorbs to victory?");
    _createHorizontalLine('-');
    std::cout << "CONTINUE TO THE MAIN MENU: ";
    _pauseSystem();
}
void UI::screenGameOver() const {
    _clearScreen();
    _createStyledTextBox("GAME OVER! R.I.P. To your last Zorb :(");

    //display a zorb appearance into the middle of the screen
    std::string deadZorb = R"(
       o   
    ./\|/\.
    ( x.x )
     > n < )";
    
    deadZorb = z_debug::CenterAlignStrings(deadZorb);

    std::cout << std::right << std::setw(0) << std::endl
    << "Q. Quit back to Title Screen" << std::endl;
    _createHorizontalLine('-');
    std::cout << "your choice: ";
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

//end region
#endif // ZUI_DISPLAY_HPP
