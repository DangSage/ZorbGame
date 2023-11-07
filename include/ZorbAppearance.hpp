#ifndef ZORBAPPEARANCE_HPP
#define ZORBAPPEARANCE_HPP

#include <map>
#include <unordered_map>
#include "pch.hpp"
#include <fstream>

appearanceEnum GetRandomAppearance();

class ZorbAppearance {
private:
    std::string_view color;
    std::string currentAppearance;
    appearanceEnum currentEnum;
    static int count; // Declare a static variable to keep track of the number of ZorbAppearance objects
public:
    ZorbAppearance(appearanceEnum _enum = GetRandomAppearance(), std::string_view _color = "") 
    : color(_color), currentEnum(_enum), currentAppearance(SetAppearance(_enum, _color)) {
        count++; 
        if(_DEBUGMODE) { std::cout << "DEBUG: ZorbAppearance() - " << appearanceNames.at(static_cast<appearanceEnum>(_enum)) 
        << " created [" << count << ']' << std::endl; z_util::clearInputBuffer();} 
    }

    ZorbAppearance(int a)   // dodging constructor (used for dodging, no enum or color)
    : color(""), currentEnum(), currentAppearance(dodgedZorb) {
        if(_DEBUGMODE) { std::cout << "DEBUG: ZorbAppearance() - " << appearanceNames.at(static_cast<appearanceEnum>(currentEnum)) 
        << " created [" << count << ']' << std::endl; z_util::clearInputBuffer();}
    }
    
    // Copy constructor
    ZorbAppearance(const ZorbAppearance& other)
    : currentAppearance(other.currentAppearance), color(other.color), currentEnum(other.currentEnum) {}

    ~ZorbAppearance() { 
        count--; 
        if(_DEBUGMODE) { std::cout << "DEBUG: ZorbAppearance() - " << appearanceNames.at(static_cast<appearanceEnum>(currentEnum)) 
        << " destroyed [" << count << ']' << std::endl; z_util::clearInputBuffer();} 
    } //destructor

    std::string SetColor(std::string _color);
    std::string SetAppearance(appearanceEnum _enum, std::string_view COLOR = "");   //setting appearance

    std::string GetAppearance() const;
    std::string_view GetColor() const;
    int GetCount() const;

    // overload == operator to compare two ZorbAppearance objects
    friend bool operator==(const ZorbAppearance& lhs, const ZorbAppearance& rhs);
};

namespace z_debug {
    void PrintZorbAppearances(int a = appearanceMap.size(), bool printNames = false, std::string_view color = ansi::YELLOW);
}

template <typename Container>
ZorbAppearance GenRandomAppearance(const Container& colors) {
    // You can now use 'colors' as any container of std::string_view
    // You can use the function 'RandomValue' from zUtility.hpp to generate a random value
    // You can use the function 'GetRandomColor' from gameDefs.hpp to get a random color

    //randomly pick an appearance from the map and return the enum
    int randomAppearance = z_util::random::value<int>(0, appearanceMap.size()-1);
    if(_DEBUGMODE)
        std::cout << "DEBUG: GetRandomAppearance() - rolled: " << randomAppearance << ": " << appearanceNames.at(static_cast<appearanceEnum>(randomAppearance)) << std::endl;

    //randomly pick a color from the container and return the string_view
    int randomColor = z_util::random::value<int>(0, colors.size()-1);
    if(_DEBUGMODE)
        std::cout << "DEBUG: GetRandomColor() - rolled: " << randomColor << ": " << colors.at(randomColor) << std::endl;

    return ZorbAppearance(static_cast<appearanceEnum>(randomAppearance), colors.at(randomColor));
}

#endif //ZORBAPPEARANCE_HPP