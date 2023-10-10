#ifndef ZORB_H
#define ZORB_H

#include "zUtility.hpp" // Include zUtility.hpp for z_debug namespace
#include "ZorbAppearance.hpp" // Include ZorbAppearance.hpp for Zorb class usage

class Zorb {
public:
    // Default constructor (Empty Space)
    Zorb() : power(0), team_id(-1), name(""), appearance(ZorbAppearance()) { z_debug::zorbCount++; }
    // Team ID constructor (Creates Default Zorb)
    Zorb(int team_id) : power(0), team_id(team_id), name(""), appearance(ZorbAppearance()) { z_debug::zorbCount++; }
    // Appearance constructor
    Zorb(appearanceEnum _enum, std::string _color) : power(0), team_id(0), name("Gleep"), appearance(ZorbAppearance(_enum, _color)) {
        z_debug::zorbCount++;
    }
    // Power, ID, name constructor
    Zorb(int power, int team_id, const std::string& name) : 
        power(power), team_id(team_id), name(name), appearance(ZorbAppearance()) { z_debug::zorbCount++; }
    // Fully parameterized constructor
    Zorb(int power, int team_id, const std::string& name, ZorbAppearance _appearance) : 
        power(power), team_id(team_id), name(name), appearance(std::move(_appearance)) { z_debug::zorbCount++; }
    // Copy constructor
    Zorb(const Zorb& other)
        : power(other.power),
        team_id(other.team_id),
        name(other.name),
        appearance(other.appearance) { z_debug::zorbCount++; }

    // Move constructor
    Zorb(Zorb&& other) noexcept
        : power(std::exchange(other.power, 0)),
        team_id(std::exchange(other.team_id, -1)),
        name(std::move(other.name)),
        appearance(std::move(other.appearance)) {}

    // Default destructor
    ~Zorb() {
        z_debug::zorbCount--;
    }

    // Accessor functions
    int GetPower() const { return power; }
    int GetTeamId() const { return team_id; }
    std::string GetName() const { return name; }
    std::string GetAppearance() const { return appearance.GetAppearance(); }
    ZorbAppearance ReturnAppearanceObject() const { return appearance; }

    // Mutator Functions
    void SetPower(int _power) { this->power = _power; }
    void SetName(std::string _name) { this->name = _name; }
    void SetAppearance(appearanceEnum _enum, std::string _color = "") { appearance.SetAppearance(_enum, _color); }

    friend std::ostream& operator<<(std::ostream& os, const Zorb& zorb); // Overload the insertion operator to print a Zorb
    friend bool operator<(const Zorb& left, const Zorb& right); // Overload the < operator to compare Zorbs
    friend Zorb operator+(const Zorb& zorb1, const Zorb& zorb2); // Overload the + operator to combine two Zorbs

private:
    int power;
    int team_id;
    std::string name;
    ZorbAppearance appearance;
};

// Overload the insertion operator
std::ostream& operator<<(std::ostream& os, const Zorb& zorb) {
    // Display the Zorb as ASCII art
    std::vector<std::string> charLines;
    std::vector<std::vector<std::string>> rowBuffers;

    // Calculate margin
    size_t margin = (CONSOLESIZE % ZORBWIDTH) + ZORBWIDTH / 2;

    std::string appearanceText = zorb.GetAppearance();
    std::vector<std::string> appearanceLines = z_debug::SplitMultilineString(appearanceText);

    std::string nameText = zorb.GetName();
    std::string powerText = z_debug::FormattedText(std::to_string(zorb.GetTeamId()) + ", " + std::to_string(zorb.GetPower()) + 'p');

    for (size_t i = 0; i < appearanceLines.size(); ++i) {
        charLines.push_back(z_debug::SpaceToPrint(margin) + appearanceLines[i]);
    }
    charLines.push_back(z_debug::SpaceToPrint(margin) + nameText);
    charLines.push_back(z_debug::SpaceToPrint(margin) + powerText);

    rowBuffers.push_back(charLines);
    for (const auto& rowBuffer : rowBuffers) {
        for (const std::string& charLine : rowBuffer) {
            os << charLine << std::endl;
        }
    }
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
    void PrintZorbAppearances(int a = appearanceMap.size(), bool printNames = false, std::string color = ANSI_YELLOW) {
        if(appearanceMap.size() < 1) {
            std::cout << "No ZorbAppearances in memory" << std::endl;
            return;
        }
        else if(a > appearanceMap.size() || a < 0) {
            std::cout << "Invalid number of appearances to print" << std::endl
                << "current number of appearances: " << appearanceMap.size() << std::endl;
            return;
        }
        ZorbAppearance _debugappearance;
        std::vector<std::string> charLines;
        std::vector<std::vector<std::string>> rowBuffers;
        std::string appearanceText,
            nameText;
        int _enumNUM = 0;
        
        // Calculate margin
        size_t margin = (CONSOLESIZE%ZORBWIDTH) + ZORBWIDTH/2;

        while (_enumNUM < a) {
            _debugappearance.SetAppearance(static_cast<appearanceEnum>(_enumNUM), color);
            
            std::string nameText = appearanceNames.at(static_cast<appearanceEnum>(_enumNUM));
            appearanceText = _debugappearance.GetAppearance();
            std::vector<std::string> appearanceLines = SplitMultilineString(appearanceText);

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

            if ((charLines.back().length() + GetLengthWithoutEscapeCodes(appearanceLines[0]) + ZORBWIDTH) >= CONSOLESIZE) {
                rowBuffers.push_back(charLines);
                charLines.clear();
            } else {
                for (size_t i = 0; i < appearanceLines.size(); ++i) {
                    charLines[i] += z_debug::SpaceToPrint(margin);
                    charLines[i] += appearanceLines[i];
                }
                if(printNames)
                    charLines[appearanceLines.size()] += z_debug::SpaceToPrint(margin) + nameText;
                _enumNUM++;
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
