
#include "zUI.hpp"
#include "gameManager.hpp"

int main() {
    UI _uiM;
    GameManager _gameManager(_uiM);
    
    //game loop
    _gameManager.gameLoop();
    return 0;
}