#ifndef ZORBAPPEARANCE_HPP
#define ZORBAPPEARANCE_HPP

#include <map>
#include <unordered_map>
#include "pch.hpp"

appearanceEnum GetRandomAppearance();

class ZorbAppearance {
private:
    std::string_view color;
    std::string currentAppearance;
    appearanceEnum currentEnum;
    static int count; // Declare a static variable to keep track of the number of ZorbAppearance objects
public:
    // Default constructor
    ZorbAppearance(appearanceEnum _enum = appearanceEnum::EMPTY, std::string_view _color = "");
    // Empty zorb constructor (used for dodging, and imploded zorbs)
    ZorbAppearance(int a);
    // Copy constructor
    ZorbAppearance(const ZorbAppearance& other);
    // Default destructor
    ~ZorbAppearance();

    std::string SetColor(std::string_view _color);
    
    std::string SetAppearance(appearanceEnum _enum, std::string_view COLOR = "");

    std::string GetAppearance() const;
    std::string_view GetColor() const;
    int GetCount() const;

    // overload == operator to compare two ZorbAppearance objects
    friend bool operator==(const ZorbAppearance& lhs, const ZorbAppearance& rhs);

};

namespace z_debug {
    void PrintZorbAppearances(int a = appearanceMap.size()-1, bool printNames = false, std::string_view color = ansi::YELLOW);
}

template <typename Container>
ZorbAppearance GenRandomAppearance(const Container& colors) {
    // You can now use 'colors' as any container of std::string_view
    // You can use the function 'RandomValue' from zUtility.hpp to generate a random value
    // You can use the function 'GetRandomColor' from gameDefs.hpp to get a random color

    //randomly pick an appearance from the map and return the enum
    int randomAppearance = z_util::random::value<int>(0, appearanceMap.size()-2);
    if(_DEBUGMODE)
        std::cout << "DEBUG: GetRandomAppearance() - rolled: " << randomAppearance << ": " << appearanceNames.at(static_cast<appearanceEnum>(randomAppearance)) << std::endl;

    //randomly pick a color from the container and return the string_view
    int randomColor = z_util::random::value<int>(0, colors.size()-1);
    if(_DEBUGMODE)
        std::cout << "DEBUG: GetRandomColor() - rolled: " << randomColor << ": " << colors.at(randomColor) << std::endl;

    return ZorbAppearance(static_cast<appearanceEnum>(randomAppearance), colors.at(randomColor));
}

#endif //ZORBAPPEARANCE_HPP