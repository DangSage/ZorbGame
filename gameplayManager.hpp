#pragma once

#include "Zorb.hpp"
#include "zUI.hpp"
/*GameplayManager class will handle all of the actual in-game logic while simultaneously accessing the logic
of the main GameManager. This will include battle behaviour between zorbs as well as recruitment of zorbs
and encounters within the gameplay of Zorb Zenith.*/

//forward declaration of the Battle class
class Battle;

//region Gameplay Generation Functions

//endregion

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

class GameplayManager {
public:
    GameplayManager(UI& ui): m_ui(ui) {} //default constructor
    ~GameplayManager() {
        //clear and destroy all shared pointers, then clear the vector
        m_playerParty.clear();
        m_enemyParty.clear();
        m_zorbs.clear();
        m_winCount = 0;
        m_winCount = nullptr;

    }

    //region Gameplay Functions
    void gameplayLoop();
    void updateZorbs();
    //endregion

    void EraseZorb(std::shared_ptr<Zorb> zorb) { // Erase a zorb from the zorb vector
        m_zorbs.erase(std::find(m_zorbs.begin(), m_zorbs.end(), zorb));

        // Check if the zorb is a in the player party or enemy party
        if (zorb->GetTeamId() == 1)
            m_playerParty.erase(std::find(m_playerParty.begin(), m_playerParty.end(), zorb));
        else if (zorb->GetTeamId() == 2)
            m_enemyParty.erase(std::find(m_enemyParty.begin(), m_enemyParty.end(), zorb));
    }
private:
    //region Gameplay Variables
    friend class Battle;
    UI& m_ui;
    Battle* battle = nullptr;
    std::vector<std::shared_ptr<Zorb>> m_zorbs;         // All zorbs in the game
    std::vector<std::shared_ptr<Zorb>> m_playerParty;   //shared pointer vector of zorbs that are in the player's party
    std::vector<std::shared_ptr<Zorb>> m_enemyParty;    //shared pointer of zorbs that are in the enemy's party
    GameplayState m_gpState = GameplayState::Intro;
    //endregion

    int* m_winCount = &winCounter;
    //create list of strings of all the names of the zorbs
    std::vector<std::string> zorbNameRecord;

    //region State Handling Functions
    void handleIntroState();
    void handleBarberState();
    void handleRecruitState();
    void handleBattleState();
    void handleInfoMenuState();
    void handleDeathState();
    void handleExitGameState();
    //endregion

    //region input handling functions
    void handleBattleInput();
    void handleBarberInput();
    void handleRecruitInput(Zorb& recruit);
    void handleGameOverInput();
    //endregion

    Zorb GenerateRecruit(std::vector<std::shared_ptr<Zorb>>& team, std::array<std::string_view, 2> colorvar);
};

class Battle {
public:
    Battle(GameplayManager& manager) : _gpM(manager),
    b_zorbs(_gpM.m_zorbs), b_enemies(_gpM.m_enemyParty), b_players(_gpM.m_playerParty), leaveBattle(false)
    {}
    ~Battle() {
        casualties = 0;
        turnCount = 1;
    }
    void Update();
    void End();
    
    bool leaveBattle;

    void handleEnemyTurn();
    void generateEnemyParty();
    std::shared_ptr<Zorb> attackLogic(std::shared_ptr<Zorb> attacker, std::shared_ptr<Zorb> defender); //attack logic function
private:
    GameplayManager& _gpM;
    GameplayState& gameplayState = _gpM.m_gpState;
    int& turnCount = turnCounter;
    int& casualties = casualtyCounter;

    // references to the vectors in the GameplayManager class
    std::vector<std::shared_ptr<Zorb>>& b_zorbs;
    std::vector<std::shared_ptr<Zorb>>& b_enemies;
    std::vector<std::shared_ptr<Zorb>>& b_players;    
};

