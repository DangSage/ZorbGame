#include <vector>
#include <memory>
#include <string>

// forward declaration
class Zorb;
std::string instantiateUUID();

// This is your game data that you want to save
struct SaveData {
    std::string UUID = instantiateUUID(); // unique identifier for this save data
    std::string journeyName; // name of the save file and player team
    std::vector<std::string> zorbNameRecord; //vector of zorb names
    std::vector<std::shared_ptr<Zorb>> m_zorbs; //vector of zorbs
    int turnCounter = 1; //turn counter for the battle segment
    int winCounter = 0; //win counter for the game
    int casualtyCounter = 0; //casualties counter for the game
    int battleCounter = 0; //battle counter for the game
};

// Create the UID for this save data
std::string instantiateUUID();
