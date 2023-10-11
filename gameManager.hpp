#include <vector>
#include "Zorb.hpp"
#include "zUI.hpp"
#include "gameDefs.hpp"
#include "gameplayManager.hpp"

class GameManager {
public:
    GameManager(UI& ui) : m_ui(ui), m_gameplayManager(GameplayManager(*this)) { 
        ForceTerminal();
        initAppearanceMaps();
    } //default constructor, initialize game manager with a UI object, then format terminal and initialize appearance maps

    void startGame();
    void endGame();

protected:
    // Add a GameplayManager object to the GameManager that is a friend
    GameplayManager m_gameplayManager;
    GameState m_gameState = GameState::InfoMenu;
    std::vector<Zorb> m_zorbs;  // All zorbs
    UI& m_ui;   // UI object
    

    //region Zorb Management
    int m_playerScore = 0;
    int m_enemyScore = 0;
    //endregion

private:
    //region Input Handling
    void handleMainMenuInput();
    void handleOptionsMenuInput();
    void handleGameOverInput();
    //endregion
};

//region GameManager Functions
void GameManager::startGame() {
    // Initialize game objects
    
    m_zorbs.push_back(Zorb(0, 1, "Neep Narp"));
    m_zorbs.at(0).SetAppearance(static_cast<appearanceEnum>(0), ANSI_GREEN);
    m_zorbs.push_back(Zorb(0)); // NPC zorb

    // Game loop
    while (true) {
        switch (m_gameState) {
            case GameState::MainMenu:
                m_ui.screenMainMenu();
                handleMainMenuInput();
                break;
            case GameState::OptionsMenu:
                m_ui.screenDebugOptions();
                handleOptionsMenuInput();
                break;
            case GameState::Game:
                break;
            case GameState::InfoMenu:
                m_ui.screenInfo(m_zorbs);
                m_gameState = GameState::MainMenu;
                break;
            case GameState::GameOver:
                m_ui.screenGameOver();
                handleGameOverInput();
                break;
            default:
                z_debug::PrintError("GameManager::startGame() - Invalid GameState");
                break;
        }
        if(m_gameState == GameState::End)
            break;
    }
    _clearScreen();

    std::cout << "\n\n\n\n\n" << std::endl;
    std::string asciiArt = "/\\/\\      o      /\\/\\\n\\  /   ./\\|/\\.   \\  /\n \\/    ( ^.^ )    \\/ \n       b> ^ <d       \n";
    _createStyledTextBox(asciiArt+"\n<3 Thank you for playing Zorb Zenith! <3\nMay you attain your own inner zen!\n\n-Dang");
    _pauseSystem();
}
void GameManager::endGame() {
    // Clean up game objects
    z_debug::CountGameObjectsInMemory();
    _pauseSystem();
    m_zorbs.clear();
}
//endregion

//region Input Handling
void GameManager::handleMainMenuInput() {
    switch (validatedInput({'1','2','3','Q'})) {
        case '1':
            //m_gameState = GameState::Game;
            break;
        case '2':
            m_gameState = GameState::InfoMenu;
            break;
        case '3':
            m_gameState = GameState::OptionsMenu;
            break;
        case 'Q':
            endGame();
            z_debug::CountGameObjectsInMemory();
            m_gameState = GameState::End;
            break;
        default:
            z_debug::PrintError("GameManager::handleMainMenuInput() - HDWGH?");
            break;
    }
}
void GameManager::handleOptionsMenuInput() {
    switch (validatedInput({'1','2','3','4','A','B','C','Q'})) {
        case '1':
            m_ui.SetDisplayFormat(SIMPLE);
            break;
        case '2':
            m_ui.SetDisplayFormat(ASCII_ART);
            break;
        case '3':
            m_ui.SetDisplayFormat(TABLE);
            break;
        case '4':
            m_ui.SetDisplayFormat(COMPACT);
            break;
        case 'A':
            m_ui.screenDebugColors();
            break;
        case 'B':
            m_ui.screenDebugZorbs();
            break;
        case 'C':
            ChangeConsoleTheme();
            break;
        case 'Q':
            m_gameState = GameState::MainMenu;
            break;
        default:
            z_debug::PrintError("GameManager::handleOptionsMenuInput() - HDWGH?");
            break;
    }
}
void GameManager::handleGameOverInput() {
    endGame();
    switch (validatedInput({'1','Q'}, true)) {
        case '1':
            m_gameState = GameState::MainMenu;
            break;
        case 'Q':
            break;
        default:
            z_debug::PrintError("GameManager::handleGameOverInput() - HDWGH?");
            break;
    }
}
//endregion

