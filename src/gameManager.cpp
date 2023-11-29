#include "pch.hpp"
#include "gameManager.hpp"
#include "gameplayManager.hpp"
#include "Zorb.hpp"
#include "zUI.hpp"

static int m_playerScore = 0;  // Player's score

GameManager::GameManager(UI& ui) : m_ui(ui) {
        ForceTerminal();
        initAppearanceMaps();
    } //default constructor, initialize game manager with a UI object, then format terminal and initialize appearance maps

void GameManager::setGameState(GameState gameState) { 
    m_gameState = gameState; 
}

//region GameManager Functions
void GameManager::gameLoop() {
    // Game loop
    while (true) {
        switch (m_gameState) {
            case GameState::InfoMenu:
                m_ui.screenIntro();
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
                zException exc = UnexpectedCallException("GameManager::gameLoop()");
                z_debug::PrintError(exc);
                break;
        }
        if(m_gameState == GameState::Game) {
            startGame();
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
    char input[] = {'1','2','3',USERGIT,USERHELP,USEREXIT};
    switch (validatedInput<char>(input)) {
        case '1':
            m_gameState = GameState::Game;
            break;
        case '2':
            m_gameState = GameState::InfoMenu;
            break;
        case '3':
            m_gameState = GameState::OptionsMenu;
            break;
        case USERGIT:
            system(command.c_str());
            break;
        case USERHELP:
            std::cout << ansi::DLINE << text::help::general;
            std::cin.get();
            for(int i=0; i<z_util::GetHeight(text::help::general); i++)
                std::cout << ansi::DLINE;
            break;
        case USEREXIT:
            endGame();
            z_debug::CountGameObjectsInMemory();
            m_gameState = GameState::End;
            break;
        default:
            zException exc = UnexpectedCallException("GameManager::handleMainMenuInput()");
            z_debug::PrintError(exc);
            break;
    }
}
void GameManager::handleOptionsMenuInput() {
    char input[] = {'1','2','3','4','A','B','C','D',USEREXIT,USERHELP};
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
        case USERHELP:
            std::cout << ansi::DLINE << text::help::debug;
            std::cin.get();
            for(int i=0; i<z_util::GetHeight(text::help::debug); i++)
                std::cout << ansi::DLINE;
            break;
        case USEREXIT:
            m_gameState = GameState::MainMenu;
            break;
        default:
            zException exc = UnexpectedCallException("GameManager::handleOptionsMenuInput()");
            z_debug::PrintError(exc);
            break;
    }
}
void GameManager::handleGameOverInput() {
    endGame();
    char input[] = {'1',USEREXIT};
    switch (validatedInput<char>(input)) {
        case '1':
            m_gameState = GameState::MainMenu;
            break;
        case 'Q':
            break;
        default:
            zException exc = UnexpectedCallException("GameManager::handleGameOverInput()");
            z_debug::PrintError(exc);
            break;
    }
}
//endregion

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