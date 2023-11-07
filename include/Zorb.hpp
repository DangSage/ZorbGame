#ifndef ZORB_H
#define ZORB_H

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
    Zorb(ZorbAppearance _appearance, int power = 0, int team_id = -1, const std::string& name = zorb::RandomName()) 
    : power(power), 
    team_id(team_id), 
    name(name), 
    appearance(_appearance) {
        count++; 
        if(_DEBUGMODE) { std::cout << "DEBUG: Zorb() - " << name << " created [" << count << ']' << std::endl; z_util::clearInputBuffer();} 
    }

    // Empty zorb constructor (used for dodging, and imploded zorbs)
    Zorb(int team_id) 
    : team_id(team_id), name("~dodged"), appearance(0), power(-1){
        count++; 
        if(_DEBUGMODE) { std::cout << "DEBUG: Zorb() - " << name << " created [" << count << ']' << std::endl; z_util::clearInputBuffer();} 
    }

    // Copy constructor
    Zorb(const Zorb& other)
        : power(other.power),
        team_id(other.team_id),
        name(other.name),
        appearance(other.appearance) { 
            count++;
            if(_DEBUGMODE) { std::cout << "DEBUG: Zorb() - " << name << " copied [" << count << ']' << std::endl; z_util::clearInputBuffer();} 
        }

    // Default destructor
    ~Zorb() { 
        count--; 
        if(_DEBUGMODE) { std::cout << "DEBUG: Zorb() - " << name << " destroyed [" << count << ']' << std::endl; z_util::clearInputBuffer();} 
    }

    // Accessor functions
    int GetPower() const { return power; }
    const int GetTeamId() const { return team_id; }
    static int GetCount() { return count; }
    std::string GetName() const { return name; }
    std::string GetAppearance() const { return appearance.GetAppearance(); }

    // Mutator Functions
    void SetPower(int _power) { this->power = _power; }
    void SetName(std::string _name) { this->name = _name; }
    void SetAppearance(appearanceEnum _enum, std::string_view _color = "") { appearance.SetAppearance(_enum, _color); }

    friend std::ostream& operator<<(std::ostream& os, const Zorb& zorb); // Overload the insertion operator to print a Zorb
    friend bool operator<(const Zorb& left, const Zorb& right); // Overload the < operator to compare Zorbs
    friend Zorb operator+(const Zorb& zorb1, const Zorb& zorb2); // Overload the + operator to combine two Zorbs
    
    //define operator== to check Zorb objects to shared pointers of Zorb objects
    friend bool operator==(const Zorb& lhs, const std::shared_ptr<Zorb>& rhs) {
        return lhs.power == rhs->power && lhs.team_id == rhs->team_id && lhs.name == rhs->name && lhs.appearance == rhs->appearance;
    }
    friend bool operator==(const Zorb& lhs, const Zorb& rhs) {
        return lhs.team_id == rhs.team_id && lhs.name == rhs.name && lhs.appearance == rhs.appearance;
    }
};

int Zorb::count = 0; // Initialize the static variable

// Overload the insertion operator
std::ostream& operator<<(std::ostream& os, const Zorb& zorb) {
    // Display the Zorb as ASCII art
    std::string appearanceText = zorb.GetAppearance();
    std::string infoText;
    
    infoText = z_util::FormattedText((std::to_string(zorb.GetPower()) + " power"), ansi::YELLOW);

    std::vector<std::string> appearanceLines = z_util::SplitMultilineString(appearanceText);

    // Calculate margin
    size_t longestLineLength = 0;
    for (const auto& line : appearanceLines) {
        longestLineLength = std::max(longestLineLength, line.length());
    }
    size_t margin = (CONSOLESIZE - longestLineLength) / 2;

    for (const auto& line : appearanceLines) {
        os << z_util::SpaceToPrint(margin+ZORBWIDTH) << line << std::endl;
    }

    os << z_util::CenterAlignStrings(zorb.GetName(), CONSOLESIZE+(ZORBWIDTH/2))
    << z_util::CenterAlignString(infoText, CONSOLESIZE-(ZORBWIDTH/2)) << std::endl;
    return os;
}

// Overload the < operator
bool operator<(const Zorb& left, const Zorb& right) {
    return (left.team_id != right.team_id) && (left.power < right.power);
}

// Overload the + operator
Zorb operator+(const Zorb& zorb1, const Zorb& zorb2) {
    // Combine the power of the two Zorbs, the weaker Zorb is destroyed and the stronger Zorb is returned with the combined power and same team id
    int combined_power = zorb1.power + zorb2.power;
    if(zorb1.power > zorb2.power) {
        return Zorb(zorb1.appearance, combined_power, zorb1.team_id, zorb1.name);
    }
    else if(zorb1.power < zorb2.power) {
        return Zorb(zorb2.appearance, combined_power, zorb2.team_id, zorb2.name);
    }
    else {
        // If the Zorbs have the same power, the team id is turned to negative and the Zorb implodes
        if(z_util::random::value(1,2) == 1) {
            ZorbAppearance new_appearance = ZorbAppearance(static_cast<appearanceEnum>(0), ansi::RED);
            if(_DEBUGMODE) {
                std::cout << "DEBUG: operator+() - Zorbs have the same power, both Zorbs will implode" << std::endl;
                z_util::clearInputBuffer();
            }
            return Zorb(new_appearance, -1, -1, "Imploded Zorb");
        } else {
            return Zorb(zorb1.appearance, combined_power, zorb1.team_id, zorb1.name);
        }        
    }
}

namespace z_debug {
    void PrintZorbAppearances(int a = appearanceMap.size(), bool printNames = false, std::string_view color = ansi::YELLOW) {
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
            std::vector<std::string> appearanceLines = z_util::SplitMultilineString(appearanceText);

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

            if ((charLines.back().length() + z_util::GetLengthWithoutEscapeCodes(appearanceLines.back()) + ZORBWIDTH) > CONSOLESIZE-margin) {
                rowBuffers.push_back(charLines);
                charLines.clear();
                _enumNUM--;
            } else {
                for (size_t i = 0; i < appearanceLines.size(); ++i) {
                    charLines[i] += z_util::SpaceToPrint(margin);
                    charLines[i] += appearanceLines[i];
                }
                if(printNames)
                    charLines[appearanceLines.size()] += z_util::SpaceToPrint(margin) + nameText;
            }
        }
        rowBuffers.push_back(charLines);
        for (const auto& rowBuffer : rowBuffers) {
            for (const std::string& charLine : rowBuffer) {
                std::cout << std::right << std::setw(margin) << charLine << std::endl;
            }
        }
    }
}

#endif // ZORB_H
