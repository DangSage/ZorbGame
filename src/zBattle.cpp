#include "pch.hpp"
#include "zUI.hpp"
#include "gameplayManager.hpp"
#include "Zorb.hpp"

static bool jumpFlag = false;

Battle::Battle(GameplayManager& manager) 
: _gpM(manager), leaveBattle(false), 
playerTeam(_gpM.m_playerz, _gpM.journeyName), 
enemyTeam(_gpM.m_enemyz, "") { battleCounter++; }

void Battle::Update() {
    //update the enemy zorbs
    generateEnemyParty();
    if(jumpFlag)
        _gpM.m_ui.screenBattleEncounterJump(playerTeam, enemyTeam);
    else
        _gpM.m_ui.screenBattleEncounter(playerTeam, enemyTeam);
    
    // while the player party and enemy party are not empty
    while((playerTeam.first.size() > 0 && enemyTeam.first.size() > 0) && leaveBattle == false) {
        
        _gpM.m_ui.screenBattle(playerTeam, enemyTeam);      //display the battle screen
        _gpM.handleBattleInput();                           //handle the battle input
        //handleEnemyTurn();                                // Enemy turn
        _gpM.updateZorbs();                                 //update the zorbs
    }
    //end the battle
    End();
}
void Battle::End() {
    std::cout << z_util::CenterAlignString("AFTERMATH", CONSOLESIZE) << std::endl;
    // Check if the player party is empty
    if (playerTeam.first.size() == 0) {
        std::cout << "You lost the battle!" << std::endl;
        _pauseSystem();
        gameplayState = GameplayState::Death;
    }
    // Check if the enemy party is empty
    else if (enemyTeam.first.size() == 0) {
        std::cout << "You won the battle!" << std::endl;
        _pauseSystem();
        winCounter++;
        gameplayState = GameplayState::Game;
    }
    else {  // Runaway        
        if(gameplayState != GameplayState::ExitGame) {
            if (z_util::random::value(1,2) == 1) {  // enemy party dissapears
                jumpFlag = false;
                enemyTeam.first.clear();
                auto new_end = std::remove_if(b_zorbs.begin(), b_zorbs.end(),
                    [](const auto& zorb) { return (*zorb).GetTeamId() == 2; });
                
                b_zorbs.erase(new_end, b_zorbs.end());
                gameplayState = GameplayState::Game;
            } else {    // enemy party stays in place
                jumpFlag = true;
                gameplayState = GameplayState::Game;
            }
            _pauseSystem();
        } else {
            gameplayState = GameplayState::ExitGame;
        }
    }

    // apply buffs and debuffs
    for (auto& zorb : playerTeam.first) {
        zorb->UpdateBuffs();
    }
    casualties = 0;
}
std::shared_ptr<Zorb> Battle::attackLogic(std::shared_ptr<Zorb> attacker, std::shared_ptr<Zorb> defender) {
    Zorb newZorb = *attacker + *defender; // addition operator overload to get attack result in a new zorb

    // if newZorb is empty, return a shared pointer to the new zorb
    if(newZorb == Zorb()) {
        // It's not possible for the attacker to attack with an empty zorb, so the defender must have dodged
        std::cout << defender->GetName() << " dodged the attack!" << std::endl;
        return std::make_shared<Zorb>(newZorb);
    }

    // tell the player the about the attack
    std::cout << ansi::DLINE << attacker->GetName() << " attacked " << defender->GetName() << " with " << attacker->GetPower() << " power!" << std::endl;

    if (newZorb.GetTeamId() == -1) {
        casualties+=2;
        std::cout << "OOPS!";
        z_util::clearInputBuffer();
        _gpM.m_ui.screenFightOutcome(newZorb, *defender, "OH SHOOT! backfire!");   

    } else if (newZorb.GetTeamId() == attacker->GetTeamId()) {
        casualties++;
        _gpM.m_ui.screenFightOutcome(newZorb, *defender, "The attack was effective!");
        
    } else if (newZorb.GetTeamId() == defender->GetTeamId()) {
        casualties++;
        _gpM.m_ui.screenFightOutcome(newZorb, *attacker, "The attack was ineffective!");

    } else {
        std::cout << attacker->GetTeamId() << ' ' << defender->GetTeamId() << std::endl << "New Zorb team id: " << newZorb.GetTeamId() << std::endl;
        zException exc = UnexpectedCallException("Battle::attackLogic()");
            z_debug::PrintError(exc);
    }

    _gpM.EraseZorb(attacker);
    _gpM.EraseZorb(defender);
    // Return a shared pointer to the new zorb
    return std::make_shared<Zorb>(newZorb);
}
void Battle::Dodge(std::shared_ptr<Zorb>& zorb, int& dodgeIndex) {
    // make sure the zorb is not a nullptr
    if(zorb == nullptr) {
        zException exc = UnexpectedCallException("Battle::Dodge() - zorb is nullptr");
        z_debug::PrintError(exc);
        return;
    }

    auto& team = enemyTeam;
    if(zorb->GetTeamId() == PLAYERTEAM) team = playerTeam;

    // BuffDodge dodgeBuff(1, team.first[dodgeIndex]->GetTeamId()); but shared pointer
    std::shared_ptr<BuffDodge> dodgeBuff = std::make_shared<BuffDodge>(team.first[dodgeIndex]->GetTeamId());

    // add the dodge buff to the zorb
    (*zorb).AddBuff(dodgeBuff);
    dodgeIndex == team.first.size() - 1 ? dodgeIndex = 0 : dodgeIndex++;

    // add the dodged zorb to the b_zorbs vector
    b_zorbs.emplace_back(dodgeBuff->GetDodged());

    // The zorb in the player party vector "dodges" by swapping spaces with a new empty spot in the party vector
    // find the zorb in the b_zorbs vector and swap it with the last zorb in the vector
    std::iter_swap(std::find(b_zorbs.begin(), b_zorbs.end(), zorb), b_zorbs.end() - 1);
}

