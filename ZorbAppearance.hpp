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
    APPEARANCE_NINJA,   // ninja zorb enum
    APPEARANCE_SILLY,   // silly zorb enum
    APPEARANCE_SKEL,    // skeleton zorb enum
    APPEARANCE_ZOMBIE,  // zombie zorb enum
    APPEARANCE_VAMP,     // zamp zorb enum
    //generate some more appearances enums here using the naming scheme APPEARANCE_<NAME>


    // Placeholder for the total number of appearances
    NUM_APPEARANCES
};

std::map<appearanceEnum, const std::string> appearanceNames = {
    //All of these names must be 7 characters long and unique to the enum
    {APPEARANCE_DEFAULT, "default"}, // normal cat enum
    {APPEARANCE_HAPPY, "happier"},   // happy cat enum
    {APPEARANCE_WINK, "winking"},    // winking cat enum
    {APPEARANCE_SLEEPY, "sleeper"},  // sleepy cat enum
    {APPEARANCE_SNOTTY, "snotter"},  // snotty cat enum
    {APPEARANCE_ANGRY, "angrily"},   // angry cat enum
    {APPEARANCE_MONEY, "greeder"},   // merchant cat enum
    {APPEARANCE_FOLD, "foldear"},    // scottish-fold cat enum
    {APPEARANCE_MONK, "nirvana"},    // bald zorb enum
    {APPEARANCE_DEVIL, "hellish"},   // devil zorb enum
    {APPEARANCE_KNIGHT, "knighty"},  // knight zorb enum
    {APPEARANCE_WIZARD, "magical"},   // wizard zorb enum
    {APPEARANCE_NINJA, "sneaker"},   // ninja zorb enum
    {APPEARANCE_SILLY, "clownly"},   // silly zorb enum
    {APPEARANCE_SKEL, "skeletn"},    // skeleton zorb enum
    {APPEARANCE_ZOMBIE, "zombier"},  // zombie zorb enum
    {APPEARANCE_VAMP, "vampier"}     // zamp zorb enum
};

class ZorbAppearance {
public:
    ZorbAppearance();
    ZorbAppearance(appearanceEnum _enum, std::string _color = "") { 
        ZorbAppearance();
        SetAppearance(_enum, _color); 
    }
    ZorbAppearance(const ZorbAppearance* other) {
        // Copy the values from the other object
        this->appearanceMap = other->appearanceMap;
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
 > W <|)"},
    {APPEARANCE_NINJA,
R"(       
   o   
./\|/\.
([o.o])
 >   < )"},
    {APPEARANCE_SILLY,
R"(       
.( - ).
(/\|/\)
(.oCo.)
 > = < )"},
    {APPEARANCE_SKEL,
R"(       
   .   
./\|/\.
< 0+0 >
 >)w(< )"},
    {APPEARANCE_ZOMBIE,
R"(       
   o   
./\|/\.
(#o.0 )
 >_C#< )"},
    {APPEARANCE_VAMP,
R"(       
   o   
./\|/\.
( o^o )
^>v v<^)"}
    // Add more appearance strings here
}; // Unordered map to store appearance strings
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
        std::cout << "EXCEPTION: appearance not set, invalid enum." << std::endl
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


#endif // ZORBAPPEARANCE_HPP
