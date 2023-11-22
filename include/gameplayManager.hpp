#ifndef GAMEPLAYMANAGER_HPP
#define GAMEPLAYMANAGER_HPP

#include <string>
#include <vector>

// Forward declarations
class Zorb;
class UI;
class Battle;
class TempBuff;

enum class GameplayState {
    Start,
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
        m_playerz.clear();
        m_enemyz.clear();
        m_zorbs.clear();
        m_winCount = nullptr;
    }

    std::string journeyName;

    void gameplayLoop();
    void updateZorbs();    
    void EraseZorb(std::shared_ptr<Zorb> zorb);
private:
    friend class Battle;
    UI& m_ui;

    Battle* battle = nullptr;

    std::vector<std::shared_ptr<Zorb>> m_zorbs;
    std::vector<std::shared_ptr<Zorb>> m_playerz;
    std::vector<std::shared_ptr<Zorb>> m_enemyz;

    GameplayState m_gpState = GameplayState::Start;

    int* m_winCount = &winCounter;
    int* m_battleCount = &battleCounter;
    std::vector<std::string> zorbNameRecord;

    void handleStartState();
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
    Battle(GameplayManager& manager);

    ~Battle() {
        casualties = 0;
        turnCount = 1;
        z_debug::_tag("Battle ended");
    }

    void Update();
    void End();

    bool leaveBattle;
    void handleEnemyTurn();
    void generateEnemyParty();
    void Dodge(std::shared_ptr<Zorb>& zorb, int& dodgeIndex);
    std::shared_ptr<Zorb> attackLogic(std::shared_ptr<Zorb> attacker, std::shared_ptr<Zorb> defender);
private:
    GameplayManager& _gpM;
    GameplayState& gameplayState = _gpM.m_gpState;

    int& turnCount = turnCounter;
    int& casualties = casualtyCounter;

    std::vector<std::shared_ptr<Zorb>>& b_zorbs = _gpM.m_zorbs;

    std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string> playerTeam;
    std::pair<std::vector<std::shared_ptr<Zorb>>&, std::string> enemyTeam;
};


#endif // GAMEPLAYMANAGER_HPP