//region GameplayManager Functions
void GameplayManager::gameplayLoop() {
    // Gamestate loop for the gameplay
    while (true) {
        updateZorbs();
        if(m_gpState == GameplayState::Game) {
            int chance = z_debug::RandomValue(1, 200);
            if(chance >= 100)
                m_gpState = GameplayState::Battle;
            else if(chance >= 50)
                m_gpState = GameplayState::Recruit;
            else
                m_gpState = GameplayState::Barber;
        }
        // Handle each game state
        switch (m_gpState) {
            case GameplayState::Intro:
                handleIntroState();
                break;
            case GameplayState::Barber:
                handleBarberState();
                break;
            case GameplayState::Recruit:
                handleRecruitState();
                break;
            case GameplayState::Battle:
                handleBattleState();
                break;
            case GameplayState::Death:
                handleDeathState();
                break;
            case GameplayState::ExitGame:
                handleExitGameState();
                return;
            default:
                break;
        }
    }
}
void GameplayManager::updateZorbs() {
    // clear the player and enemy party vectors
    m_playerParty.clear();
    m_enemyParty.clear();

    // Loop through the m_zorbs vector
    for (const auto& zorb : m_zorbs) {
        if (SharedCast(zorb).GetTeamId() == 1) {
            m_playerParty.emplace_back(zorb);   // Add the zorb to the player party vector
        }
        else if (SharedCast(zorb).GetTeamId() == 2) {
            m_enemyParty.emplace_back(zorb);    // Add the zorb to the enemy party vector
        }

        // delete imploded zorbs
        if (SharedCast(zorb).GetPower() == -1) {
            m_zorbs.erase(std::find(m_zorbs.begin(), m_zorbs.end(), zorb));
        }
    }
}

//endregion

