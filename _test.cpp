/*
This is a quick driver to show all of the debug stuff that is 
going to be core functions of the game before we make the actual game driver.
*/

#include "Zorb.hpp" // Include the Zorb class
#include "zUI.hpp"   // Include the UI class

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
    // _pauseSystem();


    // /*This is the current iteration of the debug menu. 
    // It is very rudimentary and will include eventually ways to 
    // see every debug option we are giving in the next screen display*/
    // ui.DisplayDebugScreen();
    // std::cout << std::endl;
    // _pauseSystem();


    // /*Here we have couple of debug tools that are a part of the zUI.hpp header file 
    // created for tiny operations related to the game in general that are not in 
    // direct interaction with the player.*/
    // _clearScreen();
    // z_debug::CountGameObjectsInMemory();
    // z_debug::DisplayDebugColors();
    // std::cout << std::endl;
    // _pauseSystem();


    /*This is a display of the entire enumerator set of Zorb Appearances. 
    This does NOT account for stats or colors or names. Zorb Appearances 
    are a class object that is builtin as children of the larger Zorb object*/
    _clearScreen();
    _createStyledTextBox("All possible Zorb Appearances right now");
    z_debug::PrintAllZorbAppearances();
    std::cout << std::endl;
    _pauseSystem();


    return 0;
}
