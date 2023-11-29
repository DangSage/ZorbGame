#include "saveManager.hpp"
#include "gameManager.hpp"
#include "gameplayManager.hpp"
#include "Zorb.hpp"
#include <fstream>
#include <filesystem>

static std::string SAVEFOLDER = ".saves";

void GameManager::startGame() {
    GameplayManager* m_gameplayManager = new GameplayManager(m_ui);
    m_gameplayManager->writeSaveData();
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

void GameplayManager::writeSaveData() {
    SaveData data;
    // copy all data from the GameplayManager to the SaveData struct
    z_debug::_tag("Save UUID: " + data.UUID);
    data.journeyName = journeyName;
    data.zorbNameRecord = zorbNameRecord;
    data.m_zorbs = m_zorbs;
    data.winCounter = winCounter;
    data.battleCounter = battleCounter;

    ensureSaveFolder();

    // path to the save file is the save folder + the UUID + the extension
    std::string filePath = SAVEFOLDER + "/" + data.UUID + ".zzsave";
    std::ofstream out(filePath, std::ios::binary);

    size_t nameSize = data.journeyName.size();
    out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
    out.write(data.journeyName.c_str(), nameSize);

    out.write(reinterpret_cast<const char*>(&data.winCounter), sizeof(data.winCounter));
    out.write(reinterpret_cast<const char*>(&data.battleCounter), sizeof(data.battleCounter));

    size_t size = data.m_zorbs.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& zorb : data.m_zorbs) {
        zorb->serialize(out);
    }

    size = data.zorbNameRecord.size();
    out.write(reinterpret_cast<const char*>(&size), sizeof(size));
    for (const auto& name : data.zorbNameRecord) {
        nameSize = name.size();
        out.write(reinterpret_cast<const char*>(&nameSize), sizeof(nameSize));
        out.write(name.c_str(), nameSize);
    }

    out.close();
}

std::vector<SaveData> readSaveFiles() {
    ensureSaveFolder();

    std::vector<SaveData> saveData;

    for (const auto& entry : std::filesystem::directory_iterator(SAVEFOLDER)) {
        std::ifstream in(entry.path(), std::ios::binary);

        SaveData data;

        size_t nameSize;
        in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
        data.journeyName.resize(nameSize);
        in.read(&data.journeyName[0], nameSize);

        in.read(reinterpret_cast<char*>(&data.winCounter), sizeof(data.winCounter));
        in.read(reinterpret_cast<char*>(&data.battleCounter), sizeof(data.battleCounter));

        size_t size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        data.m_zorbs.resize(size);
        for (auto& zorb : data.m_zorbs) {
            zorb->deserialize(in);
        }

        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        data.zorbNameRecord.resize(size);
        for (auto& name : data.zorbNameRecord) {
            in.read(reinterpret_cast<char*>(&nameSize), sizeof(nameSize));
            name.resize(nameSize);
            in.read(&name[0], nameSize);
        }

        saveData.emplace_back(data);
    }

    return saveData;
}