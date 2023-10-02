#include <iostream>
#include <vector>
#include <string>
#include "zUtility.hpp"
#include "Zorb.hpp" // Include the Zorb class
#include "UI.hpp"   // Include the UI class

int main() {
    // Enable ANSI escape code support
    ANSIConsole::EnableANSIEscapeSupport();
    UI ui;

    _createStyledTextBox("Printing Zorbs");
    ui.SetDisplayFormat(ui.ASCII_ART);
    std::vector<Zorb> zorbs;
    for(int i=0; i<10; i++)
    {
        zorbs.push_back(Zorb(APPEARANCE_DEFAULT, ANSI_GREEN));
        zorbs.at(i).SetName("Yelper");
        zorbs.at(i).SetPower(15);
    }
    ui.DisplayZorbs(zorbs);

    _pauseSystem();
        
    return 0;
}
