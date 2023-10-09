#ifndef ZORBAPPEARANCE_HPP3
#define ZORBAPPEARANCE_HPP

#include <map>
#include <unordered_map>
#include "gameDefs.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

//region appearance enums and maps
enum class appearanceEnum : int {};

//create a dynamic enum for each appearance from the JSON file (appearance.json), and map it to a string of the appearance that can be called later to an integer
std::map<appearanceEnum, std::string> appearanceMap;
std::map<appearanceEnum, std::string> appearanceNames;

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
                std::cout << ANSI_YELLOW << "EXCEPTION: appearance width != to ZORBWIDTH, initalization error." << std::endl
                << "\tLine: " << lineCount << " of enum " << appearance["enum"] << std::endl
                << "\tAppearance width: " << static_cast<std::string>(line).length() << ANSI_RESET << std::endl;
                exit(0);
            }
            appearanceString += line;
            appearanceString += "\n";
            lineCount++;
        }
        appearanceMap[static_cast<appearanceEnum>(appNum)] = appearanceString;
        appearanceNames[static_cast<appearanceEnum>(appNum)] = appearance["name"];
        appNum++;
    }
}

class ZorbAppearance {
public:
    ZorbAppearance() { SetAppearance(static_cast<appearanceEnum>(0)); z_debug::zorbAppearanceCount++; } //default constructor
    ZorbAppearance(appearanceEnum _enum, std::string _color = "") { 
        ZorbAppearance();
        SetAppearance(_enum, _color); 
    }
    ZorbAppearance(const ZorbAppearance* other) {
        // Copy the values from the other object
        this->color = other->color;
        this->currentAppearance = other->currentAppearance;
    }

    ~ZorbAppearance() { z_debug::zorbAppearanceCount--; }

    void SetAppearance(appearanceEnum zorbAppearance, std::string COLOR = "");   //setting appearance

    std::string GetAppearance() const;
    std::string GetColor() const { return color; }
private:
    std::string color;
    std::string currentAppearance;
};

//region ZorbAppearance Functions
void ZorbAppearance::SetAppearance(appearanceEnum _enum, std::string COLOR)
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
                _appearance += ANSI_RESET;
            _appearance += "\n";
        }
        currentAppearance = _appearance;
        color = COLOR;
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
