#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

// Forward declarations
class UI;
void initAppearanceMaps();
void ForceTerminal();

//enum for game states to be used in the game driver
enum class GameState {
    MainMenu,
    OptionsMenu,
    OptionsMenuTheme,
    Game,
    InfoMenu,
    End = -1
};

class GameManager {
public:
    GameManager(UI& ui);
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