#include <vector>
#include "Zorb.hpp"
#include "zUI.hpp"
#include "gameDefs.hpp"

class GameManager {
public:
    GameManager(UI& ui) : m_ui(ui) {}

    void startGame();
    void pauseGame();
    void resumeGame();
    void endGame();

    void debugScreen();

private:
    UI& m_ui;
    std::vector<Zorb> m_zorbs;  // All zorbs
    std::vector<Zorb*> m_playerZorbs;   // Player zorbs
    std::vector<Zorb*> m_enemyZorbs;    // Enemy zorbs
    std::vector<Zorb*> m_nzorbs;    // npc zorbs
    bool m_isPaused = false;

    void updateGameLogic();
    void renderGame();
};

void GameManager::startGame() {
    // Initialize game objects
    m_zorbs.push_back(Zorb(1)); // Player zorb
    m_zorbs.push_back(Zorb(2)); // Enemy zorb
    m_zorbs.push_back(Zorb(0)); // Neutral zorb

    // Sort zorbs by team ID
    std::sort(m_zorbs.begin(), m_zorbs.end(), [](const Zorb& z1, const Zorb& z2) {
        return z1.GetTeamId() < z2.GetTeamId();
    });

    // Game loop
    while (true) {
        break;
    }
}

void GameManager::pauseGame() {
    m_isPaused = true;
}

void GameManager::resumeGame() {
    m_isPaused = false;
}

void GameManager::endGame() {
    // Clean up game objects
    m_zorbs.clear();
    m_playerZorbs.clear();
    m_enemyZorbs.clear();
}

void GameManager::debugScreen() {
    // Display debug options
    m_ui.DisplayDebugScreen();
}
