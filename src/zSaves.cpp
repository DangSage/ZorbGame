#include "saveManager.hpp"
#include "gameManager.hpp"
#include "gameplayManager.hpp"
#include "Zorb.hpp"
#include <fstream>
#include <filesystem>

static std::string SAVEFOLDER = ".saves";

void GameManager::startGame() {
    GameplayManager* m_gameplayManager = new GameplayManager(m_ui);
    m_gameplayManager->gameplayLoop();
    delete m_gameplayManager;
}

// If the save folder doesn't exist, create it.
void ensureSaveFolder() {
    if (!std::filesystem::exists(SAVEFOLDER)) {
        std::filesystem::create_directory(SAVEFOLDER);
    }
}

std::string instantiateUUID() {
    static const char* chars = "0123456789abcdef";
    std::string uuid;

    for (int i = 0; i < 36; ++i) {
        if (i == 8 || i == 13 || i == 18 || i == 23) {
            uuid += "-";
        } else {
            uuid += chars[z_util::random::value(0, 15)];
        }
    }

    return uuid;
}

void GameplayManager::saveSave() {
    SaveData data;
    // copy all data from the GameplayManager to the SaveData struct
    z_debug::_tag("Save UUID: " + data.UUID);
    data.journeyName = journeyName;
    data.zorbNameRecord = zorbNameRecord;
    data.m_zorbs = m_zorbs;
    data.turnCounter = turnCounter;
    data.winCounter = winCounter;
    data.casualtyCounter = casualtyCounter;
    data.battleCounter = battleCounter;

    ensureSaveFolder();

    std::string fileName = data.UUID + ".zzsave";
    std::ofstream out(fileName, std::ios::binary);

    // strings, write their length and then their characters
    size_t nameSize = data.journeyName.size();
    out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    out.write(data.journeyName.c_str(), data.journeyName.size());

    // integers
    out.write(reinterpret_cast<const char*>(&data.turnCounter), sizeof(data.turnCounter));
    out.write(reinterpret_cast<const char*>(&data.winCounter), sizeof(data.winCounter));
    out.write(reinterpret_cast<const char*>(&data.casualtyCounter), sizeof(data.casualtyCounter));
    out.write(reinterpret_cast<const char*>(&data.battleCounter), sizeof(data.battleCounter));

    
    // Write the m_zorbs vector to the file.
    size_t size = data.m_zorbs.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for(const auto& zorb : data.m_zorbs) {
        zorb->serialize(out);
    }

    // Write the zorbNameRecord vector to the file.
    size = data.zorbNameRecord.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for(const auto& name : data.zorbNameRecord) {
        size_t nameSize = name.size();
        out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        out.write(name.c_str(), name.size());
    }

    out.close();
    
    
}