//region Battle Functions
void Battle::Update() {
    //update the zorbs
    generateEnemyParty();
    
    // while the player party and enemy party are not empty
    while((b_players.size() > 0 && b_enemies.size() > 0) && leaveBattle == false) {
        //display the battle screen
        _gpM.m_ui.screenBattle(SharedCast(b_players), SharedCast(b_enemies), "Player Team", "Enemy Team");
        _gpM.handleBattleInput(); //handle the battle input
    }
    //end the battle
    End();
}
void Battle::End() {
    std::cout << z_debug::CenterAlignString("AFTERMATH", CONSOLESIZE) << std::endl;
    // Check if the player party is empty
    if (b_players.size() == 0) {
        std::cout << "You lost the battle!" << std::endl;
        _pauseSystem();
        gameplayState = GameplayState::Death;
    }
    // Check if the enemy party is empty
    else if (b_enemies.size() == 0) {
        std::cout << "You won the battle!" << std::endl;
        _pauseSystem();
        winCounter++;
        gameplayState = GameplayState::Game;
    }
    else {  // Runaway
        std::cout << "You ran away from the battle!" << std::endl;
        
        if(gameplayState != GameplayState::ExitGame) {
            if(z_debug::RandomValue(1, 2) == 1) {
                std::cout << std::endl << "The enemy team lost your trail and you escaped!" << std::endl;

                // empty the enemy party vector and get rid of corresponding zorbs
                b_enemies.clear();
                auto new_end = std::remove_if(b_zorbs.begin(), b_zorbs.end(),
                    [](const auto& zorb) { return SharedCast(zorb).GetTeamId() == 2; });

                b_zorbs.erase(new_end, b_zorbs.end());
            } else {
                std::cout << std::endl << z_debug::FormattedText("The enemy team lost you for now...", ansi::YELLOW) << std::endl;
            }
            _pauseSystem();
            gameplayState = GameplayState::Game;
        } else {
            gameplayState = GameplayState::ExitGame;
        }
    }
    casualties = 0;
}
void Battle::handleEnemyTurn() {
    if (z_debug::RandomValue(1, 2) == 1) {
        int zorbIndex = z_debug::RandomValue(0, static_cast<int>(_gpM.m_enemyParty.size()) - 1); // Choose a random zorb from the enemy party vector
        int enemyZorbIndex = z_debug::RandomValue(0, static_cast<int>(_gpM.m_playerParty.size()) - 1); // Choose a random zorb from the player party vector
        
        _gpM.m_zorbs.emplace_back(attackLogic(_gpM.m_enemyParty[zorbIndex], _gpM.m_playerParty[enemyZorbIndex])); // Call the attackLogic function with the zorb in the enemy party vector and the zorb in the player party vector
    } else {
        int zorbIndex = z_debug::RandomValue(0, static_cast<int>(_gpM.m_enemyParty.size()) - 1); // Choose a random zorb from the enemy party vector

        _gpM.m_zorbs.emplace_back(std::make_shared<Zorb>(2)); // pushback a new empty sharedptr zorb into the zorb vector
        _gpM.updateZorbs();
        _gpM.m_enemyParty[zorbIndex].swap(_gpM.m_enemyParty.back()); // The zorb in the enemy party vector "dodges" by swapping spaces with a new empty spot in the party vector
    }
}
void Battle::generateEnemyParty() {
    std::cout << "Generating enemy party..." << std::endl;

    //Randomly generate a number of enemies based on the number of player zorbs
    int lowerBound,
        upperBound;
    b_players.size() > 4 ? lowerBound = b_players.size() - 4 : lowerBound = 1;
    b_players.size() > 4 ? upperBound = b_players.size() + 2 : upperBound = b_players.size() + 1;

    //generate a random number of enemies
    int numEnemies = z_debug::RandomValue(lowerBound, upperBound);
    
    //loop through the player party vector
    for(int i=0; i<numEnemies; i++) {
        Zorb recruit = _gpM.GenerateRecruit(b_enemies, CG_ENEMY); //generate a random enemy zorb
        
        //emplace the new zorb into the zorb vector
        b_zorbs.emplace_back(std::make_shared<Zorb>(recruit));
    }

    _gpM.updateZorbs();
}
std::shared_ptr<Zorb> Battle::attackLogic(std::shared_ptr<Zorb> attacker, std::shared_ptr<Zorb> defender) {
    
    Zorb newZorb = *attacker + *defender; // addition operator overload to get attack result in a new zorb
    // tell the player the about the attack
    std::cout << DLINE << attacker->GetName() << " attacked " << defender->GetName() << " with " << attacker->GetPower() << " power!" << std::endl;
    
    // check if the attack was dodged
    if(newZorb.GetName() == "~dodged") {
        std::cout << defender->GetName() << " dodged the attack!" << std::endl;
        // Return a shared pointer to the new zorb
        return std::make_shared<Zorb>(newZorb);
    }

    const int attackerTeamId = (*attacker).GetTeamId();
    const int defenderTeamId = (*defender).GetTeamId();
    if (newZorb.GetTeamId() == attackerTeamId) {
        casualties++;
        _gpM.m_ui.screenFightOutcome(newZorb, *defender, "The attack was effective!");
        
    } else if (newZorb.GetTeamId() == defenderTeamId) {
        casualties++;
        _gpM.m_ui.screenFightOutcome(*attacker, *defender, "The attack was ineffective!");

    } else if (newZorb.GetTeamId() == -1) {
        casualties+=2;
        std::cout << ansi::RED << "OOPS!" << ansi::RESET;
        z_debug::clearInputBuffer();
        _gpM.m_ui.screenFightOutcome(newZorb, *defender, "OH SHOOT! backfire!");   

    } else {
        std::cout << attackerTeamId << ' ' << defenderTeamId << std::endl << "New Zorb team id: " << newZorb.GetTeamId() << std::endl;
        z_debug::PrintError("Battle::attackLogic() - HDWGH?");
    }    

    _gpM.EraseZorb(attacker);
    _gpM.EraseZorb(defender);
    // Return a shared pointer to the new zorb
    return std::make_shared<Zorb>(newZorb);
}
//endregion

//region State Handling Functions
void GameplayManager::handleIntroState() {
    m_zorbs.emplace_back(std::make_shared<Zorb>(ZorbAppearance(static_cast<appearanceEnum>(0), ansi::GREEN), 1, 1, "Neep Narp"));
    zorbNameRecord.emplace_back("Neep Narp");
    m_gpState = GameplayState::Battle;
}
void GameplayManager::handleBarberState() {
    m_ui.screenBarber(SharedCast(m_playerParty));
    handleBarberInput();
}
void GameplayManager::handleRecruitState() {
    Zorb recruit = GenerateRecruit(m_playerParty, CG_PLAYER);
    m_ui.screenRecruitment(recruit, SharedCast(m_playerParty));
    handleRecruitInput(recruit);
    m_gpState = GameplayState::Battle;
}
void GameplayManager::handleBattleState() {
    if(battle != nullptr) {
        delete battle;
        battle = nullptr;
    }
    battle = new Battle(*this);
    battle->Update();
}
void GameplayManager::handleInfoMenuState() {}
void GameplayManager::handleDeathState() {
    m_ui.screenGameOver(zorbNameRecord);
    handleGameOverInput();
}
void GameplayManager::handleExitGameState() {
    // If the game state is ExitGame, break the loop
    m_winCount = 0;
    std::cout << std::endl << "Quitting game..." << std::endl;
            _pauseSystem();
    return;
}
//endregion

