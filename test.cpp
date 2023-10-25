/*
This is a quick driver to show all of the debug stuff that is 
going to be core functions of the game before we make the actual game driver.
*/
#include "gameManager.hpp" // Include gameManager.hpp for GameManager class usage and literally everything else
using namespace std;
int main() {
    ForceTerminal();
    initAppearanceMaps();

    UI _uiM;

    _pauseSystem();
    
    cout << "There are " << appearanceMap.size() << " Zorb Appearances" << endl;
    cout << "There are " << appearanceNames.size() << " Zorb Names" << endl;
    
    z_debug::PrintZorbAppearances(appearanceMap.size(), true, ansi::GREEN);
    _pauseSystem();
    // make a function to test difference screen displays
    _uiM.screenGameOver();
    _pauseSystem();

    while(true) {
        _uiM.screenRecruitment(Zorb(0, 1, zorb::RandomName(), ZorbAppearance(GetRandomAppearance(), ansi::GetRandomColor())));
        if(cin.get() == 'n') {
            break;
        }
    }

    while(true){
        //create a test for the battle screen and randomly generate 2 different teams to do so
        vector<Zorb> team1, team2;
        for(int i=0; i<z_debug::RandomValue(1,100); i++) {
            team1.emplace_back(0, 1, zorb::RandomName(), ZorbAppearance(GetRandomAppearance(), randomChoice({ansi::GREEN, ansi::CYAN})));
        }
        for(int i=0; i<z_debug::RandomValue(1,100); i++) {
            team2.emplace_back(0, 2, zorb::RandomName(), ZorbAppearance(GetRandomAppearance(), randomChoice({ansi::MAGENTA, ansi::RED})));
        }
        _uiM.screenBattle(team1, team2, "Player Team", "Enemy Team");  
        if(cin.get() == 'n') {
            break;
        }  
    }
    return 0;
}