#include "Zorb.hpp"
#include "gameplayManager.hpp"

std::string url = "https://github.com/DangSage/ZorbGame";
std::string command = "start " + url; // "start" is a Windows command to open a URL in the default browser

static int m_playerScore = 0;  // Player's score

class GameManager {
public:
    GameManager(UI& ui) : m_ui(ui) {
        ForceTerminal();
        initAppearanceMaps();
    } //default constructor, initialize game manager with a UI object, then format terminal and initialize appearance maps
    ~GameManager() {} //default destructor

    void gameLoop();
    void endGame();

    void setGameState(GameState gameState) { m_gameState = gameState; }
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
    void CountGameObjectsInMemory() {
        std::cout << ansi::YELLOW << std::endl
        << Zorb::GetCount() << " Zorb Object(s) in Memory" << std::endl
        // << " ZorbAppearance Object(s) in Memory" << std::endl
        //reset color and end of zorb objects
        << ansi::RESET << std::endl;
    }
}

//region GameManager Functions
void GameManager::gameLoop() {
    // Game loop
    while (true) {
        switch (m_gameState) {
            case GameState::InfoMenu:
                m_ui.screenInfo();
                m_gameState = GameState::MainMenu;
                break;
            case GameState::MainMenu:
                m_ui.screenMainMenu();
                handleMainMenuInput();
                break;
            case GameState::OptionsMenu:
                m_ui.screenDebugOptions();
                handleOptionsMenuInput();
                break;
            default:
                z_debug::PrintError("GameManager::gameLoop() - Invalid GameState");
                break;
        }
        if(m_gameState == GameState::Game) {
            GameplayManager* m_gameplayManager = new GameplayManager(m_ui);
            m_gameplayManager->gameplayLoop();
            delete m_gameplayManager;
            m_gameState = GameState::MainMenu;
        }
        if(m_gameState == GameState::End)
            break;
    }
    _clearScreen();

    std::cout << "\n\n\n\n\n" << std::endl;
    _createStyledTextBox(z_art::endCard+"\n<3 Thank you for playing Zorb Zenith! <3\nMay you attain your own inner zen!\n\n-Dang");
    _pauseSystem();
}
void GameManager::endGame() {
    // Clean up game objects
    z_debug::CountGameObjectsInMemory();
    _pauseSystem();
}
//endregion

//region Input Handling
void GameManager::handleMainMenuInput() {
    char input[] = {'0','1','2','3','Q'};
    switch (validatedInput<char>(input)) {
        case '0':
            system(command.c_str());
            break;
        case '1':
            m_gameState = GameState::Game;
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
    char input[] = {'1','2','3','4','A','B','C','D','Q'};
    switch (validatedInput<char>(input)) {
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
        case 'D':
            ChangeDebugMode();
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
    char input[] = {'1','Q'};
    switch (validatedInput<char>(input)) {
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

