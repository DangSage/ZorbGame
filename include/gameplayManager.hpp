#ifndef GAMEPLAYMANAGER_HPP
#define GAMEPLAYMANAGER_HPP

#include "zUI.hpp"

class Battle;

enum class GameplayState {
    Intro,
    Game,
    Battle,
    Barber,
    Recruit,
    InfoMenu,
    Death,
    ExitGame = -1
};

// GameplayManager class that manages the gameplay loop, gamestate and input handling
class GameplayManager {
public:
    GameplayManager(UI& ui): m_ui(ui) {}

    ~GameplayManager() {
        m_playerParty.clear();
        m_enemyParty.clear();
        m_zorbs.clear();
        m_winCount = nullptr;
    }

    void gameplayLoop();
    void updateZorbs();    
    void EraseZorb(std::shared_ptr<Zorb> zorb);
private:
    friend class Battle;
    UI& m_ui;

    Battle* battle = nullptr;

    std::vector<std::shared_ptr<Zorb>> m_zorbs;
    std::vector<std::shared_ptr<Zorb>> m_playerParty;
    std::vector<std::shared_ptr<Zorb>> m_enemyParty;

    GameplayState m_gpState = GameplayState::Intro;

    int* m_winCount = &winCounter;
    std::vector<std::string> zorbNameRecord;

    void handleIntroState();
    void handleBarberState();
    void handleRecruitState();
    void handleBattleState();
    void handleInfoMenuState();
    void handleDeathState();
    void handleExitGameState();

    void handleBattleInput();
    void handleBarberInput();
    void handleRecruitInput(Zorb& recruit);
    void handleGameOverInput();    

    Zorb GenerateRecruit(std::vector<std::shared_ptr<Zorb>>& team, std::array<std::string_view, 2> colorvar);
};

// Battle class that manages the battle loop, battlestate and attack logic
class Battle {
public:
    Battle(GameplayManager& manager) : _gpM(manager),
    b_zorbs(_gpM.m_zorbs), b_enemies(_gpM.m_enemyParty), b_players(_gpM.m_playerParty), leaveBattle(false) {}

    ~Battle() {
        casualties = 0;
        turnCount = 1;
    }

    void Update();
    void End();
    bool leaveBattle;
    void handleEnemyTurn();
    void generateEnemyParty();
    std::shared_ptr<Zorb> attackLogic(std::shared_ptr<Zorb> attacker, std::shared_ptr<Zorb> defender);
private:
    GameplayManager& _gpM;
    GameplayState& gameplayState = _gpM.m_gpState;

    int& turnCount = turnCounter;
    int& casualties = casualtyCounter;

    std::vector<std::shared_ptr<Zorb>>& b_zorbs;
    std::vector<std::shared_ptr<Zorb>>& b_enemies;
    std::vector<std::shared_ptr<Zorb>>& b_players;    
};

#endif // GAMEPLAYMANAGER_HPP