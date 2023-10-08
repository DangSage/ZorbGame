#include "zUtility.hpp" // Include zUtility.hpp for z_debug namespace
#include "gameManager.hpp"

int main() {
    UI _uiM;
    GameManager _gM(_uiM);

    //call the game manager's start function to start running the game with game states
    _gM.startGame();
    return 0;
}