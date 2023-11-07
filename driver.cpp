// Creator: Ethan Dang
/*
This is a test driver for the game. It is used to test game features and debug the game.
*/
#include "gameManager.hpp"

int main() {
    UI _uiM;
    GameManager _gameManager(_uiM);
    
    //game loop
    _gameManager.gameLoop();
    return 0;
}