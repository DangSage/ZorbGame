#ifndef ZORBAPPEARANCE_HPP3
#define ZORBAPPEARANCE_HPP

#include <map>
#include <unordered_map>
#include "pch.hpp"
#include <fstream>

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
