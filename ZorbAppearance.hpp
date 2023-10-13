#ifndef ZORBAPPEARANCE_HPP3
#define ZORBAPPEARANCE_HPP

#include <map>
#include <unordered_map>
#include "gameDefs.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

//region appearance enums and maps
enum class appearanceEnum : int {}; //dynamic enum for appearance
std::map<appearanceEnum, std::string> appearanceMap; //map of appearance enums to appearance strings
std::map<appearanceEnum, std::string> appearanceNames; //map of appearance enums to names

//initialize appearance maps
void initAppearanceMaps() {
    std::ifstream file("appearances.json");
    json j;
    file >> j;
    int appNum = 0;
    // Loop through the JSON array and add each appearance to the appearance map
    for (auto& appearance : j) {
        std::string appearanceString;
        int lineCount = 0;
        /* Example JSON entry:
            "       ",
            "   o   ",
            "./\\|/\\.",
            "( o.o )",
            " > ^ < "
        ]
        */
        for (auto& line : appearance["appearance"]) {
            if (static_cast<std::string>(line).length() != ZORBWIDTH) { //exception handling for invalid appearance width
                std::cout << ansi::YELLOW << "EXCEPTION: appearance width != to ZORBWIDTH, initalization error." << std::endl
                << "\tLine: " << lineCount << " of enum " << appearance["enum"] << std::endl
                << "\tAppearance width: " << static_cast<std::string>(line).length() << ansi::RESET << std::endl;
                exit(0);
            }
            appearanceString += line;
            appearanceString += "\n";
            lineCount++;
        }
        appearanceMap[static_cast<appearanceEnum>(appNum)] = appearanceString;
        appearanceNames[static_cast<appearanceEnum>(appNum)] = appearance["name"];
        appNum++;
        if(_DEBUGMODE)
            std::cout << "appearance " << appNum << '-' << appearance["name"] << std::endl;
    }
    if(_DEBUGMODE) {
            std::cout << "DEBUG: initAppearanceMaps() - total appearances: " << appNum << std::endl;
            std::cout << "DEBUG: initAppearanceMaps() - appearanceMap size: " << appearanceMap.size() << std::endl;
            std::cout << "DEBUG: initAppearanceMaps() - appearanceNames size: " << appearanceNames.size() << std::endl;
    }
}

appearanceEnum GetRandomAppearance() {
    //randomly pick an appearance from the map and return the enum
    int randomAppearance = z_debug::RandomValue<int>(0, appearanceMap.size()-1);
    if(_DEBUGMODE)
        std::cout << "DEBUG: GetRandomAppearance() - rolled: " << randomAppearance << ": " << appearanceNames.at(static_cast<appearanceEnum>(randomAppearance)) << std::endl;
    return static_cast<appearanceEnum>(randomAppearance);
}
//endregion

class ZorbAppearance {
private:
    std::string color;
    std::string currentAppearance;
    appearanceEnum currentEnum;
public:
    ZorbAppearance(appearanceEnum _enum = GetRandomAppearance(), std::string _color = "") 
    : color(_color), currentEnum(_enum), currentAppearance(SetAppearance(_enum, _color)) {}
    
    ZorbAppearance(const ZorbAppearance* other) {
        // Copy the values from the other object
        this->color = other->color;
        this->currentAppearance = other->currentAppearance;
        this->currentAppearance = other->currentAppearance;
    }

    ~ZorbAppearance() {} //destructor

    std::string SetColor(std::string _color) { color = _color; currentAppearance = SetAppearance(currentEnum, color); return currentAppearance; }
    std::string SetAppearance(appearanceEnum _enum, std::string COLOR = "");   //setting appearance

    std::string GetAppearance() const;
    std::string GetColor() const { return color; }
};

//region ZorbAppearance Functions
std::string ZorbAppearance::SetAppearance(appearanceEnum _enum, std::string COLOR)
{
    // Check if the specified appearanceEnum exists in the map
    auto it = appearanceMap.find(_enum);
    if (it != appearanceMap.end()) {
        std::string before = it->second; // Use the mapped string
        std::string _appearance;

        // Split the multi-line string into individual lines
        std::istringstream iss(before);
        std::string line;

        while (std::getline(iss, line)) {
            _appearance += COLOR;
            _appearance += line;
            if(COLOR != "")
                _appearance += ansi::RESET;
            _appearance += "\n";
        }
        return _appearance;
    } else {
        std::cout << "EXCEPTION: appearance not set, invalid enum. (ZorbAppearance::SetAppearance)" << std::endl
            << "No appearance set for enum " << static_cast<int>(_enum) << std::endl;
        exit(0);
    }
}
std::string ZorbAppearance::GetAppearance() const
{
    if(currentAppearance == "")
    {   
        std::cout << "EXCEPTION: no appearance, initalization error." << std::endl
            << "Check that the appearance is set before calling GetAppearance()" << std::endl;
        exit(0);
    }
    return currentAppearance;
}
//endregion

#endif // ZORBAPPEARANCE_HPP
