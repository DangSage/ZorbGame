// Creator: Ethan Dang
#include "gameManager.hpp"

int main_() {
    UI _uiM;
    GameManager _gameManager(_uiM);
    
    _gameManager.startGame();
    return 0;
}