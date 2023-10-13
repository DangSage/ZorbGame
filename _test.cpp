/*
This is a quick driver to show all of the debug stuff that is 
going to be core functions of the game before we make the actual game driver.
*/
#include "gameDefs.hpp" // Include gameDefs.hpp for game definitions
#include "zUtility.hpp" // Include zUtility.hpp for z_debug namespace
#include "Zorb.hpp" // Include the Zorb class
#include "zUI.hpp"   // Include the UI class

int dmain() {
    ForceTerminal();
    initAppearanceMaps();
    
    std::cout << "There are " << appearanceMap.size() << " Zorb Appearances" << std::endl;
    std::cout << "There are " << appearanceNames.size() << " Zorb Names" << std::endl;

    z_debug::PrintZorbAppearances(appearanceMap.size(), true, ansi::GREEN);

    return 0;
}
