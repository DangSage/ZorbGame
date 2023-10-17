#ifndef ZORB_H
#define ZORB_H

#include "pch.hpp"
#include "ZorbAppearance.hpp" // Include ZorbAppearance.hpp for Zorb class usage
#include <memory>

class Zorb {
private:
    int power;
    int team_id;
    std::string name;
    ZorbAppearance appearance;
    static int count; // Static variable to keep track of the number of Zorbs in memory
public:
    Zorb(int power = 0, int team_id = -1, const std::string& name = zorb::RandomName(), ZorbAppearance _appearance = ZorbAppearance()) 
    : power(power), 
    team_id(team_id), 
    name(name), 
    appearance(_appearance) {
        count++; 
        if(_DEBUGMODE) { std::cout << "DEBUG: Zorb() - " << name << " created [" << count << ']' << std::endl; z_debug::clearInputBuffer();} 
    }

    // Copy constructor
    Zorb(const Zorb& other)
        : power(other.power),
        team_id(other.team_id),
        name(other.name),
        appearance(other.appearance) { 
            count++;
            if(_DEBUGMODE) { std::cout << "DEBUG: Zorb() - " << name << " copied [" << count << ']' << std::endl; z_debug::clearInputBuffer();} 
        }

    // Move constructor
    Zorb(Zorb&& other) noexcept
        : power(std::exchange(other.power, 0)),
        team_id(std::exchange(other.team_id, -1)),
        name(std::move(other.name)),
        appearance(std::move(other.appearance)) {}

    // Default destructor
    ~Zorb() { 
        count--; 
        if(_DEBUGMODE) { std::cout << "DEBUG: Zorb() - " << name << " destroyed [" << count << ']' << std::endl; z_debug::clearInputBuffer();} 
    }

    // Accessor functions
    int GetPower() const { return power; }
    int GetTeamId() const { return team_id; }
    static int GetCount() { return count; }
    std::string GetName() const { return name; }
    std::string GetAppearance() const { return appearance.GetAppearance(); }

    // Mutator Functions
    void SetPower(int _power) { this->power = _power; }
    void SetName(std::string _name) { this->name = _name; }
    void SetAppearance(appearanceEnum _enum, std::string _color = "") { appearance.SetAppearance(_enum, _color); }

    friend std::ostream& operator<<(std::ostream& os, const Zorb& zorb); // Overload the insertion operator to print a Zorb
    friend bool operator<(const Zorb& left, const Zorb& right); // Overload the < operator to compare Zorbs
    friend Zorb operator+(const Zorb& zorb1, const Zorb& zorb2); // Overload the + operator to combine two Zorbs
};

int Zorb::count = 0; // Initialize the static variable

// Overload the insertion operator
std::ostream& operator<<(std::ostream& os, const Zorb& zorb) {
    // Display the Zorb as ASCII art
    std::string appearanceText = zorb.GetAppearance();
    std::string infoText;
    
    if(_DEBUGMODE)
        infoText = z_debug::FormattedText(std::to_string(zorb.GetTeamId()) + ", " + std::to_string(zorb.GetPower()) + 'p', ansi::YELLOW);
    else
        infoText = z_debug::FormattedText((std::to_string(zorb.GetPower()) + " power"), ansi::YELLOW);

    std::vector<std::string> appearanceLines = z_debug::SplitMultilineString(appearanceText);

    // Calculate margin
    size_t longestLineLength = 0;
    for (const auto& line : appearanceLines) {
        longestLineLength = std::max(longestLineLength, line.length());
    }
    size_t margin = (CONSOLESIZE - longestLineLength) / 2;

    for (const auto& line : appearanceLines) {
        os << z_debug::SpaceToPrint(margin+ZORBWIDTH) << line << std::endl;
    }

    os << z_debug::CenterAlignStrings(zorb.GetName(), CONSOLESIZE+(ZORBWIDTH/2))
    << z_debug::CenterAlignString(infoText, CONSOLESIZE-(ZORBWIDTH/2)) << std::endl;
    return os;
}

// Overload the < operator
bool operator<(const Zorb& left, const Zorb& right) {
    return (left.team_id != right.team_id) && (left.power < right.power);
}

// Overload the + operator
Zorb operator+(const Zorb& zorb1, const Zorb& zorb2) {
    int combined_power = zorb1.power + zorb2.power;
    int new_team_id = (zorb1.power >= zorb2.power) ? zorb1.team_id : zorb2.team_id;
    std::string new_name = (zorb1.power >= zorb2.power) ? zorb1.name : zorb2.name;
    ZorbAppearance new_appearance = (zorb1.power >= zorb2.power) ? zorb1.appearance : zorb2.appearance;
    
    Zorb new_zorb(combined_power, new_team_id, new_name, new_appearance);
    return new_zorb;
}

namespace z_debug {
    void PrintZorbAppearances(int a = appearanceMap.size(), bool printNames = false, std::string color = ansi::YELLOW) {
        if(appearanceMap.size() < 1) {
            std::cout << "No ZorbAppearances in memory" << std::endl;
            return;
        }
        else if(a > appearanceMap.size() || a < 0) {
            std::cout << "Invalid number of appearances to print" << std::endl
                << "current number of appearances: " << appearanceMap.size() << std::endl;
            return;
        }
        std::vector<std::string> charLines;
        std::vector<std::vector<std::string>> rowBuffers;
        
        // Calculate margin
        size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

        for(int _enumNUM=0; _enumNUM<a; _enumNUM++) {
            //define debug appearance object that is the current appearance
            ZorbAppearance debugAppearance(static_cast<appearanceEnum>(_enumNUM), color);
            std::string appearanceText = debugAppearance.GetAppearance();
            std::vector<std::string> appearanceLines = SplitMultilineString(appearanceText);

            std::string nameText = appearanceNames.at(static_cast<appearanceEnum>(_enumNUM));

            if(_DEBUGMODE)
                std::cout << "DEBUG: PrintZorbAppearances() - appearance (" << _enumNUM+1 << "): " << nameText << std::endl;

            if(printNames) {
                if (charLines.size() < appearanceLines.size()) {
                    charLines.resize(appearanceLines.size()+1);
                }
            } else {
                if (charLines.size() < appearanceLines.size()) {
                    charLines.resize(appearanceLines.size());
                }
            }

            if(nameText.size() < ZORBWIDTH){
                nameText.resize(ZORBWIDTH, ' ');
            }

            if ((charLines.back().length() + GetLengthWithoutEscapeCodes(appearanceLines.back()) + ZORBWIDTH) > CONSOLESIZE-margin) {
                rowBuffers.push_back(charLines);
                charLines.clear();
            } else {
                for (size_t i = 0; i < appearanceLines.size(); ++i) {
                    charLines[i] += z_debug::SpaceToPrint(margin);
                    charLines[i] += appearanceLines[i];
                }
                if(printNames)
                    charLines[appearanceLines.size()] += z_debug::SpaceToPrint(margin) + nameText;
            }
        }
        rowBuffers.push_back(charLines);
        for (const auto& rowBuffer : rowBuffers) {
            for (const std::string& charLine : rowBuffer) {
                std::cout << charLine << std::endl;
            }
        }
    }
}

#endif // ZORB_H
