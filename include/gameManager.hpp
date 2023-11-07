#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "gameplayManager.hpp"

class GameManager {
public:
    GameManager(UI& ui) : m_ui(ui) {
        ForceTerminal();
        initAppearanceMaps();
    } //default constructor, initialize game manager with a UI object, then format terminal and initialize appearance maps
    ~GameManager() {} //default destructor

    void gameLoop();
    void endGame();

    void setGameState(GameState gameState);
protected:
    GameState m_gameState = GameState::InfoMenu;
    UI& m_ui;   // UI object
    
private:
    //region Input Handling
    void handleMainMenuInput();
    void handleOptionsMenuInput();
    void handleGameOverInput();
    //endregion
};

namespace z_debug {
    // Returns the static number of objects in memory
    void CountGameObjectsInMemory();
}

#endif //GAMEMANAGER_HPP