#include <vector>
#include "Zorb.hpp"
#include "zUI.hpp"
#include "gameDefs.hpp"

class GameManager {
public:
    GameManager(UI& ui) : m_ui(ui) { 
        ForceTerminal();
        initAppearanceMaps();
    } //default constructor, initialize game manager with a UI object, then format terminal and initialize appearance maps

    void startGame();
    void endGame();

private:
    UI& m_ui;
    std::vector<Zorb> m_zorbs;  // All zorbs
    std::vector<Zorb*> m_playerZorbs;   // Player zorbs
    std::vector<Zorb*> m_enemyZorbs;    // Enemy zorbs
    std::vector<Zorb*> m_nzorbs;    // npc zorbs
    bool m_isPaused = false;
    GameState m_gameState = GameState::InfoMenu;

    //region Zorb Management
    void SortZorbsByTeamId();
    void GenerateZorb(int team_id);
    //endregion

    //region Input Handling
    void handleMainMenuInput(char input);
    void handleOptionsMenuInput(char input);
    void handleGameOverInput(char input);
    void handleDebugThemeInput(char input);
    //endregion

    //region Stats
    int m_playerScore = 0;
    int m_enemyScore = 0;
    //endregion
};

//region GameManager Functions
void GameManager::startGame() {
    // Initialize game objects
    
    m_zorbs.push_back(Zorb(0, 1, "Neep Narp"));
    m_zorbs.at(0).SetAppearance(static_cast<appearanceEnum>(0), ANSI_GREEN);
    m_zorbs.push_back(Zorb(0)); // NPC zorb
    SortZorbsByTeamId();

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
            case GameState::InfoMenu:
                m_ui.screenInfo(m_zorbs);
                m_gameState = GameState::MainMenu;
                break;
            case GameState::GameOver:
                m_ui.screenGameOver();
                char gameOverInput;
                std::cin >> gameOverInput;
                handleGameOverInput(gameOverInput);
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
    m_playerZorbs.clear();
    m_enemyZorbs.clear();
}
//endregion

//region Input Handling
void GameManager::handleMainMenuInput(char input) {
    switch (input) {
        case '1':
            //m_gameState = GameState::Game;
            break;
        case '2':
            m_gameState = GameState::InfoMenu;
            break;
        case '3':
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
            m_ui.SetDisplayFormat(COMPACT);
            break;
        case 'A':
        case 'a':
            m_ui.screenDebugColors();
            break;
        case 'B':
        case 'b':
            m_ui.screenDebugZorbs();
            break;
        case 'C':
        case 'c':
            ChangeConsoleTheme();
            break;
        case 'q':
        case 'Q':
            m_gameState = GameState::MainMenu;
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
//endregion

//region Zorb Management
void GameManager::SortZorbsByTeamId() {
    for (auto& zorb : m_zorbs) {
        if (zorb.GetTeamId() == 0) {
            m_playerZorbs.push_back(&zorb);
        }
        else if (zorb.GetTeamId() == 1) {
            m_enemyZorbs.push_back(&zorb);
        }
        else {
            m_nzorbs.push_back(&zorb);
        }
    }
}
//have a function that generates a random zorb
void GameManager::GenerateZorb(int team_id) {
    //randomly generate the power for this zorb object
    //make there a factor on the integer power depending on the amount of wins the player has and the size of the vector of zorbs the player has
    int power;
    power = rand() % (5 + m_playerScore) + 1;

    //generate a random zorb
    Zorb newZorb = Zorb(power, team_id, zorbNames[rand() % zorbNames.size()]+' '+zorbNames[rand() % zorbNames.size()]);
    //add it to the vector of zorbs
    m_zorbs.push_back(newZorb);
}

//endregion
