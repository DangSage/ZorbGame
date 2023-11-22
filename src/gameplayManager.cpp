#include "pch.hpp"
#include "gameplayManager.hpp"
#include "zUI.hpp"
#include "Zorb.hpp"

std::string journeyName; // The name of the save and the player team

const int USER_BACK_INPUT = -2;
const int INDEX_OFFSET = -1;


//region GameplayManager Functions
void GameplayManager::gameplayLoop() {
    // Gamestate loop for the gameplay
    while (true) {
        updateZorbs();
        if(m_gpState == GameplayState::Game) {
            int chance = z_util::random::value(1, 200);
            if(chance >= 100)
                m_gpState = GameplayState::Battle;
            else if(chance >= 50)
                m_gpState = GameplayState::Recruit;
            else
                m_gpState = GameplayState::Barber;
        }
        // Handle each game state
        switch (m_gpState) {
            case GameplayState::Start:
                handleStartState();
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
    m_playerz.clear();
    m_enemyz.clear();

    // remove all zorbs that are nullptrs and don't have powers or names
    m_zorbs.erase(std::remove_if(m_zorbs.begin(), m_zorbs.end(),
        [](const auto& zorb) { return zorb == nullptr || *zorb == Zorb{} || zorb->GetPower()<=0; }), m_zorbs.end());

    // Loop through the m_zorbs vector
    for (const auto& zorb : m_zorbs) {
        zorb->UpdateBuffs();    // Update the zorb's buffs
        if ((*zorb).GetTeamId() == PLAYERTEAM) {
            m_playerz.emplace_back(zorb);   // Add the zorb to the player party vector
        }
        else if ((*zorb).GetTeamId() == ENEMYTEAM) {
            m_enemyz.emplace_back(zorb);    // Add the zorb to the enemy party vector
        }
    }
}
//endregion

void GameplayManager::EraseZorb(std::shared_ptr<Zorb> zorb) { // Erase a zorb from the zorb vector
    m_zorbs.erase(std::find(m_zorbs.begin(), m_zorbs.end(), zorb));

    // Check if the zorb is a in the player party or enemy party
    if (zorb->GetTeamId() == PLAYERTEAM)
        m_playerz.erase(std::find(m_playerz.begin(), m_playerz.end(), zorb));
    else if (zorb->GetTeamId() == ENEMYTEAM)
        m_enemyz.erase(std::find(m_enemyz.begin(), m_enemyz.end(), zorb));
}

//region State Handling Functions
void GameplayManager::handleStartState() {
    m_zorbs.emplace_back(std::make_shared<Zorb>(Zorb(ZorbAppearance(appearanceEnum::A_DEFAULT, ansi::GREEN), 1, 1, "Neep Narp")));
    zorbNameRecord.emplace_back("Neep Narp");

    m_ui.screenStart();
    while(true) {
        std::cout << std::string((CONSOLESIZE/3),' ') << ">" << ansi::GREEN;
        std::getline(std::cin, journeyName);
        if(journeyName.size() > 0 && journeyName.size() <= 50) {
            std::cout << ansi::RESET << z_util::CenterAlignString("You have named this journey: " + journeyName) 
                << "\n\n\n\n" << std::endl;
            _pauseSystem();
            break;
        }
        else if (journeyName.size() > 50)
            std::cout << z_util::CenterAlignString(z_util::FormattedText("Enter a shorter name!", ansi::RED))
            << ansi::UPLINE << ansi::UPLINE << std::endl;
        else {
            journeyName = "Expedition 1";
            std::cout << ansi::RESET << z_util::CenterAlignString("You have named this journey: " + journeyName) 
                << "\n\n\n\n" << std::endl;
            _pauseSystem();
            break;
        }
    }
    m_gpState = GameplayState::Battle;
}
void GameplayManager::handleBarberState() {
    m_ui.screenBarber(SharedCast(m_playerz));
    handleBarberInput();
}
void GameplayManager::handleRecruitState() {
    Zorb recruit = GenerateRecruit(m_playerz, CG_PLAYER);
    m_ui.screenRecruitment(recruit, SharedCast(m_playerz));
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
    winCounter = 0;
    std::cout << std::endl << "Quitting game..." << std::endl;
            _pauseSystem();
    return;
}
//endregion

//region Input Handling
void GameplayManager::handleBattleInput() {
    int zorbIndex, enemyZorbIndex;
    bool confirmed = false;
    static int dodgeIndex;

    auto updateZorbsAndBuildIndexList = [&](std::vector<std::shared_ptr<Zorb>>& zorbs) {
        std::vector<int> indexList = {-1};
        for (int i = 0; i < zorbs.size(); i++) {
            if (zorbs[i]->GetPower() > 0) {
                indexList.emplace_back(i+1);
            }
        }
        return indexList;
    };
    
    std::vector<int> zorbIndexList = updateZorbsAndBuildIndexList(m_playerz);
    std::vector<int> enemyIndexList = updateZorbsAndBuildIndexList(m_enemyz);

    // Get the player's input for choices in the battle
    char inputChoice[] = {'1','2','3',EXITCOMMAND};
    while (!confirmed) {
        std::cout << "Enter your choice: ";
        switch(validatedInput<char>(inputChoice)) {
            case '1':
                std::cout << ansi::DLINE << z_util::CenterAlignString("Select: Attack (-1 to return)", CONSOLESIZE) << std::endl 
                        << "Choose a Zorb to attack with: ";
                zorbIndex = validatedInput<int>(zorbIndexList) - 1;

                if (zorbIndex == USER_BACK_INPUT) { // User entered -1, but we subtracted 1
                    for(int i=0; i<2; i++)
                        std::cout << ansi::DLINE;
                    continue; // Go back to the start of the loop
                }

                std::cout << *m_playerz[zorbIndex] << std::endl;
                for(int i=0; i<8; i++)
                        std::cout << ansi::UPLINE;
                std::cout << ansi::DLINE << "Choose a Zorb to attack: ";
                enemyZorbIndex = validatedInput<int>(enemyIndexList) - 1;

                if (enemyZorbIndex == USER_BACK_INPUT) { // User entered -1, but we subtracted 1
                    for(int i=0; i<9; i++)
                        std::cout << std::endl;
                    for(int i=0; i<11; i++)
                        std::cout << ansi::DLINE;
                    continue; // Go back to the start of the loop
                }

                // attackLogic function with the zorb in the player party vector and the zorb in the enemy party vector
                m_zorbs.emplace_back(battle->attackLogic(m_playerz[zorbIndex], m_enemyz[enemyZorbIndex]));
                break;
            case '2':
                std::cout << ansi::DLINE << z_util::CenterAlignString("Select: Dodging (-1 to return)", CONSOLESIZE) << std::endl 
                    << "Choose a Zorb to dodge with: ";
                dodgeIndex = validatedInput<int>(zorbIndexList)-1; // input for which zorb to dodge with

                if (dodgeIndex == USER_BACK_INPUT) { // User entered -1, but we subtracted 1
                    for(int i=0; i<2; i++)
                        std::cout << ansi::DLINE;
                    continue; // Go back to the start of the loop
                }

                battle->Dodge(m_playerz[dodgeIndex], dodgeIndex); // dodge function with the zorb in the player party vector
                break;
            case '3':
                std::cout << "You tried to flee!" << std::endl << "\nRolling to see if you can escape..." << std::endl;
                std::cin.get();
                if(z_util::random::value(1, 100) >= 35) {
                    std::cout << "Successfully ran away!" << std::endl;
                    battle->leaveBattle = true;
                    m_gpState = GameplayState::Game;
                    return;
                } else {
                    std::cout << "Failed to run away!" << std::endl;
                }
                break;
            case EXITCOMMAND:
                battle->leaveBattle = true;
                m_gpState = GameplayState::ExitGame;
                return;
            default:
                zException exc = UnexpectedCallException("GameManager::handleBattleInput()");
                z_debug::PrintError(exc);
                return;
        }
        confirmed = true;
    }

    m_zorbs.erase(std::remove_if(m_zorbs.begin(), m_zorbs.end(),
            [](const auto& zorb) { return zorb == nullptr || *zorb == Zorb{} || zorb->GetPower()<0; }), m_zorbs.end());
    turnCounter++;
}
void GameplayManager::handleBarberInput() {
    zException NIexc = NotImplementedException("GameManager::handleBarberInput()");
    std::cout << "Do you trust him? : ";
    char inputs[] = {'Y','N',EXITCOMMAND};
    std::string goodbyeText = z_util::random::choice({"Au voire", "See you later", "Goodbye", "Bye", "See ya", "Adios", "Sayonara", "Adieu", "Farewell", "Ciao"});
    switch (validatedInput<char>(inputs)) {
        case 'Y':
            z_debug::PrintError(NIexc);
            break;
        case 'N':
            std::cout << std::endl << z_util::FormattedText("''"+goodbyeText+", friend(s)''", ansi::YELLOW) << std::endl;
            _pauseSystem();
            m_gpState = GameplayState::Battle;
            break;
        case EXITCOMMAND:
            m_gpState = GameplayState::ExitGame;
            break;
        default:
            zException UCexc = UnexpectedCallException("GameManager::handleBarberInput()");
            z_debug::PrintError(UCexc);
            break;
    }
}
void GameplayManager::handleRecruitInput(Zorb& recruit) {
    std::cout << "Do you trust him? : ";
    std::string chant = z_util::random::choice({"Hooray!", "Yay!", "Yippee!"});
    char inputs[] = {'Y','N',EXITCOMMAND};
    switch (validatedInput<char>(inputs)) {
        case 'Y':
            m_zorbs.emplace_back(std::make_shared<Zorb>(recruit));
            zorbNameRecord.emplace_back(recruit.GetName());  //add the new zorbs name to the list of zorb names
            std::cout << std::endl << z_util::FormattedText("'"+recruit.GetName()+"' joined your party! ", ansi::YELLOW) << chant << std::endl;
            _pauseSystem();
            m_gpState = GameplayState::Battle;
            break;
        case 'N':
            std::cout << std::endl << z_util::FormattedText("'"+recruit.GetName()+"' wanders back into the wild...", ansi::YELLOW) << std::endl;
            _pauseSystem();
            m_gpState = GameplayState::Game;
            break;
        case EXITCOMMAND:
            m_gpState = GameplayState::ExitGame;
            break;
        default:
            zException exc = UnexpectedCallException("GameManager::handleRecruitInput()");
            z_debug::PrintError(exc);
            break;
    }
}
void GameplayManager::handleGameOverInput() {
    char input[] = {EXITCOMMAND};
    switch (validatedInput<char>(input)) {
        case EXITCOMMAND:
            m_gpState = GameplayState::ExitGame;
            break;
        default:
            zException exc = UnexpectedCallException("GameManager::handleGameOverInput()");
            z_debug::PrintError(exc);
            break;
    }
}
//endregion

Zorb GameplayManager::GenerateRecruit(std::vector<std::shared_ptr<Zorb>>& team, std::array<std::string_view, 2> colorvar) {
    // Generate a random appearance
    ZorbAppearance recruitAppearance = GenRandomAppearance(colorvar);
        // Generate a random name
    std::string recruitName = zorb::RandomName();
        // Generate a random power, using the some of the player's zorbs' power as a weight
    int recruitPower = 1;
    int recruitId;

    if (team.empty()) // if the team is empty, set recruitId to 2 (the first recruitId for the enemy team)
        recruitId = 2;  // Only the enemy team would be empty and calling this function
    else
        recruitId = team.back()->GetTeamId(); // set recruitId to the id of the last zorb in the team + 1

    
    for (const auto& zorb : m_playerz) {
        recruitPower += zorb->GetPower();
    }

    // create a scaling system for the recruit power that takes into account the following
    // 1. the number of zorbs in the player's party
    // 2. the average power of the player's zorbs
    // 3. the amount of wins the player has
    if(winCounter > 0) {
        recruitPower = (recruitPower / m_playerz.size()) + z_util::random::value(-winCounter, (recruitPower / (2*winCounter)));
    } else {
        recruitPower = 1;
    }

    return Zorb(recruitAppearance, recruitPower, recruitId, recruitName);
}
//endregion