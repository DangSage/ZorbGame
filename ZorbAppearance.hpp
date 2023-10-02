#ifndef ZORBAPPEARANCE_HPP3
#define ZORBAPPEARANCE_HPP

#include <sstream>
#include "zUtility.hpp"

enum appearanceEnum {
    APPEARANCE_DEFAULT,
    APPEARANCE_HAPPY, // normal player cat
    APPEARANCE_WINK, // winking player cat
    APPEARANCE_SLEEPY, // sleepy player cat
    APPEARANCE_SNOTTY, // snotty player cat
    APPEARANCE_ANGRY, // enemy cat
    APPEARANCE_MONEY, // special merchant zorb
    APPEARANCE_FOLD,
    APPEARANCE_MONK,
    APPEARANCE_DEVIL,

    // Placeholder for the total number of appearances
    NUM_APPEARANCES
};

class ZorbAppearance {
public:
    ZorbAppearance();
    ZorbAppearance(appearanceEnum _enum, std::string _color = "") { SetAppearance(_enum, _color); }

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
    {APPEARANCE_DEFAULT, R"(
   o   
./\|/\.
( o.o )
 > ^ < )"},
    {APPEARANCE_HAPPY, R"(
   o   
./\|/\.
( ^.^ )
 > ^ < )"},
    {APPEARANCE_WINK, R"(
   o   
./\|/\.
( o.^ )
 > u < )"},
    {APPEARANCE_SLEEPY, R"(
   o   
./\|/\.
( =,= )
Zzz^ < )"},
    {APPEARANCE_SNOTTY, R"(
   o   
./\|/\.
( o,o )
 > ^ < )"},
    {APPEARANCE_ANGRY, R"(
   o   
./\|/\.
( \./ )
 > ^ < )"},
    {APPEARANCE_MONEY, R"(
   o   
./\|/\.
( $.$ )
 > w < )"},
    {APPEARANCE_FOLD, R"(
   o   
.n_|_n.
( o.o )
 > ^ < )"},
    {APPEARANCE_MONK, R"(
        
 _---_ 
( v.v )
H> ^ <n)"},
    {APPEARANCE_DEVIL, R"(
 ._*_.
 /|-|\
( p.q )
\> v </)"}
    // Add more appearance strings here
}; // Unordered map to store appearance strings
    SetAppearance(APPEARANCE_DEFAULT);
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
            _appearance += COLOR;
            _appearance += line;
            _appearance += ANSI_RESET;
            _appearance += "\n";
        }
        currentAppearance = _appearance;
        color = COLOR;
    } else {
        std::cout << "ERROR: could not set appearance, no character assigned" << std::endl;
    }
}

#endif // ZORBAPPEARANCE_HPP
