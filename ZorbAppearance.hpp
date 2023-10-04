#ifndef ZORBAPPEARANCE_HPP3
#define ZORBAPPEARANCE_HPP

#include <map>
#include <unordered_map>
#include "gameDefs.hpp"

enum appearanceEnum {
    APPEARANCE_DEFAULT, // normal cat enum
    APPEARANCE_HAPPY,   // happy cat enum
    APPEARANCE_WINK,    // winking cat enum
    APPEARANCE_SLEEPY,  // sleepy cat enum
    APPEARANCE_SNOTTY,  // snotty cat enum
    APPEARANCE_ANGRY,   // angry cat enum
    APPEARANCE_MONEY,   // merchant cat enum
    APPEARANCE_FOLD,    // scottish-fold cat enum
    APPEARANCE_MONK,    // bald zorb enum
    APPEARANCE_DEVIL,   // devil zorb enum
    APPEARANCE_KNIGHT,  // knight zorb enum
    APPEARANCE_WIZARD,  // wizard zorb enum

    // Placeholder for the total number of appearances
    NUM_APPEARANCES
};

std::map<appearanceEnum, const std::string> appearanceNames = {
    {APPEARANCE_DEFAULT, "default"}, // normal cat enum
    {APPEARANCE_HAPPY, "happier"},   // happy cat enum
    {APPEARANCE_WINK, "winking"},    // winking cat enum
    {APPEARANCE_SLEEPY, "sleeper"},  // sleepy cat enum
    {APPEARANCE_SNOTTY, "snotter"},  // snotty cat enum
    {APPEARANCE_ANGRY, "angrily"},   // angry cat enum
    {APPEARANCE_MONEY, "greeder"},   // merchant cat enum
    {APPEARANCE_FOLD, "foldear"},    // scottish-fold cat enum
    {APPEARANCE_MONK, "enlyten"},    // bald zorb enum
    {APPEARANCE_DEVIL, "hellish"},   // devil zorb enum
    {APPEARANCE_KNIGHT, "knighty"},  // knight zorb enum
    {APPEARANCE_WIZARD, "magical"}   // wizard zorb enum

};

class ZorbAppearance {
public:
    ZorbAppearance();
    ZorbAppearance(appearanceEnum _enum, std::string _color = "") { 
        SetAppearance(_enum, _color); 
        z_debug::zorbAppearanceCount++;
    }
    ZorbAppearance(const ZorbAppearance* other) {
        // Copy the values from the other object
        this->color = other->color;
        this->currentAppearance = other->currentAppearance;
        z_debug::zorbAppearanceCount++;
    }

    ~ZorbAppearance() { z_debug::zorbAppearanceCount--; }

    void SetAppearance(appearanceEnum zorbAppearance, std::string COLOR = "");   //setting appearance

    std::string GetAppearance() const { return currentAppearance; }
    std::string GetColor() const { return color; }
private:
    std::string color;
    std::string currentAppearance;
    std::unordered_map<appearanceEnum, std::string> appearanceMap;
};

ZorbAppearance::ZorbAppearance() {
    appearanceMap = {
    {APPEARANCE_DEFAULT, 
R"(       
   o   
./\|/\.
( o.o )
 > ^ < )"},
    {APPEARANCE_HAPPY, 
R"(       
   o   
./\|/\.
( ^.^ )
 > ^ < )"},
    {APPEARANCE_WINK, 
R"(       
   o   
./\|/\.
( o.^ )
 > u < )"},
    {APPEARANCE_SLEEPY, 
R"(       
   o   
./\|/\.
( =,= )
Zzz^ < )"},
    {APPEARANCE_SNOTTY, 
R"(       
   o   
./\|/\.
( o,o )
 > ^ < )"},
    {APPEARANCE_ANGRY, 
R"(       
   o   
./\|/\.
{(\./)}
 > ^ < )"},
    {APPEARANCE_MONEY, 
R"(       
   o   
./\|/\.
( $.$ )
 > u < )"},
    {APPEARANCE_FOLD, 
R"(       
   o   
.n_|_n.
( o.o )
 > ^ < )"},
    {APPEARANCE_MONK, 
R"(       
       
 _---_ 
( v.v )
H> ^ <n)"},
    {APPEARANCE_DEVIL, 
R"(       
  _*_  
./|-|\.
( @.@ )
=> v <=)"},
    {APPEARANCE_KNIGHT, 
R"(       
   V   
./\|/\.
|\\|//)
T> u < )"},
    {APPEARANCE_WIZARD, 
R"(       
  ,/^\_
_/___{_
( o.o T
 > W <|)"}
    // Add more appearance strings here
}; // Unordered map to store appearance strings
    SetAppearance(APPEARANCE_DEFAULT);
    z_debug::zorbAppearanceCount++; 
}

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
                std::cout << "EXCEPTION: appearance not set, line width error" << std::endl;
                std::cout << "Character @ " << static_cast<int>(_enum) << std::endl;
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
        std::cout << "EXCEPTION: appearance not set, null enum error" << std::endl;
        exit(0);
    }
}


#endif // ZORBAPPEARANCE_HPP
