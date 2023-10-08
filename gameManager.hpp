#include <vector>
#include "Zorb.hpp"
#include "zUI.hpp"
#include "gameDefs.hpp"

enum class GameState {
    MainMenu,
    OptionsMenu,
    Game,
    GameOver,
    End = -1
};

class GameManager {
public:
    GameManager(UI& ui) : m_ui(ui) {}

    void startGame();
    void endGame();

private:
    UI& m_ui;
    std::vector<Zorb> m_zorbs;  // All zorbs
    std::vector<Zorb*> m_playerZorbs;   // Player zorbs
    std::vector<Zorb*> m_enemyZorbs;    // Enemy zorbs
    std::vector<Zorb*> m_nzorbs;    // npc zorbs
    bool m_isPaused = false;
    GameState m_gameState = GameState::OptionsMenu;

    void handleMainMenuInput(char input);
    void handleOptionsMenuInput(char input);
    void handlePauseMenuInput(char input);
    void handleGameOverInput(char input);
};

void GameManager::startGame() {
    // Initialize game objects
    m_zorbs.push_back(Zorb(1)); // Player zorb
    m_zorbs.push_back(Zorb(2)); // Enemy zorb
    m_zorbs.push_back(Zorb(0)); // Player zorb

    // Game loop
    while (true) {
        switch (m_gameState) {
            case GameState::MainMenu:
                m_ui.screenMainMenu();
                char mainMenuInput;
                std::cin >> mainMenuInput;
                handleMainMenuInput(mainMenuInput);
                break;
            case GameState::OptionsMenu:
                m_ui.screenDebugOptions();
                char optionsMenuInput;
                std::cin >> optionsMenuInput;
                handleOptionsMenuInput(optionsMenuInput);
                break;
            case GameState::Game:
                break;
            case GameState::GameOver:
                m_ui.screenGameOver();
                char gameOverInput;
                std::cin >> gameOverInput;
                handleGameOverInput(gameOverInput);
                break;
            default:
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
    m_playerZorbs.clear();
    m_enemyZorbs.clear();
}

void GameManager::handleMainMenuInput(char input) {
    switch (input) {
        case '1':
            m_gameState = GameState::Game;
            break;
        case '2':
            m_gameState = GameState::OptionsMenu;
            break;
        case 'q':
        case 'Q':
            endGame();
            z_debug::CountGameObjectsInMemory();
            m_gameState = GameState::End;
            break;
        default:
            break;
    }
}

void GameManager::handleOptionsMenuInput(char input) {
    switch (input) {
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
            m_ui.SetDisplayFormat(COLOR);
            break;
        case 'q':
        case 'Q':
            m_gameState = GameState::MainMenu;
            break;
        default:
            break;
    }
}

void GameManager::handlePauseMenuInput(char input) {
    switch (input) {
        case '1':
            m_gameState = GameState::Game;
            break;
        case '2':
            m_gameState = GameState::OptionsMenu;
            break;
        case 'q':
        case 'Q':
            m_gameState = GameState::GameOver;
            break;
        default:
            break;
    }
}

void GameManager::handleGameOverInput(char input) {
    endGame();
    switch (input) {
        case '1':
            m_gameState = GameState::MainMenu;
            break;
        case 'q':
        case 'Q':
            break;
        default:
            break;
    }
}
