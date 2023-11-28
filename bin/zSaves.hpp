#include <fstream>
#include <vector>
#include <memory>

// forward declaration
class Zorb;

// This is your game data that you want to save
struct SaveData {
    std::string journeyName; // name of the save file and player team
    std::vector<std::string> zorbNameRecord; //vector of zorb names
    std::vector<std::shared_ptr<Zorb>> m_zorbs; //vector of zorbs
    int turnCounter = 1; //turn counter for the battle segment
    int winCounter = 0; //win counter for the game
    int casualtyCounter = 0; //casualties counter for the game
    int battleCounter = 0; //battle counter for the game
};

// save game data to file, filename is the name of the save file
void saveGame(const SaveData& data, const std::string& filename);

// load game data from file, all data will be loaded into the global variables in zDefs.hpp
bool loadGame(SaveData& data, const std::string& filename);