//region Input Handling
void GameplayManager::handleBattleInput() {
    int zorbIndex,
        enemyZorbIndex;
    static int dodgeIndex = -1;

    // Generate a list of numbers from 1 to the size of the player party vector that is viable for input
    // Any zorbs that have been dodged will not be able to be selected
    std::vector<int> zorbIndexList;
    for (int i = 0; i < m_playerParty.size(); i++) {
        if (m_playerParty[i]->GetPower() != -1) {
            zorbIndexList.emplace_back(i+1);
        }
    }
    // list of numbers from 1 to the size of the enemy party vector that is viable for input
    // Any zorbs that have been dodged will not be able to be selected
    std::vector<int> enemyIndexList;
    for (int i = 0; i < m_enemyParty.size(); i++) {
        if (m_enemyParty[i]->GetPower() != -1) {
            enemyIndexList.emplace_back(i+1);
        }
    }

    // Get the player's input for choices in the battle
    char inputChoice[] = {'1','2','3','Q'};
    switch(validatedInput<char>(inputChoice)) {
        case '1':
            std::cout << DLINE << z_debug::CenterAlignString("Attacking:", CONSOLESIZE) << std::endl 
                << "Choose a Zorb to attack with: ";
            zorbIndex = validatedInput<int>(zorbIndexList)-1; // input for which zorb to attack with
            std::cout << DLINE << '[' << zorbIndex+1 << "] Choose an Enemy Zorb to attack: ";
            enemyZorbIndex = validatedInput<int>(enemyIndexList)-1; // input for which enemy zorb to attack

            // attackLogic function with the zorb in the player party vector and the zorb in the enemy party vector
            m_zorbs.emplace_back(battle->attackLogic(m_playerParty[zorbIndex], m_enemyParty[enemyZorbIndex]));
            updateZorbs();
            break;
        case '2':
            std::cout << DLINE << z_debug::CenterAlignString("Dodging:", CONSOLESIZE) << std::endl 
                << "Choose a Zorb to dodge with: ";
            dodgeIndex = validatedInput<int>(zorbIndexList)-1; // input for which zorb to dodge with

            // pushback a new empty sharedptr zorb into the zorb vector
            m_zorbs.emplace_back(std::make_shared<Zorb>(1));

            //temporarily add 1 power to the zorb that dodged
            m_playerParty[dodgeIndex]->SetPower(m_playerParty[dodgeIndex]->GetPower()+1);
            updateZorbs();

            // The zorb in the player party vector "dodges" by swapping spaces with a new empty spot in the party vector
            m_playerParty[dodgeIndex].swap(m_playerParty.back());
            
            // Reset the dodgeIndex to be the index of the new empty zorb
            dodgeIndex = m_playerParty.size()-1;
            break;
        case '3':
            battle->leaveBattle = true;
            m_gpState = GameplayState::Game;
            return;
        case 'Q':
            battle->leaveBattle = true;
            m_gpState = GameplayState::ExitGame;
            return;
        default:
            z_debug::PrintError("GameManager::handleMainMenuInput() - HDWGH?");
            break;
    }

    // Enemy turn
    //battle->handleEnemyTurn();
    // delete dodged zorbs
    for (auto& zorb : m_zorbs) {
        if (SharedCast(zorb).GetPower() == -1 && SharedCast(zorb).GetName() == "~dodged") {
            m_zorbs.erase(std::find(m_zorbs.begin(), m_zorbs.end(), zorb));
        }
    }

    turnCounter++;
    // remove 1 power from the zorbs that have dodged using the static dodgeIndex variable once one turn has passed
    // if (dodgeIndex != -1 && m_playerParty[dodgeIndex]->GetPower() > 0) {
    //     m_playerParty[dodgeIndex]->SetPower(m_playerParty[dodgeIndex]->GetPower()-1);
    //     dodgeIndex = -1;
    // }
}
void GameplayManager::handleBarberInput() {
    std::cout << "Do you trust him? : ";
    char inputs[] = {'Y','N','Q'};
    std::string goodbyeText = randomChoice({"Au voire", "See you later", "Goodbye", "Bye", "See ya", "Adios", "Sayonara", "Adieu", "Farewell", "Ciao"});
    switch (validatedInput<char>(inputs)) {
        case 'Y':
            break;
        case 'N':
            std::cout << std::endl << z_debug::FormattedText("''"+goodbyeText+", friend(s)''", ansi::YELLOW) << std::endl;
            _pauseSystem();
            m_gpState = GameplayState::Battle;
            break;
        case 'Q':
            m_gpState = GameplayState::ExitGame;
            break;
        default:
            z_debug::PrintError("GameManager::handleMainMenuInput() - HDWGH?");
            break;
    }
}
void GameplayManager::handleRecruitInput(Zorb& recruit) {
    std::cout << "Do you trust him? : ";
    std::string chant = randomChoice({"Hooray!", "Yay!", "Yippee!"});
    char inputs[] = {'Y','N','Q'};
    switch (validatedInput<char>(inputs)) {
        case 'Y':
            m_zorbs.emplace_back(std::make_shared<Zorb>(recruit));
            zorbNameRecord.emplace_back(recruit.GetName());  //add the new zorbs name to the list of zorb names
            std::cout << std::endl << z_debug::FormattedText("'"+recruit.GetName()+"' joined your party! ", ansi::YELLOW) << chant << std::endl;
            _pauseSystem();
            m_gpState = GameplayState::Battle;
            break;
        case 'N':
            std::cout << std::endl << z_debug::FormattedText("'"+recruit.GetName()+"' wanders back into the wild...", ansi::YELLOW) << std::endl;
            _pauseSystem();
            m_gpState = GameplayState::Game;
            break;
        case 'Q':
            m_gpState = GameplayState::ExitGame;
            break;
        default:
            z_debug::PrintError("GameManager::handleMainMenuInput() - HDWGH?");
            break;
    }
}
void GameplayManager::handleGameOverInput() {
    char input[] = {'Q'};
    switch (validatedInput<char>(input)) {
        case 'Q':
            m_gpState = GameplayState::ExitGame;
            break;
        default:
            z_debug::PrintError("GameManager::handleMainMenuInput() - HDWGH?");
            break;
    }
}
//endregion

