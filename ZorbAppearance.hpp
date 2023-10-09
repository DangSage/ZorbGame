#ifndef ZORBAPPEARANCE_HPP3
#define ZORBAPPEARANCE_HPP

#include <map>
#include <unordered_map>
#include "gameDefs.hpp"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

//region appearance enums and map
enum appearanceEnum {
    APPEARANCE_DEFAULT, // normal cat enum
    APPEARANCE_ANGRY,   // angry cat enum
    APPEARANCE_HAPPY,   // happy cat enum
    APPEARANCE_WINK,    // winking cat enum
    APPEARANCE_SLEEPY,  // sleepy cat enum
    APPEARANCE_SNOTTY,  // snotty cat enum
    APPEARANCE_MONEY,   // merchant cat enum
    APPEARANCE_FOLD,    // scottish-fold cat enum
    APPEARANCE_MONK,    // bald zorb enum
    APPEARANCE_DEVIL,   // devil zorb enum
    APPEARANCE_KNIGHT,  // knight zorb enum
    APPEARANCE_WIZARD,  // wizard zorb enum
    APPEARANCE_NINJA,   // ninja zorb enum
    APPEARANCE_SILLY,   // silly zorb enum
    APPEARANCE_SKEL,    // skeleton zorb enum
    APPEARANCE_ZOMBIE,  // zombie zorb enum
    APPEARANCE_VAMP,     // zamp zorb enum

    // Placeholder for the total number of appearances
    NUM_APPEARANCES
};
std::map<appearanceEnum, const std::string> appearanceNames = {
    //All of these names must be 7 characters long and unique to the enum
    {APPEARANCE_DEFAULT, "default"}, // normal cat enum
    {APPEARANCE_ANGRY, "angrily"}   // angry cat enum
    // {APPEARANCE_HAPPY, "happier"},   // happy cat enum
    // {APPEARANCE_WINK, "winking"},    // winking cat enum
    // {APPEARANCE_SLEEPY, "sleeper"},  // sleepy cat enum
    // {APPEARANCE_SNOTTY, "snotter"},  // snotty cat enum
    // {APPEARANCE_MONEY, "greeder"},   // merchant cat enum
    // {APPEARANCE_FOLD, "foldear"},    // scottish-fold cat enum
    // {APPEARANCE_MONK, "nirvana"},    // bald zorb enum
    // {APPEARANCE_DEVIL, "hellish"},   // devil zorb enum
    // {APPEARANCE_KNIGHT, "knighty"},  // knight zorb enum
    // {APPEARANCE_WIZARD, "magical"},   // wizard zorb enum
    // {APPEARANCE_NINJA, "sneaker"},   // ninja zorb enum
    // {APPEARANCE_SILLY, "clownly"},   // silly zorb enum
    // {APPEARANCE_SKEL, "skeletn"},    // skeleton zorb enum
    // {APPEARANCE_ZOMBIE, "zombier"},  // zombie zorb enum
    // {APPEARANCE_VAMP, "vampire"}     // zamp zorb enum
};
std::unordered_map<appearanceEnum, std::string> appearanceMap = {
    {APPEARANCE_DEFAULT, 
R"(       
   o   
./\|/\.
( o.o )
 > ^ < )"},
    {APPEARANCE_ANGRY, 
R"(       
   o   
./\|/\.
{(\./)}
 > ^ < )"}
    };
//endregion

// Reads appearances.json file and inserts every entry into appearanceEnum, and appearance & names maps
void initAppearanceMaps() {
    std::ifstream i("appearances.json");
    json j;
    i >> j;

    // in the json file, the first entry is the 3rd appearance, so we start at 2 and go to the end and apply those names and appearances of each object in the JSON to the corresponding maps
    for (int i = 2; i < j.size(); i++) {
        appearanceNames.insert({ static_cast<appearanceEnum>(i), j[i]["name"] });
        appearanceMap.insert({ static_cast<appearanceEnum>(i), j[i]["appearance"] });
    }
}

class ZorbAppearance {
public:
    ZorbAppearance() { SetAppearance(APPEARANCE_DEFAULT); z_debug::zorbAppearanceCount++; } //default constructor
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
            if(line.size() > ZORBWIDTH || line.size() < ZORBWIDTH)
            {
                std::cout << "EXCEPTION: appearance not set, line width error (ZorbAppearance::SetAppearance)" << std::endl;
                std::cout << "Character associated w/ enum " << static_cast<int>(_enum) << ' ' << std::to_string(_enum) << std::endl;
                //remember that the enum 0-1 are indexed, so check the JSON file for the correct enum
                exit(0);
            }
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
            << "No appearance set for enum " << static_cast<int>(_enum) << ' ' << std::to_string(_enum) << std::endl;
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