void Battle::handleEnemyTurn() {
    if (z_util::random::value() == 1) {
        int zorbIndex = z_util::random::value(0, static_cast<int>(_gpM.m_enemyz.size()) - 1); // Choose a random zorb from the enemy party vector
        int enemyZorbIndex = z_util::random::value(0, static_cast<int>(_gpM.m_playerz.size()) - 1); // Choose a random zorb from the player party vector
        
        _gpM.m_zorbs.emplace_back(attackLogic(_gpM.m_enemyz[zorbIndex], _gpM.m_playerz[enemyZorbIndex])); // Call the attackLogic function with the zorb in the enemy party vector and the zorb in the player party vector
    } else {
        int zorbIndex = z_util::random::value(0, static_cast<int>(_gpM.m_enemyz.size()) - 1); // Choose a random zorb from the enemy party vector

        _gpM.m_zorbs.emplace_back(std::make_shared<Zorb>(2)); // pushback a new empty sharedptr zorb into the zorb vector
        _gpM.updateZorbs();
        _gpM.m_enemyz[zorbIndex].swap(_gpM.m_enemyz.back()); // The zorb in the enemy party vector "dodges" by swapping spaces with a new empty spot in the party vector
    }
}
void Battle::generateEnemyParty() {
    std::cout << "Generating enemy party..." << std::endl;
    static std::string ENEMYTEAMNAME;
    // randomly generate a name for the enemy team
    if(!jumpFlag) {
        std::string_view pre = zorb::TEAMPREFIX[z_util::random::value(static_cast<size_t>(0), zorb::TEAMPREFIX.size() - 1)];
        enemyTeam.second = pre.data();
        if(z_util::random::value() == 1) {
            std::string_view suf = zorb::TEAMSUFFIX[z_util::random::value(static_cast<size_t>(0), zorb::TEAMSUFFIX.size() - 1)];
            enemyTeam.second+= ' ';
            enemyTeam.second+= suf.data();
        }
    } else
        enemyTeam.second = ENEMYTEAMNAME;

    //Randomly generate a number of enemies based on the number of player zorbs
    int lowerBound,
        upperBound;
    playerTeam.first.size() > 4 ? lowerBound = playerTeam.first.size() - 4 : lowerBound = 1;
    playerTeam.first.size() > 4 ? upperBound = playerTeam.first.size() + 2 : upperBound = playerTeam.first.size() + 1;

    //generate a random number of enemies
    int numEnemies = z_util::random::value(lowerBound, upperBound);
    
    //loop through the player party vector
    for(int i=0; i<numEnemies; i++) {
        Zorb recruit = _gpM.GenerateRecruit(enemyTeam.first, CG_ENEMY); //generate a random enemy zorb
        
        //emplace the new zorb into the zorb vector
        b_zorbs.emplace_back(std::make_shared<Zorb>(recruit));
    }

    ENEMYTEAMNAME = enemyTeam.second;
    _gpM.updateZorbs();
}

//endregion