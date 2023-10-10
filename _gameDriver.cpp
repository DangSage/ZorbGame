#include "zUtility.hpp" // Include zUtility.hpp for z_debug namespace
#include "gameManager.hpp"

int main() {
    UI _uiM;
    GameManager _gameManager(_uiM);
    _gameManager.startGame();
    return 0;
}