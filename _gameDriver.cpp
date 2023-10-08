#include "zUtility.hpp" // Include zUtility.hpp for z_debug namespace
#include "gameManager.hpp"

int main() {
    // UI ui;
    // ui.SetDisplayFormat(ASCII_ART);
    // _createStyledTextBox("This is a quick driver to show all of the debug stuff that is going to be core functions of the game.");
    // _pauseSystem();

    // /*This is the title screen! Here the player will be able to inevitably 
    // do one of the following options given above. Sadly there is no implementation 
    // yet for a Instructions screen since I haven't defined a game driver as of today (4/10/2023)*/
    // ui.DisplayTitleScreen();
    // std::cout << std::endl;
    // _pauseSystem()

    //declare a game manager object from our gameManager.hpp class
    UI _uiM;
    GameManager _gM(_uiM);

    //call the game manager's show debug screen
    _gM.startGame();
    return 0;
}