//region Gameplay Generation Functions
Zorb GameplayManager::GenerateRecruit(std::vector<std::shared_ptr<Zorb>>& team, std::array<std::string_view, 2> colorvar) {
    // Generate a random appearance
    ZorbAppearance recruitAppearance = GenRandomAppearance(colorvar);
        // Generate a random name
    std::string recruitName = zorb::RandomName();
        // Generate a random power, using the some of the player's zorbs' power as a weight
    int recruitPower = 1;
    int recruitId;

    if (team.empty()) // if the team is empty, set recruitId to 2 (the first recruitId for the enemy team)
        recruitId = 2;
    else
        recruitId = team.back()->GetTeamId(); // set recruitId to the id of the last zorb in the team + 1

    
    for (const auto& zorb : m_playerParty) {
        recruitPower += zorb->GetPower();
    }

    // create a scaling system for the recruit power that takes into account the following
    // 1. the number of zorbs in the player's party
    // 2. the average power of the player's zorbs
    // 3. the amount of wins the player has
    if(winCounter > 0) {
        recruitPower = (recruitPower / m_playerParty.size()) + z_debug::RandomValue(-winCounter, (recruitPower / (2*winCounter)));
    } else {
        recruitPower = 1;
    }

    return Zorb(recruitAppearance, recruitPower, recruitId, recruitName);
}
//endregion